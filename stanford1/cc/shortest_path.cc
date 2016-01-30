// Type converting:
// "if you use Boost.Tokenizer, why not to replace atoi by boost::lexical_cast?"
//
// String processing: 
//   http://www.codeproject.com/Articles/23198/C-String-Toolkit-StrTk-Tokenizer
//
// STL alg.L:
// http://stackoverflow.com/questions/11343116/rotating-a-vector-array
//
// TODO:
// shared_ptr< const V > - нужно для пероизовдительности при работе алгоритмов - но это обращение к куче!
// Own ref. counting - 
//
// OpenCV - ass Wrapper, ReferenceHandler, refcount in core_c.h, refcount->, int* refcount;, int refcount
//   struct PtrOwner - ptr.inl.hpp, struct ImplMutex::Impl
//   Mat-objects - ?
// Hypertable: ReferenceCount, intrusive_ptr
//
// Summary:
// - используют как базовый
// - можно как-то работать не только с классами, но и со структурами - см. OpenCV
// OpenCV: //! Smart pointer for OpenGL 2D texture memory with reference counting. -
//
// Correctness:
// http://rosettacode.org/wiki/Dijkstra's_algorithm#Python

// C
#include <cassert>

// C++
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <set>
#include <limits>       // std::numeric_limits

// http://www.onlamp.com/pub/a/onlamp/2006/04/06/boostregex.html?page=3
//#include <boost/regex.hpp>  // too hard
//#include "boost/lexical_cast.hpp"  // не то, что нужно - это уже для строк со снятым форматированием

#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <boost/foreach.hpp>

// App
#include "details/graph_details.h"

#define for_each_ BOOST_FOREACH

int main() 
{
  using namespace std;
  using namespace tbb;

  using graph_details::Neighbors;
  using graph_details::Neighbor;
  using graph_details::extract_records;
  using graph_details::build_graph;
  using graph_statistic::NodeInfo;
  using graph_statistic::kMaxVal;
  
  try {
    /// IO and build graph
    // DbC debug only!
    //vector<string> records = extract_records("../input_data/dijkstraData_test.txt");
    vector<string> records = extract_records("../input_data/dijkstraData.txt");
    vector<Neighbors> graph = build_graph(records);
   
    // Validate graph
    assert(true);
    
    vector<NodeInfo> track(graph.size());
    
    NodeInfo* info = &track[0];
    for (size_t i = 0; i < track.size(); ++i)
      info[i].idx = i;
    int v_active = 1;
    track[v_active].d = 0;
    
    // Work
    // похоже нужна очередь как в BFS
    // цикл не такой
    while (true) {  // как выйти то? А если граф не связан!? 
      // ищем минимальный элемент среди не помеченных
      vector<NodeInfo>::iterator at = min_element(begin(track), end(track), 
	[](const NodeInfo& o1, const NodeInfo& o2) -> bool {
	  int d1 = o1.d;
	  int d2 = o2.d;
	  if (o1.visited) d1 = kMaxVal;
	  if (o2.visited) d2 = kMaxVal;
	  return d1 < d2;
	}
      );
      
      assert(at != end(track));  // не должны попасть, если все правильно
      NodeInfo min_node = *at;
      
      // TODO: достаточное ли условие окончания?
      if (min_node.d == kMaxVal)
	break;
      
      at->visited = true;
      int idx = distance(track.begin(), at);  // если удаляем то выходить ерунда - индексы сбиваются
      
      // Обходим соседей
      const Neighbors& neighbors = graph[idx];
      int root_d = min_node.d;
      
      for_each_(Neighbor n, neighbors) {
	int v = n.end;
	if (!track[v].visited) {
	  int w = n.weight;
	  int current_d = track[v].d;  // текущее расстояние
	  int new_d = root_d + w;
	  if (current_d > new_d)
	    track[v].d = new_d;
	}
      }
    }
 
    // Task mask
    int mask[] = {7,37,59,82,99,115,133,165,188,197};
    vector<int> mask_s;
    mask_s.assign(mask, mask+10);
    for_each(begin(mask_s), end(mask_s), [&track] (int i) {
      cout << track[i].d << ",";
    });
    cout << endl;
  } catch (const exception& e) {
    cout << e.what() << endl;
  }  
  return 0;
}


/// With heap
// https://sites.google.com/site/indy256/algo_cpp/dijkstra_heap
// http://stackoverflow.com/questions/23592252/implementing-dijkstras-shortest-path-algorithm-using-c-and-stl


