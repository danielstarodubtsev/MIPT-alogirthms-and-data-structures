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

template <typename A, typename B, typename C>
struct Triplet {
  A first;
  B second;
  C third;
};

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
  int qst;
  std::cin >> vertex_cnt >> edge_cnt >> qst;

  for (int i = 0; i < edge_cnt; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
  }

  DSU dsu(vertex_cnt + 1);
  for (int vertex = 1; vertex <= vertex_cnt; ++vertex) {
    dsu.Add(vertex);
  }

  std::vector<Triplet<std::string, int, int>> quests(qst);

  for (int i = 0; i < qst; ++i) {
    std::string q;
    int u;
    int v;
    std::cin >> q >> u >> v;
    quests[i] = {q, u, v};
  }

  std::vector<bool> ans;

  std::reverse(quests.begin(), quests.end());

  for (Triplet<std::string, int, int> cur : quests) {
    std::string q = cur.first;
    int u = cur.second;
    int v = cur.third;

    if (q == "ask") {
      ans.push_back(dsu.Check(u, v));
    } else {
      dsu.Unite(u, v);
    }
  }

  std::reverse(ans.begin(), ans.end());

  for (bool b : ans) {
    std::cout << (b ? "YES" : "NO") << "\n";
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
