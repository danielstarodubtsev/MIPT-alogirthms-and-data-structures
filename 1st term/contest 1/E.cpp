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

std::vector<int> Merge(std::vector<int> vector1, std::vector<int> vector2) {
  int index1 = 0;
  int index2 = 0;
  std::vector<int> result;

  while (index1 < (int)vector1.size() || index2 < (int)vector2.size()) {
    if (index2 == (int)vector2.size() ||
        (index1 < (int)vector1.size() && vector1[index1] < vector2[index2])) {
      result.push_back(vector1[index1]);
      index1++;
    } else {
      result.push_back(vector2[index2]);
      index2++;
    }
  }
  return result;
}

std::vector<int> MergeSort(std::vector<int> data) {
  if (data.size() == 1) {
    return data;
  }

  return Merge(MergeSort(Slice(data, 0, data.size() / 2)),
               MergeSort(Slice(data, data.size() / 2, data.size())));
}

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<int> starts(cnt);
  std::vector<int> ends(cnt);

  for (int i = 0; i < cnt; i++) {
    std::cin >> starts[i] >> ends[i];
  }

  starts = MergeSort(starts);
  ends = MergeSort(ends);

  int current_sum = 0;
  int index1 = 0;
  int index2 = 0;
  int elem;

  std::vector<int> result_starts;
  std::vector<int> result_ends;

  while (index1 < cnt || index2 < cnt) {
    if (index1 == cnt) {
      elem = ends[index2];
    } else if (index2 == cnt) {
      elem = starts[index1];
    } else {
      elem = Min(starts[index1], ends[index2]);
    }
    if (current_sum == 0) {
      result_starts.push_back(elem);
    }
    while (index1 < cnt && starts[index1] == elem) {
      index1++;
      current_sum++;
    }
    while (index2 < cnt && ends[index2] == elem) {
      index2++;
      current_sum--;
    }
    if (current_sum == 0) {
      result_ends.push_back(elem);
    }
  }
  std::cout << result_ends.size() << "\n";
  for (int i = 0; i < (int)result_ends.size(); i++) {
    std::cout << result_starts[i] << " " << result_ends[i] << "\n";
  }
}