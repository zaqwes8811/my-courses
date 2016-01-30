#include "details/io_details.h"

#include <gtest/gtest.h>
// no standart now
//#include <ext/hash_set>
#include <boost/unordered_set.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>

using namespace std;
using namespace io_details;
using io_details::Job;
//using io_details::JobDecreaseWeightCompator;
//using io_details::operator<<;

namespace hided {
  
// TODO: почему-то не заработало!
class Comp {
public:
  explicit Comp(ostream* _o) : o(_o) { }
  
  // TODO: что такое шаблонный метод класса
  // https://toster.ru/q/20466
  template <typename U>
  void operator()(const U& elem) {
    *o << elem << " / ";
  }
  
private:
  ostream* const o;
};
  
template <typename T>
ostream& operator<<(ostream& o, const vector<T>& v) {
  Comp cmp(&o);
  for_each(v.begin(), v.end(), cmp);
  o << endl;
  return o;
}

struct JobDecreaseWeightCompator {
  bool operator() (const Job& j1, const Job& j2) {
    return j1.weight > j2.weight;
  }
};

struct JobDecreaseDeltaCompator {
  bool operator() (const Job& j1, const Job& j2) {
    return j1.get_delta() > j2.get_delta();
  }
};

// Calc completed time 

}


TEST(W1, All) {
  /// Q1
  using io_details::Job;
  using namespace hided;
  
  string w1_1_filename = "../stanford_algoritms_part2/in_data/jobs.txt";
  vector<Job> jobs = io_details::get_jobs_fake(w1_1_filename);
  cout << jobs;
  
  // Overhead, но возможно так лучше, т.к. как потом выстроить по весу не ясно
  sort(jobs.begin(), jobs.end(), JobDecreaseWeightCompator());  // сперва по уменьшения веса
  cout << jobs;
  
  // Нужна стабильность
  stable_sort(jobs.begin(), jobs.end(), JobDecreaseDeltaCompator());  // сперва по уменьшения разницы
  cout << jobs;
  
  // считаем для ответа

  
  /// Q2
  
  /// Q3

  
}
