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
// constexpr int cInf = 2e9 + 9;
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

void Dfs(Triplet<int, int, int> ints, std::vector<int>& tin,
         std::vector<int>& tout, int& timer, std::vector<std::vector<int>>& up,
         std::pair<std::vector<std::vector<int>>&, std::vector<int>&> pair) {
  std::vector<std::vector<int>>& graph = pair.first;
  std::vector<int>& heights = pair.second;
  int cur_v = ints.first;
  int p = ints.second;
  int depth = ints.third;
  tin[cur_v] = ++timer;
  up[cur_v][0] = p;

  for (int i = 1; i <= depth; ++i) {
    up[cur_v][i] = up[up[cur_v][i - 1]][i - 1];
  }

  for (int next : graph[cur_v]) {
    if (next != p) {
      heights[next] = heights[cur_v] + 1;
      Dfs({next, cur_v, depth}, tin, tout, timer, up, {graph, heights});
    }
  }

  tout[cur_v] = ++timer;
}

bool IsAncestor(int v, int u, std::vector<int>& tin, std::vector<int>& tout) {
  return tin[v] <= tin[u] && tout[v] >= tout[u];
}

int FindAnswer(int u, int v, std::vector<int>& tin, std::vector<int>& tout,
               std::vector<std::vector<int>>& up, int depth) {
  if (IsAncestor(u, v, tin, tout)) {
    return u;
  }

  if (IsAncestor(v, u, tin, tout)) {
    return v;
  }

  for (int i = depth; i >= 0; --i) {
    if (!IsAncestor(up[u][i], v, tin, tout)) {
      u = up[u][i];
    }
  }

  return up[u][0];
}

void Solve() {
  int cnt;
  std::cin >> cnt;

  if (cnt == 1) {
    int qst;
    std::cin >> qst;
    for (int i = 0; i < qst; ++i) {
      std::cout << 0 << "\n";
    }
    return;
  }

  std::vector<std::vector<int>> graph(cnt);

  for (int i = 0; i < cnt - 1; ++i) {
    int v;
    int u;
    std::cin >> v >> u;
    graph[v - 1].push_back(u - 1);
    graph[u - 1].push_back(v - 1);
  }

  std::vector<int> tin(cnt);
  std::vector<int> tout(cnt);
  int depth = 1;
  while ((1 << depth) <= cnt) {
    ++depth;
  }
  std::vector<std::vector<int>> up(cnt, std::vector<int>(depth + 1));
  int timer = 0;

  std::vector<int> heights(cnt, 0);

  Dfs({0, 0, depth}, tin, tout, timer, up, {graph, heights});

  int qst;
  std::cin >> qst;

  while (qst-- > 0) {
    int u;
    int v;
    std::cin >> u >> v;
    --u;
    --v;
    int w = FindAnswer(u, v, tin, tout, up, depth);
    std::cout << heights[u] - heights[w] + heights[v] - heights[w] << "\n";
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