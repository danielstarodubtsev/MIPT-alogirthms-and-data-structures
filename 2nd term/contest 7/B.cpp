#include <iostream>
#include <string>
#include <vector>

void Dfs(std::vector<std::vector<int>>& connection,
         std::vector<std::string>& color, std::vector<int>& found,
         int current_vertex) {
  color[current_vertex] = "GREY";
  found.push_back(current_vertex);

  for (int next_vertex : connection[current_vertex]) {
    if (color[next_vertex] == "WHITE") {
      Dfs(connection, color, found, next_vertex);
    }
  }
  color[current_vertex] = "BLACK";
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
    connection[end - 1].push_back(start - 1);
  }

  std::vector<std::vector<int>> answer;
  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      std::vector<int> found;
      Dfs(connection, color, found, cur_vertex);
      answer.push_back(found);
    }
  }

  std::cout << answer.size() << "\n";
  for (std::vector<int> group : answer) {
    std::cout << group.size() << "\n";
    for (int elem : group) {
      std::cout << elem + 1 << " ";
    }
    std::cout << "\n";
  }
}