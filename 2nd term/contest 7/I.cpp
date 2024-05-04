#include <iostream>
#include <map>
#include <set>
#include <vector>

const int KInf = 1e9 + 9;

void Dfs(std::map<std::pair<int, int>, int>& edges,
         std::vector<std::vector<int>>& connection, std::vector<bool>& used,
         std::vector<int>& free_num, std::vector<int>& x_coef,
         std::pair<int, int> vertices) {
  int current_vertex = vertices.first;
  int parent = vertices.second;
  used[current_vertex] = true;
  if (parent == -1) {
    x_coef[current_vertex] = 1;
  } else {
    free_num[current_vertex] = edges[{std::min(current_vertex, parent),
                                      std::max(current_vertex, parent)}] -
                               free_num[parent];
    x_coef[current_vertex] = -x_coef[parent];
  }

  used[current_vertex] = true;

  for (int next_vertex : connection[current_vertex]) {
    if (!used[next_vertex]) {
      Dfs(edges, connection, used, free_num, x_coef,
          {next_vertex, current_vertex});
    }
  }
}

bool FindAnswer(int x_value, std::map<std::pair<int, int>, int>& edges,
                std::vector<int>& free_num, std::vector<int>& x_coef) {
  std::vector<bool> found(free_num.size(), false);

  std::vector<int> real_values(free_num.size());
  for (size_t i = 0; i < free_num.size(); ++i) {
    real_values[i] = free_num[i] + x_value * x_coef[i];
    if (real_values[i] < 0 ||
        real_values[i] >= static_cast<int>(free_num.size()) ||
        found[real_values[i]]) {
      return false;
    }
    found[real_values[i]] = true;
  }

  for (std::pair<std::pair<int, int>, int> edge : edges) {
    if (real_values[edge.first.first] + real_values[edge.first.second] !=
        edge.second) {
      return false;
    }
  }

  for (int elem : real_values) {
    std::cout << elem + 1 << " ";
  }

  return true;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int vertex_cnt;
  int edge_cnt;
  std::cin >> vertex_cnt >> edge_cnt;

  std::map<std::pair<int, int>, int> edges;
  std::vector<std::vector<int>> connection(vertex_cnt, std::vector<int>(0));
  std::vector<bool> used(vertex_cnt, false);

  for (int i = 0; i < edge_cnt; ++i) {
    int first;
    int second;
    int third;
    std::cin >> first >> second >> third;
    --first;
    --second;
    edges[{std::min(first, second), std::max(first, second)}] = third - 2;
    connection[first].push_back(second);
    connection[second].push_back(first);
  }

  std::vector<int> free_num(vertex_cnt, 0);
  std::vector<int> x_coef(vertex_cnt, 0);

  Dfs(edges, connection, used, free_num, x_coef, {0, -1});

  int min_free_plus = KInf;
  int min_free_minus = KInf;

  for (int i = 0; i < vertex_cnt; ++i) {
    if (x_coef[i] == -1) {
      min_free_minus = std::min(min_free_minus, free_num[i]);
    } else if (x_coef[i] == 1) {
      min_free_plus = std::min(min_free_plus, free_num[i]);
    }
  }

  int first_possible = -min_free_plus;
  int second_possible = min_free_minus;

  bool found = FindAnswer(second_possible, edges, free_num, x_coef);

  if (!found) {
    FindAnswer(first_possible, edges, free_num, x_coef);
  }
}