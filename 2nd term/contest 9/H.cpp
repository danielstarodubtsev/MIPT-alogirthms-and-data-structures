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

bool Augment(int vertex, std::vector<bool>& used,
             std::vector<std::vector<int>>& g, std::vector<int>& match) {
  if (used.at(vertex)) {
    return false;
  }

  used.at(vertex) = true;

  for (int next : g.at(vertex)) {
    if (match.at(next) == -1 || Augment(match.at(next), used, g, match)) {
      match.at(next) = vertex;
      return true;
    }
  }

  return false;
}

void Solve() {
  int height;
  int width;
  int double_cost;
  int single_cost;
  std::cin >> height >> width >> double_cost >> single_cost;

  int total_cnt = 0;
  std::vector<std::vector<bool>> grid(height, std::vector<bool>(width));
  for (int i = 0; i < height; ++i) {
    std::string row;
    std::cin >> row;

    for (int j = 0; j < width; ++j) {
      grid.at(i).at(j) = (row.at(j) == '*');
      if (row.at(j) == '*') {
        ++total_cnt;
      }
    }
  }

  std::vector<int> match(height * width, -1);
  std::vector<std::vector<int>> graph(height * width);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (!grid.at(y).at(x) || (y + x) % 2 == 0) {
        continue;
      }

      if (y > 0 && grid.at(y - 1).at(x)) {
        graph.at(y * width + x).push_back((y - 1) * width + x);
      }

      if (y < height - 1 && grid.at(y + 1).at(x)) {
        graph.at(y * width + x).push_back((y + 1) * width + x);
      }

      if (x > 0 && grid.at(y).at(x - 1)) {
        graph.at(y * width + x).push_back(y * width + x - 1);
      }

      if (x < width - 1 && grid.at(y).at(x + 1)) {
        graph.at(y * width + x).push_back(y * width + x + 1);
      }
    }
  }

  std::vector<bool> used(height * width, false);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if ((x + y) % 2 == 1) {
        int vertex = y * width + x;
        Augment(vertex, used, graph, match);
        std::fill(all(used), false);
      }
    }
  }

  int found = 0;
  for (int i = 0; i < height * width; ++i) {
    if (match.at(i) != -1) {
      ++found;
    }
  }

  if (single_cost * 2 <= double_cost) {
    std::cout << single_cost * total_cnt;
  } else {
    std::cout << found * double_cost + single_cost * (total_cnt - 2 * found);
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