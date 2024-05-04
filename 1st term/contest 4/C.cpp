#include <iostream>
#include <string>

const long long kMult = 321823;
const long long kMod = 962292269;

long long Hash(std::string str) {
  long long result = 0;

  for (char ch : str) {
    result = result * kMult + ch;
    result %= kMod;
  }

  return result;
}

struct Node {
  long long value = 0;
  std::string str;
  Node* left_son = nullptr;
  Node* right_son = nullptr;
  Node* parent = nullptr;
  int depth = 0;

  int Delta() const {
    if (left_son == nullptr && right_son == nullptr) {
      return 0;
    }

    if (left_son == nullptr) {
      return -right_son->depth;
    }

    if (right_son == nullptr) {
      return left_son->depth;
    }

    return left_son->depth - right_son->depth;
  }

  void SmallRotateLeft() {
    Node* temp = right_son;
    right_son = right_son->left_son;
    temp->left_son = this;
  }

  void SmallRotateRight() {
    Node* temp = left_son;
    left_son = left_son->right_son;
    temp->right_son = this;
  }

  void BigRotateLeft() {
    right_son->SmallRotateRight();
    SmallRotateLeft();
  }

  void BigRotateRight() {
    left_son->BigRotateLeft();
    SmallRotateRight();
  }

  Node(long long value) : value(value) {}
  Node(long long value, std::string str) : value(value), str(str) {}
  Node(long long value, Node* parent) : value(value), parent(parent) {}
  Node(long long value, std::string str, Node* parent)
      : value(value), str(str), parent(parent) {}

  ~Node() {
    delete left_son;
    delete right_son;
  }
};

struct SearchTreeAVL {
  Node* root = nullptr;

  void Add(long long value, std::string str, Node* cur_node) {
    if (cur_node == nullptr) {
      Node* new_node = new Node{value, str};
      root = new_node;
      return;
    }

    if (value < cur_node->value) {
      if (cur_node->left_son != nullptr) {
        Add(value, str, cur_node->left_son);
      } else {
        Node* new_node = new Node{value, str, cur_node};
        cur_node->left_son = new_node;
      }
    } else if (value > cur_node->value) {
      if (cur_node->right_son != nullptr) {
        Add(value, str, cur_node->right_son);
      } else {
        Node* new_node = new Node{value, str, cur_node};
        cur_node->right_son = new_node;
      }
    }

    Balance(cur_node);
  }

  std::string Find(long long value, Node* cur_node) {
    if (cur_node == nullptr) {
      return "";
    }

    if (value == cur_node->value) {
      return cur_node->str;
    }

    if (value < cur_node->value) {
      return Find(value, cur_node->left_son);
    }

    return Find(value, cur_node->right_son);
  }

  static void Balance(Node* cur_node) {
    if (cur_node->Delta() == 2) {
      if (cur_node->right_son->Delta() == 1) {
        cur_node->BigRotateRight();
      } else {
        cur_node->SmallRotateRight();
      }
    }

    if (cur_node->Delta() == -2) {
      if (cur_node->left_son->Delta() == -1) {
        cur_node->BigRotateLeft();
      } else {
        cur_node->SmallRotateLeft();
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  size_t cnt;
  std::cin >> cnt;

  SearchTreeAVL first_tree;
  SearchTreeAVL second_tree;

  std::string first_str;
  std::string second_str;

  long long first_hash;
  long long second_hash;

  for (size_t i = 0; i < cnt; ++i) {
    std::cin >> first_str >> second_str;

    first_hash = Hash(first_str);
    second_hash = Hash(second_str);

    first_tree.Add(first_hash, second_str, first_tree.root);
    second_tree.Add(second_hash, first_str, second_tree.root);
  }

  size_t qst;
  std::cin >> qst;

  std::string str;
  long long hash;

  std::string possible_res;

  for (size_t i = 0; i < qst; ++i) {
    std::cin >> str;
    hash = Hash(str);

    possible_res = first_tree.Find(hash, first_tree.root);

    if (!possible_res.empty()) {
      std::cout << possible_res << "\n";
    } else {
      std::cout << second_tree.Find(hash, second_tree.root) << "\n";
    }
  }

  delete first_tree.root;
  delete second_tree.root;
}