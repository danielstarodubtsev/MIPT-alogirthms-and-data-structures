#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

bool HasSolution(std::vector<std::vector<int>>& grid) {
  int s = 0;

  for (int x1 = 0; x1 < 3; ++x1) {
    for (int y1 = 0; y1 < 3; ++y1) {
      for (int x2 = 0; x2 < 3; ++x2) {
        for (int y2 = 0; y2 < 3; ++y2) {
          if (grid[y2][x2] != 0 && grid[y1][x1] > grid[y2][x2] &&
              (y2 > y1 || (y1 == y2 && x2 > x1))) {
            ++s;
          }
        }
      }
    }
  }

  return s % 2 == 0;
}

std::vector<std::pair<std::vector<std::vector<int>>, int>> NextGrids(
    std::vector<std::vector<int>>& grid) {
  int zero_x = -1;
  int zero_y = -1;
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      if (grid[y][x] == 0) {
        zero_x = x;
        zero_y = y;
      }
    }
  }

  std::vector<std::pair<std::vector<std::vector<int>>, int>> ans;
  if (zero_x > 0) {
    std::swap(grid[zero_y][zero_x], grid[zero_y][zero_x - 1]);
    ans.push_back({grid, 0});
    std::swap(grid[zero_y][zero_x], grid[zero_y][zero_x - 1]);
  }
  if (zero_x < 2) {
    std::swap(grid[zero_y][zero_x], grid[zero_y][zero_x + 1]);
    ans.push_back({grid, 1});
    std::swap(grid[zero_y][zero_x], grid[zero_y][zero_x + 1]);
  }
  if (zero_y > 0) {
    std::swap(grid[zero_y][zero_x], grid[zero_y - 1][zero_x]);
    ans.push_back({grid, 2});
    std::swap(grid[zero_y][zero_x], grid[zero_y - 1][zero_x]);
  }
  if (zero_y < 2) {
    std::swap(grid[zero_y][zero_x], grid[zero_y + 1][zero_x]);
    ans.push_back({grid, 3});
    std::swap(grid[zero_y][zero_x], grid[zero_y + 1][zero_x]);
  }

  return ans;
}

void ShowGrid(std::vector<std::vector<int>>& grid) {
  std::cout << "\n";
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cout << grid[i][j] << " ";
    }
    std::cout << "\n";
  }
}

int Evristic(std::vector<std::vector<int>>& grid) {
  int res = 0;

  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      if (grid[y][x] != 0) {
        int v = grid[y][x] - 1;
        int other_x = v % 3;
        int other_y = v / 3;
        res += std::abs(x - other_x) + std::abs(y - other_y);
      }
    }
  }

  return res;
}

int main() {
  std::vector<std::vector<int>> grid(3, std::vector<int>(3));
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cin >> grid[i][j];
    }
  }

  if (!HasSolution(grid)) {
    std::cout << -1;
    return 0;
  }

  std::set<std::vector<std::vector<int>>> used;
  std::vector<std::vector<int>> result_grid = {
      {1, 2, 3}, {4, 5, 3 * 2}, {4 + 3, 4 * 2, 0}};
  std::priority_queue<std::pair<int, std::vector<std::vector<int>>>> q;
  std::map<std::vector<std::vector<int>>, int> g;
  g[grid] = 0;

  std::map<std::vector<std::vector<int>>, int> f;
  f[grid] = g[grid] + Evristic(grid);

  q.push({-f[grid], grid});

  std::map<std::vector<std::vector<int>>,
           std::pair<std::vector<std::vector<int>>, int>>
      parent;

  // int nodes_visited = 0;

  while (!q.empty()) {
    // ++nodes_visited;
    std::vector<std::vector<int>> cur_grid = q.top().second;
    if (cur_grid == result_grid) {
      break;
    }
    q.pop();
    used.insert(cur_grid);
    for (std::pair<std::vector<std::vector<int>>, int> pair :
         NextGrids(cur_grid)) {
      auto next_grid = pair.first;
      int direction = pair.second;
      int score = g[cur_grid] + 1;
      bool contains = used.find(next_grid) != used.end();
      if (contains && score >= g[next_grid]) {
        continue;
      }
      if (!contains || score < g[next_grid]) {
        parent[next_grid] = {cur_grid, direction};
        g[next_grid] = score;
        f[next_grid] = g[next_grid] + Evristic(next_grid);
        q.push({-f[next_grid], next_grid});
      }
    }
  }

  int a = 0;
  std::string ans;
  while (result_grid != grid) {
    ++a;
    int direct = parent[result_grid].second;
    result_grid = parent[result_grid].first;
    if (direct == 0) {
      ans += "L";
    } else if (direct == 1) {
      ans += "R";
    } else if (direct == 2) {
      ans += "U";
    } else {
      ans += "D";
    }
  }
  std::reverse(ans.begin(), ans.end());
  std::cout << a << '\n';
  std::cout << ans;
}
