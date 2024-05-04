#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> FindAnswer(int cnt, std::vector<int>& nums) {
  std::vector<int> answer(1, nums[0]);

  for (int i = 1; i < cnt; ++i) {
    if (answer.size() % 2 == 0) {
      if (answer.back() > nums[i]) {
        answer.push_back(nums[i]);
      } else {
        answer.back() = nums[i];
      }
    } else {
      if (answer.back() < nums[i]) {
        answer.push_back(nums[i]);
      } else {
        answer.back() = nums[i];
      }
    }
  }

  return answer;
}

int main() {
  int cnt;
  std::cin >> cnt;
  std::vector<int> nums(cnt);

  for (int i = 0; i < cnt; ++i) {
    std::cin >> nums[i];
  }

  std::vector<int> result1 = FindAnswer(cnt, nums);
  for (int i = 0; i < cnt; ++i) {
    nums[i] *= -1;
  }

  std::vector<int> result2 = FindAnswer(cnt, nums);

  std::cout << std::max(result1.size(), result2.size()) << "\n";

  if (result1.size() >= result2.size()) {
    for (int elem : result1) {
      std::cout << elem << " ";
    }
  } else {
    for (int elem : result2) {
      std::cout << -elem << " ";
    }
  }
}