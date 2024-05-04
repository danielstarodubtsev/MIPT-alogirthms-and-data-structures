#include <iostream>
#include <queue>
#include <vector>

const int cInf = 2e9 + 7;
const int cMaxVertexCnt = 1e6;

void Expand(
    int cur_vertex,
    std::pair<std::vector<int>&, std::priority_queue<std::pair<int, int>>&>
        pair,
    std::vector<bool>& used, std::pair<std::pair<int, int>, int> costs,
    std::vector<std::vector<std::vector<int>*>>& in_which_sets) {
  if (used[cur_vertex]) {
    return;
  }

  std::vector<int>& dist = pair.first;
  std::priority_queue<std::pair<int, int>>& q = pair.second;

  for (std::vector<int>* cur_set : in_which_sets[cur_vertex]) {
    for (int to_vertex : *cur_set) {
      int edge_weight = costs.second;

      if (dist[cur_vertex] + edge_weight < dist[to_vertex]) {
        dist[to_vertex] = dist[cur_vertex] + edge_weight;
        q.push({-dist[to_vertex], to_vertex});
      }
    }
  }

  if (cur_vertex < cMaxVertexCnt - 1) {
    int to_vertex = cur_vertex + 1;
    int edge_weight = costs.first.first;

    if (dist[cur_vertex] + edge_weight < dist[to_vertex]) {
      dist[to_vertex] = dist[cur_vertex] + edge_weight;
      q.push({-dist[to_vertex], to_vertex});
    }
  }

  if (cur_vertex > 0) {
    int to_vertex = cur_vertex - 1;
    int edge_weight = costs.first.second;

    if (dist[cur_vertex] + edge_weight < dist[to_vertex]) {
      dist[to_vertex] = dist[cur_vertex] + edge_weight;
      q.push({-dist[to_vertex], to_vertex});
    }
  }

  used[cur_vertex] = true;
}

int main() {
  int target_vertex;
  int cost_floor_up;
  int cost_floor_down;
  int cost_tel_enter;
  int cost_tel_leave;
  int tel_cnt;
  std::cin >> target_vertex >> cost_floor_up >> cost_floor_down >>
      cost_tel_enter >> cost_tel_leave >> tel_cnt;
  --target_vertex;
  std::vector<std::vector<int>> tel_stops(tel_cnt);

  std::vector<std::vector<std::vector<int>*>> in_which_sets(cMaxVertexCnt);

  for (int i = 0; i < tel_cnt; ++i) {
    int stops_cnt;
    std::cin >> stops_cnt;

    for (int j = 0; j < stops_cnt; ++j) {
      int floor;
      std::cin >> floor;
      tel_stops[i].push_back(floor - 1);
      in_which_sets[floor - 1].push_back(&tel_stops[i]);
    }
  }

  std::vector<int> dist_from_start(cMaxVertexCnt, cInf);
  dist_from_start[0] = 0;
  std::priority_queue<std::pair<int, int>> q_from_start;
  std::vector<bool> used_from_start(cMaxVertexCnt, false);
  q_from_start.push({0, 0});

  while (!q_from_start.empty()) {
    int vertex = q_from_start.top().second;
    q_from_start.pop();
    Expand(vertex, {dist_from_start, q_from_start}, used_from_start,
           {{cost_floor_up, cost_floor_down}, cost_tel_enter + cost_tel_leave},
           in_which_sets);
  }

  std::cout << dist_from_start[target_vertex];
}