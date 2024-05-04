#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2147483647;

bool Compare(int first, int second) { return first > second; }

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int cnt;
  std::cin >> cnt;

  std::vector<int> nums(cnt);
  for (int i = 0; i < cnt; ++i) {
    std::cin >> nums[i];
  }

  std::vector<int> dp(cnt + 1, -kInf);
  dp[0] = kInf;

  std::vector<int> pos(cnt + 1, -1);
  std::vector<int> prev(cnt, -1);
  int max_l = 0;

  for (int i = 0; i < cnt; ++i) {
    int found_index =
        std::lower_bound(dp.begin(), dp.end(), nums[i], Compare) - dp.begin();
    if (dp[found_index - 1] > nums[i] && nums[i] >= dp[found_index]) {
      dp[found_index] = nums[i];
      pos[found_index] = i;
      prev[i] = pos[found_index - 1];
      max_l = std::max(max_l, found_index);
    }
  }

  std::vector<int> ans;
  int cur_pos = pos[max_l];
  while (cur_pos != -1) {
    ans.push_back(cur_pos + 1);
    cur_pos = prev[cur_pos];
  }

  std::cout << max_l << "\n";

  for (size_t i = ans.size(); i >= 1; --i) {
    std::cout << ans[i - 1] << " ";
  }
}
