// http://www.careercup.com/question?id=5717567253512192

#include <gtest/gtest.h>

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

namespace ref {
template <class BidirectionalIterator>
  void reverse (BidirectionalIterator first, BidirectionalIterator last)
{
  while ((first != last) && (first != --last)) {
    std::iter_swap(first, last);
    //char r = *first;
    //*last = r;  // segfault
    //std::swap(*first, *last);
    ++first;
    break;
  }
}

//template <>
//  void reverse (char* first, char* last)
//{
  //while ((first != last) && (first != --last)) {
    //std::iter_swap(first, last);
    //char r = *first;
    //*last = r;  // segfault
  //  std::swap(*first, *last);
    //++first;
    //break;
  //}
//}


}  // space

// http://stackoverflow.com/questions/17813074/stdstring-getting-char-instead-of-const-char
// http://stackoverflow.com/questions/6600725/pass-stdstring-to-a-function-fchar?rq=1
void inplace_reverse(char * arr, const size_t length) {
  // your solution
  size_t b = 0;
  size_t e = 0;
  while (true) {
    while (arr[e] != ' ') {
      ++e;
      if (e >= length) {
        e = length;
        break;
      }
    }

    reverse(arr + b, arr + e);

    if (e >= length)
      break;
    else {
      ++e;
      b = e;
    }
  }

  reverse(arr, arr + length);
}


TEST(Facebook, ReverseStringInPlace) {
  // troubles with put string
  // end reverse
  // http://stackoverflow.com/questions/9850135/stdfind-backwards-on-c-style-array
  //char* v = "sadf"; // Non stack! it's ptr to const! std::mod_algs failed!
  char v[] = "no spaces at the ends of a sentence";
  inplace_reverse(v, strlen(v));

  cout << v << endl;
}
