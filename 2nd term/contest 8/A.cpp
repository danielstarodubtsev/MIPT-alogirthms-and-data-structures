#include <iostream>
#include <queue>
#include <set>
#include <vector>

const int cInf = 2009000999;

void Expand(int cur_vertex,
            std::vector<std::vector<std::pair<int, int>>>& graph,
            std::vector<int>& dist, std::priority_queue<std::pair<int, int>>& q,
            std::vector<bool>& used) {
  if (used[cur_vertex]) {
    return;
  }

  for (std::pair<int, int> pair : graph[cur_vertex]) {
    int to_vertex = pair.first;
    int edge_weight = pair.second;

    if (dist[cur_vertex] + edge_weight < dist[to_vertex]) {
      dist[to_vertex] = dist[cur_vertex] + edge_weight;
      q.push({-dist[to_vertex], to_vertex});
    }
  }

  used[cur_vertex] = true;
}

int main() {
  int qst;
  std::cin >> qst;

  while (qst-- > 0) {
    int vertex_cnt;
    int edge_cnt;
    std::cin >> vertex_cnt >> edge_cnt;

    std::vector<std::vector<std::pair<int, int>>> graph(
        vertex_cnt, std::vector<std::pair<int, int>>(0));
    for (int i = 0; i < edge_cnt; ++i) {
      int first;
      int second;
      int weight;
      std::cin >> first >> second >> weight;
      graph[first].push_back({second, weight});
      graph[second].push_back({first, weight});
    }

    int cur_vertex;
    std::cin >> cur_vertex;

    std::vector<int> dist(vertex_cnt, cInf);
    std::priority_queue<std::pair<int, int>> q;
    dist[cur_vertex] = 0;

    std::vector<bool> used(vertex_cnt, false);

    for (int i = 0; i < vertex_cnt; ++i) {
      q.push(i != cur_vertex ? std::make_pair(-cInf, i) : std::make_pair(0, i));
    }

    while (!q.empty()) {
      int index = q.top().second;
      q.pop();
      Expand(index, graph, dist, q, used);
    }

    for (int d : dist) {
      std::cout << d << " ";
    }

    std::cout << "\n";
  }
}