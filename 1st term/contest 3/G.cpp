#include <iostream>
#include <string>
#include <vector>

void Add(int x_coord, int y_coord, int delta,
         std::vector<std::vector<int>>& fenwick_tree) {
  int size = fenwick_tree.size();

  for (int i = x_coord; i < size + 1; i = i | (i + 1)) {
    for (int j = y_coord; j < size + 1; j = j | (j + 1)) {
      fenwick_tree[i][j] += delta;
    }
  }
}

int GetSum(std::pair<int, int> x_coords, std::pair<int, int> y_coords,
           std::vector<std::vector<int>>& fenwick_tree) {
  int result = 0;

  int left_x = x_coords.first;
  int right_x = x_coords.second;
  int low_y = y_coords.first;
  int high_y = y_coords.second;

  for (int i = right_x; i >= 1; i = (i & (i + 1)) - 1) {
    for (int j = high_y; j >= 1; j = (j & (j + 1)) - 1) {
      result += fenwick_tree[i][j];
    }
  }
  for (int i = left_x - 1; i >= 1; i = (i & (i + 1)) - 1) {
    for (int j = high_y; j >= 1; j = (j & (j + 1)) - 1) {
      result -= fenwick_tree[i][j];
    }
  }
  for (int i = right_x; i >= 1; i = (i & (i + 1)) - 1) {
    for (int j = low_y - 1; j >= 1; j = (j & (j + 1)) - 1) {
      result -= fenwick_tree[i][j];
    }
  }
  for (int i = left_x - 1; i >= 1; i = (i & (i + 1)) - 1) {
    for (int j = low_y - 1; j >= 1; j = (j & (j + 1)) - 1) {
      result += fenwick_tree[i][j];
    }
  }

  return result;
}

int main() {
  const int kMaxSize = 1024;
  int cnt;
  int qst;
  std::cin >> cnt >> qst;

  std::vector<std::vector<int>> fenwick_tree(kMaxSize,
                                             std::vector<int>(kMaxSize, 0));
  std::string command;

  for (int i = 0; i < qst; i++) {
    std::cin >> command;

    if (command == "ADD") {
      int x_coord;
      int y_coord;
      std::cin >> x_coord >> y_coord;

      Add(x_coord, y_coord, 1, fenwick_tree);
    } else if (command == "GET") {
      int left_x;
      int right_x;
      int low_y;
      int high_y;
      std::cin >> left_x >> low_y >> right_x >> high_y;

      if (left_x > right_x) {
        std::swap(left_x, right_x);
      }
      if (low_y > high_y) {
        std::swap(low_y, high_y);
      }

      int result = GetSum({left_x, right_x}, {low_y, high_y}, fenwick_tree);

      std::cout << result << "\n";
    }
  }
}