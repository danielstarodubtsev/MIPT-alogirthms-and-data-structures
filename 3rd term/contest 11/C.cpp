#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const long long cAlphabetSize = 10;

signed main() {
  long long n;
  long long m;
  std::cin >> n >> m;
  std::vector<long long> len = {0};
  std::vector<long long> link = {-1};
  std::vector<std::vector<long long>> children = {
      std::vector<long long>(cAlphabetSize, -1)};
  std::vector<long long> cnt = {0};
  long long last = 0;
  long long next_num = 1;
  for (long long i = 0; i < n; ++i) {
    long long c;
    std::cin >> c;
    --c;
    long long cur = next_num++;
    cnt.push_back(1);
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
      cnt.push_back(0);
      link[cur] = clone;
      link[q] = clone;
      while (p != -1 && children[p][c] == q) {
        children[p][c] = clone;
        p = link[p];
      }
    }
    last = cur;
  }
  std::vector<std::pair<long long, long long>> help;
  for (long long v = 0; v < next_num; ++v) {
    help.push_back({len[v], v});
  }
  std::sort(help.begin(), help.end());
  std::reverse(help.begin(), help.end());
  for (auto [_, cur] : help) {
    if (link[cur] != -1) {
      cnt[link[cur]] += cnt[cur];
    }
  }
  long long best_vertex = -1;
  long long ans = -1;
  for (long long v = 0; v < next_num; ++v) {
    if (ans < len[v] * cnt[v]) {
      ans = len[v] * cnt[v];
      best_vertex = v;
    }
  }
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
  std::vector<long long> result;
  while (cur_vertex != 0) {
    bool found = false;
    for (long long c = 0; c < cAlphabetSize; ++c) {
      for (long long next : inverse_children[cur_vertex][c]) {
        if (next != -1 && len[next] == len[cur_vertex] - 1) {
          found = true;
          result.push_back(c);
          cur_vertex = next;
          break;
        }
      }
      if (found) {
        break;
      }
    }
  }
  std::cout << ans << "\n" << len[best_vertex] << "\n";
  for (long long i = (long long)(result.size()) - 1; i >= 0; --i) {
    std::cout << result[i] + 1 << " ";
  }
}