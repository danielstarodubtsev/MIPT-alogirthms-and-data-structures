#include <algorithm>
#include <iostream>
#include <vector>

struct PersistentSegmentTree {
  int size = 1;

  std::vector<int> tree;
  std::vector<int> left;
  std::vector<int> right;
  std::vector<int> versions;

  void Create(int index, int left_bound, int right_bound,
              std::vector<int>& data) {
    if (right_bound - left_bound == 1 && left_bound < (int)data.size()) {
      tree[index] = data[left_bound];
      return;
    }
    if (right_bound - left_bound == 1) {
      return;
    }
    left[index] = 2 * index;
    right[index] = 2 * index + 1;
    Create(2 * index, left_bound, (left_bound + right_bound) / 2, data);
    Create(2 * index + 1, (left_bound + right_bound) / 2, right_bound, data);
  }

  int UpdatePoint(int index, int left_bound, int right_bound, int pos,
                  int new_value) {
    if (left_bound > pos || right_bound <= pos) {
      return index;
    }

    if (right_bound - left_bound == 1) {
      left.push_back(-1);
      right.push_back(-1);
      tree.push_back(new_value);
      return tree.size() - 1;
    }

    int middle = (left_bound + right_bound) / 2;
    int new_index = tree.size();
    left.push_back(-1);
    right.push_back(-1);
    tree.push_back(new_value);

    left[new_index] =
        UpdatePoint(left[index], left_bound, middle, pos, new_value);
    right[new_index] =
        UpdatePoint(right[index], middle, right_bound, pos, new_value);

    return new_index;
  }

  int GetValue(int index, int left_bound, int right_bound, int pos) {
    if (left_bound > pos || right_bound <= pos) {
      return -1;
    }

    if (right_bound - left_bound == 1) {
      return tree[index];
    }

    int middle = (left_bound + right_bound) / 2;
    int result = GetValue(left[index], left_bound, middle, pos);

    if (result != -1) {
      return result;
    }

    return GetValue(right[index], middle, right_bound, pos);
  }

  PersistentSegmentTree(std::vector<int>& data) {
    while (size < (int)data.size()) {
      size *= 2;
    }

    left.resize(2 * size, -1);
    right.resize(2 * size, -1);
    tree.resize(2 * size, 0);
    versions.push_back(1);
    Create(1, 0, size, data);
  }
};

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<int> data(cnt);
  for (int i = 0; i < cnt; i++) {
    std::cin >> data[i];
  }

  PersistentSegmentTree segment_tree{data};

  int qst;
  std::string command;
  std::cin >> qst;

  for (int i = 0; i < qst; i++) {
    std::cin >> command;

    if (command == "create") {
      int version;
      int index;
      int new_value;
      std::cin >> version >> index >> new_value;

      int new_start =
          segment_tree.UpdatePoint(segment_tree.versions[version - 1], 0,
                                   segment_tree.size, index - 1, new_value);
      segment_tree.versions.push_back(new_start);
    } else if (command == "get") {
      int version;
      int index;
      std::cin >> version >> index;
      std::cout << segment_tree.GetValue(segment_tree.versions[version - 1], 0,
                                         segment_tree.size, index - 1)
                << "\n";
    }
  }
}