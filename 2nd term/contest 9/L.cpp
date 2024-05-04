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
constexpr int cInf = 2e9 + 9;
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
         std::vector<std::vector<int>>& graph, std::vector<int>& next,
         std::map<std::pair<int, int>, std::pair<int, int>>& edges) {
  used[cur_v] = true;

  if (cur_v == goal) {
    return true;
  }

  for (int to : graph[cur_v]) {
    if (!used[to] && edges[{cur_v, to}].first != 0) {
      next[cur_v] = to;
      if (Dfs(to, goal, used, graph, next, edges)) {
        return true;
      }
    }
  }

  return false;
}

void Dfs2(int cur_v, std::vector<std::vector<int>>& graph,
          std::vector<bool>& used,
          std::map<std::pair<int, int>, std::pair<int, int>>& edges) {
  used[cur_v] = true;

  for (int to : graph[cur_v]) {
    if (!used[to] && edges[{cur_v, to}].first != 0) {
      Dfs2(to, graph, used, edges);
    }
  }
}

int MinCut(int s, int t, std::vector<std::vector<int>>& graph,
           std::map<std::pair<int, int>, std::pair<int, int>>& edges) {
  int vertex_cnt = graph.size();
  int ans = 0;

  while (true) {
    std::vector<bool> used(vertex_cnt, false);
    std::vector<int> next(vertex_cnt, -1);

    bool found = Dfs(s, t, used, graph, next, edges);

    if (!found) {
      break;
    }

    int min_found = cInf;
    int cur_v = s;
    std::pair<int, int> best_edge;

    while (cur_v != t) {
      if (edges[{cur_v, next[cur_v]}].first < min_found) {
        min_found = edges[{cur_v, next[cur_v]}].first;
        best_edge = {cur_v, next[cur_v]};
      }

      cur_v = next[cur_v];
    }

    ans += min_found;
    cur_v = s;

    while (cur_v != t) {
      edges[{cur_v, next[cur_v]}].first -= min_found;
      edges[{next[cur_v], cur_v}].first += min_found;
      cur_v = next[cur_v];
    }
  }

  return ans;
}

void Solve() {
  int vertex_cnt;
  int edge_cnt;

  std::cin >> vertex_cnt >> edge_cnt;

  std::vector<std::vector<int>> graph(vertex_cnt);
  std::map<std::pair<int, int>, std::pair<int, int>> edges;

  for (int i = 0; i < edge_cnt; ++i) {
    int from;
    int to;
    int weight;
    std::cin >> from >> to >> weight;
    graph[from - 1].push_back(to - 1);
    graph[to - 1].push_back(from - 1);
    edges[{from - 1, to - 1}] = {weight, i + 1};
    edges[{to - 1, from - 1}] = {weight, i + 1};
  }

  int max_flow = MinCut(0, vertex_cnt - 1, graph, edges);

  std::vector<bool> used(vertex_cnt, false);
  Dfs2(0, graph, used, edges);

  std::vector<std::pair<int, int>> ans;

  for (int from = 0; from < vertex_cnt; ++from) {
    for (int to = 0; to < vertex_cnt; ++to) {
      if (edges.find({from, to}) != edges.end() && used[from] && !used[to]) {
        ans.push_back({from, to});
      }
    }
  }

  std::cout << ans.size() << " " << max_flow << "\n";
  for (std::pair<int, int>& edge : ans) {
    std::cout << edges[edge].second << " ";
  }
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