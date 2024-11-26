#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const int cAlphabetSize = 26;
const char cFirstChar = 'a';

std::vector<int> SortCyclicShifts(std::string s) {
  // ! списано с https://cp-algorithms.com/string/suffix-array.html
  int n = s.size();
  std::vector<int> p(n);
  std::vector<int> c(n);
  std::vector<int> cnt(std::max(cAlphabetSize, n), 0);
  for (int i = 0; i < n; i++) {
    cnt[s[i] - cFirstChar]++;
  }
  for (int i = 1; i < cAlphabetSize; i++) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = 0; i < n; i++) {
    p[--cnt[s[i] - cFirstChar]] = i;
  }
  c[p[0]] = 0;
  int classes = 1;
  for (int i = 1; i < n; i++) {
    if (s[p[i]] != s[p[i - 1]]) {
      classes++;
    }
    c[p[i]] = classes - 1;
  }
  std::vector<int> new_p(n);
  std::vector<int> new_c(n);
  for (int h = 0; (1 << h) < n; ++h) {
    for (int i = 0; i < n; i++) {
      new_p[i] = p[i] - (1 << h);
      if (new_p[i] < 0) {
        new_p[i] += n;
      }
    }
    std::fill(cnt.begin(), cnt.begin() + classes, 0);
    for (int i = 0; i < n; i++) {
      cnt[c[new_p[i]]]++;
    }
    for (int i = 1; i < classes; i++) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
      p[--cnt[c[new_p[i]]]] = new_p[i];
    }
    new_c[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      std::pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
      std::pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
      if (cur != prev) {
        ++classes;
      }
      new_c[p[i]] = classes - 1;
    }
    c.swap(new_c);
  }
  return p;
}

int main() {
  std::string s;
  std::cin >> s;

  std::vector<int> p = SortCyclicShifts(s);

  std::string res;

  for (size_t i = 0; i < p.size(); ++i) {
    res.push_back(s[(p[i] - 1 + s.size()) % s.size()]);
  }
  std::cout << res;
}