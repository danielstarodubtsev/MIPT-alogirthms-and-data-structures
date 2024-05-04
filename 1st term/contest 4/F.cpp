#include <iostream>
#include <random>
#include <string>

std::mt19937 rnd(0);

struct Node {
  const long long kBigNum = 1e8 + 179;
  long long value;
  double priority;
  long long subtree_sum = 0;
  Node* left_son = nullptr;
  Node* right_son = nullptr;

  void UpdateSum() {
    if (left_son == nullptr && right_son == nullptr) {
      subtree_sum = value;
    } else if (left_son == nullptr) {
      subtree_sum = right_son->subtree_sum + value;
    } else if (right_son == nullptr) {
      subtree_sum = left_son->subtree_sum + value;
    } else {
      subtree_sum = left_son->subtree_sum + right_son->subtree_sum + value;
    }
  }

  Node(long long value) : value(value), subtree_sum(value) {
    priority = rnd() % kBigNum;
  }
};

std::string Exists(Node* root, long long value) {
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
    first_root->UpdateSum();
    return first_root;
  }

  second_root->left_son = Merge(first_root, second_root->left_son);
  second_root->UpdateSum();
  return second_root;
}

std::pair<Node*, Node*> Split(Node* root, long long key) {
  if (root == nullptr) {
    return {nullptr, nullptr};
  }

  if (key >= root->value) {
    std::pair<Node*, Node*> new_pair = Split(root->right_son, key);
    root->right_son = new_pair.first;
    root->UpdateSum();
    return {root, new_pair.second};
  }

  std::pair<Node*, Node*> new_pair = Split(root->left_son, key);
  root->left_son = new_pair.second;
  root->UpdateSum();
  return {new_pair.first, root};
}

Node* Insert(Node* root, long long value, std::vector<Node*>& ever_allocated) {
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

long long GetSum(Node*& root, long long min_value, long long max_value) {
  if (max_value < min_value || root == nullptr) {
    return 0;
  }

  std::pair<Node*, Node*> split_by_min = Split(root, min_value - 1);
  std::pair<Node*, Node*> split_by_max = Split(split_by_min.second, max_value);

  if (split_by_max.first == nullptr) {
    root = Merge(split_by_min.first,
                 Merge(split_by_max.first, split_by_max.second));
    return 0;
  }

  long long result = split_by_max.first->subtree_sum;

  root =
      Merge(split_by_min.first, Merge(split_by_max.first, split_by_max.second));

  return result;
}

int main() {
  const long long kMod = 1e9;

  long long cnt;
  std::cin >> cnt;

  std::string command;
  long long value;
  long long min_value;
  long long max_value;

  Node* root = nullptr;
  std::vector<Node*> ever_allocated;

  std::string prev_command = "+";
  long long prev_result = 0;

  for (long long i = 0; i < cnt; i++) {
    std::cin >> command;

    if (command == "+") {
      std::cin >> value;

      if (prev_command == "+") {
        root = Insert(root, value, ever_allocated);
      } else {
        root = Insert(root, (value + prev_result) % kMod, ever_allocated);
      }
    } else if (command == "?") {
      std::cin >> min_value >> max_value;
      prev_result = GetSum(root, min_value, max_value);
      std::cout << prev_result << "\n";
    }

    prev_command = command;
  }

  for (Node* elem : ever_allocated) {
    delete elem;
  }
}