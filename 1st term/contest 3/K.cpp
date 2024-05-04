#include <algorithm>
#include <iostream>
#include <vector>

const size_t kInf = 1073741824;

struct Change {
  size_t x_coord = 0;
  size_t low_y = 0;
  size_t high_y = 0;
  int delta = 0;

  Change() {}
  Change(size_t x_coord, size_t low_y, size_t high_y, int delta)
      : x_coord(x_coord), low_y(low_y), high_y(high_y), delta(delta) {}
};

struct Node {
  size_t left_bound = 0;
  size_t right_bound = 0;
  size_t min = 0;
  size_t number_of_min = 1;
  size_t addition = 0;
  Node* left_son = nullptr;
  Node* right_son = nullptr;

  Node() {}
  Node(size_t left_bound, size_t right_bound)
      : left_bound(left_bound), right_bound(right_bound) {}
  Node(size_t left_bound, size_t right_bound, size_t min)
      : left_bound(left_bound), right_bound(right_bound), min(min) {}
  Node(size_t left_bound, size_t right_bound, size_t min, size_t number_of_min)
      : left_bound(left_bound),
        right_bound(right_bound),
        min(min),
        number_of_min(number_of_min) {}

  ~Node() {
    delete left_son;
    delete right_son;
  }
};

void Push(Node* node) {
  size_t length = node->right_bound - node->left_bound + 1;
  size_t middle = (node->right_bound + node->left_bound) / 2;

  if (length > 1) {
    if (node->left_son == nullptr) {
      Node* left_son = new Node{node->left_bound, middle, 0, length / 2};
      node->left_son = left_son;
    }

    if (node->right_son == nullptr) {
      Node* right_son = new Node{middle + 1, node->right_bound, 0, length / 2};
      node->right_son = right_son;
    }
  }

  if (node->left_son != nullptr) {
    node->left_son->addition += node->addition;
    node->left_son->min += node->addition;
  }

  if (node->right_son != nullptr) {
    node->right_son->addition += node->addition;
    node->right_son->min += node->addition;
  }

  node->addition = 0;
}

void Update(Node* node, size_t left_index, size_t right_index, size_t delta) {
  if (node->left_bound == left_index && node->right_bound == right_index) {
    node->addition += delta;
    node->min += delta;
    return;
  }

  Push(node);

  size_t middle = (node->left_bound + node->right_bound) / 2;

  if (left_index <= middle) {
    Update(node->left_son, left_index, std::min(middle, right_index), delta);
  }
  if (right_index > middle) {
    Update(node->right_son, std::max(middle + 1, left_index), right_index,
           delta);
  }

  size_t min1 = node->left_son->min;
  size_t number_of_min1 = node->left_son->number_of_min;
  size_t min2 = node->right_son->min;
  size_t number_of_min2 = node->right_son->number_of_min;

  if (min1 < min2) {
    node->min = min1;
    node->number_of_min = number_of_min1;
  } else if (min2 < min1) {
    node->min = min2;
    node->number_of_min = number_of_min2;
  } else {
    node->min = min1;
    node->number_of_min = number_of_min1 + number_of_min2;
  }
}

std::pair<size_t, size_t> GetNumberOfMin(Node* node, size_t left,
                                         size_t right) {
  const size_t kInf = 1073741824;
  Push(node);

  if (node->left_bound == left && node->right_bound == right) {
    return {node->min, node->number_of_min};
  }

  size_t middle = (node->left_bound + node->right_bound) / 2;

  size_t min = kInf;
  size_t min2 = kInf;
  size_t number_of_min = 1;
  size_t number_of_min2 = 1;

  if (left <= middle) {
    min = GetNumberOfMin(node->left_son, left, std::min(middle, right)).first;
    number_of_min =
        GetNumberOfMin(node->left_son, left, std::min(middle, right)).second;
  }
  if (right > middle) {
    min2 = GetNumberOfMin(node->right_son, std::max(left, middle + 1), right)
               .first;
    number_of_min2 =
        GetNumberOfMin(node->right_son, std::max(left, middle + 1), right)
            .second;
  }

  if (min < min2) {
    return {min, number_of_min};
  }
  if (min > min2) {
    return {min2, number_of_min2};
  }
  return {min, number_of_min + number_of_min2};
}

bool Compare(Change first_change, Change second_change) {
  return first_change.x_coord < second_change.x_coord;
}

int main() {
  size_t cnt;
  std::cin >> cnt;

  if (cnt == 0) {
    std::cout << 0;
    return 0;
  }

  std::vector<Change> changes(2 * cnt);

  size_t left_x;
  size_t right_x;
  size_t low_y;
  size_t high_y;

  for (size_t i = 0; i < cnt; ++i) {
    std::cin >> left_x >> low_y >> right_x >> high_y;

    Change open_change{left_x, low_y + 1, high_y + 1, 1};
    Change close_change{right_x, low_y + 1, high_y + 1, -1};

    changes[2 * i] = open_change;
    changes[2 * i + 1] = close_change;
  }

  std::sort(changes.begin(), changes.end(), Compare);

  Node* root = new Node{0, kInf - 1};

  size_t current_x = changes[0].x_coord;
  unsigned long long answer = 0;
  size_t next_x = 0;

  for (size_t i = 0; i < 2 * cnt; ++i) {
    size_t next_x = changes[i].x_coord;

    if (next_x == current_x) {
      Update(root, changes[i].low_y, changes[i].high_y - 1, changes[i].delta);
    } else {
      size_t number_of_zero = GetNumberOfMin(root, 0, kInf - 1).second;
      answer += (next_x - current_x) * (kInf - number_of_zero);
      current_x = next_x;
      Update(root, changes[i].low_y, changes[i].high_y - 1, changes[i].delta);
    }
  }
  size_t number_of_zero = GetNumberOfMin(root, 0, kInf - 1).second;
  answer += (next_x - current_x) * (kInf - number_of_zero);

  std::cout << answer;

  delete root;
}