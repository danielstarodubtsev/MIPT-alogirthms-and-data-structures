#include <iostream>
#include <queue>
#include <set>
#include <vector>

const int cInf = 30000000;
const int cSmallInf = 30000;

struct Edge {
  int first;
  int second;
  int weight;
};

int main() {
  int vertex_cnt;
  int edge_cnt;
  std::cin >> vertex_cnt >> edge_cnt;

  std::vector<Edge> graph(vertex_cnt);

  for (int i = 0; i < edge_cnt; ++i) {
    int first;
    int second;
    int weight;
    std::cin >> first >> second >> weight;
    --first;
    --second;
    graph.push_back(Edge{first, second, weight});
  }

  std::vector<int> dp(vertex_cnt, cInf);
  dp[0] = 0;

  for (int k = 0; k < vertex_cnt; ++k) {
    for (Edge edge : graph) {
      int u = edge.first;
      int v = edge.second;
      int c = edge.weight;

      if (dp[v] > dp[u] + c) {
        dp[v] = dp[u] + c;
      }
    }
  }

  for (int ans : dp) {
    std::cout << (ans < cInf / 2 ? ans : cSmallInf) << " ";
  }
}