#include <iostream>
#include <vector>

int main() {
  int cnt;
  std::cin >> cnt;
  std::vector<int> speeds(cnt);
  std::vector<int> minleft(cnt);
  std::vector<int> minright(cnt);

  for (int i = 0; i < cnt; ++i) {
    std::cin >> speeds[i];
  }

  minleft[0] = speeds[0];
  minright[cnt - 1] = speeds[cnt - 1];
  for (int i = 1; i < cnt; ++i) {
    minleft[i] = std::min(speeds[i], minleft[i - 1]);
    minright[cnt - 1 - i] = std::min(speeds[cnt - 1 - i], minright[cnt - i]);
  }

  int quantity;
  std::cin >> quantity;

  int left;
  int right;
  for (int i = 0; i < quantity; ++i) {
    std::cin >> left >> right;
    std::cout << std::min(minleft[left - 1], minright[right - 1]) << "\n";
  }

  return 0;
}
