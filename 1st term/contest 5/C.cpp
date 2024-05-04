#include <iostream>
#include <vector>

const long long kDefaultCapacity = 100;
const long long kFirstPrime = 100003;
const long long kSecondPrime = 100000007;
const int kRatio = 8;
const int kAlphabetSize = 26;

long long Hash(long long num) { return kFirstPrime * num + kSecondPrime; }

long long Hash(std::string str) {
  long long result = 0;

  for (char ch : str) {
    result = result * kFirstPrime + ch;
    result %= kSecondPrime;
  }

  return result;
}

std::string NormalizeWord(std::string word) {
  std::vector<char> substitutions(kAlphabetSize, '-');
  std::string result;
  char cur_char = 'a';

  for (char ch : word) {
    if (substitutions[ch - 'a'] == '-') {
      substitutions[ch - 'a'] = cur_char;
      ++cur_char;
    }
    result.push_back(substitutions[ch - 'a']);
  }

  return result;
}

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

  Set(size_t capacity) : capacity(capacity) { data.resize(capacity); }
  Set() : Set(kDefaultCapacity) {}

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

  size_t cnt;
  std::cin >> cnt;

  std::string command;
  std::string word;

  for (size_t i = 0; i < cnt; ++i) {
    std::cin >> command >> word;

    if (command == "?") {
      std::cout << set.Contains(Hash(NormalizeWord(word))) << "\n";
    } else if (command == "+") {
      set.Add(Hash(NormalizeWord(word)));
    }
  }
}