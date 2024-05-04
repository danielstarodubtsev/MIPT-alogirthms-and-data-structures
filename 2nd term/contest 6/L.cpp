#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2e9 + 9;

int main() {
  int cnt;
  int number_of_groups;
  int max_overall_capacity;

  std::cin >> cnt >> number_of_groups >> max_overall_capacity;
  std::vector<std::vector<std::pair<int, int>>> data(
      number_of_groups, std::vector<std::pair<int, int>>());

  int cur_weigth;
  int cur_cost;
  int cur_group;

  for (int i = 0; i < cnt; ++i) {
    std::cin >> cur_weigth >> cur_cost >> cur_group;
    data[cur_group - 1].push_back({cur_weigth, cur_cost});
  }

  for (int i = 0; i < number_of_groups; ++i) {
    std::sort(data[i].begin(), data[i].end());
  }

  std::vector<std::vector<int>> dp(
      number_of_groups + 1, std::vector<int>(max_overall_capacity + 1, -kInf));

  for (int i = 0; i <= number_of_groups; ++i) {
    dp[i][0] = 0;
  }

  for (int groups_taken = 1; groups_taken <= number_of_groups; ++groups_taken) {
    for (int cur_weigth = 0; cur_weigth <= max_overall_capacity; ++cur_weigth) {
      dp[groups_taken][cur_weigth] = dp[groups_taken - 1][cur_weigth];

      for (std::pair<int, int> item : data[groups_taken - 1]) {
        if (cur_weigth < item.first) {
          break;
        }

        dp[groups_taken][cur_weigth] = std::max(
            dp[groups_taken][cur_weigth],
            dp[groups_taken - 1][cur_weigth - item.first] + item.second);
      }
    }
  }

  int max_res = 0;

  for (int answer_weigth = 0; answer_weigth <= max_overall_capacity;
       ++answer_weigth) {
    max_res = std::max(max_res, dp[number_of_groups][answer_weigth]);
  }

  std::cout << max_res << "\n";
}
