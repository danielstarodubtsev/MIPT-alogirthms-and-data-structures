#include <iostream>
#include <string>
#include <vector>

void UpdatePoint(int index, int delta, std::vector<int>& segment_tree) {
  do {
    segment_tree.at(index) += delta;
    index = (index - 1) / 2;
  } while (index != 0);
  segment_tree.at(0) += delta;
}

int GetSum(int index, std::pair<int, int> cur_bounds,
           std::pair<int, int> bounds, std::vector<int>& segment_tree) {
  int left = bounds.first;
  int right = bounds.second;
  int cur_left = cur_bounds.first;
  int cur_right = cur_bounds.second;
  if (cur_left == left && cur_right == right) {
    return segment_tree.at(index);
  }
  int answer = 0;
  int middle = (cur_left + cur_right) / 2;
  if (left <= middle) {
    answer += GetSum(2 * index + 1, {cur_left, middle},
                     {left, std::min(right, middle)}, segment_tree);
  }
  if (right > middle) {
    answer += GetSum(2 * index + 2, {middle + 1, cur_right},
                     {std::max(left, middle + 1), right}, segment_tree);
  }
  return answer;
}

int main() {
  const int kMaxIndex = 65536 * 2;
  int qst;
  std::cin >> qst;

  std::vector<int> last_page(kMaxIndex + 5, -1);
  std::vector<int> segment_tree(kMaxIndex * 2 + 1, 0);

  std::string command;
  int registered_users = 0;

  for (int i = 0; i < qst; i++) {
    std::cin >> command;

    if (command == "CHEER") {
      int user;
      std::cin >> user;

      if (last_page.at(user) == -1) {
        std::cout << 0 << "\n";
        continue;
      }

      int sum =
          GetSum(0, {0, kMaxIndex - 1}, {0, last_page.at(user)}, segment_tree);
      if (registered_users == 1) {
        std::cout << 1 << "\n";
        continue;
      }
      std::cout << ((float)sum / (registered_users - 1)) << "\n";
    } else {
      int user;
      int page;
      std::cin >> user >> page;

      if (last_page.at(user) != -1) {
        int index = last_page.at(user) + kMaxIndex;
        UpdatePoint(index, -1, segment_tree);
      } else {
        registered_users++;
      }
      last_page.at(user) = page;
      UpdatePoint(last_page.at(user) + kMaxIndex, 1, segment_tree);
    }
  }
}