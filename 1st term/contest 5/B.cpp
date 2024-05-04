#include <iostream>
#include <vector>

const long long kDefaultCapacity = 100;
const long long kFirstPrime = 100003;
const long long kSecondPrime = 16777619;
const int kRatio = 8;

long long Hash(long long num) { return kFirstPrime * num + kSecondPrime; }

struct Node {
  long long value = 0;
  Node* next = nullptr;

  Node() = default;
  Node(long long value) : value(value) {}
  Node(long long value, Node* next) : value(value), next(next) {}
};

struct Set {
  size_t size = 0;
  size_t capacity = kDefaultCapacity;
  std::vector<Node*> data;

  void Rehash() {
    std::vector<Node*> new_data(capacity * kRatio);

    for (size_t i = 0; i < capacity; ++i) {
      Node* cur_node = data[i];

      while (cur_node != nullptr) {
        long long hash = Hash(cur_node->value) % (kRatio * capacity);
        Node* node = new_data[hash];

        Node* new_node = new Node{cur_node->value};
        new_node->next = node;
        new_data[hash] = new_node;

        cur_node = cur_node->next;
      }
    }

    for (size_t i = 0; i < capacity; ++i) {
      Node* cur_node = data[i];

      while (cur_node != nullptr) {
        Node* next = cur_node->next;
        delete cur_node;
        cur_node = next;
      }
    }

    data = new_data;
    capacity *= kRatio;
  }

  void Add(long long num) {
    if (kRatio * size > 3 * capacity) {
      Rehash();
    }

    long long hash = Hash(num) % capacity;
    Node* new_node = new Node{num, data[hash]};
    data[hash] = new_node;
    ++size;
  }

  bool Erase(long long num) {
    long long hash = Hash(num) % capacity;
    Node* cur_node = data[hash];

    if (cur_node == nullptr) {
      return false;
    }

    if (cur_node->value == num) {
      Node* new_next = cur_node->next;
      delete cur_node;
      data[hash] = new_next;
      --size;
      return true;
    }

    while (cur_node->next != nullptr) {
      if (cur_node->next->value == num) {
        Node* new_next = cur_node->next->next;
        delete cur_node->next;
        cur_node->next = new_next;
        --size;
        return true;
      }
      cur_node = cur_node->next;
    }

    return false;
  }

  Set(size_t capacity) : capacity(capacity) { data.resize(capacity); }

  ~Set() {
    for (size_t i = 0; i < capacity; ++i) {
      Node* cur_node = data[i];

      while (cur_node != nullptr) {
        Node* next = cur_node->next;
        delete cur_node;
        cur_node = next;
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  size_t first_size;
  size_t second_size;
  int num;

  std::cin >> first_size;
  Set first(first_size);
  for (size_t i = 0; i < first_size; ++i) {
    std::cin >> num;
    first.Add(num);
  }

  std::vector<int> results;

  std::cin >> second_size;
  for (size_t i = 0; i < second_size; ++i) {
    std::cin >> num;
    if (first.Erase(num)) {
      results.push_back(num);
    }
  }

  std::cout << results.size() << "\n";

  for (int res : results) {
    std::cout << res << " ";
  }
}