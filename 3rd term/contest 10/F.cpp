#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main() {
  std::string str;
  std::string text;
  std::cin >> str >> text;

  std::string work_str = str + "#" + text;
  int n = work_str.size();

  std::vector<int> z(n);
  int left = 0;
  int right = 0;
  for (int i = 1; i < n; ++i) {
    z[i] = std::max(0, std::min(right - i, z[i - left]));
    while (i + z[i] < n && work_str[z[i]] == work_str[i + z[i]]) {
      ++z[i];
    }
    if (i + z[i] > right) {
      left = i;
      right = i + z[i];
    }
  }

  std::reverse(str.begin(), str.end());
  std::reverse(text.begin(), text.end());
  work_str = str + "#" + text;

  std::vector<int> z2(n);
  left = 0;
  right = 0;
  for (int i = 1; i < n; ++i) {
    z2[i] = std::max(0, std::min(right - i, z2[i - left]));
    while (i + z2[i] < n && work_str[z2[i]] == work_str[i + z2[i]]) {
      ++z2[i];
    }
    if (i + z2[i] > right) {
      left = i;
      right = i + z2[i];
    }
  }

  std::vector<int> d(text.size() + 2, 0);
  int ans = 0;
  for (int div = 0; div <= int(text.size()); ++div) {
    int have_right = (div == int(text.size()) ? 0 : z[div + 1 + str.size()]);
    int have_left = (div == 0 ? 0 : z2[text.size() - div + 1 + str.size()]);
    // std::cout << div << have_left << have_right << "\n";
    int x1 = str.size() - have_right;
    int x2 = have_left;
    x1 = std::max(x1, 1);
    x2 = std::min(x2, int(str.size()));
    if (x2 >= x1) {
      ++d[div - x2];
      --d[div - x1 + 1];
    }
  }
  int sum = 0;
  for (int cur : d) {
    // std::cout << cur << " ";
    sum += cur;
    if (sum > 0) {
      ++ans;
    }
  }
  std::cout << ans;
}