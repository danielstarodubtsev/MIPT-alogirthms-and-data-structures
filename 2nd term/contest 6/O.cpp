#include <bitset>
#include <iostream>
#include <vector>

const int kInf = 2e9 + 9;
const int kBitsInInt = 32;

int CountOnes(int mask) {
  std::bitset<kBitsInInt> bits(mask);
  return bits.count();
}

std::vector<int> Remove(std::vector<int> vector, int elem) {
  std::vector<int> result;

  for (int cur : vector) {
    if (cur != elem) {
      result.push_back(cur);
    }
  }

  return result;
}

int GetBit(int mask, int bit) { return (mask >> bit) & 1; }

void MakeDp(int cnt, std::vector<std::vector<int>>& dp,
            std::vector<std::vector<int>>& matrix) {
  for (int mask = 0; mask < (1 << cnt); ++mask) {
    for (int last_vertex_index = 0; last_vertex_index < cnt;
         ++last_vertex_index) {
      if (CountOnes(mask) == 1 && GetBit(mask, last_vertex_index) == 1) {
        dp[mask][last_vertex_index] = 0;
      } else if (CountOnes(mask) > 1 && GetBit(mask, last_vertex_index) == 1) {
        for (int j = 0; j < cnt; ++j) {
          if (j == last_vertex_index) {
            continue;
          }

          dp[mask][last_vertex_index] =
              std::min(dp[mask][last_vertex_index],
                       dp[mask ^ (1 << last_vertex_index)][j] +
                           matrix[last_vertex_index][j]);
        }
      } else {
        dp[mask][last_vertex_index] = kInf;
      }
    }
  }
}

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<std::vector<int>> matrix(cnt, std::vector<int>(cnt));
  for (int i = 0; i < cnt; ++i) {
    for (int j = 0; j < cnt; ++j) {
      std::cin >> matrix[i][j];
    }
  }

  std::vector<std::vector<int>> dp(1 << cnt, std::vector<int>(cnt, kInf));
  MakeDp(cnt, dp, matrix);

  int min_distance = kInf;
  int last_vertex_index = 0;

  std::vector<int> answer;

  for (int i = 0; i < cnt; ++i) {
    if (dp[(1 << cnt) - 1][i] < min_distance) {
      min_distance = dp[(1 << cnt) - 1][i];
      last_vertex_index = i;
    }
  }

  std::cout << min_distance << "\n";

  std::vector<int> vertices_left(cnt);
  for (int i = 0; i < cnt; ++i) {
    vertices_left[i] = i;
  }

  int cur_mask = (1 << cnt) - 1;
  int prev = 0;
  while (!vertices_left.empty()) {
    answer.push_back(last_vertex_index);
    vertices_left = Remove(vertices_left, last_vertex_index);
    prev = last_vertex_index;

    for (int next : vertices_left) {
      if (dp[cur_mask][last_vertex_index] ==
          dp[cur_mask ^ (1 << last_vertex_index)][next] +
              matrix[last_vertex_index][next]) {
        last_vertex_index = next;
        break;
      }
    }
    cur_mask ^= (1 << prev);
  }

  for (size_t i = answer.size(); i >= 1; --i) {
    std::cout << answer[i - 1] + 1 << " ";
  }
}