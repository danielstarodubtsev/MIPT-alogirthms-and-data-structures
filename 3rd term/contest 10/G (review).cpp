#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

bool IsSubstrPal(const std::string& str, int begin, int end) {
  for (int i = 0; i < std::min(end - begin + 1, (end - begin) / 2 + 2); ++i) {
    if (str[begin + i] != str[end - i]) {
      return false;
    }
  }
  return true;
}

signed main() {
  int n;
  std::cin >> n;
  std::unordered_map<std::string, std::vector<int>> strings;
  std::unordered_map<std::string, std::vector<int>> reversed_strings;
  std::string cur;
  for (int i = 0; i < n; ++i) {
    std::cin >> cur;
    strings[cur].push_back(i);
    std::reverse(cur.begin(), cur.end());
    reversed_strings[cur].push_back(i);
  }

  std::vector<std::pair<int, int>> ans;

  for (std::pair<const std::string&, std::vector<int>> p : strings) {
    const std::string& first = p.first;
    std::string pref;
    for (int pref_size = 1; pref_size <= int(first.size()); ++pref_size) {
      pref = pref + first[pref_size - 1];
      if (reversed_strings.find(pref) != reversed_strings.end() &&
          IsSubstrPal(first, pref_size, first.size() - 1)) {
        for (int a : p.second) {
          for (int b : reversed_strings[pref]) {
            if (a != b) {
              ans.push_back({a, b});
            }
          }
        }
      }
    }
    std::string suf;
    for (int suf_size = 1; suf_size < int(first.size()); ++suf_size) {
      suf.push_back(first[first.size() - suf_size]);
      if (strings.find(suf) != strings.end() &&
          IsSubstrPal(first, 0, first.size() - suf_size - 1)) {
        for (int a : p.second) {
          for (int b : strings[suf]) {
            if (a != b) {
              ans.push_back({b, a});
            }
          }
        }
      }
    }
  }

  std::cout << ans.size() << '\n';
  for (auto x : ans) {
    std::cout << x.first + 1 << " " << x.second + 1 << "\n";
  }
}