#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

const int KInf = 2e9 + 9;

void Dfs(std::vector<std::pair<int, int>>& bridges,
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
      Dfs(bridges, connection, color, {next_vertex, current_vertex}, timer,
          {tin, time});
      time[current_vertex] = std::min(time[current_vertex], time[next_vertex]);
      if (time[next_vertex] > tin[current_vertex]) {
        bridges.push_back({current_vertex, next_vertex});
      }
    }
  }
  color[current_vertex] = "BLACK";
}

void Dfs2(int cur_comp, std::vector<std::vector<int>>& connection,
          std::vector<std::string>& color, std::vector<std::set<int>>& comp,
          int current_vertex) {
  color[current_vertex] = "GREY";
  comp[cur_comp].insert(current_vertex);

  for (int next_vertex : connection[current_vertex]) {
    if (color[next_vertex] == "WHITE") {
      Dfs2(cur_comp, connection, color, comp, next_vertex);
    }
  }
  color[current_vertex] = "BLACK";
}

std::vector<int> Remove(std::vector<int>& vec, int elem) {
  std::vector<int> result;

  for (int cur : vec) {
    if (cur != elem) {
      result.push_back(cur);
    }
  }

  return result;
}

void PrintAnswer(std::vector<std::pair<int, int>> bridges,
                 std::vector<std::set<int>> components) {
  std::vector<int> bridge_count(bridges.size() + 1);
  for (std::pair<int, int> bridge : bridges) {
    for (size_t i = 0; i < components.size(); ++i) {
      if (components[i].find(bridge.first) != components[i].end()) {
        ++bridge_count[i];
      }
      if (components[i].find(bridge.second) != components[i].end()) {
        ++bridge_count[i];
      }
    }
  }

  int res = 0;
  for (int count : bridge_count) {
    if (count == 1) {
      ++res;
    }
  }

  std::cout << (res + 1) / 2;
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
  std::vector<std::pair<int, int>> bridges;
  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      Dfs(bridges, connection, color, {cur_vertex, -1}, timer, {tin, time});
    }
  }
  std::vector<int> real_bridges;
  for (std::pair<int, int> pair : bridges) {
    if (pair.first < pair.second) {
      std::swap(pair.first, pair.second);
    }
    if (edges_find[pair] != -1) {
      real_bridges.push_back(edges_find[pair] + 1);
    }
  }
  std::sort(real_bridges.begin(), real_bridges.end());
  std::fill(color.begin(), color.end(), "WHITE");
  for (std::pair<int, int> bridge : bridges) {
    connection[bridge.first] = Remove(connection[bridge.first], bridge.second);
    connection[bridge.second] = Remove(connection[bridge.second], bridge.first);
  }
  std::vector<std::set<int>> components(bridges.size() + 1);
  int cur_component = 0;
  for (int cur_vertex = 0; cur_vertex < vertex_cnt; ++cur_vertex) {
    if (color[cur_vertex] == "WHITE") {
      Dfs2(cur_component, connection, color, components, cur_vertex);
      ++cur_component;
    }
  }
  PrintAnswer(bridges, components);
}