#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

std::vector<int> Merge(std::vector<int>& vector1, std::vector<int>& vector2) {
  size_t index1 = 0;
  size_t index2 = 0;
  std::vector<int> result;

  while (index1 < vector1.size() || index2 < vector2.size()) {
    if (index2 == vector2.size() ||
        (index1 < vector1.size() && vector1[index1] < vector2[index2])) {
      result.push_back(vector1[index1]);
      index1++;
    } else {
      result.push_back(vector2[index2]);
      index2++;
    }
  }

  return result;
}

int FindAnswer(int cur_index, std::pair<int, int> cur_bounds,
               std::pair<int, int> bounds, std::pair<int, int> min_max_values,
               std::vector<std::vector<int>>& segment_tree) {
  int cur_left_bound = cur_bounds.first;
  int cur_right_bound = cur_bounds.second;
  int left = bounds.first;
  int right = bounds.second;
  int min_value = min_max_values.first;
  int max_value = min_max_values.second;

  if (cur_left_bound > right || cur_right_bound < left) {
    return 0;
  }

  if (left <= cur_left_bound && right >= cur_right_bound) {
    return std::upper_bound(segment_tree[cur_index].begin(),
                            segment_tree[cur_index].end(), max_value) -
           std::lower_bound(segment_tree[cur_index].begin(),
                            segment_tree[cur_index].end(), min_value);
  }

  int middle = (cur_left_bound + cur_right_bound) / 2;
  return FindAnswer(2 * cur_index + 1, {cur_left_bound, middle}, {left, right},
                    {min_value, max_value}, segment_tree) +
         FindAnswer(2 * cur_index + 2, {middle + 1, cur_right_bound},
                    {left, right}, {min_value, max_value}, segment_tree);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  size_t cnt;
  size_t qst;

  std::cin >> cnt >> qst;

  size_t fake_cnt = pow(2, ceil(log(cnt) / log(2)));

  std::vector<std::vector<int>> segment_tree(2 * fake_cnt - 1,
                                             std::vector<int>(0));
  int num;

  for (size_t i = fake_cnt - 1; i < fake_cnt - 1 + cnt; i++) {
    std::cin >> num;
    segment_tree[i].push_back(num);
  }

  for (size_t i = fake_cnt - 1 + cnt; i < 2 * fake_cnt - 1; i++) {
    segment_tree[i].push_back(cnt + 1);
  }

  for (int i = fake_cnt - 2; i >= 0; i--) {
    segment_tree[i] = Merge(segment_tree[2 * i + 1], segment_tree[2 * i + 2]);
  }

  int left;
  int right;
  int min_value;
  int max_value;

  for (size_t i = 0; i < qst; i++) {
    std::cin >> left >> right >> min_value >> max_value;
    std::cout << FindAnswer(0, {0, fake_cnt - 1}, {left - 1, right - 1},
                            {min_value, max_value}, segment_tree)
              << "\n";
  }
}