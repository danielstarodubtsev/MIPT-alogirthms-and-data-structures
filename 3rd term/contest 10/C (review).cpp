#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

const int cAlphabetSize = 26;

std::vector<int> parent = {-1};
std::vector<int> link = {-1};
std::vector<std::vector<int>> step = {std::vector<int>(cAlphabetSize, -1)};
std::vector<int> compressed = {-1};
std::vector<bool> term = {false};
std::vector<std::unordered_map<int, int>> children = {
    std::unordered_map<int, int>()};
std::vector<char> char_to_parent = {-1};
std::vector<std::vector<int>> met = {std::vector<int>()};

void Dfs(int cur, std::string& s, std::unordered_map<std::string, int>& ans) {
  if (cur == -1) {
    return;
  }
  if (term[cur]) {
    ans[s] = cur;
  }
  for (char c = 'a'; c <= 'z'; ++c) {
    if (children[cur].find(c - 'a') != children[cur].end()) {
      s += c;
      Dfs(children[cur][c - 'a'], s, ans);
      s.pop_back();
    }
  }
}

signed main() {
  int root = 0;
  std::string text;
  int n;
  std::cin >> text >> n;
  std::string cur;
  std::vector<std::string> strings;
  int total_nodes = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> cur;
    strings.push_back(cur);
    int node = root;
    for (char c : cur) {
      if (children[node].find(c - 'a') == children[node].end()) {
        ++total_nodes;
        parent.push_back(node);
        link.push_back(-1);
        step.push_back(std::vector<int>(cAlphabetSize, -1));
        compressed.push_back(-1);
        term.push_back(false);
        children.push_back(std::unordered_map<int, int>());
        char_to_parent.push_back(c);
        met.push_back(std::vector<int>());
        children[node][c - 'a'] = total_nodes;
        parent[total_nodes] = node;
      }
      node = children[node][c - 'a'];
    }
    term[node] = true;
  }

  for (char c = 'a'; c <= 'z'; ++c) {
    step[root][c - 'a'] = (children[root].find(c - 'a') == children[root].end()
                               ? 0
                               : children[root][c - 'a']);
  }
  compressed[root] = 0;

  std::queue<int> q;
  q.push(root);
  int u;
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (char c = 'a'; c <= 'z'; ++c) {
      if (children[v].find(c - 'a') == children[v].end()) {
        continue;
      }
      u = children[v].at(c - 'a');
      if (link[u] == -1) {
        if (parent[u] == root) {
          link[u] = root;
        } else {
          link[u] = step[link[v]][char_to_parent[u] - 'a'];
        }
      }
      for (char d = 'a'; d <= 'z'; ++d) {
        if (children[u].find(d - 'a') != children[u].end()) {
          step[u][d - 'a'] = children[u][d - 'a'];
        } else {
          step[u][d - 'a'] = step[link[u]][d - 'a'];
        }
      }
      compressed[u] = (term[link[u]] ? link[u] : compressed[link[u]]);
      if (compressed[u] == -1) {
        compressed[u] = root;
      }
      q.push(u);
    }
  }

  int node = root;
  int index = -1;
  for (char c : text) {
    ++index;
    node = step[node][c - 'a'];
    int p = node;
    while (p != -1 && p != root) {
      if (term[p]) {
        met[p].push_back(index);
      }
      p = compressed[p];
    }
  }

  std::unordered_map<std::string, int> ans;
  std::string f;
  Dfs(root, f, ans);

  for (std::string s : strings) {
    std::cout << met[ans[s]].size() << " ";
    for (int i : met[ans[s]]) {
      std::cout << i - s.size() + 2 << " ";
    }
    std::cout << "\n";
  }
}