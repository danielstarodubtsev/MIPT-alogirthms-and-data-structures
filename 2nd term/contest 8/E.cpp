#include <algorithm>
#include <iostream>
#include <vector>

const int cInf = 30000000;
const int cSmallInf = 100000;

struct Edge {
  int first;
  int second;
  int weight;
};

int main() {
  int vertex_cnt;
  std::cin >> vertex_cnt;

  std::vector<Edge> graph(vertex_cnt);

  for (int first = 0; first < vertex_cnt; ++first) {
    for (int second = 0; second < vertex_cnt; ++second) {
      int weight;
      std::cin >> weight;

      if (weight != cSmallInf) {
        graph.push_back(Edge{first, second, weight});
      }
    }
  }

  std::vector<int> dp(vertex_cnt, cInf);
  dp[0] = 0;

  std::vector<int> p(vertex_cnt, -1);

  for (int k = 0; k < vertex_cnt; ++k) {
    for (Edge edge : graph) {
      int u = edge.first;
      int v = edge.second;
      int c = edge.weight;

      if (dp[v] > dp[u] + c) {
        dp[v] = dp[u] + c;
        p[v] = u;
      }
    }
  }

  std::vector<int> ans;

  for (Edge edge : graph) {
    int u = edge.first;
    int v = edge.second;
    int c = edge.weight;

    if (dp[v] > dp[u] + c) {
      for (int i = 0; i < vertex_cnt; ++i) {
        v = p[v];
      }

      u = v;

      while (true) {
        ans.push_back(v);
        v = p[v];

        if (u == v) {
          break;
        }
      }
      std::reverse(ans.begin(), ans.end());
      ans.push_back(ans[0]);
      break;
    }
  }

  if (!ans.empty()) {
    std::cout << "YES\n";
    std::cout << ans.size() << "\n";
    for (int a : ans) {
      std::cout << a + 1 << " ";
    }
  } else {
    std::cout << "NO";
  }
}