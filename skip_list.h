#ifndef SKIP_LIST__SKIP_LIST_H_
#define SKIP_LIST__SKIP_LIST_H_
#include <cstring>
#include <cstdlib>
#include <random>

template<typename KeyT, typename ValueT>
class SkipList;

template<typename KeyT, typename ValueT>
class Node {
 public:
  friend class SkipList<KeyT, ValueT>;
  Node(size_t level) : next_(nullptr), key_(), value_(), level_(0) {}
  Node(KeyT key, ValueT value, size_t level) : key_(key), value_(value), level_(level) {
    next_ = new Node *[level_ + 1];

    for (int i = 0; i <= level; i++) {
      next_[i] = nullptr;
    }
  }
  ~Node() { delete next_; }

  KeyT GetKey() const { return key_; }
  ValueT GetValue() const { return value_; }
  [[nodiscard]] size_t GetLevel() const { return level_; }

 private:
  Node<KeyT, ValueT> **next_;
  KeyT key_;
  ValueT value_;
  size_t level_;
};

template<typename KeyT, typename ValueT>
class SkipList {
 public:
  SkipList() : header_(nullptr), curr_max_level_(0), size_(0) {
    header_ = new Node<KeyT, ValueT>(KeyT(), ValueT(), KMaxLevel);
  }
  ~SkipList() {
    delete header_;
  }
  SkipList(const SkipList &) = delete;
  SkipList &operator=(const SkipList &) = delete;

  [[nodiscard]] size_t Size() const { return size_; }
  bool Insert(KeyT, ValueT);
  bool Delete(KeyT);
  bool Get(KeyT, ValueT *);

  template<typename F>
  void Foreach(F);
 private:
  Node<KeyT, ValueT> *AllocNewNode(KeyT, ValueT, size_t);
  size_t Rand();

  const size_t KMaxLevel = 8;
  Node<KeyT, ValueT> *header_;
  size_t curr_max_level_;
  size_t size_;
};

template<typename KeyT, typename ValueT>
bool SkipList<KeyT, ValueT>::Get(KeyT key, ValueT *val) {
  auto curr = header_;
  for (int i = curr_max_level_; i >= 0; i--) {
    while (curr->next_[i] != nullptr && curr->next_[i]->key_ < key) {
      curr = curr->next_[i];
    }
  }

  curr = curr->next_[0];
  if (curr != nullptr && curr->key_ == key) {
    *val = curr->value_;
    return true;
  }
  return false;
}

template<typename KeyT, typename ValueT>
bool SkipList<KeyT, ValueT>::Delete(KeyT key) {
  auto curr = header_;
  Node<KeyT, ValueT> *updates[KMaxLevel];

  for (int i = curr_max_level_; i >= 0; i--) {
    while (curr->next_[i] != nullptr && curr->next_[i]->key_ < key) {
      curr = curr->next_[i];
    }
    updates[i] = curr;
  }

  curr = curr->next_[0];
  if (curr != nullptr && curr->key_ == key) {
    for (int i = 0; i <= curr->level_; i++) {
      updates[i]->next_[i] = curr->next_[i];
    }
    delete curr;
    return true;
  }
  return false;
}

template<typename KeyT, typename ValueT>
bool SkipList<KeyT, ValueT>::Insert(KeyT key, ValueT value) {
  auto curr = header_;
  Node<KeyT, ValueT> *updates[KMaxLevel];
  for (int i = curr_max_level_; i >= 0; i--) {
    while (curr->next_[i] != nullptr && curr->next_[i]->key_ < key) {
      curr = curr->next_[i];
    }
    updates[i] = curr;
  }

  curr = curr->next_[0];
  if (curr != nullptr && curr->key_ == key) {
    curr->value_ = value;
    return true;
  }

  // alloc a new node
  auto new_level = Rand();
  auto new_node = AllocNewNode(key, value, new_level);
  if (new_level > curr_max_level_) {
    for (size_t i = new_level; i > curr_max_level_; i--)
      updates[i] = header_;
    curr_max_level_ = new_level;
  }

  for (size_t i = 0; i <= new_level; i++) {
    new_node->next_[i] = updates[i]->next_[i];
    updates[i]->next_[i] = new_node;
  }
  size_++;
  return true;
}

template<typename KeyT, typename ValueT>
template<typename F>
void SkipList<KeyT, ValueT>::Foreach(F f) {
  auto curr = header_;
  while (curr != nullptr) {
    f(curr);
    curr = curr->next_[0];
  }
}

template<typename KeyT, typename ValueT>
Node<KeyT, ValueT> *SkipList<KeyT, ValueT>::AllocNewNode(KeyT key, ValueT value, size_t level) {
  return new Node<KeyT, ValueT>(key, value, level);
}

template<typename KeyT, typename ValueT>
size_t SkipList<KeyT, ValueT>::Rand() {
  size_t k = 1;
  // Seed with a real random value, if available
  std::random_device r;
  // Choose a random mean between 1 and 10
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(1, 10);
  while (uniform_dist(e1) % 2 == 0) {
    k++;
  }
  return k > KMaxLevel ? KMaxLevel : k;
}
#endif //SKIP_LIST__SKIP_LIST_H_
