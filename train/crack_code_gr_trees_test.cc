 // "Not all binary trees are binary search trees"

#include "visuality/view.h"

#include <gtest/gtest.h>

#include <iostream>
#include <memory>

//
#include <stack>
#include <queue>

namespace crack_trees_and_gr {
using namespace view;
using namespace std;

//template <typename T>
struct node
{
  node() : left(0), right(0), data(0) {}
  explicit node(int _data) 
    : left(0), right(0)  //node() - c++11 delegation ctor
    , data(_data) {}
  node* left;
  node* right;
  //T 
  int
  data;
};

//template <typename T> 
node* create_tree() {
  node* root = new node(1);

  root->left = new node(2);
  node* current = root->left;
  current->left = new node(4);
  current->right = new node(5);

  current = root;
  current->right = new node(3);
  return root;
}

void destroy_tree(node* root) {
  // снизу вверх, хотя может и не нужно
  // post-orger way
  if (!root)
    return;

  destroy_tree(root->left);
  destroy_tree(root->right);
  auto_ptr<node> _(root);  // visited
}

// вряд ли нужно хранить трек
void post_order_tree(node* root, int level) {
  // снизу вверх, хотя может и не нужно
  // post-orger way
  if (!root)
    return;

  ++level;
  post_order_tree(root->left, level);
  post_order_tree(root->right, level);

  // extra space
  if (!root->left && !root->right)
    cout << root->data << "/" << level << " ";
}


void print_tree_pre_order_iterative(node* const root) 
{
  // root -> l -> r
  // FIXME: как снять данные

  stack<node*> parent_stack;
  parent_stack.push(0);

  node* top = root;

  while (top) {
    cout << top->data << ", ";  

    // правый уходит вниз
    if (top->right)
      parent_stack.push(top->right);

    if (top->left)
      parent_stack.push(top->left);

    top = parent_stack.top();
    parent_stack.pop();
  }
}

/// Traversal
//   http://en.wikipedia.org/wiki/Corecursion
//   http://programmers.stackexchange.com/questions/144274/whats-the-difference-between-recursion-and-corecursion
//
// depth-first order:
//   pre-order,[1] in-order,[1] and post-order - l->r но положение r меняется
//
// In Bra
TEST(Crack, CreateTree) {
  using namespace crack_trees_and_gr;
  node* root = create_tree();
  print_tree_pre_order_iterative(root);
  cout << endl;

  destroy_tree(root);
}

// std::stack and std::queue
TEST(Std, StackAndQueue) {
  // некоторые функции двухфазные как бы - но вообще это правильное разделение обяз.

  // Stack
  // for top and pop size > 0 - else UB

  // Queue - кстати deque по умолчанию, а в многопоточных использвал список
  // if size == 0 - front(), back(), pop() - UB!!
}

void check_balansing(node* const root) {
  queue<node*> q;
  q.push(root);
  int level = -1;

  // одна итерация не равна обработки одного уровня
  while (!q.empty()) {
    // как выделить границу?
    node* tmp = q.front();
    q.pop();
    cout << tmp->data << ", ";

    if (tmp->left)
      q.push(tmp->left);

    if (tmp->right)
      q.push(tmp->right);

    // если одинаковые пустые
    //if ()

    //++level;
  }
}

// 4.1 - балансировка
//
// FAIL: решил, но понадобилась дополнительная память.
//
// FAIL: если найти мнимум и максимум, то не все тестовые кейсы срабатывают
TEST(Crack, CheckBalansing) {
  // delta(li, lj) <= 1
  // у листа оба указателя нулевые? т.е. нет дочерних
  using namespace crack_trees_and_gr;
  node* root = create_tree();
  int level = -1;
  
  // можно сделать два прохода
  //post_order_tree(root, level);
  cout << endl;

  destroy_tree(root);
}

// 4.2 - связанность вершин

// 4.3. - создание bst
// http://leetcode.com/2010/04/binary-search-tree-in-order-traversal.html
void in_order_traversal(node *p) {
  if (!p) 
    return;
  in_order_traversal(p->left);
  cout << p->data;
  in_order_traversal(p->right);
}

node* make_bst(vector<int>& v) 
{
  if (v.empty())
    return 0;

  // ищем центр
  int elem_idx = v.size() / 2;
  int elem = v[elem_idx];
  node* root = new node(elem);

  vector<int> v_left(v.begin(), v.begin()+elem_idx);
  vector<int> v_right(v.begin()+elem_idx+1, v.end());

  //cout << v_left << v_right;

  root->left = make_bst(v_left);
  root->right = make_bst(v_right);

  return root;
}

TEST(Crack, BSTCreate) {
  int arr[] = {1, 2, 3, 4, 5, 6, 7};
  vector<int> v(arr, arr + sizeof(arr)/sizeof(arr[0]));

  // раз отсортирован, значит корень - где-то в середине

  node* root = make_bst(v);

  in_order_traversal(root);
  destroy_tree(root);
}

// 4.4 - make layers
// вряд ли нужно хранить трек
//
// FAIL: не все граничные условия выполнены
void split_into_layers(node* root, int level, vector<vector<int> >& vv) {
  // снизу вверх, хотя может и не нужно
  // post-orger way
  if (!root)
    return;

  ++level;  // она локальная копия
  split_into_layers(root->left, level, vv);
  split_into_layers(root->right, level, vv);

  // FIXME: здесь ошибка в границе
  // Расширяем список - мы не знаем сколько слоев
  while (vv.size() < level+1) {  // <= добавляет пустой элемент в конец
    vv.push_back(vector<int>());
  }

  vv[level].push_back(root->data);
}

TEST(Crack, BSTLayers) {
  // список списков, а потом обход в глубину - что-то не сто с поиском в ширину, не ясно
  //   как разбить на слои.

  int arr[] = {1, 2, 3, 4, 5, 6, 7};
  vector<int> v(arr, arr + sizeof(arr)/sizeof(arr[0]));

  // раз отсортирован, значит корень - где-то в середине

  node* root = make_bst(v);
  cout << endl;

  vector<vector<int> > vv;
  split_into_layers(root, -1, vv);

  //cout << vv;

  //in_order_traversal(root);
  destroy_tree(root);
}

}

namespace leet_code {
using namespace crack_trees_and_gr;

// не оптимально, если дерево не сбалансировано
int find_min_depth(node* const root) 
{
  if (!root)
    return 0;

  // что-то не так с понятием глубины
  // 1+std::min(find_min_depth(root->left), find_min_depth(root->right));

  int l = find_min_depth(root->left);
  int r = find_min_depth(root->right);

  if (!l && !r)
    return 1;
            
  if (!l || !r)
    return std::max(l, r) + 1;

  return std::min(l, r) + 1;
}

TEST(LeetCode, MinDepthBT) {
  node* root = create_tree();
  cout << find_min_depth(root) << " < min\n";
}
}

namespace glassdoor_com {
// http://www.glassdoor.com/Interview/Find-the-minimum-depth-of-binary-search-tree-QTN_127018.htm

}
