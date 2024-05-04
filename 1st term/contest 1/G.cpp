#include <iostream>
#include <vector>

const size_t kByte = 256;
const size_t kBitsInByte = 8;

std::vector<std::pair<long long, int>> StableEnumerationSort(
    std::vector<std::pair<long long, int>> nums, size_t max_value) {
  std::vector<int> cnt(max_value + 1, 0);
  std::vector<std::pair<long long, int>> answer(nums.size());

  for (std::pair<long long, int> elem : nums) {
    cnt[elem.second]++;
  }

  for (size_t i = 1; i < max_value + 1; i++) {
    cnt[i] += cnt[i - 1];
  }

  for (int i = static_cast<int>(nums.size()) - 1; i >= 0; i--) {
    answer[cnt[nums[i].second] - 1] = nums[i];
    cnt[nums[i].second]--;
  }

  return answer;
}

std::vector<long long> ByteLSDSort(std::vector<long long> nums) {
  std::vector<std::pair<long long, int>> temp_result(nums.size());

  for (size_t byte_num = 0; byte_num <= kBitsInByte - 1; byte_num++) {
    for (size_t i = 0; i < nums.size(); i++) {
      if (byte_num == 0) {
        temp_result[i].first = nums[i];
      }
      temp_result[i].second =
          (temp_result[i].first >> (byte_num * kBitsInByte)) % kByte;
    }
    temp_result = StableEnumerationSort(temp_result, kByte + 1);
  }

  std::vector<long long> result(nums.size());
  for (size_t i = 0; i < nums.size(); i++) {
    result[i] = temp_result[i].first;
  }

  return result;
}

int main() {
  size_t cnt;
  std::cin >> cnt;

  std::vector<long long> nums(cnt);

  for (size_t i = 0; i < cnt; i++) {
    std::cin >> nums[i];
  }

  std::vector<long long> sorted_nums;
  sorted_nums = ByteLSDSort(nums);

  for (long long num : sorted_nums) {
    std::cout << num << "\n";
  }
}