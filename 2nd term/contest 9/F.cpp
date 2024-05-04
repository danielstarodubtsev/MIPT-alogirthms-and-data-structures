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

constexpr int cBigNum = 1e8 + 179;

std::mt19937 rnd(0);

struct Node {
  const int kBigNum = 1e8 + 1779;
  int value;
  double priority;
  int subtree_size = 0;
  Node* left_son = nullptr;
  Node* right_son = nullptr;

  void UpdateSize() {
    if (left_son == nullptr && right_son == nullptr) {
      subtree_size = 0;
    } else if (left_son == nullptr) {
      subtree_size = right_son->subtree_size + 1;
    } else if (right_son == nullptr) {
      subtree_size = left_son->subtree_size + 1;
    } else {
      subtree_size = left_son->subtree_size + right_son->subtree_size + 2;
    }
  }

  Node(int value) : value(value) { priority = rnd() % kBigNum; }
};

int GetSize(Node* node) {
  if (node == nullptr) {
    return -1;
  }

  return node->subtree_size;
}

int GetIndex(Node* node) { return GetSize(node->left_son) + 1; }

bool Exists(Node* root, int value) {
  if (root == nullptr) {
    return false;
  }

  if (root->value == value) {
    return true;
  }

  if (root->value < value) {
    return Exists(root->right_son, value);
  }

  return Exists(root->left_son, value);
}

Node* Merge(Node* first_root, Node* second_root) {
  if (first_root == nullptr) {
    return second_root;
  }

  if (second_root == nullptr) {
    return first_root;
  }

  if (first_root->priority < second_root->priority) {
    first_root->right_son = Merge(first_root->right_son, second_root);
    first_root->UpdateSize();
    return first_root;
  }

  second_root->left_son = Merge(first_root, second_root->left_son);
  second_root->UpdateSize();
  return second_root;
}

std::pair<Node*, Node*> Split(Node* root, int key) {
  if (root == nullptr) {
    return {nullptr, nullptr};
  }

  if (key >= GetSize(root->left_son)) {
    std::pair<Node*, Node*> new_pair =
        Split(root->right_son, key - GetSize(root->left_son) - 2);
    root->right_son = new_pair.first;
    root->UpdateSize();
    return {root, new_pair.second};
  }

  std::pair<Node*, Node*> new_pair = Split(root->left_son, key);
  root->left_son = new_pair.second;
  root->UpdateSize();
  return {new_pair.first, root};
}

Node* Insert(Node* root, int index_key, int actual_value,
             std::vector<Node*>& ever_allocated) {
  Node* new_node = new Node{actual_value};
  ever_allocated.push_back(new_node);
  std::pair<Node*, Node*> new_nodes = Split(root, index_key);
  root = Merge(new_node, new_nodes.second);
  root = Merge(new_nodes.first, root);
  return root;
}

Node* Erase(Node* root, int index) {
  std::pair<Node*, Node*> new_nodes = Split(root, index - 1);
  std::pair<Node*, Node*> even_newer_nodes = Split(new_nodes.first, index - 2);
  return Merge(even_newer_nodes.first, new_nodes.second);
}

Node* Set(Node* root, int index, int new_value,
          std::vector<Node*>& ever_allocated) {
  root = Erase(root, index);
  root = Insert(root, index - 2, new_value, ever_allocated);
  return root;
}

int Get(Node* root, int left_bound, int right_bound, int min_value,
        int max_value) {
  if (root == nullptr) {
    return 0;
  }

  int answer = 0;

  if (root->value >= min_value && root->value <= max_value &&
      GetIndex(root) >= left_bound && GetIndex(root) <= right_bound) {
    // std::cout << "U";
    answer++;
  }

  if (right_bound < 0) {
    // std::cout << "p";
    return 0;
  }

  if (left_bound < 0 && right_bound <= GetIndex(root)) {
    return answer +
           Get(root->left_son, left_bound, right_bound, min_value, max_value);
  }

  if (left_bound >= GetIndex(root) && right_bound > GetSize(root) + 1) {
    return answer + Get(root->right_son, left_bound - GetIndex(root) - 1,
                        right_bound - GetIndex(root) - 1, min_value, max_value);
  }

  answer += Get(root->left_son, left_bound, right_bound, min_value, max_value);
  answer += Get(root->right_son, left_bound - GetIndex(root) - 1,
                right_bound - GetIndex(root) - 1, min_value, max_value);

  return answer;
}

void Solve() {
  Node* root = nullptr;
  std::vector<Node*> ever_allocated;

  int index = 1;
  int input_index = 1;
  Insert(root, index, input_index, ever_allocated);

  int cnt;
  std::cin >> cnt;

  while (cnt-- > 0) {
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "+") {
      int parent;
      std::cin >> parent;

      ++index;
    } else if (cmd == "-") {
      int vertex;
      std::cin >> vertex;
    } else {
      int first;
      int second;
      std::cin >> first >> second;
    }
  }

  debug("input done");


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