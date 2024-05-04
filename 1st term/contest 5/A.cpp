#include <iostream>
#include <vector>

const long long kDefaultCapacity = 4;
const long long kFirstPrime = 100003;
const long long kSecondPrime = 16777619;

long long Hash(long long num) { return kFirstPrime * num + kSecondPrime; }

struct Node {
  long long value = 0;
  Node* next = nullptr;

  Node() = default;
  Node(long long value) : value(value) {}
};

struct Set {
  size_t size = 0;
  size_t capacity = kDefaultCapacity;
  std::vector<Node*> data;

  void Rehash() {
    std::vector<Node*> new_data(capacity * 2);

    for (size_t i = 0; i < capacity; ++i) {
      Node* cur_node = data[i];

      while (cur_node != nullptr) {
        long long hash = Hash(cur_node->value) % (2 * capacity);
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
    capacity *= 2;
  }

  void Add(long long num) {
    if (4 * size > 3 * capacity) {
      Rehash();
    }

    long long hash = Hash(num) % capacity;
    Node* cur_node = data[hash];

    if (cur_node == nullptr) {
      data[hash] = new Node{num};
      ++size;
      return;
    }

    while (cur_node->next != nullptr) {
      if (cur_node->value == num) {
        return;
      }

      cur_node = cur_node->next;
    }

    if (cur_node->value == num) {
      return;
    }

    Node* new_node = new Node(num);
    cur_node->next = new_node;
    ++size;
  }

  std::string Contains(long long num) const {
    long long hash = Hash(num) % capacity;
    Node* cur_node = data[hash];

    while (cur_node != nullptr) {
      if (cur_node->value == num) {
        return "YES";
      }

      cur_node = cur_node->next;
    }

    return "NO";
  }

  void Erase(long long num) {
    long long hash = Hash(num) % capacity;
    Node* cur_node = data[hash];

    if (cur_node == nullptr) {
      return;
    }

    if (cur_node->value == num) {
      Node* new_next = cur_node->next;
      delete cur_node;
      data[hash] = new_next;
      --size;
      return;
    }

    while (cur_node->next != nullptr) {
      if (cur_node->next->value == num) {
        Node* new_next = cur_node->next->next;
        delete cur_node->next;
        cur_node->next = new_next;
        --size;
        return;
      }
      cur_node = cur_node->next;
    }
  }

  Set() { data.resize(kDefaultCapacity); }
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

  Set set;

  size_t qst;
  std::cin >> qst;
  std::string command;
  long long number;

  for (size_t i = 0; i < qst; ++i) {
    std::cin >> command >> number;

    if (command == "+") {
      set.Add(number);
    } else if (command == "-") {
      set.Erase(number);
    } else if (command == "?") {
      std::cout << set.Contains(number) << "\n";
    }
  }
}