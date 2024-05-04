#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

inline namespace MyTemplate {

#define ll long long
#define ld long double
#define ull unsigned long long

#define all(a) (a).begin(), (a).end()

constexpr int cMySchool = 179;
constexpr char cInf = 127;
// constexpr ll cInf = 2e18 + 9;

std::mt19937 random_new_seed(
    std::chrono::steady_clock::now().time_since_epoch().count());
std::mt19937 random_fixed_seed(cMySchool);

template <typename A, typename B, typename C>
struct Triplet {
  A first;
  B second;
  C third;
};

template <typename A, typename B, typename C>
bool operator==(const Triplet<A, B, C>& first, const Triplet<A, B, C>& second) {
  return first.first == second.first && first.second == second.second &&
         first.third == second.third;
}

template <typename A, typename B, typename C>
bool operator!=(const Triplet<A, B, C>& first, const Triplet<A, B, C>& second) {
  return !(first == second);
}

template <typename A, typename B, typename C>
bool operator<(const Triplet<A, B, C>& first, const Triplet<A, B, C>& second) {
  if (first.first < second.first) {
    return true;
  }

  if (first.first > second.first) {
    return false;
  }

  if (first.second < second.second) {
    return true;
  }

  if (first.second > second.second) {
    return false;
  }

  return first.third < second.third;
}

template <typename A, typename B, typename C>
bool operator>(const Triplet<A, B, C>& first, const Triplet<A, B, C>& second) {
  return second < first;
}

template <typename A, typename B, typename C>
bool operator<=(const Triplet<A, B, C>& first, const Triplet<A, B, C>& second) {
  return !(first > second);
}

template <typename A, typename B, typename C>
bool operator>=(const Triplet<A, B, C>& first, const Triplet<A, B, C>& second) {
  return !(first < second);
}

template <typename A, typename B, typename C, typename D>
struct Quadro {
  A first;
  B second;
  C third;
  D fourth;
};

template <typename A, typename B>
std::ostream& operator<<(std::ostream& ostream, const std::pair<A, B>& pair) {
  return ostream << '(' << pair.first << ", " << pair.second << ')';
}

template <typename A, typename B, typename C>
std::ostream& operator<<(std::ostream& ostream,
                         const Triplet<A, B, C>& triplet) {
  return ostream << '(' << triplet.first << ", " << triplet.second << ", "
                 << triplet.third << ')';
}

template <typename A, typename B, typename C, typename D>
std::ostream& operator<<(std::ostream& ostream,
                         const Quadro<A, B, C, D>& quadro) {
  return ostream << '(' << quadro.first << ", " << quadro.second << ", "
                 << quadro.third << ", " << quadro.fourth << ')';
}

template <typename TContainer,
          typename T = typename std::enable_if<
              !std::is_same<TContainer, std::string>::value,
              typename TContainer::value_type>::type>
std::ostream& operator<<(std::ostream& ostream, const TContainer& container) {
  ostream << '{';
  for (const T& elem : container) {
    ostream << elem << ", ";
  }
  return ostream << '}';
}

void DebugCerr() { std::cerr << std::endl; }

template <typename Head, typename... Tail>
void DebugCerr(const Head& head, const Tail&... tail) {
  std::cerr << head << " ";
  DebugCerr(tail...);
}

template <typename T>
inline T Min(const std::vector<T>& vec) {
  return *std::min_element(all(vec));
}

template <typename T>
inline T Max(const std::vector<T>& vec) {
  return *std::max_element(all(vec));
}

int RandRange(int min_val, int max_val, bool fix_seed = true) {
  if (fix_seed) {
    return std::abs(static_cast<int>(random_fixed_seed())) %
               (max_val - min_val + 1) +
           min_val;
  }

  return std::abs(static_cast<int>(random_new_seed())) %
             (max_val - min_val + 1) +
         min_val;
}

template <typename T>
void RemoveDuplicates(std::vector<T>& vec) {
  std::sort(all(vec));
  vec.resize(std::unique(all(vec)) - vec.begin());
}

#define debug DebugCerr

}  // namespace MyTemplate

bool Dfs(int cur_v, int goal, std::vector<bool>& used,
         std::vector<std::vector<int>>& graph, std::vector<int>& prev,
         std::vector<std::vector<char>>& edges) {
  std::queue<int> q;
  q.push(cur_v);
  used[cur_v] = true;

  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (int to : graph[v]) {
      if (!used[to] && edges[v][to] != 0) {
        used[to] = true;
        prev[to] = v;
        q.push(to);

        if (to == goal) {
          return true;
        }
      }
    }
  }

  return false;
}

int MinCut(int s, int t, std::vector<std::vector<int>>& graph,
           std::vector<std::vector<char>> edges) {
  int vertex_cnt = graph.size();
  int ans = 0;

  while (true) {
    std::vector<bool> used(vertex_cnt, false);
    std::vector<int> prev(vertex_cnt, -1);

    bool found = Dfs(s, t, used, graph, prev, edges);

    if (!found) {
      break;
    }

    int min_found = cInf;
    int cur_v = t;

    while (cur_v != s) {
      if (edges[prev[cur_v]][cur_v] < min_found) {
        min_found = edges[prev[cur_v]][cur_v];
      }

      cur_v = prev[cur_v];
    }

    ans += min_found;
    cur_v = t;

    while (cur_v != s) {
      edges[cur_v][prev[cur_v]] += min_found;
      edges[prev[cur_v]][cur_v] -= min_found;
      cur_v = prev[cur_v];
    }
  }

  return ans;
}

void FindAnswer(std::pair<int, int> max, std::pair<int, int> verts,
                int max_flow, std::vector<std::vector<int>>& graph,
                std::vector<std::vector<char>>& edges,
                std::pair<std::vector<bool>&, std::vector<std::pair<int, int>>&>
                    can_block_pair) {
  int max_x = max.first;
  int max_y = max.second;
  int start = verts.first;
  int end = verts.second;
  std::vector<bool>& can_block = can_block_pair.first;
  std::vector<std::pair<int, int>>& can_block_vec = can_block_pair.second;

  std::vector<std::pair<int, int>> ans;

  for (std::pair<int, int> pair : can_block_vec) {
    int x = pair.first;
    int y = pair.second;
    int v = y * max_x + x;

    int old1 = edges[v][v + max_x * max_y];
    int old2 = edges[v + max_x * max_y][v];

    edges[v][v + max_x * max_y] = 0;
    edges[v + max_x * max_y][v] = 0;

    int new_max_flow = MinCut(start, end, graph, edges);

    if (new_max_flow < max_flow) {
      std::cout << x + 1 << " " << y + 1 << "\n";
      max_flow = new_max_flow;
    } else {
      edges[v][v + max_x * max_y] = old1;
      edges[v + max_x * max_y][v] = old2;
    }
    if (max_flow == 0) {
      return;
    }
  }
}

int Dist(std::pair<int, int> first, int second_x, int second_y) {
  int first_x = first.first;
  int first_y = first.second;
  return std::abs(first_x - second_x) + std::abs(first_y - second_y);
}

void Solve() {
  int max_x;
  int max_y;
  std::cin >> max_x >> max_y;
  int already_blocked_cnt;
  int can_block_cnt;
  std::cin >> already_blocked_cnt >> can_block_cnt;
  std::vector<bool> already_blocked(max_x * max_y, false);
  std::vector<bool> can_block(max_x * max_y, false);
  std::vector<std::pair<int, int>> can_block_vec;
  for (int i = 0; i < already_blocked_cnt; ++i) {
    int x;
    int y;
    std::cin >> x >> y;
    already_blocked[(y - 1) * max_x + x - 1] = true;
  }

  for (int i = 0; i < can_block_cnt; ++i) {
    int x;
    int y;
    std::cin >> x >> y;
    can_block[(y - 1) * max_x + x - 1] = true;
    can_block_vec.push_back({x - 1, y - 1});
  }

  int start_x;
  int start_y;
  int end_x;
  int end_y;
  std::cin >> start_x >> start_y >> end_x >> end_y;

  std::vector<std::vector<int>> graph(2 * max_x * max_y);
  std::vector<std::vector<char>> edges(2 * max_x * max_y,
                                       std::vector<char>(2 * max_x * max_y, 0));

  std::sort(all(can_block_vec), [start_x, end_x, start_y, end_y](
                                    std::pair<int, int> first,
                                    std::pair<int, int> second) {
    return std::min(Dist(first, start_x, start_y), Dist(first, end_x, end_y)) <
           std::min(Dist(second, start_x, start_y), Dist(second, end_x, end_y));
  });

  for (int x = 0; x < max_x; ++x) {
    for (int y = 0; y < max_y; ++y) {
      int v = y * max_x + x;

      if (already_blocked[v]) {
        continue;
      }

      int down = v + max_x + max_x * max_y;
      int up = v - max_x + max_x * max_y;
      int left = v - 1 + max_x * max_y;
      int right = v + 1 + max_x * max_y;

      graph[v + max_x * max_y].push_back(v);
      graph[v].push_back(v + max_x * max_y);

      if (x > 0) {
        graph[v].push_back(left);
        graph[left].push_back(v);
        edges[v][left] = cInf;
      }
      if (y > 0) {
        graph[v].push_back(up);
        graph[up].push_back(v);
        edges[v][up] = cInf;
      }
      if (x < max_x - 1) {
        graph[v].push_back(right);
        graph[right].push_back(v);
        edges[v][right] = cInf;
      }
      if (y < max_y - 1) {
        graph[v].push_back(down);
        graph[down].push_back(v);
        edges[v][down] = cInf;
      }

      if (can_block[v]) {
        edges[v + max_x * max_y][v] = 1;
      } else {
        edges[v + max_x * max_y][v] = cInf;
      }
    }
  }

  int start = (start_y - 1) * max_x + start_x - 1;
  int end = (end_y - 1) * max_x + end_x - 1;

  int max_flow = MinCut(start, end, graph, edges);

  if (max_flow > cInf / 2) {
    std::cout << -1;
    return;
  }

  std::cout << max_flow << "\n";
  FindAnswer({max_x, max_y}, {start, end}, max_flow, graph, edges,
             {can_block, can_block_vec});
}

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int test_cases = 1;
  // std::cin >> test_cases;

  while (test_cases-- > 0) {
    Solve();
  }
}