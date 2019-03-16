
#include <gtest/gtest.h>

#include <vector>
#include <utility>

#include <stdlib.h>

using namespace std;

TEST(CCupGoogle, One) {
  // http://www.careercup.com/question?id=5122639288860672
  // O(1) space
  //
  // Deep math + work with stream
  vector<pair<int, int>> stream({{1,2},{2,2},{3,4},{4,10}});

  int sum = 0;
  size_t size = 0;
  for (auto& e: stream) {
    sum += e.second;
    ++size;
  }

  size_t id = ::rand() % size;

  // FIXME: can I do it for stream? - No.
  auto elem = stream[id];

  int w = elem.second;
}

// http://www.careercup.com/question?id=4925398016065536

/*
// what store?
int match (string target, string source){
    int [][] f = new int[target.size() + 1][source.size() + 1] ;

    for (int i = 0 ; i <= source.size() ; ++i) {
      f[0][i] = 1;
    }

    for (int i = 0 ; i <= target.size() ; ++i) {
      f[i][0] = 0;
    }

    // not init memory used

    f[0][0] = 1;

    for (int targetIdx = 1 ; targetIdx <= target.size() ; ++targetIdx) {
     for (int srcIdx = 1 ; srcIdx <= source.size() ; ++srcIdx) {
       source.at(srcIdx - 1);
       target.at(targetIdx - 1);

       if (t == s) {
         f[targetIdx][srcIdx]
             =   f[targetIdx - 1][srcIdx - 1]
               + f[targetIdx][srcIdx - 1] ;
       } else{
         f[targetIdx][srcIdx]
             = f[targetIdx][srcIdx - 1] ;
       }
     }
    }

    // reconstr
    return f[target.size()][source.size()] ;
}
*/

TEST(Google, Discount) {
  // http://www.careercup.com/question?id=5757597581836288
  // use DP
}

TEST(Google, DP) {
  // http://www.careercup.com/question?id=5739350128066560
  // Don't understood
}
