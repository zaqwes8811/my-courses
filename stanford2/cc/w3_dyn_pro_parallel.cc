
#include "visuality/view.h"
#include "details/io_details.h"

#include <gtest/gtest.h>
#include <boost/unordered_map.hpp>

#ifdef USE_TBB
#  include "tbb/task_scheduler_init.h"
#  include "tbb/task.h"
#  include "tbb/concurrent_hash_map.h"
#  include <tbb/scalable_allocator.h>
#endif

// C++98
#include <vector>
#include <ostream>
#include <string>


using namespace std;
#ifdef USE_TBB
using namespace tbb;
#endif

using view::operator<<;
using io_details::Item;
using io_details::get_test_items;
using io_details::get_dyn_items;

struct TaskId {
  TaskId(int _w_bound, int _idx) : w_bound(_w_bound), idx(_idx) {}
  TaskId() : w_bound(0), idx(0) {}
  int w_bound;
  int idx;
};

// Structure that defines hashing and comparison operations for user's type.
struct TBBHashCompare {
    static size_t hash( const TaskId& k ) {
        return boost::hash<int>()(k.idx) ^ (boost::hash<int>()(k.w_bound) << 1);//KeyHash()(k);
    }
    //! True if strings are equal
    static bool equal( const TaskId& lhs, const TaskId& rhs ) {
        return lhs.idx == rhs.idx && lhs.w_bound == rhs.w_bound;//KeyEqual()(x,y);
    }
};

#ifdef USE_TBB

// Аллокатор тоже не помогает
//  tbb::scalable_allocator<pair<TaskId, int> >
typedef concurrent_hash_map<TaskId, int, TBBHashCompare> TaskTable;

// Alse exist:
//   http://www.threadingbuildingblocks.org/docs/help/reference/containers_overview/concurrent_unordered_map_cls.htm

// скорости не добавляет, а наоборот медленнее работает
template <typename Store>
struct Future: public task {
    int* const r_;
    int x;
    int y;
    Store* const store_;
    const TaskId id_;
    int delta;
    
    Future( int* sum_, Store* store, const TaskId& id) : r_(sum_), store_(store), id_(id) {}
    task* execute() {
      
      x += delta;
      *r_ = std::max(x, y);
        
      bool connect = true;      
      if (connect) {
        // DANGER: похоже некоторые задачи таки решаются не один раз - и это уменьшает быстродействие
        TaskTable::accessor a;
        if (store_->insert(a, id_)) {
          a->second = *r_;
        } else {
          // DANGER: промахов очень много
          //cout << "BAD: Vain solved\n";
        }
      }
      return NULL;
    }
};

template <typename Store>
class KnapsackTaskFuture : public task  {
  Store* const store_;
  const vector<Item>* items_;
  int* const r_;
  const TaskId id_;
  
  KnapsackTaskFuture(const KnapsackTaskFuture&);
  KnapsackTaskFuture& operator=(const KnapsackTaskFuture&);
  
public:
  KnapsackTaskFuture(const vector<Item>& items, Store& store, int& result, const TaskId& id) 
    : store_(&store), items_(&items), r_(&result), id_(id)
  {}
  
  task* execute()
  { 
    const bool connect = true;
    // Похоже так лихо не выйдет - задача обязуется выполнить вычисления а сумма фейковая
    // Если решена то вернуть результат
    // Распространить лок не удалось пока что.
    if (connect) {
      //TaskTable::accessor a;
      TaskTable::const_accessor a;
      if (store_->find(a, id_)) {
        *r_ = a->second;
        return NULL;
      }
    }
    
    // Некоторые задачи отбросили, но некоторые в процессе решения и будут готовы к моменту записи
    {
      // Work
      int n = id_.idx;
      int w_bound = id_.w_bound;
      
      // Base Case
      if (n == 0 || w_bound == 0) {
        *r_ = 0;  // можно сохранять, но на производительность сильно не влияет
        return NULL;
      }
      
      // Prepare
      int delta = 0;
      TaskId work_id(w_bound, n-1);
      if (!((*items_)[n-1].w > w_bound)) {
        work_id = TaskId(w_bound-(*items_)[n-1].w, n-1);
        delta = (*items_)[n-1].v;
      }
      TaskId old_task(w_bound, n-1);

      // Actions 
      Future<TaskTable>& c = *new( allocate_continuation() ) Future<TaskTable>(r_, store_, id_);
      c.delta = delta;
      KnapsackTaskFuture& a = *new( c.allocate_child() ) KnapsackTaskFuture(*items_, *store_, c.x, work_id);
      KnapsackTaskFuture& b = *new( c.allocate_child() ) KnapsackTaskFuture(*items_, *store_, c.y, old_task);
      // Set ref_count to 'two children plus one for the wait".
      c.set_ref_count(2);
      // Start b running.
      spawn( b );
      //spawn( a );
      return &a;  // кажется чуть быстрее чем spawn(a), но все равно медленно
      // Start a running and wait for all children (a and b).
      //spawn_and_wait_for_all(a);
      
    }   
    return NULL;
  }
};

// DANGER: алгоритм пробуксовывает - иногда решает уже решенные задачи
template <typename Store>
class KnapsackTaskSerial : public task  {
  Store* const store_;
  const vector<Item>* items_;
  int* const r_;
  const TaskId id_;
  
  KnapsackTaskSerial(const KnapsackTaskSerial&);
  KnapsackTaskSerial& operator=(const KnapsackTaskSerial&);
  
public:
  KnapsackTaskSerial(const vector<Item>& items, Store& store, int& result, const TaskId& id) 
    : store_(&store), items_(&items), r_(&result), id_(id)
  {}
  
  task* execute()
  { 
    const bool connect = true;
    // Похоже так лихо не выйдет - задача обязуется выполнить вычисления а сумма фейковая
    // Если решена то вернуть результат
    // Распространить лок не удалось пока что.
    if (connect) {
      //TaskTable::accessor a;
      TaskTable::const_accessor a;
      if (store_->find(a, id_)) {
        *r_ = a->second;
        return NULL;
      }
    }
    
    // Некоторые задачи отбросили, но некоторые в процессе решения и будут готовы к моменту записи
    {
      // Work
      int n = id_.idx;
      int w_bound = id_.w_bound;
      
      // Base Case
      if (n == 0 || w_bound == 0) {
        *r_ = 0;  // можно сохранять, но на производительность сильно не влияет
        return NULL;
      }
      
      // Prepare
      int delta = 0;
      TaskId work_id(w_bound, n-1);
      if (!((*items_)[n-1].w > w_bound)) {
        work_id = TaskId(w_bound-(*items_)[n-1].w, n-1);
        delta = (*items_)[n-1].v;
      }
      TaskId old_task(w_bound, n-1);

      // Actions 
      int x = 0;
      int y = 0;
      KnapsackTaskSerial& a = *new( allocate_child() ) KnapsackTaskSerial(*items_, *store_, x, work_id);
      KnapsackTaskSerial& b = *new( allocate_child() ) KnapsackTaskSerial(*items_, *store_, y, old_task);
      // Set ref_count to 'two children plus one for the wait".
      set_ref_count(3);
      // Start b running.
      spawn( b );
      // Start a running and wait for all children (a and b).
      spawn_and_wait_for_all(a);  // BAD: NEED NO WAIT!
      
      x += delta;
      *r_ = std::max(x, y);
    }
    
    if (connect) {
      // DANGER: похоже некоторые задачи таки решаются не один раз - и это уменьшает быстродействие
      TaskTable::accessor a;
      if (store_->insert(a, id_)) {
        a->second = *r_;
      } else {
        // DANGER: промахов очень много
        //cout << "BAD: Vain solved\n";
      }
    }
    
    return NULL;
  }
};

TEST(W3, ParallelKnap) {
  TaskTable g_task_table;
  pair<int, vector<Item > > tmp = 
    get_dyn_items("./input_data/knapsack_big.txt");
    //get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  int count = items.size();
  int result = 0;
  
  TaskId root(W, count);
  //int store = 0;
  KnapsackTaskSerial<TaskTable>& a = *new(task::allocate_root()) 
      KnapsackTaskSerial<TaskTable>(items, g_task_table, result, root);
  task::spawn_root_and_wait(a);
  //KnapsackTaskSerial<int>(items, store, result, root).execute();
  
  printf("sum(v(i)) = %d \n", result);
  //assert(result == 8);
  assert(result == 4243395);
}

TEST(W3, ParallelKnapNoWait) {
  TaskTable g_task_table;
  pair<int, vector<Item > > tmp = 
    get_dyn_items("./input_data/knapsack_big.txt");
    //get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  int count = items.size();
  int result = 0;
  
  TaskId root(W, count);
  //int store = 0;
  KnapsackTaskFuture<TaskTable>& a = *new(task::allocate_root()) 
      KnapsackTaskFuture<TaskTable>(items, g_task_table, result, root);
  task::spawn_root_and_wait(a);
  //KnapsackTaskSerial<int>(items, store, result, root).execute();
  
  printf("sum(v(i)) = %d \n", result);
  //assert(result == 8);
  assert(result == 4243395);
}
#endif
