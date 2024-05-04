#include <iostream>
#include <vector>

int main() {
  int item_cnt;
  int capacity;
  std::cin >> item_cnt >> capacity;

  std::vector<int> masses(item_cnt);
  std::vector<int> costs(item_cnt);

  for (int i = 0; i < item_cnt; ++i) {
    std::cin >> masses[i];
  }

  for (int i = 0; i < item_cnt; ++i) {
    std::cin >> costs[i];
  }

  std::vector<std::vector<int>> dp(item_cnt + 1,
                                   std::vector<int>(capacity + 1, 0));

  for (int item_num = 1; item_num <= item_cnt; ++item_num) {
    for (int cur_weight = 1; cur_weight <= capacity; ++cur_weight) {
      if (cur_weight - masses[item_num - 1] < 0) {
        dp[item_num][cur_weight] = dp[item_num - 1][cur_weight];
      } else {
        dp[item_num][cur_weight] =
            std::max(dp[item_num - 1][cur_weight],
                     costs[item_num - 1] +
                         dp[item_num - 1][cur_weight - masses[item_num - 1]]);
      }
    }
  }

  int max_found = -1;
  int best_weight = -1;

  for (int i = 0; i <= capacity; ++i) {
    if (dp[item_cnt][i] > max_found) {
      max_found = dp[item_cnt][i];
      best_weight = i;
    }
  }

  std::vector<int> answer;

  while (dp[item_cnt][best_weight] != 0) {
    if (dp[item_cnt - 1][best_weight] != dp[item_cnt][best_weight]) {
      answer.push_back(item_cnt);
      best_weight -= masses[item_cnt - 1];
    }
    --item_cnt;
  }

  for (size_t i = answer.size(); i >= 1; --i) {
    std::cout << answer[i - 1] << " ";
  }
}