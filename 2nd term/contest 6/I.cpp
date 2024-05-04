#include <iostream>
#include <vector>

int main() {
  long long cnt1;
  long long cnt2;
  std::cin >> cnt1 >> cnt2;

  std::vector<long long> nums1(cnt1);
  std::vector<long long> nums2(cnt2);

  for (long long i = 0; i < cnt1; ++i) {
    std::cin >> nums1[i];
    ++nums1[i];
  }

  for (long long i = 0; i < cnt2; ++i) {
    std::cin >> nums2[i];
    ++nums2[i];
  }

  std::vector<std::vector<long long>> dp(cnt1 + 1,
                                         std::vector<long long>(cnt2 + 1, 0));
  std::vector<long long> prev(cnt2 + 1, -1);

  for (long long i = 1; i <= cnt1; ++i) {
    long long index = 0;
    long long best = 0;

    for (long long j = 1; j <= cnt2; ++j) {
      dp[i][j] = dp[i - 1][j];
      if (nums1[i - 1] == nums2[j - 1] && dp[i - 1][j] < best + 1) {
        dp[i][j] = best + 1;
        prev[j] = index;
      }
      if (nums1[i - 1] > nums2[j - 1] && dp[i - 1][j] > best) {
        best = dp[i - 1][j];
        index = j;
      }
    }
  }
  long long answer = 0;
  for (int i = 0; i <= cnt1; ++i) {
    for (int j = 0; j <= cnt2; ++j) {
      answer = std::max(answer, dp[i][j]);
    }
  }

  std::cout << answer;
}
