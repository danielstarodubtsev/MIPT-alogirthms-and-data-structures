#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const int kInf = 2e9 + 9;

std::vector<std::vector<int>> BuildDp(int boy_cnt, int girl_cnt,
                                      std::vector<std::pair<int, int>> boys,
                                      std::vector<std::pair<int, int>> girls) {
  std::vector<std::vector<int>> dp(boy_cnt + 1,
                                   std::vector<int>(girl_cnt + 1, kInf));
  dp[0][0] = 0;

  for (int i = 0; i <= boy_cnt; ++i) {
    for (int j = 0; j <= girl_cnt; ++j) {
      for (int next_boys = i + 1; next_boys <= std::min(boy_cnt, i + 4);
           ++next_boys) {
        for (int next_girls = j; next_girls <= std::min(girl_cnt, j + 3);
             ++next_girls) {
          if (next_boys + next_girls - i - j <= 4) {
            dp[next_boys][next_girls] = std::min(
                dp[next_boys][next_girls],
                dp[i][j] + std::max(boys[i].first,
                                    next_girls != j ? girls[j].first : 0));
          }
        }
      }
    }
  }

  return dp;
}

std::vector<std::pair<int, int>> MakeTaxis(
    int x_coord, int y_coord, std::vector<std::pair<int, int>> boys,
    std::vector<std::pair<int, int>> girls, std::vector<std::vector<int>> dp) {
  std::vector<std::pair<int, int>> result(1, {x_coord, y_coord});

  while (x_coord + y_coord != 0) {
    for (int boy_coord = 1; boy_coord <= std::min(4, x_coord); ++boy_coord) {
      bool found = false;
      for (int girl_coord = 0; girl_coord <= std::min(4, y_coord);
           ++girl_coord) {
        if (boy_coord + girl_coord > 4) {
          continue;
        }

        int first = boys[x_coord - boy_coord].first;
        int second = girl_coord != 0 ? girls[y_coord - girl_coord].first : 0;
        if (dp[x_coord][y_coord] ==
            dp[x_coord - boy_coord][y_coord - girl_coord] +
                std::max(first, second)) {
          x_coord -= boy_coord;
          y_coord -= girl_coord;
          result.push_back({x_coord, y_coord});
          found = true;
        }
      }
      if (found) {
        break;
      }
    }
  }

  return result;
}

void PrintNames(std::vector<std::string> names) {
  if (names.size() == 1) {
    std::cout << names[0] << ".\n";
  } else if (names.size() == 2) {
    std::cout << names[0] << " and " << names[1] << ".\n";
  } else if (names.size() == 3) {
    std::cout << names[0] << ", " << names[1] << " and " << names[2] << ".\n";
  } else {
    std::cout << names[0] << ", " << names[1] << ", " << names[2] << " and "
              << names[3] << ".\n";
  }
}

int main() {
  int boy_cnt;
  std::cin >> boy_cnt;
  std::vector<std::string> boys_names(boy_cnt);
  std::vector<std::pair<int, int>> boys_coord(boy_cnt);
  int coord;
  for (int i = 0; i < boy_cnt; ++i) {
    std::cin >> boys_names[i];
    std::cin >> coord;
    boys_coord[i] = {coord, i};
  }
  std::sort(boys_coord.begin(), boys_coord.end());
  std::reverse(boys_coord.begin(), boys_coord.end());
  int girl_cnt;
  std::cin >> girl_cnt;
  std::vector<std::string> girls_names(girl_cnt);
  std::vector<std::pair<int, int>> girls_coord(girl_cnt);
  for (int i = 0; i < girl_cnt; ++i) {
    std::cin >> girls_names[i];
    std::cin >> coord;
    girls_coord[i] = {coord, i};
  }
  std::sort(girls_coord.begin(), girls_coord.end());
  std::reverse(girls_coord.begin(), girls_coord.end());
  std::vector<std::vector<int>> dp =
      BuildDp(boy_cnt, girl_cnt, boys_coord, girls_coord);
  std::vector<std::pair<int, int>> result =
      MakeTaxis(boy_cnt, girl_cnt, boys_coord, girls_coord, dp);
  std::cout << dp[boy_cnt][girl_cnt] << "\n";
  std::cout << result.size() - 1 << "\n";

  for (size_t i = 0; i < result.size() - 1; ++i) {
    std::cout << "Taxi " << i + 1 << ": ";
    std::vector<std::string> names;

    for (int j = result[i + 1].second; j <= result[i].second - 1; ++j) {
      names.push_back(girls_names[girls_coord[j].second]);
    }
    for (int j = result[i + 1].first; j <= result[i].first - 1; ++j) {
      names.push_back(boys_names[boys_coord[j].second]);
    }
    PrintNames(names);
  }
}