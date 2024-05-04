#include <algorithm>
#include <iostream>
#include <vector>

void PrintAnswer(int max_l_overall, std::vector<int>& best_ever_ans,
                 int best_shift, int cnt) {
  std::cout << max_l_overall << "\n";
  for (size_t i = best_ever_ans.size();
       i >= best_ever_ans.size() - max_l_overall + 1; --i) {
    int res = (best_ever_ans[i - 1] + best_shift + 1) % cnt;
    if (res == 0) {
      res = cnt;
    }
    if (res > cnt / 2) {
  //    res -= cnt / 2;
    }
    std::cout << res << " ";
  }
}

std::vector<std::vector<bool>> IsDivisible(std::vector<long long>& nums) {
  size_t cnt = nums.size();
  std::vector<std::vector<bool>> result(cnt, std::vector<bool>(cnt, false));

  for (size_t i = 0; i < cnt; ++i) {
    for (size_t j = 0; j < cnt; ++j) {
      if (nums[i] % nums[j] == 0 && nums[i] != nums[j]) {
        result[i][j] = true;
      }
    }
  }

  return result;
}

int SolveWithoutAns(int cnt, std::vector<std::vector<bool>>& is_div,
                    int shift) {
  std::vector<int> dp(cnt, 1);
  int max_l = 1;
  for (int i = 0; i < cnt; ++i) {
    for (int found_index = 0; found_index < i; ++found_index) {
      if (is_div[(i + shift) % cnt][(found_index + shift) % cnt] &&
          dp[found_index] >= dp[i]) {
        dp[i] = dp[found_index] + 1;
      }
    }
    if (dp[i] > max_l) {
      max_l = dp[i];
    }
  }
  return max_l;
}

std::vector<int> SolveForAns(int cnt, std::vector<std::vector<bool>>& is_div,
                             int shift) {
  std::vector<int> dp(cnt, 1);
  std::vector<int> prev(cnt, -1);
  int pos = 0;
  int max_l = 1;
  for (int i = 0; i < cnt; ++i) {
    for (int found_index = 0; found_index < i; ++found_index) {
      if (is_div[(i + shift) % cnt][(found_index + shift) % cnt] &&
          dp[found_index] >= dp[i]) {
        dp[i] = dp[found_index] + 1;
        prev[i] = found_index;
      }
    }
    if (dp[i] > max_l) {
      pos = i;
      max_l = dp[i];
    }
  }

  std::vector<int> ans;
  while (pos != -1) {
    ans.push_back(pos);
    pos = prev[pos];
  }

  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int cnt;
  std::cin >> cnt;

  std::vector<long long> nums(2 * cnt);
  for (int i = 0; i < cnt; ++i) {
    std::cin >> nums[i];
    nums[cnt + i] = nums[i];
  }
  cnt *= 2;
  std::vector<std::vector<bool>> is_div = IsDivisible(nums);
  int max_l_overall = -1;
  std::vector<int> best_ever_ans;
  int best_shift = 0;

  for (int shift = 0; shift < 1; ++shift) {
    int cur_l = SolveWithoutAns(cnt, is_div, shift);
    if (cur_l > max_l_overall) {
      max_l_overall = cur_l;
      best_shift = shift;
    }
  }

  best_ever_ans = SolveForAns(cnt, is_div, best_shift);

  PrintAnswer(max_l_overall, best_ever_ans, best_shift, cnt);
}
