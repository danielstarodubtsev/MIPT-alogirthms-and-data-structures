#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const int KInf = 2e9 + 9;

void Dfs(std::vector<int>& answer, std::vector<std::vector<int>>& connection,
         std::vector<std::string>& color, std::pair<int, int> vertices,
         int& timer, std::pair<std::vector<int>&, std::vector<int>&> times) {
  int current_vertex = vertices.first;
  int parent = vertices.second;
  std::vector<int>& tin = times.first;
  std::vector<int>& time = times.second;
  tin[current_vertex] = timer++;
  time[current_vertex] = tin[current_vertex];
  color[current_vertex] = "GREY";
  int cnt = 0;

  for (int next_vertex : connection[current_vertex]) {
    if (next_vertex == parent) {
      continue;
    }
    if (color[next_vertex] != "WHITE") {
      time[current_vertex] = std::min(time[current_vertex], tin[next_vertex]);
    } else {
      Dfs(answer, connection, color, {next_vertex, current_vertex}, timer,
          {tin, time});
      ++cnt;
      time[current_vertex] = std::min(time[current_vertex], time[next_vertex]);
      if (time[next_vertex] >= tin[current_vertex] && parent != -1) {
        answer.push_back(current_vertex);
      }
    }
  }
  if (parent == -1 && cnt >= 2) {
    answer.push_back(current_vertex);
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
  int timer = 0;
  for (int i = 0; i < edge_cnt; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    connection[start - 1].push_back(end - 1);
    connection[end - 1].push_back(start - 1);
  }

  std::vector<int> answer;

  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      Dfs(answer, connection, color, {cur_vertex, -1}, timer, {tin, time});
    }
  }

  std::sort(answer.begin(), answer.end());
  answer.resize(std::unique(answer.begin(), answer.end()) - answer.begin());
  std::cout << answer.size() << "\n";

  for (int elem : answer) {
    std::cout << elem + 1 << " ";
  }
}