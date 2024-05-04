#include <iostream>
#include <vector>

signed main() {
  int height;
  int num;
  std::cin >> height >> num;

  if (num != 0) {
    std::vector<std::vector<int>> dp(3, std::vector<int>(num + 1, 1));

    for (int i = 1; i <= height; ++i) {
      for (int j = 1; j <= num; ++j) {
        dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1];
      }
      if (dp[i][num] >= height) {
        std::cout << i;
        exit(0);
      }
      dp.resize(dp.size() + 1, std::vector<int>(num + 1, 1));
    }
  } else {
    std::cout << (height == 1 ? 0 : -1);
  }
}