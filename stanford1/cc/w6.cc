#include <gtest/gtest.h>

#include <cassert>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// C++11
#include <unordered_map>

// no standart now
//#include <ext/hash_set>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

// DANGER: Кажется с пользовательскими типами как-то не очень. Что то похоже и про строки слышал, кажется.
#include <google/dense_hash_map>

// 3rdpary
#include <boost/foreach.hpp>

// App
#include "details/io_details.h"
#include "details/fp_details.h"

using namespace std;

using std::hash;  // C++11
//using boost::hash;  // нужно точечно указать

// Precond.:
//  Keys is uniq.
//
int _2sum_naive(const vector<int>& in, const int target)
{
  int count_unique = 0;
  for (vector<int>::const_iterator at = in.begin(), end = in.end(); at != end; ++at) {
    
    // С границами что-то не то
    // Ищет только первый, как и нужно
    vector<int>::const_iterator finded_it = find_if(
	at, // begin  TODO: may be bug
	//in.begin()
	in.end(), 
	fp::compose_f_gx(bind2nd(equal_to<int>(), target), bind2nd(plus<int>(), *at)));
    
    if (finded_it != in.end())
      count_unique++;
  }
  return count_unique;
}

int q1(const vector<int>& in) {
  // http://publib.boulder.ibm.com/infocenter/comphelp/v9v111/index.jsp?topic=/com.ibm.xlcpp9.aix.doc/standlib/stl_unordered_set.htm
  boost::unordered_set<int> htbl;
  int low = 2500;
  int high = 4000;

  // de-duplication
  BOOST_FOREACH(int val, in) {
    if (htbl.end() == htbl.find(val))
      if (val < high+1)  // можно ужать еще
	htbl.insert(val); 
  };
  assert(htbl.size() < in.size());
  assert(!htbl.empty());

  int count_unique = 0;
  for (int t = low; t < high+1; ++t) {
    BOOST_FOREACH(int x, htbl) {
      int y = t - x;
      if (htbl.find(y) != htbl.end()) {
	if (x != y) {
	  count_unique++;
	  break;
	}
      }
    }
  }
  return count_unique;
}

long q2_nth(const vector<int>& in) {
  vector<int> in_stream = in;
  vector<int> cursor;
  long mean = 0;
  cursor.reserve(in.size());
  BOOST_FOREACH(int elem, in_stream) {
    cursor.push_back(elem);   
    int size = cursor.size();
    int nth = 0;
    if (size % 2 == 1) {
      nth = (size + 1) / 2 - 1;
    } else {
      nth = size / 2 - 1;
    }
    
    // Возможно частичная сортировка
    assert(cursor.begin()+nth != cursor.end());
    nth_element(cursor.begin(), cursor.begin()+nth, cursor.end());
    mean += cursor[nth];
  }
  return mean;
}

TEST(W5_6, PQuestions) {
  /// Q1
  // Задание не понятно
  //int finded = _2sum_naive(in, target);
  //assert(finded > 0);
  // TODO: сделать все три варианта
  vector<int> in = io_details::extract_records("./input_data/HashInt.txt");
  int count_unique = q1(in);  
  assert(count_unique > 0 && count_unique < 1501);
  assert(count_unique == 1477);
  assert(!in.empty());

  /// Q2
  in = io_details::extract_records("./input_data/Median.txt");
  // TODO: heap and bfs based impl.
  assert(q2_nth(in) % 10000 == 1213); 
}
