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

void Dfs(int cur, std::vector<std::vector<int>>& graph,
         std::vector<bool>& used) {
  used[cur] = true;

  for (int next : graph[cur]) {
    if (!used[next]) {
      Dfs(next, graph, used);
    }
  }
}

void Solve() {
  int l;
  int r;
  std::cin >> l >> r;

  std::vector<std::vector<int>> graph(l);
  for (int i = 0; i < l; ++i) {
    int cnt;
    std::cin >> cnt;
    for (int j = 0; j < cnt; ++j) {
      int to;
      std::cin >> to;
      graph[i].push_back(to - 1);
    }
  }

  std::vector<int> match(r, -1);
  for (int i = 0; i < l; ++i) {
    int to;
    std::cin >> to;
    if (to != 0) {
      match[to - 1] = i;
    }
  }

  std::vector<std::vector<int>> new_graph(l + r);

  for (int from = 0; from < l; ++from) {
    for (int to : graph[from]) {
      if (match[to] == from) {
        new_graph[l + to].push_back(from);
      } else {
        new_graph[from].push_back(l + to);
      }
    }
  }
  std::unordered_set<int> set;
  for (int i = 0; i < r; ++i) {
    set.insert(match[i]);
  }

  std::vector<bool> used(l + r, false);
  for (int v = 0; v < l; ++v) {
    if (set.find(v) == set.end() && !used[v]) {
      Dfs(v, new_graph, used);
    }
  }
  std::vector<int> first_ans;
  std::vector<int> second_ans;

  for (int v = 0; v < l; ++v) {
    if (!used[v]) {
      first_ans.push_back(v + 1);
    }
  }

  for (int v = l; v < l + r; ++v) {
    if (used[v]) {
      second_ans.push_back(v - l + 1);
    }
  }
  std::cout << first_ans.size() + second_ans.size() << "\n";
  std::cout << first_ans.size() << " ";
  for (int i : first_ans) {
    std::cout << i << " ";
  }
  std::cout << "\n" << second_ans.size() << " ";
  for (int i : second_ans) {
    std::cout << i << " ";
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