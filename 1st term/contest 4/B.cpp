#include <iostream>
#include <string>

const int kMod = 1e9;

struct Node {
  int value = 0;
  Node* left_son = nullptr;
  Node* right_son = nullptr;
  Node* parent = nullptr;
  size_t depth = 0;

  int Delta() const {
    if (left_son == nullptr && right_son == nullptr) {
      return 0;
    }

    if (left_son == nullptr) {
      return -right_son->depth - 1;
    }

    if (right_son == nullptr) {
      return left_son->depth + 1;
    }

    return left_son->depth - right_son->depth;
  }

  void UpdateDepth() {
    if (left_son == nullptr && right_son == nullptr) {
      depth = 0;
    } else if (left_son == nullptr) {
      depth = right_son->depth + 1;
    } else if (right_son == nullptr) {
      depth = left_son->depth + 1;
    } else {
      depth = std::max(left_son->depth, right_son->depth) + 1;
    }
  }

  void UpdateBranch() {
    UpdateDepth();

    if (parent != nullptr) {
      parent->UpdateBranch();
    }
  }

  void SmallRotateLeft() {
    Node* temp = right_son;
    right_son = right_son->left_son;
    if (right_son != nullptr) {
      right_son->parent = this;
    }
    temp->left_son = this;
    temp->parent = parent;
    parent = temp;
    UpdateBranch();
  }

  void SmallRotateRight() {
    Node* temp = left_son;
    left_son = left_son->right_son;
    if (left_son != nullptr) {
      left_son->parent = this;
    }
    temp->right_son = this;
    temp->parent = parent;
    parent = temp;
    UpdateBranch();
  }

  void BigRotateLeft() {
    right_son->SmallRotateRight();
    SmallRotateLeft();
  }

  void BigRotateRight() {
    left_son->SmallRotateLeft();
    SmallRotateRight();
  }

  Node(int value) : value(value) {}
  Node(int value, Node* parent) : value(value), parent(parent) {}

  ~Node() {
    delete left_son;
    delete right_son;
  }
};

struct SearchTreeAVL {
  Node* root = nullptr;

  void Add(int value, Node* cur_node) {
    if (cur_node == nullptr) {
      Node* new_node = new Node{value};
      root = new_node;
      return;
    }

    if (value < cur_node->value) {
      if (cur_node->left_son != nullptr) {
        Add(value, cur_node->left_son);
      } else {
        Node* new_node = new Node{value, cur_node};
        cur_node->left_son = new_node;
      }
    } else if (value > cur_node->value) {
      if (cur_node->right_son != nullptr) {
        Add(value, cur_node->right_son);
      } else {
        Node* new_node = new Node{value, cur_node};
        cur_node->right_son = new_node;
      }
    }

    Node* cur = cur_node;
    cur->UpdateBranch();
    Balance(cur);
  }

  int Next(int value, Node* cur_node) {
    if (value == cur_node->value) {
      return value;
    }

    int result;

    if (value < cur_node->value) {
      if (cur_node->left_son != nullptr) {
        result = Next(value, cur_node->left_son);

        if (result != -1) {
          return result;
        }

        return cur_node->value;
      }

      return cur_node->value;
    }

    if (cur_node->right_son != nullptr) {
      return Next(value, cur_node->right_son);
    }

    return -1;
  }

  void Balance(Node* cur_node) {
    bool flag = (cur_node == root);

    if (cur_node->Delta() == 2) {
      if (cur_node->right_son != nullptr && cur_node->right_son->Delta() == 1) {
        if (flag) {
          root = cur_node->left_son->right_son;
        }
        cur_node->BigRotateRight();
      } else {
        if (flag) {
          root = cur_node->left_son;
        }
        cur_node->SmallRotateRight();
      }
    }

    if (cur_node->Delta() == -2) {
      if (cur_node->left_son != nullptr && cur_node->left_son->Delta() == -1) {
        if (flag) {
          root = cur_node->right_son->left_son;
        }
        cur_node->BigRotateLeft();
      } else {
        if (flag) {
          root = cur_node->right_son;
        }
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

  SearchTreeAVL tree;
  std::string command;
  int prev_res = 0;
  std::string prev_command = "+";
  int cur_value = 0;

  for (size_t i = 0; i < cnt; ++i) {
    std::cin >> command >> cur_value;

    if (command == "+") {
      if (prev_command == "+") {
        tree.Add(cur_value, tree.root);
      } else {
        tree.Add((cur_value + prev_res) % kMod, tree.root);
      }
      std::cout << "ok" <<std::endl;
    } else if (command == "?") {
      prev_res = tree.Next(cur_value, tree.root);
      std::cout << prev_res << std::endl;
    }

    prev_command = command;
  }

  delete tree.root;
}