#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

std::vector<long long> Merge(std::vector<long long>& vector1,
                             std::vector<long long>& vector2) {
  long long index1 = 0;
  long long index2 = 0;
  std::vector<long long> result;

  while (index1 < (long long)vector1.size() ||
         index2 < (long long)vector2.size()) {
    if (index2 == (long long)vector2.size() ||
        (index1 < (long long)vector1.size() &&
         vector1[index1] < vector2[index2])) {
      result.push_back(vector1[index1]);
      index1++;
    } else {
      result.push_back(vector2[index2]);
      index2++;
    }
  }

  return result;
}

long long FindAnswer(long long cur_index,
                     std::pair<long long, long long> cur_bounds,
                     std::pair<long long, long long> bounds,
                     std::pair<long long, long long> min_max_values,
                     std::vector<std::vector<long long>>& segment_tree) {
  long long cur_left_bound = cur_bounds.first;
  long long cur_right_bound = cur_bounds.second;
  long long left = bounds.first;
  long long right = bounds.second;
  long long min_value = min_max_values.first;
  long long max_value = min_max_values.second;

  if (cur_left_bound > right || cur_right_bound < left) {
    return 0;
  }

  if (left <= cur_left_bound && right >= cur_right_bound) {
    return std::upper_bound(segment_tree[cur_index].begin(),
                            segment_tree[cur_index].end(), max_value) -
           std::lower_bound(segment_tree[cur_index].begin(),
                            segment_tree[cur_index].end(), min_value);
  }

  long long middle = (cur_left_bound + cur_right_bound) / 2;
  return FindAnswer(2 * cur_index + 1, {cur_left_bound, middle}, {left, right},
                    {min_value, max_value}, segment_tree) +
         FindAnswer(2 * cur_index + 2, {middle + 1, cur_right_bound},
                    {left, right}, {min_value, max_value}, segment_tree);
}

bool Compare(std::pair<long long, long long> first,
             std::pair<long long, long long> second) {
  return (first.first < second.first ||
          (first.first == second.first && first.second > second.second));
}

long long CountEqualShit(std::vector<std::pair<long long, long long>> data) {
  long long total_sum = 0;

  std::pair<long long, long long> current = data[0];
  long long current_streak = 1;
  for (size_t i = 1; i < data.size(); i++) {
    if (data[i] != current) {
      total_sum += current_streak * (current_streak - 1) / 2;
      current = data[i];
      current_streak = 1;
    } else {
      current_streak++;
    }
  }

  total_sum += (current_streak - 1) * current_streak / 2;

  return total_sum;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  const long long kMax = 1e9 + 7;

  long long cnt;
  std::cin >> cnt;

  long long fake_cnt = pow(2, ceil(log(cnt) / log(2)));

  std::vector<std::pair<long long, long long>> data(cnt);
  for (long long i = 0; i < cnt; i++) {
    std::cin >> data[i].first >> data[i].second;
  }

  std::sort(data.begin(), data.end(), Compare);
  cnt = data.size();

  std::vector<std::vector<long long>> segment_tree(2 * fake_cnt - 1,
                                                   std::vector<long long>(0));
  long long total_sum = CountEqualShit(data);

  for (long long i = fake_cnt - 1; i < fake_cnt - 1 + cnt; i++) {
    segment_tree[i].push_back(data[i - fake_cnt + 1].second);
  }

  for (long long i = fake_cnt - 1 + cnt; i < 2 * fake_cnt - 1; i++) {
    segment_tree[i].push_back(-1);
  }

  for (long long i = fake_cnt - 2; i >= 0; i--) {
    segment_tree[i] = Merge(segment_tree[2 * i + 1], segment_tree[2 * i + 2]);
  }

  long long answer = 0;

  for (long long i = 0; i < cnt; i++) {
    answer +=
        FindAnswer(0, {0, fake_cnt - 1}, {0, i},
                   {segment_tree[fake_cnt - 1 + i][0], kMax}, segment_tree) -
        1;
  }

  std::cout << answer - total_sum;
}