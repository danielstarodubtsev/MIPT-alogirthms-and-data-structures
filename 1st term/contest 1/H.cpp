#include <iostream>
#include <vector>

std::pair<int, int> Partition(std::vector<int>& data, int pivot,
                              size_t start_index, size_t end_index) {
  size_t first_part_size = 0;
  size_t second_part_size = 0;

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

void Quicksort(std::vector<int>& data, size_t start_index, size_t end_index) {
  if (end_index <= start_index) {
    return;
  }

  int pivot;
  pivot = QuickSelect(data, (end_index - start_index + 1) / 2, start_index,
                      end_index);

  auto [first_group_size, second_group_size] =
      Partition(data, pivot, start_index, end_index);

  Quicksort(data, start_index, start_index + first_group_size - 1);
  Quicksort(data, start_index + first_group_size + second_group_size,
            end_index);
}

int main() {
  size_t cnt;
  std::cin >> cnt;
  std::vector<int> data(cnt);
  for (size_t i = 0; i < cnt; i++) {
    std::cin >> data[i];
  }

  Quicksort(data, 0, cnt - 1);

  for (size_t i = 0; i + 1 < cnt; i++) {
    std::cout << data[i] << " ";
  }
  std::cout << data[cnt - 1];
}
