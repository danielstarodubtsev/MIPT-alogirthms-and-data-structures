#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const long long cAlphabetSize = 36;
const char cFirstChar = 'a' - 10;

void DoStuff(long long next_num, std::vector<std::vector<long long>>& children,
             long long best_vertex, std::vector<long long>& len) {
  std::vector<std::vector<std::vector<long long>>> inverse_children(
      next_num, std::vector<std::vector<long long>>(cAlphabetSize));
  for (long long cur = 0; cur < next_num; ++cur) {
    for (long long c = 0; c < cAlphabetSize; ++c) {
      long long next = children[cur][c];
      if (next != -1) {
        inverse_children[next][c].push_back(cur);
      }
    }
  }
  long long cur_vertex = best_vertex;
  std::string result;
  while (cur_vertex != 0) {
    bool found = false;
    for (long long c = 0; c < cAlphabetSize; ++c) {
      for (long long next : inverse_children[cur_vertex][c]) {
        if (next != -1 && len[next] == len[cur_vertex] - 1) {
          found = true;
          result.push_back(c + cFirstChar);
          cur_vertex = next;
          break;
        }
      }
      if (found) {
        break;
      }
    }
  }

  std::reverse(result.begin(), result.end());
  std::cout << result;
}

signed main() {
  long long strings_cnt;
  std::cin >> strings_cnt;
  std::string s;
  std::string cur;
  for (long long i = 0; i < strings_cnt; ++i) {
    std::cin >> cur;
    s += cur;
    s.push_back('a' - i - 1);
  }
  long long n = s.size();
  std::vector<long long> len = {0};
  std::vector<long long> link = {-1};
  std::vector<std::vector<long long>> children = {
      std::vector<long long>(cAlphabetSize, -1)};
  long long last = 0;
  long long next_num = 1;
  for (long long i = 0; i < n; ++i) {
    long long c = s[i] - cFirstChar;
    long long cur = next_num++;
    len.push_back(len[last] + 1);
    link.push_back(-1);
    children.push_back(std::vector<long long>(cAlphabetSize, -1));
    long long cur_state = last;
    while (cur_state != -1 && children[cur_state][c] == -1) {
      children[cur_state][c] = cur;
      cur_state = link[cur_state];
    }
    long long p = cur_state;
    if (p == -1) {
      link[cur] = 0;
      last = cur;
      continue;
    }
    long long q = children[p][c];
    if (len[p] + 1 == len[q]) {
      link[cur] = q;
    } else {
      long long clone = next_num++;
      len.push_back(len[p] + 1);
      link.push_back(link[q]);
      children.push_back(children[q]);
      link[cur] = clone;
      link[q] = clone;
      while (p != -1 && children[p][c] == q) {
        children[p][c] = clone;
        p = link[p];
      }
    }
    last = cur;
  }

  std::vector<std::vector<bool>> has_path_with(
      next_num, std::vector<bool>(strings_cnt, false));

  std::vector<std::pair<long long, long long>> help;
  for (long long v = 0; v < next_num; ++v) {
    help.push_back({len[v], v});
  }
  std::sort(help.begin(), help.end());
  std::reverse(help.begin(), help.end());

  for (long long i = 0; i < strings_cnt; ++i) {
    char d = 'a' - i - 1;
    for (auto [_, cur] : help) {
      for (char c = cFirstChar; c < cFirstChar + cAlphabetSize; ++c) {
        long long next = children[cur][c - cFirstChar];
        if (next != -1) {
          if (c == d || (c >= 'a' && has_path_with[next][i])) {
            has_path_with[cur][i] = true;
          }
        }
      }
    }
  }

  long long best_vertex = -1;
  long long best_len = -1;

  for (long long v = 0; v < next_num; ++v) {
    bool good = true;
    for (bool i : has_path_with[v]) {
      good = (good && i);
    }
    if (good) {
      if (len[v] > best_len) {
        best_len = len[v];
        best_vertex = v;
      }
    }
  }

  DoStuff(next_num, children, best_vertex, len);
}