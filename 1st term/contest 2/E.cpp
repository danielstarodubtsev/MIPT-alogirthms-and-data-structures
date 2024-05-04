#include <iostream>
#include <vector>

std::vector<std::pair<int, int>> Slice(std::vector<std::pair<int, int>> data,
                                       int start_index, int end_index) {
  std::vector<std::pair<int, int>> result(end_index - start_index);
  for (int i = start_index; i < end_index; i++) {
    result[i - start_index] = data[i];
  }
  return result;
}

std::vector<std::pair<int, int>> Merge(std::vector<std::pair<int, int>> vector1,
                                       std::vector<std::pair<int, int>> vector2,
                                       std::vector<int>& result) {
  int index1 = 0;
  int index2 = 0;
  std::vector<std::pair<int, int>> local_result;

  while (index1 < (int)vector1.size() || index2 < (int)vector2.size()) {
    if (index2 == (int)vector2.size() ||
        (index1 < (int)vector1.size() &&
         vector1[index1].first <= vector2[index2].first)) {
      local_result.push_back(vector1[index1]);
      result[vector1[index1].second] += index2;
      index1++;
    } else {
      local_result.push_back(vector2[index2]);
      index2++;
    }
  }
  return local_result;
}

std::vector<std::pair<int, int>> MergeSort(
    std::vector<std::pair<int, int>> data, std::vector<int>& result) {
  if (data.size() == 1) {
    return data;
  }

  return Merge(MergeSort(Slice(data, 0, data.size() / 2), result),
               MergeSort(Slice(data, data.size() / 2, data.size()), result),
               result);
}

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<int> data(cnt);
  for (int i = 0; i < cnt; i++) {
    std::cin >> data[i];
  }

  std::vector<std::pair<int, int>> new_data(cnt);
  for (int i = 0; i < cnt; i++) {
    new_data[i] = {data[i], i};
  }

  std::vector<int> result(cnt, 0);
  MergeSort(new_data, result);

  for (int elem : result) {
    std::cout << elem << " ";
  }
}