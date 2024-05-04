#include <iostream>
#include <vector>

const long long kMod = 1e9 + 7;
const long long kMaxPossibleHeight = 16;

std::vector<std::vector<long long>> MakeMatrix(long long size) {
  std::vector<std::vector<long long>> result(size,
                                             std::vector<long long>(size, 0));
  result[0][0] = 1;

  for (long long i = 1; i < size; ++i) {
    result[i][i] = 1;
    result[i][i - 1] = 1;
    result[i - 1][i] = 1;
  }

  return result;
}

std::vector<std::vector<long long>> Product(
    std::vector<std::vector<long long>> matrix1,
    std::vector<std::vector<long long>> matrix2, long long mod) {
  std::vector<std::vector<long long>> answer(
      matrix1.size(), std::vector<long long>(matrix2[0].size(), 0));

  for (size_t i = 0; i < matrix1.size(); ++i) {
    for (size_t j = 0; j < matrix2[0].size(); ++j) {
      for (size_t k = 0; k < matrix2.size(); ++k) {
        answer[i][j] = (answer[i][j] + matrix1[i][k] * matrix2[k][j]) % mod;
      }
    }
  }

  return answer;
}

std::vector<std::vector<long long>> Power(
    std::vector<std::vector<long long>> matrix, long long pow, long long mod) {
  if (pow == 1) {
    return matrix;
  }

  if (pow % 2 == 1) {
    return Product(matrix, Power(matrix, pow - 1, mod), mod);
  }

  return Power(Product(matrix, matrix, mod), pow / 2, mod);
}

std::vector<std::vector<long long>> GetNumberOfWaysForOneCorridor(
    long long max_y, long long corridor_length, long long mod) {
  std::vector<std::vector<long long>> matrix = MakeMatrix(max_y + 1);
  matrix = Power(matrix, corridor_length, mod);
  return matrix;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  long long number_of_segments;
  long long finish_coord;
  std::cin >> number_of_segments >> finish_coord;

  std::vector<std::vector<long long>> dp(
      number_of_segments + 1,
      std::vector<long long>(kMaxPossibleHeight + 1, 0));
  dp[0][0] = 1;

  for (long long cur_segment_num = 1; cur_segment_num <= number_of_segments;
       ++cur_segment_num) {
    long long start_x;
    long long end_x;
    long long max_y;
    std::cin >> start_x >> end_x >> max_y;
    end_x = std::min(end_x, finish_coord);

    std::vector<std::vector<long long>> ways =
        GetNumberOfWaysForOneCorridor(max_y, end_x - start_x, kMod);

    for (long long end_height = 0; end_height <= max_y; ++end_height) {
      for (long long start_height = 0; start_height <= max_y; ++start_height) {
        dp[cur_segment_num][end_height] +=
            ways[start_height][end_height] *
            dp[cur_segment_num - 1][start_height];
        dp[cur_segment_num][end_height] %= kMod;
      }
    }
  }

  std::cout << dp[number_of_segments][0];
}