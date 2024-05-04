#include <iostream>
#include <vector>

const int kInf = 2e9 + 9;

std::vector<int> MakePrefSums(std::vector<int> data) {
  std::vector<int> pref_sums(data.size() + 1, 0);
  for (size_t i = 1; i <= data.size(); ++i) {
    pref_sums[i] = pref_sums[i - 1] + data[i - 1];
  }
  return pref_sums;
}

int FindDist(int index1, int index2, std::vector<int> data) {
  std::vector<int> sums = MakePrefSums(data);
  int index = (index1 + index2) / 2;
  int value = data[index];
  return -((index1 + index2) % 2) * value + sums[index2] + sums[index1] -
         2 * sums[index];
}

std::pair<int, int> Min(std::pair<int, int> first, std::pair<int, int> second) {
  if (first.first < second.first ||
      (first.first == second.first && first.second < second.second)) {
    return first;
  }
  return second;
}

std::pair<int, int> MinDistance(
    int index1, int index2,
    std::vector<std::vector<std::pair<int, int>>>& cache,
    std::vector<int>& data) {
  std::pair<int, int> answer{kInf, kInf};

  for (int k = index1 - 1; k < index2; ++k) {
    answer = Min(answer, {cache[index1 - 2][k - index1 + 1].first +
                              FindDist(k, index2, data),
                          k});
  }

  return answer;
}

std::vector<int> BestPoint(
    int index, int cnt, int max_num, std::vector<int> data,
    std::vector<std::vector<std::pair<int, int>>> cache) {
  std::vector<int> result;
  result.push_back(data[(index + cnt) / 2]);

  for (int i = max_num - 1; i >= 1; --i) {
    int index2 = cache[i - 1][index - i].second;
    result.push_back(data[(index2 + index) / 2]);
    index = index2;
  }

  return result;
}

std::pair<int, std::vector<int>> FindAnswer(std::vector<int> data,
                                            int max_num) {
  int cnt = data.size();

  if (max_num == 1) {
    std::vector<int> ans;
    ans.push_back(data[cnt / 2]);
    return {FindDist(0, cnt, data), ans};
  }

  std::vector<std::vector<std::pair<int, int>>> cache(
      2 * cnt, std::vector<std::pair<int, int>>(2 * cnt, {0, 0}));

  for (int j = 1; j < cnt - max_num + 2; ++j) {
    cache[0][j - 1] = {FindDist(0, j, data), 0};
  }

  for (int i = 2; i < max_num; ++i) {
    for (int j = i; j < cnt - max_num + i + 1; ++j) {
      cache[i - 1][j - i] = MinDistance(i, j, cache, data);
    }
  }

  std::pair<int, int> res;
  res = MinDistance(max_num, cnt, cache, data);

  return {res.first, BestPoint(res.second, cnt, max_num, data, cache)};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int houses_n;
  int points_n;
  std::cin >> houses_n >> points_n;

  std::vector<int> houses(houses_n);
  for (int i = 0; i < houses_n; ++i) {
    std::cin >> houses[i];
  }

  std::pair<int, std::vector<int>> result = FindAnswer(houses, points_n);
  std::cout << result.first << "\n";

  for (size_t i = result.second.size(); i >= 1; --i) {
    std::cout << result.second[i - 1] << " ";
  }
}