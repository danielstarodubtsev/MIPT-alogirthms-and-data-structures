#include <iostream>
#include <queue>
#include <set>
#include <vector>

const long long cInf = 1e17 + 7;

void Expand(long long cur_vertex,
            std::vector<std::vector<std::pair<long long, long long>>>& graph,
            std::vector<long long>& dist,
            std::priority_queue<std::pair<long long, long long>>& q,
            std::vector<bool>& used) {
  if (used[cur_vertex]) {
    return;
  }

  for (std::pair<long long, long long> pair : graph[cur_vertex]) {
    long long to_vertex = pair.first;
    long long edge_weight = pair.second;

    if (dist[cur_vertex] + edge_weight < dist[to_vertex]) {
      dist[to_vertex] = dist[cur_vertex] + edge_weight;
      q.push({-dist[to_vertex], to_vertex});
    }
  }

  used[cur_vertex] = true;
}

int main() {
  long long vertex_cnt;
  long long edge_cnt;
  long long virus_cnt;
  std::cin >> vertex_cnt >> edge_cnt >> virus_cnt;

  std::vector<long long> virus_vertexes(virus_cnt);
  for (long long i = 0; i < virus_cnt; ++i) {
    std::cin >> virus_vertexes[i];
    --virus_vertexes[i];
  }

  std::vector<std::vector<std::pair<long long, long long>>> graph(vertex_cnt);

  for (long long i = 0; i < edge_cnt; ++i) {
    long long first;
    long long second;
    long long weight;
    std::cin >> first >> second >> weight;
    graph[first - 1].push_back({second - 1, weight});
    graph[second - 1].push_back({first - 1, weight});
  }

  long long start_vertex;
  long long end_vertex;
  std::cin >> start_vertex >> end_vertex;
  --start_vertex;
  --end_vertex;

  std::vector<long long> dist(vertex_cnt, cInf);
  std::priority_queue<std::pair<long long, long long>> q;
  dist[end_vertex] = 0;

  std::vector<bool> used(vertex_cnt, false);

  for (long long i = 0; i < vertex_cnt; ++i) {
    q.push(i != end_vertex ? std::make_pair(-cInf, i) : std::make_pair(0LL, i));
  }

  while (!q.empty()) {
    long long index = q.top().second;
    q.pop();
    Expand(index, graph, dist, q, used);
  }

  long long my_time = dist[start_vertex];

  for (long long virus_vertex : virus_vertexes) {
    if (dist[virus_vertex] <= my_time) {
      std::cout << -1;
      return 0;
    }
  }

  std::cout << my_time;
}