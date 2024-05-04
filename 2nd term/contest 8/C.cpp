#include <algorithm>
#include <iostream>
#include <vector>

const int cInf = 2e9 + 9;
const int cMagicConst1 = 10;
const double cMagicConst2 = 0.03;

struct Edge {
  int first;
  int second;
  int time;
  int cost;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int vertex_cnt;
  int edge_cnt;
  int max_time;
  std::cin >> vertex_cnt >> edge_cnt >> max_time;

  std::vector<std::vector<int>> times(vertex_cnt, std::vector<int>(vertex_cnt));

  std::vector<Edge> edges;
  for (int i = 0; i < edge_cnt; ++i) {
    int first;
    int second;
    int time;
    int cost;
    std::cin >> first >> second >> cost >> time;
    edges.push_back(Edge{first - 1, second - 1, time, cost});
    edges.push_back(Edge{second - 1, first - 1, time, cost});
    times[first - 1][second - 1] = time;
    times[second - 1][first - 1] = time;
  }

  std::vector<std::vector<std::pair<int, int>>> costs_for_time(
      vertex_cnt, std::vector<std::pair<int, int>>(max_time + 1, {cInf, -1}));
  costs_for_time[0][0].first = 0;

  std::vector<int> min_non_inf(vertex_cnt, max_time);
  min_non_inf[0] = 0;

  for (int iter = 0; iter < std::max(std::min(cMagicConst1, vertex_cnt),
                                     (int)(vertex_cnt * cMagicConst2));
       ++iter) {
    for (Edge edge : edges) {
      int u = edge.first;
      int v = edge.second;
      int time = edge.time;
      int cost = edge.cost;

      for (int u_time = min_non_inf[u]; u_time <= max_time - time; ++u_time) {
        if (costs_for_time[u][u_time].first + cost <
            costs_for_time[v][u_time + time].first) {
          costs_for_time[v][u_time + time].first =
              costs_for_time[u][u_time].first + cost;
          costs_for_time[v][u_time + time].second = u;
          min_non_inf[v] = std::min(min_non_inf[v], u_time + time);
        }
      }
    }
  }

  int min_cost = cInf;
  int best_time = -1;

  for (int i = 0; i <= max_time; ++i) {
    if (costs_for_time[vertex_cnt - 1][i].first < min_cost) {
      min_cost = costs_for_time[vertex_cnt - 1][i].first;
      best_time = i;
    }
  }

  if (min_cost > cInf / 2) {
    std::cout << -1;
    return 0;
  }

  std::cout << min_cost << "\n";
  int v = vertex_cnt - 1;
  std::vector<int> ans;

  while (true) {
    ans.push_back(v);
    int old_v = v;
    v = costs_for_time[v][best_time].second;
    if (v == -1) {
      break;
    }
    best_time -= times[v][old_v];
  }

  std::reverse(ans.begin(), ans.end());

  std::cout << ans.size() << "\n";
  for (int elem : ans) {
    std::cout << elem + 1 << " ";
  }
}