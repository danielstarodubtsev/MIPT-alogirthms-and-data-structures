#include <iostream>
#include <string>
#include <vector>

const int cAlphabetSize = 26;
const char cFirstChar = 'a';

std::vector<int> SortCyclicShifts(std::string s) {
  // ! списано с https://cp-algorithms.com/string/suffix-array.html
  s.push_back(cFirstChar -
              1);  // ! Этого нет по ссылке выше, но иначе неверно
                   // ! работает, например, для строк вида "aaaaaaaaaa"
  int n = s.size();
  std::vector<int> p(n);
  std::vector<int> c(n);
  std::vector<int> cnt(std::max(cAlphabetSize + 1, n), 0);
  for (int i = 0; i < n; i++) {
    cnt[s[i] - cFirstChar + 1]++;
  }
  for (int i = 1; i < cAlphabetSize + 1; i++) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = 0; i < n; i++) {
    p[--cnt[s[i] - cFirstChar + 1]] = i;
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

  std::vector<int> result_p;

  for (size_t i = 1; i < p.size(); ++i) {
    result_p.push_back(p[i]);
  }

  return result_p;
}

std::vector<int> MakeLcp(const std::string& s,
                         const std::vector<int>& suff_arr) {
  int n = suff_arr.size();
  std::vector<int> pos(n);
  for (int i = 0; i < n; ++i) {
    pos[suff_arr[i]] = i;
  }

  std::vector<int> lcp(n);
  int k = 0;
  for (int i = 0; i < n; ++i) {
    k = std::max(k - 1, 0);
    if (pos[i] == n - 1) {
      continue;
    }
    int j = suff_arr[pos[i] + 1];
    while (i + k < int(s.size()) && j + k < int(s.size()) &&
           s[i + k] == s[j + k]) {
      ++k;
    }
    lcp[pos[i]] = k;
  }

  return lcp;
}

signed main() {
  std::string s;
  std::cin >> s;
  int n = s.size();
  std::vector<int> suff_arr = SortCyclicShifts(s);
  std::vector<int> lcp = MakeLcp(s, suff_arr);
  std::vector<int> max_arr(n, -1);
  std::vector<int> min_arr(n, n + 3);

  int ans = 0;

  for (size_t i = 0; i < lcp.size(); ++i) {
    int prev = i > 0 ? lcp[i - 1] : 0;
    int cur = lcp[i];
    for (int j = 0; j < cur; ++j) {
      max_arr[j] = std::max(max_arr[j], std::max(suff_arr[i], suff_arr[i + 1]));
      min_arr[j] = std::min(min_arr[j], std::min(suff_arr[i], suff_arr[i + 1]));
    }
    for (int j = cur; j < prev; ++j) {
      if (max_arr[j] - min_arr[j] >= j + 1) {
        ++ans;
      }
      max_arr[j] = -1;
      min_arr[j] = n + 3;
    }
  }

  std::cout << ans;
}