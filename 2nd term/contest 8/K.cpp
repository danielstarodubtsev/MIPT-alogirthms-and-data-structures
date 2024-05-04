#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

const int cInf = 2e9 + 7;

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

void Expand(int cur_vertex, int from_where,
            std::vector<std::vector<std::pair<int, int>>>& graph,
            std::vector<std::pair<int, int>>& dist,
            std::priority_queue<std::pair<int, std::pair<int, int>>>& q,
            std::vector<bool>& used) {
  if (used[cur_vertex]) {
    return;
  }

  for (std::pair<int, int> pair : graph[cur_vertex]) {
    int to_vertex = pair.first;
    int edge_weight = pair.second;

    if (dist[cur_vertex].first + edge_weight < dist[to_vertex].first) {
      dist[to_vertex].first = dist[cur_vertex].first + edge_weight;
      dist[to_vertex].second = dist[cur_vertex].second;
      q.push({-dist[to_vertex].first, {to_vertex, from_where}});
    }
  }

  used[cur_vertex] = true;
}

int main() {
  int vertex_cnt;
  int special_vertex_cnt;
  int edge_cnt;
  std::cin >> vertex_cnt >> special_vertex_cnt >> edge_cnt;
  std::vector<int> special_vertices(special_vertex_cnt);
  for (int i = 0; i < special_vertex_cnt; ++i) {
    int a;
    std::cin >> a;
    special_vertices[i] = a - 1;
  }
  std::vector<std::vector<std::pair<int, int>>> graph(vertex_cnt);
  for (int i = 0; i < edge_cnt; ++i) {
    int from;
    int to;
    int cost;
    std::cin >> to >> from >> cost;
    --to;
    --from;
    graph[from].push_back({to, cost});
    graph[to].push_back({from, cost});
  }
  int qst;
  std::cin >> qst;
  // {{max_cost, quest_index}, {from, to}}
  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> quests;
  for (int i = 0; i < qst; ++i) {
    int from;
    int to;
    int max_cost;
    std::cin >> from >> to >> max_cost;
    quests.push_back({{max_cost, i}, {from - 1, to - 1}});
  }
  std::sort(quests.begin(), quests.end());
  // {distance, closest_special_vertex_index}
  std::vector<std::pair<int, int>> closest_need(vertex_cnt, {-1, -1});
  for (int vertex : special_vertices) {
    closest_need[vertex] = {0, vertex};
  }

  // {min_dist, {vertex, from_where}}
  std::priority_queue<std::pair<int, std::pair<int, int>>> q;
  std::vector<bool> used(vertex_cnt, false);
  std::vector<std::pair<int, int>> dist(vertex_cnt, {cInf, -1});

  for (int vertex : special_vertices) {
    q.push({0, {vertex, vertex}});
    dist[vertex] = {0, vertex};
  }

  while (!q.empty()) {
    int next = q.top().second.first;
    int from_where = q.top().second.second;
    q.pop();
    Expand(next, from_where, graph, dist, q, used);
  }

  closest_need = dist;

  // {distance, {from, to}}
  std::vector<std::pair<int, std::pair<int, int>>> edges;
  for (int start = 0; start < vertex_cnt; ++start) {
    for (std::pair<int, int>& pair : graph[start]) {
      int end = pair.first;
      int dist =
          pair.second + closest_need[start].first + closest_need[end].first;
      edges.push_back({-dist, {start, end}});
    }
  }
  std::sort(edges.begin(), edges.end());
  DSU dsu(vertex_cnt);
  for (int vertex : special_vertices) {
    dsu.Add(vertex);
  }
  std::vector<bool> answer(qst);
  for (std::pair<std::pair<int, int>, std::pair<int, int>>& quest : quests) {
    int max_cost = quest.first.first;
    int qst_index = quest.first.second;
    int start = quest.second.first;
    int end = quest.second.second;
    while (!edges.empty() && -edges.back().first <= max_cost) {
      dsu.Unite(closest_need[edges.back().second.first].second,
                closest_need[edges.back().second.second].second);
      edges.pop_back();
    }
    answer[qst_index] = dsu.Check(start, end);
  }
  for (bool b : answer) {
    std::cout << (b ? "YES" : "NO") << "\n";
  }
}