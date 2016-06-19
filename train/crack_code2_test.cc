
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <algorithm>  // hm...
#include <vector>
#include <memory>
#include <stdexcept>

#include <string.h>
#include <assert.h>

using namespace std;

namespace arr_and_strings {

// Наверное за линейное время. Ничего быстрее хэш-таблиц нельзя
// Если сортировать входной массив, то будет a*N*logN + b*N*logN
bool there_unique(const string& line) {
  // по идее нужно сохранять
  // 
  // V0:
  //   где хранить - массив размером со строку
  //   но там искать O(n).
  //
  // V1:
  //   в хэштаблице быстро искать - O(1) - в качестве замены можно использовать просто массив по числу байт, а вычислять хэш
  //
  // V2:
  //   в отсортированном массиве - поиск за логарифмическое время - но нужно поддерживать инвариант
  //
  // V3: 
  //   делаем копию входного - сортируем - тогда размер фиксирован - но это пространство
}

// 1.2
// можно воспользоваться rotate
void reverse_string(char* line
  //, const int size
  ) {
  assert(line);
  int size = strlen(line);  // O(n)

  int half = size / 2;
  for (int i = 0; i < half; ++i) {
    swap(line[i], line[size - i - 1]);  // внутри создает переменную для обмена
  }
}

// 1.3 Remove dupl. chapters
// сложнее - нелзя использовать ничего доп. (большого дополнительного) - только локальные переменные
// похож на жадный? Greedy?
// информация есть обо всех - они в начале массива, и мы удалим если будет второе вхождение
// Инвариант - все до тайла уникальные
// Если сдвигать и идти до конца то похое - O(n^3)
void unique_n2(char* str, const unsigned size) {
  // сортировать вроде бы нельзя
  //for ()
  // DANGER: precond. check
  assert(str);

  // work
  int end(1);  // нужно одновременно удалять, т.е. сдвигать?
  for (int current = 1; current < size; ++current) {
    
    int finded = 0;
    const char elem = str[current];

    // ищем в нашей псевдотаблице
    while (finded < end) {
      if (str[finded] == elem) 
        break;  // нашли. просто переходим к следующему элементу
      ++finded;
    }

    // не нашел. добавить
    if (finded == end) {
      str[end] = elem;
      ++end;
    }

  }
  str[end] = 0;  // все что за ним уже мусор
}

// 1.4

// 1.6
// нужно как-то обеспечить выполнения границ - желательно автоматом
struct Pixel {
  char r;
  char g;
  char b;
  char i;
};

// xy-координаты не очень подходят, лучше четверть вращать, а кольца
// нужно изолировать область для размышлений - стараться на каждом шаге делать задачу меньше
//
// FAIL: свое решение не ахти
//
// FAIL: базовую часть прявильно понял, но укрупнение сделал не правильно - не естественно
//   но с другой стороны если четвертью, то локальность ссылок больше(?), хотя если развертка по х, то кольцами лучше
//   хотя локальность при записи или чтении быдет все равно плохой.
//
// FAIL: в задаче есть симметрия по координатам
//
// FAIL: нужно было не добавлять разбиений - уже есть границы квадрата, зачем было делить еще пополам
//   А раз добавил границы, значит их нужно будет проверять!
template <typename T>
void rotate_img_90(std::vector<std::vector<T> >& mat, const int N) {
  assert(mat.size() == N);
  assert(mat[0].size() == N);

  // границы как-то автоматом вошли
  for (int layer = 0; layer < N/2; ++layer) {
    int first = layer;
    int last = N - 1 - layer;
    // [first, last)
    for (int i = first; i < last; ++i) {
      // i - реальный доступ
      int offset = i - first;  // локальная система координат
      
      T top = mat[first][i]; 

      // left -> top
      mat[first][i] = mat[last - offset][first];

      // bottom -> left
      mat[last - offset][first] = mat[last][last - offset];

      // right -> bottom
      mat[last][last - offset] = mat[i][last];

      // top -> right
      mat[i][last] = top;
    }
  }
}

// 1.7 - однуления столбцов
// будет какое-то ускорение - часть удалиться... 
// все может начать зависеть от пути обхода! нужно пройти несколько раз

// 1.8

bool isSubstring(const string& where, const string& what) {
  return false;
}

void rotated() {

}

TEST(Crack, ArrAndStrings) {
  string s("abcde");
  arr_and_strings::reverse_string(&s[0]);
  assert(s == "edcba");

  char str[] = "aabbcc";
  //char* str = "aabbcc";  // Seg. fault
  arr_and_strings::unique_n2(str, strlen(str));
  assert(string(str) == "abc");


  const int N = 5;
  int mat_raw[N][N] = 
  {
    {11, 12, 13, 14, 92}, 
    {15, 16, 17, 18, 92},
    {19, 10, 11, 12, 92},
    {13, 14, 15, 16, 92},
    {70, 70, 70, 70, 70}
  };
  
  std::vector<std::vector<int> > v;

  for (int i = 0; i < N; ++i) {
    v.push_back(std::vector<int>(mat_raw[i], mat_raw[i]+N));
  }

  //int** pp_mat = &mat[0];
  //arr_and_strings::rotate_img_90((int**)mat, N);  // segfault - что-то с преобразованием было не то
  arr_and_strings::rotate_img_90(v, N);
}

}


namespace stacks_and_queues {
// ?.?
// делать трек min при вставке можно за O(n). Но pop() будет O(n), т.к. если удаляем минимальный, то что его заменит?

}


