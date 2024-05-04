#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void Dfs(std::vector<std::vector<int>>& connection,
         std::vector<std::string>& color, std::vector<int>& found,
         int current_vertex) {
  color[current_vertex] = "GREY";

  for (int next_vertex : connection[current_vertex]) {
    if (color[next_vertex] == "WHITE") {
      Dfs(connection, color, found, next_vertex);
    }
  }
  color[current_vertex] = "BLACK";
  found.push_back(current_vertex);
}

int main() {
  int vertex_cnt;
  int edge_cnt;
  std::cin >> vertex_cnt >> edge_cnt;

  std::vector<std::vector<int>> connection(vertex_cnt, std::vector<int>(0));
  std::vector<std::vector<int>> revert_connection(vertex_cnt,
                                                  std::vector<int>(0));
  std::vector<std::string> color(vertex_cnt, "WHITE");
  for (int i = 0; i < edge_cnt; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    connection[start - 1].push_back(end - 1);
    revert_connection[end - 1].push_back(start - 1);
  }

  std::vector<int> found;
  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      Dfs(connection, color, found, cur_vertex);
    }
  }

  std::reverse(found.begin(), found.end());

  std::fill(color.begin(), color.end(), "WHITE");
  std::vector<std::vector<int>> components;

  for (int cur_vertex : found) {
    std::vector<int> new_found;
    if (color[cur_vertex] == "WHITE") {
      Dfs(revert_connection, color, new_found, cur_vertex);
      components.push_back(new_found);
    }
  }

  std::vector<int> answer(vertex_cnt);

  std::cout << components.size() << "\n";

  for (size_t i = 0; i < components.size(); ++i) {
    for (size_t j = 0; j < components[i].size(); ++j) {
      answer[components[i][j]] = i + 1;
    }
  }

  for (int elem : answer) {
    std::cout << elem << " ";
  }
}