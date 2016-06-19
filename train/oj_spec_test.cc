#include "visuality/view.h"

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <iostream>

using namespace view;
using namespace std;

// O(n^2), нужно O(n)
TEST(OJ, GasStantion) {
  // Похоже на dp задачу - непрер. посл. длиной N и суммой >= 0 - нет, тут не то
  //
  // цикличность может что-то дать.
  // 
  // если до конца не дойти, то вернуть -1
  int gas_[] = {4, 1, 3, 6, 1}; 
  vector<int> gas(gas_, gas_ + sizeof gas_ / sizeof gas_[0]);
  int cost_[] = {2, 4, 2, 2, 3}; 
  vector<int> cost(cost_, cost_ + sizeof cost_ / sizeof cost_[0]);  // i -> i+1 

  int idx = 0;
  for (int i = 0; i < gas.size(); ++i) {
    int tank = gas.at(0);

    idx = i;
    for (int j = 1; j < gas.size(); ++j) {
      tank -= cost[j-1];
      if (tank < 0) {
        idx = -1;
        break;
      }
      tank += gas.at(j);
    }

    // последний переход
    tank -= cost[gas.size()-1];
    if (tank < 0) idx = -1;

    if (tank >= 0)
      break;

    // now rotate
    rotate(gas.begin(), --gas.end(), gas.end());
    rotate(cost.begin(), --cost.end(), cost.end());
  }

}

// O(n) - почему 2N достаточно? не очивидно совсем
// еще можно схлопнуть массивы
TEST(OJ, GasStantionOn) {
  // Похоже на dp задачу - непрер. посл. длиной N и суммой >= 0 - нет, тут не то
  //
  // цикличность может что-то дать.
  // 
  // если до конца не дойти, то вернуть -1
  int gas_[] = {4, 1, 3, 6, 1}; 
  vector<int> gas(gas_, gas_ + sizeof gas_ / sizeof gas_[0]);
  int cost_[] = {2, 4, 2, 2, 3}; 
  vector<int> cost(cost_, cost_ + sizeof cost_ / sizeof cost_[0]);  // i -> i+1 

  vector<int> d(gas.size(), 0);  // d[i] - сколько нужно для перехода на gas[i+1]
  for (int i = 0; i < gas.size(); ++i) {
    d[i] = gas[i] - cost[i];
  }

  // 


}