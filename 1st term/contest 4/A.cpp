#include <iostream>
#include <vector>

struct Node {
  int value;
  double priority;
  int index;
  Node* parent = nullptr;
  Node* left_son = nullptr;
  Node* right_son = nullptr;

  int ParentIndex() const {
    if (parent != nullptr) {
      return parent->index;
    }

    return 0;
  }

  int LeftSonIndex() const {
    if (left_son != nullptr) {
      return left_son->index;
    }

    return 0;
  }

  int RightSonIndex() const {
    if (right_son != nullptr) {
      return right_son->index;
    }

    return 0;
  }

  Node(int value) : value(value) { priority = rand(); }
  Node(int value, double priority) : value(value), priority(priority) {}
  Node(int value, double priority, int index)
      : value(value), priority(priority), index(index) {}
  Node(int value, double priority, int index, Node* parent)
      : value(value), priority(priority), index(index), parent(parent) {}
};

struct Treap {
  Node* root = nullptr;

  void Build(std::vector<int>& nums, std::vector<double>& priorities,
             std::vector<Node*>& result_nodes) {
    root = new Node{nums[0], priorities[0], 1};
    result_nodes[1] = root;

    int cur_num;
    double cur_priority;

    Node* last_node = root;

    for (int i = 1; i < (int)nums.size(); i++) {
      cur_num = nums[i];
      cur_priority = priorities[i];

      if (cur_priority > last_node->priority) {
        Node* new_node = new Node{cur_num, cur_priority, i + 1, last_node};
        result_nodes[i + 1] = new_node;
        last_node->right_son = new_node;
        last_node = new_node;
      } else {
        while (last_node->parent != nullptr &&
               cur_priority < last_node->parent->priority) {
          last_node = last_node->parent;
        }

        if (last_node->parent == nullptr) {
          if (cur_priority < last_node->priority) {
            Node* new_node = new Node{cur_num, cur_priority, i + 1};
            result_nodes[i + 1] = new_node;
            new_node->left_son = last_node;
            last_node->parent = new_node;
            last_node = new_node;
            root = new_node;
          } else {
            Node* new_node = new Node{cur_num, cur_priority, i + 1, last_node};
            result_nodes[i + 1] = new_node;
            new_node->left_son = last_node->right_son;
            last_node->right_son = new_node;
            last_node = new_node;
          }
        } else {
          last_node = last_node->parent;
          Node* new_node = new Node{cur_num, cur_priority, i + 1, last_node};
          result_nodes[i + 1] = new_node;
          new_node->left_son = last_node->right_son;
          last_node->right_son = new_node;
          last_node = new_node;
          new_node->left_son->parent = new_node;
        }
      }
    }
  }
};

void Destroy(Node* node) {
  if (node == nullptr) {
    return;
  }

  Destroy(node->left_son);
  Destroy(node->right_son);

  delete node;
}

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<int> nums(cnt);
  std::vector<double> priorities(cnt);
  Treap treap;

  std::vector<Node*> result_nodes(cnt + 1);

  for (int i = 0; i < cnt; i++) {
    std::cin >> nums[i] >> priorities[i];
  }

  treap.Build(nums, priorities, result_nodes);
  std::cout << "YES\n";

  for (int i = 1; i <= cnt; i++) {
    std::cout << result_nodes[i]->ParentIndex() << " "
              << result_nodes[i]->LeftSonIndex() << " "
              << result_nodes[i]->RightSonIndex() << "\n";
  }

  Destroy(treap.root);
}