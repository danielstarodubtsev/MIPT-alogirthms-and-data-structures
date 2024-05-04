#include <iostream>
#include <string>
#include <vector>

int GetBit(int mask, int bit) { return (mask >> bit) & 1; }

int RemoveBits(int mask, int bit1, int bit2) {
  return (mask ^ (1 << bit1)) ^ (1 << bit2);
}

int main() {
  int cnt;
  std::cin >> cnt;

  std::vector<std::vector<int>> matrix(cnt, std::vector<int>(cnt, 0));

  for (int i = 0; i < cnt; ++i) {
    std::string str;
    std::cin >> str;

    for (int j = 0; j < cnt; ++j) {
      if (str[j] == 'Y') {
        matrix[i][j] = 1;
      }
    }
  }

  std::vector<int> dp(1 << cnt, 0);

  for (int mask = 1; mask < (1 << cnt); ++mask) {
    for (int bit1 = 0; bit1 < cnt; ++bit1) {
      for (int bit2 = bit1 + 1; bit2 < cnt; ++bit2) {
        if (matrix[bit1][bit2] == 1 && GetBit(mask, bit1) == 1 &&
            GetBit(mask, bit2) == 1) {
          dp[mask] = std::max(dp[mask], dp[RemoveBits(mask, bit1, bit2)] + 2);
        }
      }
    }
  }

  std::cout << dp.back();
}