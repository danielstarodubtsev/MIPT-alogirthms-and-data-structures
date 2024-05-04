#include <cmath>
#include <iostream>
#include <vector>

struct Quadro {
  int first_statistic;
  int second_statistic;
  size_t first_index = -1;
  size_t second_index = -1;

  Quadro() {}
  Quadro(int first_statistic, int second_statistic, size_t first_index,
         size_t second_index)
      : first_statistic(first_statistic),
        second_statistic(second_statistic),
        first_index(first_index),
        second_index(second_index) {}
};

Quadro MergeQuadros(Quadro& first, Quadro& second) {
  if (first.first_index != second.first_index) {
    if (first.first_statistic < second.first_statistic &&
        first.second_statistic < second.first_statistic) {
      return Quadro{first.first_statistic, first.second_statistic,
                    first.first_index, first.second_index};
    }
    if (first.first_statistic < second.first_statistic) {
      return Quadro{first.first_statistic, second.first_statistic,
                    first.first_index, second.first_index};
    }
    if (second.second_statistic < first.first_statistic) {
      return Quadro{second.first_statistic, second.second_statistic,
                    second.first_index, second.second_index};
    }
    return Quadro{second.first_statistic, first.first_statistic,
                  second.first_index, first.first_index};
  }
  if (first.second_index != second.second_index) {
    if (first.second_statistic < second.second_statistic) {
      return Quadro{first.first_statistic, first.second_statistic,
                    first.first_index, first.second_index};
    }
    return Quadro{second.first_statistic, second.second_statistic,
                  second.first_index, second.second_index};
  }
  return Quadro{first.first_statistic, first.second_statistic,
                first.first_index, first.second_index};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  const int kInf = 1e9 + 7;

  size_t cnt;
  size_t qst;
  std::cin >> cnt >> qst;

  size_t max_power = log2(cnt);

  std::vector<int> data(cnt);
  for (size_t i = 0; i < cnt; ++i) {
    std::cin >> data[i];
  }

  std::vector<std::vector<Quadro>> sparse(max_power + 1,
                                          std::vector<Quadro>(0));
  for (size_t i = 0; i <= cnt - 1; ++i) {
    Quadro current{data[i], kInf, i, i};
    sparse[0].push_back(current);
  }

  for (size_t cur_pow2 = 1; cur_pow2 <= max_power; ++cur_pow2) {
    for (size_t i = 0; i <= cnt - pow(2, cur_pow2); ++i) {
      sparse[cur_pow2].push_back(
          MergeQuadros(sparse[cur_pow2 - 1][i],
                       sparse[cur_pow2 - 1][i + pow(2, cur_pow2 - 1)]));
    }
  }

  int left;
  int right;
  int need_pow;
  int length;

  for (size_t i = 0; i < qst; ++i) {
    std::cin >> left >> right;
    need_pow = log2(right - left + 1);
    length = pow(2, need_pow);

    Quadro result = MergeQuadros(sparse[need_pow][left - 1],
                                 sparse[need_pow][right - length]);
    std::cout << result.second_statistic << "\n";
  }
}