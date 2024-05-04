#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> MakeCycle(std::vector<int> vector) {
  int last = vector.back();
  std::vector<int> res(1, last);
  for (size_t i = vector.size() - 1; i >= 1; --i) {
    if (vector[i - 1] == last) {
      std::reverse(res.begin(), res.end());
      return res;
    }
    res.push_back(vector[i - 1]);
  }
  return res;
}

std::string Dfs(std::vector<std::vector<int>>& connection,
                std::vector<std::string>& color, std::vector<int>& found,
                int current_vertex) {
  color[current_vertex] = "GREY";
  found.push_back(current_vertex);

  for (int next_vertex : connection[current_vertex]) {
    if (color[next_vertex] == "GREY") {
      found.push_back(next_vertex);
      return "YES";
    }
    if (color[next_vertex] == "WHITE") {
      std::string res = Dfs(connection, color, found, next_vertex);
      if (res == "YES") {
        return res;
      }
    }
  }

  color[current_vertex] = "BLACK";
  found.pop_back();
  return "NO";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

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

  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      std::vector<int> found;
      std::string res = Dfs(connection, color, found, cur_vertex);
      if (res == "YES") {
        std::cout << "YES\n";
        for (int vertex : MakeCycle(found)) {
          std::cout << vertex + 1 << " ";
        }
        return 0;
      };
    }
  }
  std::cout << "NO";
}