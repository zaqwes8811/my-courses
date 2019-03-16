// Notes:
//   much opt problems
//   work with big amount of data
//
#include <visuality/view.h>

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>

using namespace std;
using view::operator <<;

TEST(CCupAmazon, One) {
  // http://www.careercup.com/question?id=5758712897601536
  // "Bin Packing Problem"
  //
  // More important non calc but optimality of result

  size_t tapeSize = 100;
  vector<int> files({70, 10, 20, 30, 100});

  // V1
  // 1
  sort(files.begin(), files.end(), greater<int>());

  // 2
  size_t b = 0;
  size_t e = files.size() - 1;
  size_t res = 0;

  while(true) {
    if (b >= e)
      break;

    ++res;  // forever
    if (files[b] + files[e] <= tapeSize) {
      ++b;
      --e;
    } else {
      ++b;
    }

    if (b == e)
      ++res;  // even size case
  }

  EXPECT_EQ(3, res);

  // V2 - binary search - it's change something?
}


class StreamGenerator {
public:
  StreamGenerator() :
    ptr(0),
    stream("The angry dog was red. And the cat was also angry.") { }

private:
  size_t ptr;
  string stream;
};

TEST(CCup, Two) {
  // http://www.careercup.com/question?id=5695840750403584
  // FIXME: Stream may be > RAM size!
  //
  // In Java LinkedHashMap - C++ - ?
  //
  // Dyn progr - max len + min weight ?
  //
  // Store to disk some thing?
}

// http://www.careercup.com/question?id=5715661219168256

// http://www.careercup.com/question?id=5676765995335680

// prizoners
// http://www.careercup.com/question?id=5734324101971968

TEST(Amazon, Arrays3) {
  // http://www.careercup.com/question?id=5717493200977920
}

// No shift!
template<typename T>
struct OnQueue {
  OnQueue()
    : m_buffer(3)
    , m_rd_ptr(0)
    , m_wr_ptr(0)
    , m_can_wr(true)
    , m_can_rd(false) { }

  // must be O(1)
  bool push(const T& val)
  {
    if (!m_can_wr)
      return false;

    m_buffer[m_wr_ptr] = val;
    m_wr_ptr = inc(m_wr_ptr);

    m_can_rd = true;
    if (m_rd_ptr == m_wr_ptr)  // can't write in this case - speed wr > rd
      m_can_wr = false;

    return true;
  }

  // bad api
  bool pop(T& r_val) {
    if (!m_can_rd)
      return false;

    r_val = m_buffer[m_rd_ptr];
    m_rd_ptr = inc(m_rd_ptr);

    m_can_wr = true;
    if (m_rd_ptr == m_wr_ptr)
      m_can_rd = false;

    return true;
  }

private:
  size_t inc(size_t val) {
    ++val;
    return val % m_buffer.size();
  }

public:
  std::vector<T> m_buffer;

private:
  size_t m_rd_ptr;  // ptr to next read
  size_t m_wr_ptr;  // ptr to next wr
  bool m_can_wr;  // looks like need 2 flags
  bool m_can_rd;
};

template <typename T>
ostream& operator<<(ostream& o, const OnQueue<T>& q) {
  o << q.m_buffer;
  return o;
}

TEST(Amazon, TrickyQueue) {
  // http://www.careercup.com/question?id=14781667

  OnQueue<int> q;
  //EXPECT_TRUE
  q.push(1);
  cout << q;
  q.push(2);
  cout << q;
  q.push(3);
  cout << q;
  q.push(4);
  cout << q;
  q.push(5);
  cout << q;

  int v = 0;
  q.pop(v);
  cout << q;

  q.push(3);
  cout << q;

  q.pop(v);
  cout << q;
  q.pop(v);
  cout << q;
  q.pop(v);
  cout << q;
  EXPECT_FALSE(q.pop(v));
  cout << q;

  // if array or vector - shift = O(n)
}

TEST(Amazon, OptBigBin) {
  // http://stackoverflow.com/questions/5420317/reading-and-writing-binary-file

  // http://www.careercup.com/question?id=13243679
  int i = 0;
  long sum = 0;

  ifstream file("binary.dat", ios::in|ios::binary);  // big binary file
  // Need buffering
  if(file.is_open()) {
    while(!file.eof()) {
      // read with buffer
      char* buff = reinterpret_cast<char*>(&i);
      file.read(buff, sizeof(unsigned int));
      sum += i;
      i = 0;
    }
  }
  file.close();
}

TEST(Amazon, OptBigBin_mod) {
  // http://stackoverflow.com/questions/5420317/reading-and-writing-binary-file

  // http://www.careercup.com/question?id=13243679
  long sum = 0;

  ifstream file("binary.dat", ios::in|ios::binary);  // big binary file

  // know size - bad idea - out of ranges basic types
  //file.seekg(0, std::ios::end);
  // 1 billion chars - 1 000 000 000
  // 2**32 = 4 294 967 296
  //
  // Troubles with big values
  const size_t pointSizeof = sizeof (unsigned int);
  const size_t countPoints =
      1024 * 2 * 2;
      //file.tellg() / pointSizeof;  // in !bytes in ints
  //file.seekg(0, std::ios::beg);

  // Need buffering
  vector<int> buffer(countPoints);
  if(file.is_open()) {
    while(!file.eof()) {
      // read with buffer
      char* buff = reinterpret_cast<char*>(buffer.data());
      file.read(buff, pointSizeof * countPoints);

      for(size_t j = 0; j < file.gcount(); ++j)
        sum += buffer[j];

    }
  }
  file.close();
}

TEST(Amazon, Ball) {
  // http://www.careercup.com/question?id=5145121580384256
  vector<int> balls(81);
  std::generate(balls.begin(), balls.end(), std::rand);
  balls[0] = 1;
  std::random_shuffle(begin(balls), end(balls));

  // design
}
