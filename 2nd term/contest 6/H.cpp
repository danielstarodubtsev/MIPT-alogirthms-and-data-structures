#include <iostream>
#include <string>
#include <vector>

int main() {
  std::string first;
  std::string second;
  std::cin >> first >> second;

  size_t len1 = first.size();
  size_t len2 = second.size();

  std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1, 0));

  for (size_t i = 1; i <= len1; ++i) {
    for (size_t j = 1; j <= len2; ++j) {
      if (first[i - 1] == second[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }

  int answer = dp[len1][len2];
  std::cout << answer << "\n";

  std::vector<size_t> ans1(answer);
  std::vector<size_t> ans2(answer);

  int index1 = len1 - 1;
  int index2 = len2 - 1;

  int need = answer - 1;

  while (index1 >= 0 && index2 >= 0 && need >= 0) {
    if (first[index1] == second[index2]) {
      ans1[need] = index1 + 1;
      ans2[need] = index2 + 1;
      --need;
      --index1;
      --index2;
    } else {
      if (dp[index1][index2 + 1] > dp[index1 + 1][index2]) {
        --index1;
      } else {
        --index2;
      }
    }
  }

  for (size_t cur : ans1) {
    std::cout << cur << " ";
  }

  std::cout << "\n";

  for (size_t cur : ans2) {
    std::cout << cur << " ";
  }
}