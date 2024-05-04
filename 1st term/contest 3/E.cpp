#include <iostream>
#include <string>
#include <vector>

struct Node {
  int left_bound = 0;
  int right_bound = 0;
  long long sum = 0;
  Node* left_son = nullptr;
  Node* right_son = nullptr;

  Node() {}
  Node(int left_bound, int right_bound)
      : left_bound(left_bound), right_bound(right_bound) {}
};

void Update(Node* node, int index, int delta) {
  node->sum += delta;

  if (node->left_bound == node->right_bound) {
    return;
  }

  int middle = (node->left_bound + node->right_bound) / 2;
  if (index <= middle) {
    if (node->left_son == nullptr) {
      Node* new_node = new Node{node->left_bound, middle};
      node->left_son = new_node;
    }
    Update(node->left_son, index, delta);
  } else {
    if (node->right_son == nullptr) {
      Node* new_node = new Node{middle + 1, node->right_bound};
      node->right_son = new_node;
    }
    Update(node->right_son, index, delta);
  }
}

long long GetSum(Node* node, long long left, long long right) {
  if (node->left_bound == left && node->right_bound == right) {
    return node->sum;
  }

  long long answer = 0;
  long long middle = (node->left_bound + node->right_bound) / 2;

  if (left <= middle && node->left_son != nullptr) {
    answer += GetSum(node->left_son, left, std::min(middle, right));
  }
  if (right > middle && node->right_son != nullptr) {
    answer += GetSum(node->right_son, std::max(left, middle + 1), right);
  }

  return answer;
}

void Destroy(Node* node) {
  if (node->left_son != nullptr) {
    Destroy(node->left_son);
  }
  if (node->right_son != nullptr) {
    Destroy(node->right_son);
  }
  delete node;
}

int main() {
  const int kMax = 1e9 - 1;
  int cnt;
  std::cin >> cnt;

  std::string command;
  int index;

  Node* root = new Node{0, kMax};

  for (int i = 0; i < cnt; i++) {
    std::cin >> command >> index;

    if (command == "+") {
      Update(root, index, index);
    } else if (command == "?") {
      std::cout << GetSum(root, 0, index) << "\n";
    }
  }
  Destroy(root);
}
