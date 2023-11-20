#include <iostream>
#include "assert.h"

#include "skip_list.h"

void check_node(Node<int, int> *node) {
  std::cout << node->GetKey() << " "
            << node->GetValue() << " "
            << node->GetLevel() << '\n';
}

int main() {
  SkipList<int, int> list;
  for (int i = 0; i <= 10; i++) {
    if (i % 2 == 0)
      list.Insert(i, i);
  }
  list.Foreach(check_node);
  std::cout << "------------------------------" << std::endl;
  for (int i = 0; i <= 10; i++) {
    if (i % 2 != 0)
      list.Insert(i, i);
  }
  list.Foreach(check_node);

  int val = 0;
  for (int i = 0; i <= 10; i++) {
    if (i % 2 != 0) {
      list.Get(i, &val);
      assert(val == i);
    }
  }
  std::cout << "------------------------------" << std::endl;
  for (int i = 0; i <= 10; i++) {
    if (i % 2 != 0)
      list.Delete(i);
  }
  list.Foreach(check_node);
  return 0;
}
