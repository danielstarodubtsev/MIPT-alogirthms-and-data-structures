#include <iostream>
#include <vector>

const int kFirstMultiplier = 123;
const int kSecondMultiplier = 45;
const int kModulo = 1e7 + 4321;

std::pair<int, int> Partition(std::vector<int>& data, int pivot,
                              size_t start_index, size_t end_index) {
  int first_part_size = 0;
  int second_part_size = 0;

  for (size_t i = start_index; i <= end_index; i++) {
    if (data[i] < pivot) {
      std::swap(data[i], data[start_index + first_part_size]);
      first_part_size++;
    }
  }
  for (size_t i = start_index + first_part_size; i <= end_index; i++) {
    if (data[i] == pivot) {
      std::swap(data[i],
                data[start_index + first_part_size + second_part_size]);
      second_part_size++;
    }
  }

  return {first_part_size, second_part_size};
}

int QuickSelect(std::vector<int>& data, int statistic, size_t start_index,
                size_t end_index) {
  if (start_index == end_index) {
    return data[start_index];
  }

  int pivot;

  if (end_index - start_index + 1 == 0) {
    return 0;
  }

  pivot = data[rand() % (end_index - start_index + 1) + start_index];

  auto [first_group_size, second_group_size] =
      Partition(data, pivot, start_index, end_index);

  if (statistic < first_group_size) {
    return QuickSelect(data, statistic, start_index,
                       start_index + first_group_size - 1);
  }
  if (statistic < first_group_size + second_group_size) {
    return pivot;
  }
  return QuickSelect(data, statistic - first_group_size - second_group_size,
                     start_index + first_group_size + second_group_size,
                     end_index);
}

int main() {
  size_t cnt = 0;
  int statistic = 0;
  int first_elem = 0;
  int second_elem = 0;
  std::cin >> cnt >> statistic >> first_elem >> second_elem;

  std::vector<int> numbers(cnt);
  numbers[0] = first_elem;
  numbers[1] = second_elem;

  for (size_t i = 2; i < cnt; i++) {
    numbers[i] = (numbers[i - 1] * kFirstMultiplier +
                  numbers[i - 2] * kSecondMultiplier) %
                 kModulo;
  }

  int result;
  result = QuickSelect(numbers, statistic - 1, 0, cnt - 1);

  std::cout << result;
}
