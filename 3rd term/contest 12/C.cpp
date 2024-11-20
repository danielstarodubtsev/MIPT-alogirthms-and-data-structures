#include <iostream>
#include <numeric>
#include <vector>

const int cInf = 1e9 + 7;

signed main() {
  int n;
  std::cin >> n;
  std::vector<int> a(n);
  int one_cnt = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
    if (a[i] == 1) {
      ++one_cnt;
    }
  }

  if (one_cnt > 0) {
    std::cout << n - one_cnt;
    return 0;
  }

  std::vector<std::vector<int>> part_gcd(n, std::vector<int>(n));
  int shortest_len = cInf;
  for (int start = 0; start < n; ++start) {
    int cur = a[start];
    for (int end = start; end < n; ++end) {
      cur = std::gcd(cur, a[end]);
      part_gcd[start][end] = cur;
      if (cur == 1 && end - start + 1 < shortest_len) {
        shortest_len = end - start + 1;
      }
    }
  }

  if (shortest_len == cInf) {
    std::cout << -1;
  } else {
    std::cout << shortest_len + n - 2;
  }
}