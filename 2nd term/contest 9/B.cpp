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

struct Node {
  Node* parent = nullptr;
  int value = -1;
  int subtree_depth = 1;

  Node(int val) : value(val) {}

  Node* GetRoot() {
    if (parent == nullptr) {
      return this;
    }

    return parent = parent->GetRoot();
  }
};

struct DSU {
  std::vector<Node*> pointers;

  bool Check(int first, int second) {
    return pointers[first]->GetRoot() == pointers[second]->GetRoot();
  }

  void Unite(int first, int second) {
    Node* first_root = pointers[first]->GetRoot();
    Node* second_root = pointers[second]->GetRoot();

    if (first_root == second_root) {
      return;
    }

    if (first_root->subtree_depth < second_root->subtree_depth) {
      first_root->parent = second_root;
    } else {
      second_root->parent = first_root;
      first_root->subtree_depth =
          std::min(first_root->subtree_depth, second_root->subtree_depth + 1);
    }
  }

  void Add(int element) {
    Node* new_node = new Node(element);
    pointers[element] = new_node;
  }

  DSU(int max_int) { pointers.resize(max_int); }

  DSU(std::vector<std::vector<int>>& sets) {
    for (std::vector<int>& cur : sets) {
      if (!cur.empty()) {
        for (int elem : cur) {
          Add(elem);
          Unite(elem, cur[0]);
        }
      }
    }
  }

  ~DSU() {
    for (Node* node : pointers) {
      delete node;
    }
  }
};

void Solve() {
  int vertex_cnt;
  int edge_cnt;
  std::cin >> vertex_cnt >> edge_cnt;

  std::vector<Triplet<int, int, int>> edges;
  for (int i = 0; i < edge_cnt; ++i) {
    int from;
    int to;
    int weight;
    std::cin >> from >> to >> weight;
    edges.push_back({weight, from - 1, to - 1});
  }

  std::sort(all(edges));

  DSU dsu(vertex_cnt);

  for (int vertex = 0; vertex < vertex_cnt; ++vertex) {
    dsu.Add(vertex);
  }

  int res = 0;

  for (Triplet<int, int, int> edge : edges) {
    if (!dsu.Check(edge.second, edge.third)) {
      res += edge.first;
      dsu.Unite(edge.second, edge.third);
    }
  }

  std::cout << res;
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