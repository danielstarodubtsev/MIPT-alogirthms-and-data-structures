#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

const auto kPrec = std::setprecision(4 * 5);

int main() {
  size_t cnt;
  std::cin >> cnt;

  std::vector<long double> sounds(cnt);
  for (size_t i = 0; i < cnt; i++) {
    std::cin >> sounds[i];
    sounds[i] = std::log2(sounds[i]);
  }

  std::vector<long double> pref_product(cnt, sounds[0]);
  for (size_t i = 1; i < cnt; i++) {
    pref_product[i] = sounds[i] + pref_product[i - 1];
  }

  size_t qnt;
  std::cin >> qnt;

  std::cout << kPrec;

  for (size_t i = 0; i < qnt; i++) {
    int left;
    int right;
    std::cin >> left >> right;
    if (left > 0) {
      std::cout << std::pow(2, (pref_product[right] - pref_product[left - 1]) /
                                   (right - left + 1))
                << "\n";
    } else {
      std::cout << std::pow(2, pref_product[right] / (right + 1)) << "\n";
    }
  }
}
