#include <iostream>
#include <queue>
#include <string>
#include <vector>

const int cAlphabetSize = 2;

std::vector<int> parent = {-1};
std::vector<int> link = {-1};
std::vector<std::vector<int>> step = {std::vector<int>(cAlphabetSize, -1)};
std::vector<bool> term = {false};
std::vector<int> compressed = {-1};
std::vector<bool> has_term = {false};
std::vector<std::vector<int>> children = {std::vector<int>(cAlphabetSize, -1)};
std::vector<char> char_to_parent = {-1};

bool Dfs(int cur, std::vector<char>& used) {
  used[cur] = 1;
  if ((compressed[cur] != -1 && term[compressed[cur]]) || has_term[cur]) {
    used[cur] = 2;
    return false;
  }
  for (int next : children[cur]) {
    if (next == -1 || used[next] == 2) {
      continue;
    }
    if (used[next] == 1) {
      return true;
    }
    if (!has_term[next] && Dfs(next, used)) {
      return true;
    }
  }
  for (int next : step[cur]) {
    if (used[next] == 2) {
      continue;
    }
    if (used[next] == 1) {
      return true;
    }
    if (!has_term[next] && Dfs(next, used)) {
      return true;
    }
  }
  used[cur] = 2;
  return false;
}

signed main() {
  int root = 0;
  int n;
  std::cin >> n;
  std::string cur;
  int total_nodes = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> cur;
    int node = root;
    for (char c : cur) {
      if (children[node][c - '0'] == -1) {
        ++total_nodes;
        parent.push_back(node);
        link.push_back(-1);
        compressed.push_back(-1);
        step.push_back(std::vector<int>(cAlphabetSize, -1));
        term.push_back(false);
        children.push_back(std::vector<int>(cAlphabetSize, -1));
        char_to_parent.push_back(c);
        children[node][c - '0'] = total_nodes;
        parent[total_nodes] = node;
      }
      node = children[node][c - '0'];
      if (term[node]) {
        break;
      }
    }
    term[node] = true;
  }

  for (int node = 1; node <= total_nodes; ++node) {
    has_term.push_back(has_term[parent[node]] || term[node]);
  }

  for (char c = '0'; c <= '1'; ++c) {
    step[root][c - '0'] =
        (children[root][c - '0'] == -1 ? 0 : children[root][c - '0']);
  }

  std::queue<int> q;
  q.push(root);
  int u;
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (char c = '0'; c <= '1'; ++c) {
      if (children[v][c - '0'] == -1) {
        continue;
      }
      u = children[v][c - '0'];
      if (link[u] == -1) {
        if (parent[u] == root) {
          link[u] = root;
        } else {
          link[u] = step[link[v]][char_to_parent[u] - '0'];
        }
      }
      for (char d = '0'; d <= '1'; ++d) {
        if (children[u][d - '0'] != -1) {
          step[u][d - '0'] = children[u][d - '0'];
        } else {
          step[u][d - '0'] = step[link[u]][d - '0'];
        }
      }
      compressed[u] = (term[link[u]] ? link[u] : compressed[link[u]]);
      if (compressed[u] == -1) {
        compressed[u] = root;
      }
      q.push(u);
    }
  }

  std::vector<char> used(total_nodes + 1, 0);
  std::cout << (Dfs(0, used) ? "TAK" : "NIE");
}