#include <iostream>
#include <vector>

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<long long> data(cnt);
  for (int i = 0; i < cnt; i++) {
    std::cin >> data[i];
  }

  std::vector<long long> stack;
  std::vector<int> smaller_index_right(cnt, cnt);

  for (int i = cnt - 1; i >= 0; i--) {
    while (!stack.empty() && data[i] <= data[stack.back()]) {
      stack.pop_back();
    }
    if (!stack.empty()) {
      smaller_index_right[i] = stack.back();
    }
    stack.push_back(i);
  }

  stack.clear();
  std::vector<int> smaller_index_left(cnt, -1);

  for (int i = 0; i < cnt; i++) {
    while (!stack.empty() && data[i] <= data[stack.back()]) {
      stack.pop_back();
    }
    if (!stack.empty()) {
      smaller_index_left[i] = stack.back();
    }
    stack.push_back(i);
  }

  int left_obstacle;
  int right_obstacle;
  long long current_max = 0;
  long long current_result;
  for (int i = 0; i < cnt; i++) {
    left_obstacle = smaller_index_left[i];
    right_obstacle = smaller_index_right[i];

    current_result = data[i] * (right_obstacle - left_obstacle - 1);
    if (current_result > current_max) {
      current_max = current_result;
    }
  }

  std::cout << current_max;
}