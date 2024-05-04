#include <iostream>
#include <map>
#include <string>
#include <vector>

void Dfs(int current_vertex,
         std::vector<std::vector<std::pair<int, int>>>& connection,
         bool& changing_color, std::string& bits, std::vector<bool>& used,
         std::vector<bool>& is_edge_black) {
  used[current_vertex] = true;

  if (bits[current_vertex] == '1') {
    changing_color = !changing_color;
  }

  for (std::pair<int, int> pair : connection[current_vertex]) {
    int next_vertex = pair.first;
    int edge_index = pair.second;

    if (!used[next_vertex]) {
      if (changing_color) {
        is_edge_black[edge_index] = !is_edge_black[edge_index];
      }

      Dfs(next_vertex, connection, changing_color, bits, used, is_edge_black);

      if (changing_color) {
        is_edge_black[edge_index] = !is_edge_black[edge_index];
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int qst;
  std::cin >> qst;

  while (qst-- > 0) {
    int vertex_cnt;
    int edge_cnt;
    std::cin >> vertex_cnt >> edge_cnt;
    std::vector<std::vector<std::pair<int, int>>> connection(
        vertex_cnt, std::vector<std::pair<int, int>>(0));
    std::vector<bool> is_edge_black(edge_cnt, false);
    for (int i = 0; i < edge_cnt; ++i) {
      int first;
      int second;
      std::cin >> first >> second;
      --first;
      --second;
      connection[first].push_back({second, i});
      connection[second].push_back({first, i});
    }
    std::string bits;
    std::cin >> bits;
    std::vector<bool> used(vertex_cnt, false);
    bool answer_exists = true;
    for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
      bool changing_color = false;
      if (!used[cur_vertex]) {
        Dfs(cur_vertex, connection, changing_color, bits, used, is_edge_black);
        if (changing_color) {
          answer_exists = false;
        }
      }
    }
    if (answer_exists) {
      for (bool cur : is_edge_black) {
        std::cout << (cur ? "1" : "0");
      }
      std::cout << "\n";
    } else {
      std::cout << -1 << "\n";
    }
  }
}