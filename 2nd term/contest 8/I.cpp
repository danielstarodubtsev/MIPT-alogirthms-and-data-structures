#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

const bool cDebug = false;
const int cTooBig = 8e6;

void ShowGrid(std::vector<std::vector<int>> grid) {
  std::cout << "\n";
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << grid[i][j] << " ";
    }
    std::cout << "\n";
  }
}

unsigned long long Encode(std::vector<std::vector<int>> grid) {
  unsigned long long res = 0;
  unsigned long long pow = 1;

  for (int i = 0; i < 4 * 4; ++i) {
    res += grid[i / 4][i % 4] * pow;
    pow <<= 4;
  }

  return res;
}

std::vector<std::vector<int>> Decode(unsigned long long code) {
  std::vector<std::vector<int>> result(4, std::vector<int>(4));

  for (int i = 0; i < 4 * 4; ++i) {
    result[i / 4][i % 4] = code % (4 * 4);
    code >>= 4;
  }

  return result;
}

struct Grid {
  unsigned long long grid_code;
  char zero_x;
  char zero_y;

  int g = 0;
  int f = 0;

  Grid(std::vector<std::vector<int>> grid, char zero_x, char zero_y)
      : zero_x(zero_x), zero_y(zero_y) {
    grid_code = Encode(grid);
  }

  std::vector<std::vector<int>> GetGrid() const { return Decode(grid_code); }

  bool operator==(const Grid& other) const {
    return grid_code == other.grid_code;
  }

  bool operator!=(const Grid& other) const { return !(*this == other); }
};

bool operator<(const Grid& first, const Grid& second) {
  return first.f < second.f;
}

bool HasSolution(Grid grid) {
  std::vector<std::vector<int>> vec_grid = grid.GetGrid();

  char zero_x = grid.zero_x;
  char zero_y = grid.zero_y;

  while (zero_x < 3) {
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y][zero_x + 1]);
    ++zero_x;
  }

  while (zero_y < 3) {
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y + 1][zero_x]);
    ++zero_y;
  }

  int s = 0;

  for (char x1 = 0; x1 < 4; ++x1) {
    for (char y1 = 0; y1 < 4; ++y1) {
      for (char x2 = 0; x2 < 4; ++x2) {
        for (char y2 = 0; y2 < 4; ++y2) {
          if (vec_grid[y2][x2] != 0 && vec_grid[y1][x1] > vec_grid[y2][x2] &&
              (y2 > y1 || (y1 == y2 && x2 > x1))) {
            ++s;
          }
        }
      }
    }
  }

  return s % 2 == 0;
}

std::vector<std::pair<Grid, char>> NextGrids(Grid& grid) {
  char zero_x = grid.zero_x;
  char zero_y = grid.zero_y;

  std::vector<std::pair<Grid, char>> ans;
  std::vector<std::vector<int>> vec_grid = grid.GetGrid();

  if (zero_x > 0) {
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y][zero_x - 1]);
    ans.push_back({Grid(vec_grid, zero_x - 1, zero_y), 0});
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y][zero_x - 1]);
  }
  if (zero_x < 3) {
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y][zero_x + 1]);
    ans.push_back({Grid(vec_grid, zero_x + 1, zero_y), 1});
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y][zero_x + 1]);
  }
  if (zero_y > 0) {
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y - 1][zero_x]);
    ans.push_back({Grid(vec_grid, zero_x, zero_y - 1), 2});
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y - 1][zero_x]);
  }
  if (zero_y < 3) {
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y + 1][zero_x]);
    ans.push_back({Grid(vec_grid, zero_x, zero_y + 1), 3});
    std::swap(vec_grid[zero_y][zero_x], vec_grid[zero_y + 1][zero_x]);
  }

  return ans;
}

int Evristic(Grid& grid) {
  int res = 0;

  std::vector<std::vector<int>> vec_grid = grid.GetGrid();

  for (char x = 0; x < 4; ++x) {
    for (char y = 0; y < 4; ++y) {
      if (vec_grid[y][x] != 0) {
        char v = vec_grid[y][x] - 1;
        char other_x = v % 4;
        char other_y = v / 4;
        res += std::abs(x - other_x) + std::abs(y - other_y);
      }
    }
  }

  std::set<int> used;
  for (int x = 0; x < 4; ++x) {
    for (int x1 = 0; x1 < 3; ++x1) {
      for (int x2 = x1 + 1; x2 < 4; ++x2) {
        if (vec_grid[x1][x] > vec_grid[x2][x] &&
            used.find(vec_grid[x1][x]) == used.end() &&
            used.find(vec_grid[x2][x]) == used.end() &&
            (vec_grid[x1][x] - 1) % 4 == x && (vec_grid[x2][x] - 1) % 4 == x &&
            vec_grid[x2][x] != 0) {
          used.insert(vec_grid[x1][x]);
          used.insert(vec_grid[x2][x]);
          res += 2;
        }
      }
    }
  }

  return res;
}

void Move(std::vector<std::vector<int>>& grid, int direct) {
  int zero_x = -1;
  int zero_y = -1;
  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 4; ++y) {
      if (grid[y][x] == 0) {
        zero_x = x;
        zero_y = y;
      }
    }
  }

  if (direct == 0) {
    std::swap(grid[zero_y][zero_x], grid[zero_y][zero_x + 1]);
  } else if (direct == 1) {
    std::swap(grid[zero_y][zero_x], grid[zero_y][zero_x - 1]);
  } else if (direct == 2) {
    std::swap(grid[zero_y][zero_x], grid[zero_y + 1][zero_x]);
  } else if (direct == 3) {
    std::swap(grid[zero_y][zero_x], grid[zero_y - 1][zero_x]);
  }
}

std::priority_queue<std::pair<int, Grid>> Truncate(
    std::priority_queue<std::pair<int, Grid>>& q) {
  std::priority_queue<std::pair<int, Grid>> new_q;

  for (int i = 0; i < cTooBig / 2; ++i) {
    new_q.push(q.top());
    q.pop();
  }

  return new_q;
}

int main() {
  char zero_x = -1;
  char zero_y = -1;
  std::vector<std::vector<int>> vector_grid(4, std::vector<int>(4));
  for (char i = 0; i < 4; ++i) {
    for (char j = 0; j < 4; ++j) {
      std::cin >> vector_grid[i][j];
      if (vector_grid[i][j] == 0) {
        zero_x = j;
        zero_y = i;
      }
    }
  }

  Grid grid{vector_grid, zero_x, zero_y};

  if (!HasSolution(grid)) {
    std::cout << -1;
    return 0;
  }

  std::vector<std::vector<int>> result_grid_vector = {
      {1, 2, 3, 4},
      {5, 2 * 3, 3 + 4, 4 + 4},
      {4 + 5, 5 + 5, 5 + 5 + 1, 5 + 5 + 2},
      {5 + 5 + 3, 5 + 5 + 4, 5 * 3, 0}};

  std::priority_queue<std::pair<int, Grid>> q;

  grid.g = 0;
  grid.f = grid.g + Evristic(grid);

  q.push({-grid.f, grid});

  std::unordered_map<unsigned long long, char> parent;

  int nodes_visited = 0;
  unsigned long long target_code = Encode(result_grid_vector);

  while (!q.empty()) {
    ++nodes_visited;
    Grid cur_grid = q.top().second;
    if (cur_grid.grid_code == target_code) {
      break;
    }
    q.pop();
    for (std::pair<Grid, char>& pair : NextGrids(cur_grid)) {
      Grid next_grid = pair.first;
      char direction = pair.second;
      int score = cur_grid.g + 1;
      unsigned long long next_code = Encode(next_grid.GetGrid());
      bool contains = parent.find(next_code) != parent.end();
      if (contains == score < next_grid.g) {
        parent[next_code] = direction;
        next_grid.g = score;
        next_grid.f = next_grid.g + Evristic(next_grid);
        q.push({-next_grid.f, next_grid});

        if (q.size() > cTooBig) {
          q = Truncate(q);
        }
      }
    }
  }
  char a = 1;
  std::string ans;
  while (result_grid_vector != vector_grid) {
    ++a;
    char direct = parent[Encode(result_grid_vector)];
    Move(result_grid_vector, direct);

    if (direct == 0) {
      ans += "R";
    } else if (direct == 1) {
      ans += "L";
    } else if (direct == 2) {
      ans += "D";
    } else {
      ans += "U";
    }
  }
  std::reverse(ans.begin(), ans.end());
  std::cout << a - 1 << '\n';
  std::cout << ans << "\n";

  if (cDebug) {
    std::cout << "nodes visited: " << nodes_visited << "\n";
  }
}

// 1 2 3 4 5 6 9 11 10 15 13 14 12 7 0 8