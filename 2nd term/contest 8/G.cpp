#include <iostream>
#include <vector>

int main() {
  int vertex_cnt;
  std::cin >> vertex_cnt;

  std::vector<std::vector<int>> dist(vertex_cnt, std::vector<int>(vertex_cnt));

  for (int i = 0; i < vertex_cnt; ++i) {
    for (int j = 0; j < vertex_cnt; ++j) {
      int d;
      std::cin >> d;
      dist[i][j] = 1 - d;
    }
  }

  for (int iter = 0; iter < vertex_cnt; ++iter) {
    for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
      for (int prev_vertex = 0; prev_vertex < vertex_cnt; ++prev_vertex) {
        for (int start_vertex = 0; start_vertex < vertex_cnt; ++start_vertex) {
          dist[start_vertex][cur_vertex] = std::min(
              dist[start_vertex][cur_vertex],
              dist[start_vertex][prev_vertex] + dist[prev_vertex][cur_vertex]);
        }
      }
    }
  }

  for (int i = 0; i < vertex_cnt; ++i) {
    for (int j = 0; j < vertex_cnt; ++j) {
      std::cout << (dist[i][j] == 0 ? 1 : 0) << " ";
    }

    std::cout << "\n";
  }
}