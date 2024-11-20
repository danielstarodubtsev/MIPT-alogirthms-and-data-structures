#include <iostream>
#include <vector>

signed main() {
  int n;
  std::cin >> n;

  long long ans = 0;

  std::vector<bool> used(n + 1, false);
  for (int i = 2; i * i <= n; ++i) {
    if (!used[i]) {
      for (int j = i; j <= n; j += i) {
        if (!used[j]) {
          ans += i;
          used[j] = true;
        }
      }
      ans -= i;
    }
  }

  std::cout << ans;
}