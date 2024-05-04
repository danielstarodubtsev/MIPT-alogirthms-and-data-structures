#include <iostream>
#include <vector>

long long Answer(long long cnt, long long min_num,
                 std::vector<std::vector<long long>>& calc) {
  long long cnt_copy = cnt;
  long long min_num_copy = min_num;

  if (cnt == 0 || cnt == min_num) {
    return 1;
  }

  if (min_num > cnt || cnt < 0) {
    return 0;
  }

  if (calc[cnt][min_num] != -1) {
    return calc[cnt][min_num];
  }

  long long ans = 0;

  while (min_num * 2 <= cnt - min_num) {
    ans += Answer(cnt - min_num, min_num * 2, calc);
    ++min_num;
  }

  calc[cnt_copy][min_num_copy] = ans + 1;
  return ans + 1;
}

int main() {
  long long cnt;
  std::cin >> cnt;

  std::vector<std::vector<long long>> calc(cnt + 1,
                                           std::vector<long long>(cnt + 1, -1));

  std::cout << Answer(cnt, 1, calc);
}