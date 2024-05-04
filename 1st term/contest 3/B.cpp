#include <cmath>
#include <iostream>
#include <vector>

void UpdatePoint(long long index, long long delta,
                 std::vector<long long>& segment_tree) {
  do {
    segment_tree[index] += delta;
    index = (index - 1) / 2;
  } while (index != 0);
  segment_tree[0] += delta;
}

long long GetSum(long long index, std::pair<long long, long long> cur_bounds,
                 std::pair<long long, long long> bounds,
                 std::vector<long long>& segment_tree) {
  long long left = bounds.first;
  long long right = bounds.second;
  long long cur_left = cur_bounds.first;
  long long cur_right = cur_bounds.second;
  if (cur_left == left && cur_right == right) {
    return segment_tree[index];
  }
  long long answer = 0;
  long long middle = (cur_left + cur_right) / 2;
  if (left <= middle) {
    answer += GetSum(2 * index + 1, {cur_left, middle},
                     {left, std::min(right, middle)}, segment_tree);
  }
  if (right > middle) {
    answer += GetSum(2 * index + 2, {middle + 1, cur_right},
                     {std::max(left, middle + 1), right}, segment_tree);
  }
  return answer;
}

int main() {
  long long cnt;
  std::cin >> cnt;

  long long fake_cnt = pow(2, ceil(log(cnt) / log(2)));

  std::vector<long long> segment_tree(fake_cnt * 2 - 1, 0);
  for (long long i = fake_cnt - 1; i < fake_cnt - 1 + cnt; i++) {
    std::cin >> segment_tree[i];
    if (i % 2 != 0) {
      segment_tree[i] *= -1;
    }
  }

  for (long long i = fake_cnt - 2; i >= 0; i--) {
    segment_tree[i] = segment_tree[2 * i + 1] + segment_tree[2 * i + 2];
  }

  long long qst;
  std::cin >> qst;

  for (long long i = 0; i < qst; i++) {
    long long command;
    std::cin >> command;

    if (command == 0) {
      // update in point
      long long index;
      long long delta;
      std::cin >> index >> delta;

      long long mult = 1;
      if (index % 2 == 1) {
        mult = -1;
      }

      index += fake_cnt - 2;
      delta = mult * delta - segment_tree[index];

      UpdatePoint(index, delta, segment_tree);
    } else {
      // getSum
      long long left;
      long long right;
      std::cin >> left >> right;

      long long result =
          GetSum(0, {0, fake_cnt - 1}, {left - 1, right - 1}, segment_tree);
      if (left % 2 == 1) {
        result *= -1;
      }
      std::cout << result << "\n";
    }
  }
}