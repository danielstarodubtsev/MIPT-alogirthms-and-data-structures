#include <iostream>
#include <vector>

int main() {
  const int kBigNum = 1e6;
  int shelves_num1;
  int shelves_num2;
  int shelf_capacity;
  std::cin >> shelves_num1 >> shelves_num2 >> shelf_capacity;

  std::vector<std::vector<int>> closet1(shelves_num1,
                                        std::vector<int>(shelf_capacity));
  std::vector<std::vector<int>> closet2(shelves_num2,
                                        std::vector<int>(shelf_capacity));

  for (int i = 0; i < shelves_num1; i++) {
    for (int j = 0; j < shelf_capacity; j++) {
      std::cin >> closet1[i][j];
    }
  }
  for (int i = 0; i < shelves_num2; i++) {
    for (int j = 0; j < shelf_capacity; j++) {
      std::cin >> closet2[i][j];
    }
  }

  int quest;
  std::cin >> quest;

  int shelf1;
  int shelf2;
  for (int i = 0; i < quest; i++) {
    std::cin >> shelf1 >> shelf2;
    int current_answer = 0;
    int current_max = kBigNum;
    int current;
    for (int j = 0; j < shelf_capacity; j++) {
      current = std::max(closet1[shelf1 - 1][j], closet2[shelf2 - 1][j]);
      if (current < current_max) {
        current_max = current;
        current_answer = j;
      }
    }
    std::cout << (int)current_answer + 1 << "\n";
  }
}
