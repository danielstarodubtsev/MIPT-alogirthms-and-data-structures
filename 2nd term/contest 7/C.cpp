#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int Dfs(std::vector<std::vector<int>>& connection,
        std::vector<std::string>& color, std::vector<int>& found,
        int current_vertex) {
  color[current_vertex] = "GREY";

  for (int next_vertex : connection[current_vertex]) {
    if (color[next_vertex] == "WHITE") {
      int res = Dfs(connection, color, found, next_vertex);
      if (res == 1) {
        return 1;
      }
    } else if (color[next_vertex] == "GREY") {
      return 1;
    }
  }
  color[current_vertex] = "BLACK";
  found.push_back(current_vertex + 1);
  return 0;
}

int main() {
  int vertex_cnt;
  int edge_cnt;
  std::cin >> vertex_cnt >> edge_cnt;

  std::vector<std::vector<int>> connection(vertex_cnt, std::vector<int>(0));
  std::vector<std::string> color(vertex_cnt, "WHITE");
  for (int i = 0; i < edge_cnt; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    connection[start - 1].push_back(end - 1);
  }

  std::vector<int> found;
  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      int result = Dfs(connection, color, found, cur_vertex);
      if (result == 1) {
        std::cout << -1;
        return 0;
      }
    }
  }

  std::reverse(found.begin(), found.end());
  for (int elem : found) {
    std::cout << elem << " ";
  }
}