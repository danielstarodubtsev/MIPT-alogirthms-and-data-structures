#include <iostream>
#include <vector>

int GetBit(int mask, int bit) { return (mask >> bit) & 1; }

bool Check(int mask1, int mask2, int width) {
  for (int bit = 0; bit < width - 1; ++bit) {
    if (GetBit(mask1, bit) == GetBit(mask1, bit + 1) &&
        GetBit(mask1, bit) == GetBit(mask2, bit) &&
        GetBit(mask1, bit) == GetBit(mask2, bit + 1)) {
      return false;
    }
  }

  return true;
}

int main() {
  int width;
  int height;

  std::cin >> width >> height;
  if (width > height) {
    std::swap(width, height);
  }

  std::vector<std::vector<bool>> can_be_near(1 << width,
                                             std::vector<bool>(1 << width));
  for (int mask1 = 0; mask1 < 1 << width; ++mask1) {
    for (int mask2 = 0; mask2 < 1 << width; ++mask2) {
      can_be_near[mask1][mask2] = Check(mask1, mask2, width);
    }
  }

  std::vector<std::vector<int>> dp(height + 1, std::vector<int>(1 << width, 0));

  for (int first_mask = 0; first_mask < 1 << width; ++first_mask) {
    dp[0][first_mask] = 1;
  }

  for (int k = 1; k < height; ++k) {
    for (int i = 0; i < 1 << width; ++i) {
      for (int j = 0; j < 1 << width; ++j) {
        if (can_be_near[i][j]) {
          dp[k][i] += dp[k - 1][j];
        }
      }
    }
  }

  long long result = 0;
  for (int last_mask = 0; last_mask < 1 << width; ++last_mask) {
    result += dp[height - 1][last_mask];
  }

  std::cout << result;
}