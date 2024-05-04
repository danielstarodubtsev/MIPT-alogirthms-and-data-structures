#include <iostream>
#include <vector>

std::vector<int> VectorSum(std::vector<int> vector1, std::vector<int> vector2) {
  for (int elem : vector2) {
    vector1.push_back(elem);
  }
  return vector1;
}

int Min(int num1, int num2) { return num1 < num2 ? num1 : num2; }

std::vector<int> Slice(std::vector<int> data, int start_index, int end_index) {
  std::vector<int> result(end_index - start_index);
  for (int i = start_index; i < end_index; i++) {
    result[i - start_index] = data[i];
  }
  return result;
}

std::pair<std::vector<int>, int> Merge(std::vector<int> vector1,
                                       std::vector<int> vector2) {
  int index1 = 0;
  int index2 = 0;
  std::vector<int> result;
  int inversions = 0;

  while (index1 < (int)vector1.size() || index2 < (int)vector2.size()) {
    if (index2 == (int)vector2.size() ||
        (index1 < (int)vector1.size() && vector1[index1] < vector2[index2])) {
      result.push_back(vector1[index1]);
      index1++;
    } else {
      result.push_back(vector2[index2]);
      index2++;
      inversions += (int)vector1.size() - index1;
    }
  }
  return {result, inversions};
}

std::pair<std::vector<int>, int> MergeSort(std::vector<int> data) {
  if (data.size() == 1) {
    return {data, 0};
  }

  auto [part1, inversions1] = MergeSort(Slice(data, 0, data.size() / 2));
  auto [part2, inversions2] =
      MergeSort(Slice(data, data.size() / 2, data.size()));

  auto [result, inversions3] = Merge(part1, part2);

  return {result, inversions1 + inversions2 + inversions3};
}

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<int> data(cnt);
  for (int i = 0; i < cnt; i++) {
    std::cin >> data[i];
  }

  auto [sorted_array, inversions] = MergeSort(data);

  std::cout << inversions;
}
