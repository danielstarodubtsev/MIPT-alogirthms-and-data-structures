#include <cmath>
#include <iostream>
#include <vector>

int FindAnswer(int cur_index, int target_index, int target_value,
               std::pair<int, int> cur_bounds, std::vector<int>& segment_tree) {
  int cur_left = cur_bounds.first;
  int cur_right = cur_bounds.second;
  if (cur_left == cur_right) {
    if (segment_tree[cur_index] >= target_value) {
      return cur_index;
    }
    return -1;
  }
  int middle = (cur_left + cur_right) / 2;
  int answer;
  if (target_index <= middle) {
    if (segment_tree[2 * cur_index + 1] < target_value) {
      answer = -1;
    } else {
      answer = FindAnswer(2 * cur_index + 1, target_index, target_value,
                          {cur_left, middle}, segment_tree);
    }

    if (answer != -1) {
      return answer;
    }
  }
  if (target_index <= cur_right) {
    if (segment_tree[2 * cur_index + 2] < target_value) {
      answer = -1;
    } else {
      answer = FindAnswer(2 * cur_index + 2, target_index, target_value,
                          {middle + 1, cur_right}, segment_tree);
    }
    return answer;
  }
  return -1;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int cnt;
  int qst;
  std::cin >> cnt >> qst;

  int fake_cnt = pow(2, ceil(log(cnt) / log(2)));

  std::vector<int> segment_tree(fake_cnt * 2 - 1, 0);
  for (int i = fake_cnt - 1; i < fake_cnt - 1 + cnt; i++) {
    std::cin >> segment_tree[i];
  }

  for (int i = fake_cnt - 2; i >= 0; i--) {
    segment_tree[i] =
        std::max(segment_tree[2 * i + 1], segment_tree[2 * i + 2]);
  }

  for (int i = 0; i < qst; i++) {
    int command;
    std::cin >> command;

    if (command == 0) {
      int index;
      int value;
      std::cin >> index >> value;
      index += fake_cnt - 2;
      segment_tree[index] = value;
      while (index > 0) {
        index = (index - 1) / 2;
        segment_tree[index] =
            std::max(segment_tree[2 * index + 1], segment_tree[2 * index + 2]);
      }
    } else if (command == 1) {
      int target_index;
      int target_value;
      std::cin >> target_index >> target_value;
      std::pair<int, int> cur_bounds{0, fake_cnt - 1};
      int answer = FindAnswer(0, target_index - 1, target_value, cur_bounds,
                              segment_tree);
      if (answer != -1) {
        answer -= fake_cnt - 2;
      }
      std::cout << answer << "\n";
    }
  }
}