#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const int KInf = 2e9 + 9;

void Dfs(std::vector<std::pair<int, int>>& answer,
         std::vector<std::vector<int>>& connection,
         std::vector<std::string>& color, std::pair<int, int> vertices,
         int& timer, std::pair<std::vector<int>&, std::vector<int>&> times) {
  int current_vertex = vertices.first;
  int parent = vertices.second;
  std::vector<int>& tin = times.first;
  std::vector<int>& time = times.second;
  tin[current_vertex] = timer++;
  time[current_vertex] = tin[current_vertex];
  color[current_vertex] = "GREY";

  for (int next_vertex : connection[current_vertex]) {
    if (next_vertex == parent) {
      continue;
    }
    if (color[next_vertex] != "WHITE") {
      time[current_vertex] = std::min(time[current_vertex], tin[next_vertex]);
    } else {
      Dfs(answer, connection, color, {next_vertex, current_vertex}, timer,
          {tin, time});
      time[current_vertex] = std::min(time[current_vertex], time[next_vertex]);
      if (time[next_vertex] > tin[current_vertex]) {
        answer.push_back({current_vertex, next_vertex});
      }
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
  std::vector<int> tin(vertex_cnt, KInf);
  std::vector<int> time(vertex_cnt, KInf);
  std::map<std::pair<int, int>, int> edges_find;
  int timer = 0;
  for (int i = 0; i < edge_cnt; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    connection[start - 1].push_back(end - 1);
    connection[end - 1].push_back(start - 1);
    if (start < end) {
      std::swap(start, end);
    }
    if (edges_find.find({start - 1, end - 1}) != edges_find.end()) {
      edges_find[{start - 1, end - 1}] = -1;
    } else {
      edges_find[{start - 1, end - 1}] = i;
    }
  }

  std::vector<std::pair<int, int>> answer;

  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      Dfs(answer, connection, color, {cur_vertex, -1}, timer, {tin, time});
    }
  }

  std::vector<int> real_answer;
  for (std::pair<int, int> pair : answer) {
    if (pair.first < pair.second) {
      std::swap(pair.first, pair.second);
    }
    if (edges_find[pair] != -1) {
      real_answer.push_back(edges_find[pair] + 1);
    }
  }
  std::sort(real_answer.begin(), real_answer.end());
  std::cout << real_answer.size() << "\n";
  for (int ans : real_answer) {
    std::cout << ans << " ";
  }
}