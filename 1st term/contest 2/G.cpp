#include <iostream>
#include <vector>

std::vector<long long> Insert(std::vector<long long> data, int num) {
  int index = data.size() - 1;
  for (int i = 0; i < (int)data.size(); i++) {
    if (data[i] > num) {
      index = i - 1;
      break;
    }
  }
  std::vector<long long> new_data(data.size() + 1);
  for (int i = 0; i < (int)data.size(); i++) {
    if (i <= index) {
      new_data[i] = data[i];
    } else {
      new_data[i + 1] = data[i];
    }
  }
  new_data[index + 1] = num;
  return new_data;
}

void PrintVector(std::vector<long long> vector) {
  for (long long elem : vector) {
    std::cout << elem << " ";
  }
}

int main() {
  int cnt;
  int first_few;
  std::cin >> cnt >> first_few;

  long long current_num;
  int coefficient;
  int add;
  const int kModulo = 1 << 30;
  std::cin >> current_num >> coefficient >> add;

  std::vector<long long> result;

  for (int i = 0; i < cnt; i++) {
    current_num = (coefficient * current_num + add) % kModulo;
    if ((int)result.size() < first_few) {
      result = Insert(result, current_num);
    } else if ((int)result.size() == first_few && result.back() > current_num) {
      result.pop_back();
      result = Insert(result, current_num);
    }
  }

  PrintVector(result);
}