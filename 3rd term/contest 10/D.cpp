// ! This gets TL/WA

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

const long long cB = 31;
const long long cMod = 2e9 + 11;
const long long cMaxN = 1e6 + 7;
const long long cMod2 = (9223372036854775807 / cMod / 2) * cMod;
const float cLoadFactor = 1;
const int cHashTableSize = 13'000'100;

struct Hash {
  size_t operator()(const long long& num) const {
    return (num * cB + cMaxN) % cMod;
  }
};

int DoStuff(
    std::pair<std::pair<int, int>, std::pair<int, int>> ints,
    std::pair<std::vector<std::string>&, std::vector<std::string>&> grids,
    std::vector<long long>& pow,
    std::unordered_set<long long, Hash>& possible_hashes,
    std::vector<long long>& row_hash,
    std::vector<std::vector<long long>>& column_hash) {
  int height = ints.first.first;
  int width = ints.first.second;
  int target_h = ints.second.first;
  int target_w = ints.second.second;
  std::vector<std::string>& grid = grids.first;
  std::vector<std::string>& target = grids.second;
  int ans = 0;
  long long start_hash = 0;
  for (int y = height - target_h; y < height; ++y) {
    for (int x = width - target_w; x < width; ++x) {
      start_hash =
          (start_hash +
           grid[y][x] *
               pow[(y - height + target_h) * target_w + x - width + target_w]) %
          cMod;
    }
  }
  for (int start_y = height - target_h; start_y >= 0; --start_y) {
    int start_x = width - target_w;
    long long cur_hash = start_hash;
    while (true) {
      if (possible_hashes.find(cur_hash) != possible_hashes.end() &&
          ((target.size() < 4 || target[0].size() < 4) ||
           (grid[start_y][start_x] == target[0][0] &&
            grid[start_y + 1][start_x] == target[1][0] &&
            grid[start_y][start_x + 1] == target[0][1]) ||
           (grid[start_y][start_x] == target[0][0] &&
            grid[start_y + 1][start_x] == target[1][0] &&
            grid[start_y + 1][start_x + 1] == target[1][1]) ||
           (grid[start_y + 1][start_x + 1] == target[1][1] &&
            grid[start_y + 1][start_x] == target[1][0] &&
            grid[start_y][start_x + 1] == target[0][1]) ||
           (grid[start_y][start_x] == target[0][0] &&
            grid[start_y + 1][start_x + 1] == target[1][1] &&
            grid[start_y][start_x + 1] == target[0][1]))) {
        ++ans;
      }
      if (start_x == 0) {
        break;
      }
      --start_x;
      cur_hash = (cur_hash -
                  column_hash[start_x + target_w][start_y] * pow[target_w - 1] +
                  cMod2) %
                 cMod;
      cur_hash = (cur_hash * cB) % cMod;
      cur_hash = (cur_hash + column_hash[start_x][start_y]) % cMod;
    }
    if (start_y != 0) {
      start_hash =
          (start_hash -
           row_hash[start_y + target_h - 1] * pow[(target_h - 1) * target_w] +
           cMod2) %
          cMod;
      start_hash = (start_hash * pow[target_w]) % cMod;
      start_hash = (start_hash + row_hash[start_y - 1]) % cMod;
    }
  }
  return ans;
}

int main() {
  std::vector<long long> pow(cMaxN, 1);
  for (long long i = 1; i < cMaxN; ++i) {
    pow[i] = (pow[i - 1] * cB) % cMod;
  }
  int height;
  int width;
  std::cin >> height >> width;
  std::vector<std::string> grid(height);
  for (std::string& s : grid) {
    std::cin >> s;
  }
  int target_h;
  int target_w;
  std::cin >> target_h >> target_w;
  std::vector<std::string> target(target_h);
  for (std::string& s : target) {
    std::cin >> s;
  }
  std::vector<std::vector<long long>> column_hash(
      width, std::vector<long long>(height - target_h + 1));
  for (int x = 0; x < width; ++x) {
    long long cur = 0;
    for (int y = height - 1; y >= height - target_h; --y) {
      cur = (cur * pow[target_w] + grid[y][x]) % cMod;
    }
    column_hash[x][height - target_h] = cur;
  }
  for (int start_x = 0; start_x < width; ++start_x) {
    for (int start_y = height - target_h - 1; start_y >= 0; --start_y) {
      column_hash[start_x][start_y] =
          (column_hash[start_x][start_y + 1] * pow[target_w] -
           grid[start_y + target_h][start_x] * pow[target_h * target_w] +
           grid[start_y][start_x] + cMod2) %
          cMod;
    }
  }
  std::vector<long long> row_hash(height);
  for (int y = 0; y < height; ++y) {
    long long cur = 0;
    for (int x = width - 1; x >= width - target_w; --x) {
      cur = (cur * cB + grid[y][x]) % cMod;
    }
    row_hash[y] = cur;
  }
  long long target_base_hash = 0;
  for (int y = 0; y < target_h; ++y) {
    for (int x = 0; x < target_w; ++x) {
      target_base_hash =
          (target_base_hash + target[y][x] * pow[y * target_w + x]) % cMod;
    }
  }
  int ans = 0;
  std::unordered_set<long long, Hash> possible_hashes;
  possible_hashes.max_load_factor(cLoadFactor);
  possible_hashes.reserve(cHashTableSize);
  bool counted_same = false;
  for (int y = 0; y < target_h; ++y) {
    for (int x = 0; x < target_w; ++x) {
      for (char c = 'a'; c <= 'l'; ++c) {
        if (c == target[y][x]) {
          counted_same = true;
        }
        possible_hashes.insert((target_base_hash +
                                (c - target[y][x]) * pow[y * target_w + x] +
                                cMod2) %
                               cMod);
      }
    }
  }
  ans += DoStuff({{height, width}, {target_h, target_w}}, {grid, target}, pow,
                 possible_hashes, row_hash, column_hash);
  possible_hashes.clear();
  possible_hashes.reserve(cHashTableSize);
  for (int y = 0; y < target_h; ++y) {
    for (int x = 0; x < target_w; ++x) {
      for (char c = 'm'; c <= 'z'; ++c) {
        if (c != target[y][x] || !counted_same) {
          possible_hashes.insert((target_base_hash +
                                  (c - target[y][x]) * pow[y * target_w + x] +
                                  cMod2) %
                                 cMod);
        }
      }
    }
  }
  ans += DoStuff({{height, width}, {target_h, target_w}}, {grid, target}, pow,
                 possible_hashes, row_hash, column_hash);
  std::cout << ans;
}
