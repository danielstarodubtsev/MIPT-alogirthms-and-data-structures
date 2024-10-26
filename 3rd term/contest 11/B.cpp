#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class SparseTable {
 private:
  std::vector<std::vector<std::pair<T, size_t>>> data_;
  [[no_unique_address]] Compare cmp_;

 public:
  SparseTable(const std::vector<T>& vec)
      : data_(static_cast<int>(std::log2(vec.size())) + 1,
              std::vector<std::pair<T, size_t>>(vec.size())) {
    int size = vec.size();
    int kk = static_cast<int>(std::log2(size));

    for (int i = 0; i < size; ++i) {
      data_[0][i].first = vec[i];
      data_[0][i].second = i;
    }

    for (int i = 0; i < kk; ++i) {
      for (int j = 0; j + (1 << (i + 1)) <= size; ++j) {
        if (cmp_(data_[i][j].first, data_[i][j + (1 << i)].first)) {
          data_[i + 1][j] = data_[i][j];
        } else {
          data_[i + 1][j] = data_[i][j + (1 << i)];
        }
      }
    }
  }

  T Get(size_t from, size_t to) {
    int pow = std::log2(to - from + 1);

    if (cmp_(data_[pow][from].first, data_[pow][to - (1 << pow) + 1].first)) {
      return data_[pow][from].first;
    }

    return data_[pow][to - (1 << pow) + 1].first;
  }

  size_t GetIndex(size_t from, size_t to) {
    int pow = std::log2(to - from + 1);

    if (cmp_(data_[pow][from].first, data_[pow][to - (1 << pow) + 1].first)) {
      return data_[pow][from].first;
    }

    return data_[pow][to - (1 << pow) + 1].first;
  }
};

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

std::vector<long long> MakeLcp(const std::string& s,
                               const std::vector<int>& suff_arr) {
  int n = s.size();
  std::vector<int> pos(n);
  for (int i = 0; i < n; ++i) {
    pos[suff_arr[i]] = i;
  }

  std::vector<long long> lcp(n);
  int k = 0;
  for (int i = 0; i < n; ++i) {
    k = std::max(k - 1, 0);
    if (pos[i] == n - 1) {
      continue;
    }
    int j = suff_arr[pos[i] + 1];
    while (s[i + k] == s[j + k]) {
      ++k;
    }
    lcp[pos[i]] = k;
  }

  return lcp;
}

std::vector<int> MakeClosestRight(std::vector<long long> vec) {
  int n = vec.size();
  std::vector<int> ans(n, n);

  std::stack<std::pair<long long, int>> s;
  for (int i = 0; i < n; ++i) {
    long long cur = vec[i];
    while (!s.empty() && cur < s.top().first) {
      ans[s.top().second] = i;
      s.pop();
    }
    s.push({cur, i});
  }

  return ans;
}

std::vector<int> MakeClosestLeft(std::vector<long long> vec) {
  int n = vec.size();
  std::reverse(vec.begin(), vec.end());
  std::vector<int> ans = MakeClosestRight(vec);
  std::reverse(ans.begin(), ans.end());
  for (int i = 0; i < n; ++i) {
    ans[i] = n - 1 - ans[i];
  }
  return ans;
}

int main() {
  std::string s;
  std::cin >> s;
  int n = s.size();
  std::vector<int> suff_arr = SortCyclicShifts(s);
  std::vector<long long> lcp = MakeLcp(s, suff_arr);
  SparseTable sparse_min(suff_arr);
  SparseTable<int, std::greater<int>> sparse_max(suff_arr);

  long long ans = n;
  lcp.pop_back();

  std::vector<int> closest_left = MakeClosestLeft(lcp);
  std::vector<int> closest_right = MakeClosestRight(lcp);

  for (int i = 0; i < n - 1; ++i) {
    int left = closest_left[i];
    int right = closest_right[i];
    int min = sparse_min.Get(left + 1, right);
    int max = sparse_max.Get(left + 1, right);
    ans = std::max(ans, max - min + lcp[i] * (lcp[i] + 1));
  }

  std::cout << ans;
}