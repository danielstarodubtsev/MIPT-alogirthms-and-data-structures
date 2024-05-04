#include <algorithm>
#include <cassert>
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

constexpr ll cMySchool = 179;
// constexpr ll cInf = 2e9 + 9;
constexpr ll cInf = 2e18 + 9;

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

ll RandRange(ll min_val, ll max_val, bool fix_seed = true) {
  if (fix_seed) {
    return std::abs(static_cast<ll>(random_fixed_seed())) %
               (max_val - min_val + 1) +
           min_val;
  }

  return std::abs(static_cast<ll>(random_new_seed())) %
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

const ll cInitialFlow = 1LL << 31LL;

void Bfs(ll cur_v, std::vector<bool>& used, std::vector<std::vector<ll>>& graph,
         std::vector<std::vector<ll>>& edges, std::vector<ll>& depths,
         ll min_flow) {
  std::queue<ll> q;
  q.push(cur_v);
  used[cur_v] = true;
  depths[cur_v] = 0;

  while (!q.empty()) {
    ll v = q.front();
    q.pop();

    for (ll to : graph[v]) {
      if (!used[to] && edges[v][to] >= min_flow) {
        depths[to] = depths[v] + 1;
        used[to] = true;
        q.push(to);
      }
    }
  }
}

ll Dfs(Triplet<ll, ll, ll> triplet, std::vector<ll>& ptr,
       std::vector<std::vector<ll>>& graph, std::vector<ll>& depths,
       std::vector<std::vector<ll>>& edges, ll min_flow) {
  ll v = triplet.first;
  ll t = triplet.second;
  ll f = triplet.third;
  if (v == t) {
    return f;
  }

  while (ptr[v] < (ll)(graph[v].size())) {
    ll to = graph[v][ptr[v]];
    if (depths[v] + 1 != depths[to] || edges[v][to] < min_flow) {
      ++ptr[v];
      continue;
    }

    ll x = Dfs({to, t, std::min(f, edges[v][to])}, ptr, graph, depths, edges,
               min_flow);

    if (x == 0) {
      ++ptr[v];
      continue;
    }

    edges[v][to] -= x;
    edges[to][v] += x;

    return x;
  }

  return 0;
}

ll Dinic(ll s, ll t, std::vector<std::vector<ll>>& graph,
         std::vector<std::vector<ll>>& edges, ll min_flow) {
  ll vertex_cnt = graph.size();
  ll f = 0;

  while (true) {
    std::vector<bool> used(vertex_cnt, false);
    std::vector<ll> depths(vertex_cnt, cInf);

    Bfs(s, used, graph, edges, depths, min_flow);

    if (depths[t] == cInf) {
      return f;
    }

    std::vector<ll> ptr(vertex_cnt, 0);

    f += Dfs({s, t, cInf}, ptr, graph, depths, edges, min_flow);
  }

  return 0;
}

void Solve() {
  ll vertex_cnt;
  ll edge_cnt;
  std::cin >> vertex_cnt >> edge_cnt;

  std::vector<std::vector<ll>> graph(vertex_cnt);
  std::vector<std::vector<ll>> edges(vertex_cnt,
                                     std::vector<ll>(vertex_cnt, 0));
  std::vector<Triplet<ll, ll, ll>> edges_vec(edge_cnt);

  for (ll i = 0; i < edge_cnt; ++i) {
    ll from;
    ll to;
    ll cap;
    std::cin >> from >> to >> cap;
    --from;
    --to;

    if (edges[from][to] == 0) {
      graph[from].push_back(to);
      graph[to].push_back(from);
    }

    edges[from][to] += cap;
    edges_vec[i] = {from, to, cap};
  }

  std::vector<std::vector<ll>> edges_copy = edges;
  ll flow_try = cInitialFlow;
  ll ans = 0;

  while (flow_try > 0) {
    ans += Dinic(0, vertex_cnt - 1, graph, edges, flow_try);
    flow_try >>= 1;
  }

  std::cout << ans << "\n";

  for (Triplet<ll, ll, ll> edge : edges_vec) {
    ll from = edge.first;
    ll to = edge.second;
    ll cap = edge.third;
    ll flow = edges_copy[from][to] - edges[from][to];
    if (flow > cap) {
      flow = cap;
    }
    if (flow < 0) {
      flow = 0;
    }
    edges_copy[from][to] -= cap;
    std::cout << flow << "\n";
  }
}

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  ll test_cases = 1;
  // std::cin >> test_cases;

  while (test_cases-- > 0) {
    Solve();
  }
}