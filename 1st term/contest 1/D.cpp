#include <algorithm>
#include <iostream>
#include <vector>

bool IsLengthEnough(std::vector<int> positions, int cnt, long long length,
                    int max_spells) {
  const int kMaxCoord = 2e9;
  int need = 0;
  long long last = -length - 1 - kMaxCoord;
  for (int i = 0; i < cnt; i++) {
    if (positions[i] - last > length) {
      last = positions[i];
      need++;
    }
  }
  return need <= max_spells;
}

int main() {
  int cnt;
  int max_spells;
  const int kMaxCoord = 1.5e9;

  std::cin >> cnt >> max_spells;

  std::vector<int> positions(cnt);
  for (int i = 0; i < cnt; i++) {
    std::cin >> positions[i];
  }

  std::sort(positions.begin(), positions.end());

  long long left = -1;
  long long right = kMaxCoord + 1;
  long long middle;
  while (left < right - 1) {
    middle = (left + right) / 2;
    if (IsLengthEnough(positions, cnt, middle, max_spells)) {
      right = middle;
    } else {
      left = middle;
    }
  }
  std::cout << right;
}