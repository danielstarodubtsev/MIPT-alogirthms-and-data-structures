#include <iostream>
#include <string>
#include <vector>

const int kMod = 1e9 + 7;

int GetBit(int mask, int bit) { return (mask >> bit) & 1; }

bool Matches(int mask, std::vector<std::string> grid, int column) {
  for (size_t i = 0; i < grid.size(); ++i) {
    if ((grid[i][column] == '+' && GetBit(mask, i) == 0) ||
        (grid[i][column] == '-' && GetBit(mask, i) == 1)) {
      return false;
    }
  }

  return true;
}

int BuildNextMask(int prev_mask, int first_bit, int height) {
  for (int cur_bit = 1; cur_bit < height; ++cur_bit) {
    int sum_bits = GetBit(prev_mask, cur_bit - 1) + GetBit(prev_mask, cur_bit) +
                   GetBit(first_bit, cur_bit - 1);
    if (sum_bits == 1) {
      first_bit = (1 << cur_bit) + first_bit;
    } else if (sum_bits != 2) {
      return -1;
    }
  }

  return first_bit;
}

int main() {
  unsigned int height;
  int width;
  std::cin >> height >> width;

  std::vector<std::string> grid(height);
  for (unsigned int i = 0; i < height; ++i) {
    std::cin >> grid[i];
  }

  std::vector<std::vector<int>> dp(width + 1, std::vector<int>(1 << height, 0));

  for (int first_mask = 0; first_mask < 1 << height; ++first_mask) {
    if (Matches(first_mask, grid, 0)) {
      dp[0][first_mask] = 1;
    }
  }

  for (int column = 1; column < width; ++column) {
    for (int prev_mask = 0; prev_mask < 1 << height; ++prev_mask) {
      for (int cur_mask_start = 0; cur_mask_start <= 1; ++cur_mask_start) {
        int cur_mask = BuildNextMask(prev_mask, cur_mask_start, height);
        if (cur_mask != -1 && Matches(cur_mask, grid, column)) {
          dp[column][cur_mask] += dp[column - 1][prev_mask];
          dp[column][cur_mask] %= kMod;
        }
      }
    }
  }

  int result = 0;
  for (int last_mask = 0; last_mask < 1 << height; ++last_mask) {
    result += dp[width - 1][last_mask];
    result %= kMod;
  }

  std::cout << result;
}