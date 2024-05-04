#include <iostream>
#include <random>
#include <string>

const int kBigNum = 1e8 + 179;
const int kInf = 1e9 + 179;

std::mt19937 rnd(0);

struct Node {
  int value;
  double priority;
  int subtree_size = 0;
  Node* left_son = nullptr;
  Node* right_son = nullptr;

  void UpdateSize() {
    if (left_son == nullptr && right_son == nullptr) {
      subtree_size = 0;
    } else if (left_son == nullptr) {
      subtree_size = right_son->subtree_size + 1;
    } else if (right_son == nullptr) {
      subtree_size = left_son->subtree_size + 1;
    } else {
      subtree_size = left_son->subtree_size + right_son->subtree_size + 2;
    }
  }

  Node(int value) : value(value) { priority = rnd() % kBigNum; }
};

int GetSize(Node* node) {
  if (node == nullptr) {
    return -1;
  }

  return node->subtree_size;
}

std::string Exists(Node* root, int value) {
  if (root == nullptr) {
    return "false";
  }

  if (root->value == value) {
    return "true";
  }

  if (root->value < value) {
    return Exists(root->right_son, value);
  }

  return Exists(root->left_son, value);
}

Node* Merge(Node* first_root, Node* second_root) {
  if (first_root == nullptr) {
    return second_root;
  }

  if (second_root == nullptr) {
    return first_root;
  }

  if (first_root->priority < second_root->priority) {
    first_root->right_son = Merge(first_root->right_son, second_root);
    first_root->UpdateSize();
    return first_root;
  }

  second_root->left_son = Merge(first_root, second_root->left_son);
  second_root->UpdateSize();
  return second_root;
}

std::pair<Node*, Node*> Split(Node* root, int key) {
  if (root == nullptr) {
    return {nullptr, nullptr};
  }

  if (key >= root->value) {
    std::pair<Node*, Node*> new_pair = Split(root->right_son, key);
    root->right_son = new_pair.first;
    root->UpdateSize();
    return {root, new_pair.second};
  }

  std::pair<Node*, Node*> new_pair = Split(root->left_son, key);
  root->left_son = new_pair.second;
  root->UpdateSize();
  return {new_pair.first, root};
}

Node* Insert(Node* root, int value, std::vector<Node*>& ever_allocated) {
  if (Exists(root, value) == "true") {
    return root;
  }

  Node* new_node = new Node{value};
  ever_allocated.push_back(new_node);
  std::pair<Node*, Node*> new_nodes = Split(root, value);
  root = Merge(new_node, new_nodes.second);
  root = Merge(new_nodes.first, root);
  return root;
}

Node* Erase(Node* root, int value) {
  std::pair<Node*, Node*> new_nodes = Split(root, value);
  std::pair<Node*, Node*> even_newer_nodes = Split(new_nodes.first, value - 1);
  return Merge(even_newer_nodes.first, new_nodes.second);
}

int Next(Node* root, int value) {
  if (root == nullptr) {
    return kInf;
  }

  int result;

  if (value < root->value) {
    if (root->left_son != nullptr) {
      result = Next(root->left_son, value);

      if (result != kInf) {
        return result;
      }

      return root->value;
    }

    return root->value;
  }

  if (root->right_son != nullptr) {
    return Next(root->right_son, value);
  }

  return kInf;
}

int Prev(Node* root, int value) {
  if (root == nullptr) {
    return kInf;
  }

  int result;

  if (value > root->value) {
    if (root->right_son != nullptr) {
      result = Prev(root->right_son, value);

      if (result != kInf) {
        return result;
      }

      return root->value;
    }

    return root->value;
  }

  if (root->left_son != nullptr) {
    return Prev(root->left_son, value);
  }

  return kInf;
}

int Kth(Node* root, int statistic) {
  if (GetSize(root->left_son) + 1 > statistic) {
    return Kth(root->left_son, statistic);
  }

  if (GetSize(root->left_son) + 1 == statistic) {
    return root->value;
  }

  if (root->right_son == nullptr) {
    return kInf;
  }

  return Kth(root->right_son, statistic - GetSize(root->left_son) - 2);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  Node* root = nullptr;
  std::string command;
  int cur_value;

  int result;

  std::vector<Node*> ever_allocated;

  while (std::cin >> command >> cur_value) {
    if (command == "insert") {
      root = Insert(root, cur_value, ever_allocated);
    } else if (command == "delete") {
      root = Erase(root, cur_value);
    } else if (command == "exists") {
      std::cout << Exists(root, cur_value) << "\n";
    } else if (command == "next") {
      result = Next(root, cur_value);

      if (result == kInf) {
        std::cout << "none\n";
      } else {
        std::cout << result << "\n";
      }
    } else if (command == "prev") {
      result = Prev(root, cur_value);

      if (result == kInf) {
        std::cout << "none\n";
      } else {
        std::cout << result << "\n";
      }
    } else if (command == "kth") {
      result = Kth(root, cur_value);

      if (result == kInf) {
        std::cout << "none\n";
      } else {
        std::cout << result << "\n";
      }
    }
  }

  for (Node* elem : ever_allocated) {
    delete elem;
  }
}