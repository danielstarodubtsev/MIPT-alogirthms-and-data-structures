#include <iostream>
#include <random>
#include <string>

std::mt19937 rnd(0);

struct Node {
  const int kBigNum = 1e8 + 1779;
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

int GetIndex(Node* node) { return GetSize(node->left_son) + 1; }

bool Exists(Node* root, int value) {
  if (root == nullptr) {
    return false;
  }

  if (root->value == value) {
    return true;
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

  if (key >= GetSize(root->left_son)) {
    std::pair<Node*, Node*> new_pair =
        Split(root->right_son, key - GetSize(root->left_son) - 2);
    root->right_son = new_pair.first;
    root->UpdateSize();
    return {root, new_pair.second};
  }

  std::pair<Node*, Node*> new_pair = Split(root->left_son, key);
  root->left_son = new_pair.second;
  root->UpdateSize();
  return {new_pair.first, root};
}

Node* Insert(Node* root, int index_key, int actual_value,
             std::vector<Node*>& ever_allocated) {
  Node* new_node = new Node{actual_value};
  ever_allocated.push_back(new_node);
  std::pair<Node*, Node*> new_nodes = Split(root, index_key);
  root = Merge(new_node, new_nodes.second);
  root = Merge(new_nodes.first, root);
  return root;
}

Node* Erase(Node* root, int index) {
  std::pair<Node*, Node*> new_nodes = Split(root, index - 1);
  std::pair<Node*, Node*> even_newer_nodes = Split(new_nodes.first, index - 2);
  return Merge(even_newer_nodes.first, new_nodes.second);
}

Node* Set(Node* root, int index, int new_value,
          std::vector<Node*>& ever_allocated) {
  root = Erase(root, index);
  root = Insert(root, index - 2, new_value, ever_allocated);
  return root;
}

int Get(Node* root, int left_bound, int right_bound, int min_value,
        int max_value) {
  if (root == nullptr) {
    return 0;
  }

  int answer = 0;

  if (root->value >= min_value && root->value <= max_value &&
      GetIndex(root) >= left_bound && GetIndex(root) <= right_bound) {
    // std::cout << "U";
    answer++;
  }

  if (right_bound < 0) {
    // std::cout << "p";
    return 0;
  }

  if (left_bound < 0 && right_bound <= GetIndex(root)) {
    return answer +
           Get(root->left_son, left_bound, right_bound, min_value, max_value);
  }

  if (left_bound >= GetIndex(root) && right_bound > GetSize(root) + 1) {
    return answer + Get(root->right_son, left_bound - GetIndex(root) - 1,
                        right_bound - GetIndex(root) - 1, min_value, max_value);
  }

  answer += Get(root->left_son, left_bound, right_bound, min_value, max_value);
  answer += Get(root->right_son, left_bound - GetIndex(root) - 1,
                right_bound - GetIndex(root) - 1, min_value, max_value);

  return answer;
}

/*
void Show(Node* node) {
  if (node != nullptr) {
    std::cout << "value:" << node->value << "\n";
    std::cout << "left_son\n";
    Show(node->left_son);
    std::cout << "right_son\n";
    Show(node->right_son);
  }
}
*/

int main() {
  int cnt;
  int qst;
  std::cin >> cnt >> qst;

  Node* root = nullptr;
  int next_num;
  std::vector<Node*> ever_allocated;

  for (int i = 0; i < cnt; i++) {
    std::cin >> next_num;
    root = Insert(root, i, next_num, ever_allocated);
  }

  // Show(root);

  std::string command;
  int index;
  int new_value;
  int min_value;
  int max_value;
  int left_bound;
  int right_bound;

  for (int i = 0; i < qst; i++) {
    std::cin >> command;

    if (command == "GET") {
      std::cin >> left_bound >> right_bound >> min_value >> max_value;
      std::cout << Get(root, left_bound - 1, right_bound - 1, min_value,
                       max_value)
                << "\n";
    } else if (command == "SET") {
      std::cin >> index >> new_value;
      root = Set(root, index - 1, new_value, ever_allocated);
      // Show(root);
    }
  }

  for (Node* elem : ever_allocated) {
    delete elem;
  }
}