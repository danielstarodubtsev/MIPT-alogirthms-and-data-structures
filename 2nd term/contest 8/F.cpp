#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const long long cInf = 1e15 + 9;

int main() {
  long long vertex_cnt;
  long long edge_cnt;
  long long given_vertex_cnt;
  std::cin >> vertex_cnt >> edge_cnt >> given_vertex_cnt;

  std::vector<std::vector<std::pair<long long, long long>>> matrix(
      vertex_cnt,
      std::vector<std::pair<long long, long long>>(vertex_cnt, {cInf, 0}));
  for (long long i = 0; i < edge_cnt; ++i) {
    long long from_vertex;
    long long to_vertex;
    long long weight;
    std::cin >> from_vertex >> to_vertex >> weight;
    --from_vertex;
    --to_vertex;
    weight *= -1;
    matrix[from_vertex][to_vertex] = {weight, i};
  }
  std::vector<long long> given_vertexes;
  for (long long i = 0; i < given_vertex_cnt; ++i) {
    long long n;
    std::cin >> n;
    if (given_vertexes.empty() || given_vertexes.back() != n - 1) {
      given_vertexes.push_back(n - 1);
    }
  }

  std::vector<std::vector<long long>> d(
      vertex_cnt, std::vector<long long>(vertex_cnt, cInf));
  std::vector<std::vector<long long>> next(
      vertex_cnt, std::vector<long long>(vertex_cnt, -1));
  for (long long first = 0; first < vertex_cnt; ++first) {
    for (long long second = 0; second < vertex_cnt; ++second) {
      if (first == second) {
        d[first][second] = 0;
      } else {
        d[first][second] = matrix[first][second].first;
        next[first][second] = second;
      }
    }
  }
  for (long long i = 0; i < vertex_cnt; ++i) {
    for (long long u = 0; u < vertex_cnt; ++u) {
      for (long long v = 0; v < vertex_cnt; ++v) {
        if (d[u][i] + d[i][v] < d[u][v]) {
          d[u][v] = d[u][i] + d[i][v];
          next[u][v] = next[u][i];
        }
      }
    }
  }

  for (long long i = 0; i < vertex_cnt; ++i) {
    if (d[i][i] < 0 && d[given_vertexes[0]][i] < cInf / 2 &&
        d[i][given_vertexes.back()] < cInf / 2) {
      std::cout << "infinitely kind";
      return 0;
    }
  }

  std::vector<long long> fake_ans;
  for (size_t cur_index = 0; cur_index < given_vertexes.size() - 1;
       ++cur_index) {
    long long u = given_vertexes[cur_index];
    long long v = given_vertexes[cur_index + 1];

    long long c = u;
    while (c != v) {
      fake_ans.push_back(c);
      c = next[c][v];
    }
  }
  fake_ans.push_back(given_vertexes.back());
  std::cout << fake_ans.size() - 1 << "\n";
  for (size_t i = 0; i < fake_ans.size() - 1; ++i) {
    std::cout << matrix[fake_ans[i]][fake_ans[i + 1]].second + 1 << " ";
  }
}
