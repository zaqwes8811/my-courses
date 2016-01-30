// Knapsack:
//   - параллелизация не сильно помогла - в два раза только - хотя и вышло все просто
//   - похоже нужен другой алгоритм 
//
// Если очень большой объем данных
//   - Рекурсивная реализация
//   - Использовать только 2 колонки - скорости это врядли прибавит
//   - Образать веса которых не может быть - как в задаче из 1 части - похоже в алгоритме уже есть фильтр
//   - Хэш таблица похоже безсмысленна, она реализована на основе массива - хотя...
//   - Сортировка
//
#include <gtest/gtest.h>

// C++98
#include <vector> 
#include <ostream>
#include <string>

// C++11
#include <unordered_map>

// 3rdparty
#include <boost/unordered_map.hpp>
//#include <google/dense_hash_map>
//#include <google/sparse_hash_map>
//#include <adobe/algorithm/sort.hpp>

// inner
#include "visuality/view.h"
#include "details/io_details.h"

using namespace std;

using view::operator<<;
using io_details::Item;
using io_details::get_test_items;
using io_details::get_dyn_items;

namespace {
struct TaskId {
  TaskId(int _w_bound, int _idx) : w_bound(_w_bound), idx(_idx) {}
  TaskId() : w_bound(0), idx(0) {}
  int w_bound;
  int idx;
};

struct ItemsCompare {
  bool operator()(const Item& lhs, const Item& rhs) {
    return !(lhs.w < rhs.w);
  }
};

struct KeyHash {
  std::size_t operator()(const TaskId& k) const
  {
    // Watch "Eff. Java."  
    // Проблема в том как скомбинировать.
    // TODO: other method calc.
    //
    // Good hash for pair:
    //  http://stackoverflow.com/questions/12764645/good-hash-function-with-2-integer-for-a-special-key
    //  http://stackoverflow.com/questions/2634690/good-hash-function-for-a-2d-index
    //  !!! http://web.archive.org/web/20071223173210/http://www.concentric.net/~Ttwang/tech/inthash.htm
    //
    // Hash variants:
    //return boost::hash<pair<int, int> >()(make_pair(k.idx,k.w_bound));  // slow
    //return boost::hash<int>()(k.w_bound) ^ (boost::hash<int>()(k.idx) << 1);
    //return boost::hash<int>()(k.idx) ^ (boost::hash<int>()(k.w_bound) >> 1);
    //return (size_t)((k.idx << 19) | (k.w_bound << 7));
    //return boost::hash<int>()(k.idx) * 37 + (boost::hash<int>()(k.w_bound));
    //return ((997 + boost::hash<int>()(k.idx)))*997 + boost::hash<int>()(k.w_bound);
    
    // max speed
    return boost::hash<int>()(k.idx) ^ (boost::hash<int>()(k.w_bound) << 1);  
  }
};

struct KeyHashStd {
  std::size_t operator()(const TaskId& k) const
  {
    // Watch "Eff. Java."  
    // Проблема в том как скомбинировать.
    // TODO: other method calc.
    // Влияет очень не слабо! Возможно лучше 2D version
    return ((std::hash<int>()(k.idx)) ^ (std::hash<int>()(k.w_bound) << 1));
    //return boost::hash<int>()(k.idx) * 37 + (boost::hash<int>()(k.w_bound));
    return ((51 + std::hash<int>()(k.idx)))*51 + std::hash<int>()(k.w_bound);
  }
};
 
struct KeyEqual {
  bool operator()(const TaskId& lhs, const TaskId& rhs) const
  {
    return lhs.idx == rhs.idx && lhs.w_bound == rhs.w_bound;
  }
};

ostream& operator<<(ostream& o, const TaskId& id) 
{
  o << "(" << id.w_bound << ", " << id.idx << ")\n";
  return o;
}
}  // namespace

namespace {
// Returns the maximum value that can be put in a knapsack of capacity W
template <typename Store>
int knapSack_hashtable(const TaskId& id, 
                       const vector<Item>& items, 
                       Store& store)
{
  // check task is solved
  if (store.end() != store.find(id)) {
    return store[id];
  }
  
  // Work
  int n = id.idx;
  int w_bound = id.w_bound;
  
  // Base Case
  if (n == 0 || w_bound == 0)
      return 0;

  // If weight of the nth item is more than Knapsack capacity W, then
  // this item cannot be included in the optimal solution
  if (items[n-1].w > w_bound) {
    return 
      knapSack_hashtable(TaskId(w_bound,                n-1), items, store);
  } else {
    // Return the maximum of two cases: (1) nth item included (2) not included
    int sum_values = std::max( 
      knapSack_hashtable(TaskId(w_bound - items[n-1].w, n-1), items, store) + items[n-1].v, 
      knapSack_hashtable(TaskId(w_bound,                n-1), items, store));
    
    // Добавляем решенную
    store.insert(make_pair(id, sum_values));
    return sum_values;
  } 
}

//template <typename Store>
// Too slooow
int knapSack_hashtable_2d(const TaskId& id, 
                         const vector<Item>& items, 
                         std::unordered_map<int, std::unordered_map<int, int> >& store)
{
  // check task is solved
  {
    std::unordered_map<int, std::unordered_map<int, int> >::iterator it = store.find(id.w_bound);
    if (store.end() != it) {
      std::unordered_map<int, int> tmp = it->second;
      if (tmp.end() != tmp.find(id.idx))
        return tmp[id.idx];
    }
  }
  
  // Work
  int n = id.idx;
  int w_bound = id.w_bound;
  
  // Base Case
  if (n == 0 || w_bound == 0)
      return 0;

  // If weight of the nth item is more than Knapsack capacity W, then
  // this item cannot be included in the optimal solution
  if (items[n-1].w > w_bound) {
    return 
      knapSack_hashtable_2d(TaskId(w_bound,                n-1), items, store);
  } else {
    // Return the maximum of two cases: (1) nth item included (2) not included
    int sum_values = std::max( 
      knapSack_hashtable_2d(TaskId(w_bound - items[n-1].w, n-1), items, store) + items[n-1].v, 
      knapSack_hashtable_2d(TaskId(w_bound,                n-1), items, store));
    
    // Добавляем решенную
    // Если первый раз, то создаем пустую таблицу
    if (store.end() == store.find(id.w_bound)) {
      store.insert(make_pair(id.w_bound, std::unordered_map<int, int>()));
    }
    
    store[id.w_bound].insert(make_pair(id.idx, sum_values));
      
    //store.insert(make_pair(id, sum_values));
    return sum_values;
  } 
}
}

TEST(W3, BigKnapsackProblem) 
{
  // V(i, x) = max(V(i-1, x), v(i) + V(i-1, x-w(i))
  // if w(i) > x 
  //   V(i, x) = V(i-1, x)
  //
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    const int v = items[i].v;
    const int w = items[i].w;
    
    // Значения объема может быть любым с точностью до единицы
    // Итерация по емкостям
    for (int x = 0; x <= W; ++x) {
      int w_boundary = x - w;
      
      if (w_boundary < 0) {
        A[i][x] = A[i-1][x];  // просто копирование
      } else {
        A[i][x] = std::max(A[i][x], A[i-1][w_boundary] + v);  // BUG: looks like
      }
    }
  }
  
  //cout << " " << A;
}

TEST(W3, GeeksForGeek_hashtable) 
{ 
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  TaskId root(W, count);
  boost::unordered_map<TaskId, int, KeyHash, KeyEqual> store;
  int result = knapSack_hashtable(root, items, store);
  printf("sum(v(i)) = %d \n", result);
  assert(result == 8);
}

TEST(W3, GeeksForGeek_hashtable_homework) 
{ 
  //pair<int, vector<Item > > tmp = get_dyn_items("./input_data/knapsack1.txt");
  pair<int, vector<Item > > tmp = get_dyn_items("./input_data/knapsack_big.txt");
  
  ///*
  vector<Item> items = tmp.second;
  
  // Попробовать отсортировать по весу - только дольше
  //adobe::sort(items);//, ItemsCompare());
  //std::sort(items.begin(), items.end(), ItemsCompare());
  
  int W = tmp.first;

  int count = items.size();
  TaskId root(W, count);
  
  boost::unordered_map<TaskId, int, KeyHash, KeyEqual> store;//(4000);
  //std::unordered_map<TaskId, int, KeyHashStd, KeyEqual> store;
  
  
  /* 
  //DANGER: it's slooooow or incorrect
  //google::dense_hash_map<
  google::sparse_hash_map<
    TaskId, int
    , KeyHashStd, KeyEqual
    > store;
  //store.set_empty_key(TaskId());
  //*/
  cout << store.max_bucket_count() << endl;
  
  int result = knapSack_hashtable(root, items, store);
  printf("sum(v(i)) = %d \n", result);
  assert(result == 4243395);
}

// Very slooow.
TEST(W3, GeeksForGeek_hashtable_2homework) 
{ 
  //pair<int, vector<Item > > tmp = get_dyn_items("./input_data/knapsack1.txt");
  pair<int, vector<Item > > tmp = get_test_items("./input_data/knapsack_big.txt");
  
  ///*
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  TaskId root(W, count);
  //boost::unordered_map<TaskId, int, KeyHash, KeyEqual> store;
  std::unordered_map<int, std::unordered_map<int, int> > store;
  
  int result = knapSack_hashtable_2d(root, items, store);
  printf("sum(v(i)) = %d \n", result);
  //assert(result == 4243395);
}

TEST(W3, SparseColumn) {
  // TODO: похоже скорость должна быть много больше
}

TEST(W3, BranchAndBound) {
  
}
