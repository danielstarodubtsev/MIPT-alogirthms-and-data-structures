#include <iostream>
#include <vector>

const long long kBase = 10;

std::vector<std::vector<long long>> Product(
    std::vector<std::vector<long long>> matrix1,
    std::vector<std::vector<long long>> matrix2, long long mod) {
  std::vector<std::vector<long long>> answer(
      matrix1.size(), std::vector<long long>(matrix2[0].size(), 0));

  for (size_t i = 0; i < matrix1.size(); ++i) {
    for (size_t j = 0; j < matrix2[0].size(); ++j) {
      for (size_t k = 0; k < matrix2.size(); ++k) {
        answer[i][j] = (answer[i][j] + matrix1[i][k] * matrix2[k][j]) % mod;
      }
    }
  }

  return answer;
}

std::string PopFront(std::string bigint) {
  std::string result;

  for (size_t i = 1; i < bigint.size(); ++i) {
    result.push_back(bigint[i]);
  }

  return result;
}

bool IsEven(std::string bigint) { return (bigint.back() - '0') % 2 == 0; }

std::string DivideBy2(std::string bigint) {
  long long remainder = 0;

  for (size_t i = 0; i < bigint.size(); ++i) {
    remainder *= kBase;
    remainder += bigint[i] - '0';
    bigint[i] = '0' + remainder / 2;
    remainder %= 2;
  }

  if (bigint[0] == '0') {
    bigint = PopFront(bigint);
  }

  return bigint;
}

std::string SubtractOne(std::string bigint) {
  size_t index = bigint.size() - 1;

  while (bigint[index] == '0') {
    bigint[index] = '9';
    --index;
  }

  --bigint[index];

  if (bigint[0] == '0') {
    bigint = PopFront(bigint);
  }

  return bigint;
}

std::vector<std::vector<long long>> Power(
    std::vector<std::vector<long long>> matrix, std::string power,
    long long mod) {
  if (power == "1") {
    return matrix;
  }

  if (IsEven(power)) {
    return Power(Product(matrix, matrix, mod), DivideBy2(power), mod);
  }

  return Product(matrix, Power(matrix, SubtractOne(power), mod), mod);
}

long long GetBit(long long mask, long long bit) { return (mask >> bit) & 1; }

bool CanBeNear(long long mask1, long long mask2, long long size) {
  for (long long bit = 0; bit < size - 1; ++bit) {
    if (GetBit(mask1, bit) == GetBit(mask1, bit + 1) &&
        GetBit(mask1, bit) == GetBit(mask2, bit) &&
        GetBit(mask1, bit) == GetBit(mask2, bit + 1)) {
      return false;
    }
  }

  return true;
}

int main() {
  std::string power;
  long long size;
  long long mod;
  std::cin >> power >> size >> mod;

  std::vector<std::vector<long long>> matrix(
      1 << size, std::vector<long long>(1 << size, 0));
  for (long long mask1 = 0; mask1 < 1 << size; ++mask1) {
    for (long long mask2 = 0; mask2 < 1 << size; ++mask2) {
      if (CanBeNear(mask1, mask2, size)) {
        matrix[mask1][mask2] = 1;
      }
    }
  }

  power = SubtractOne(power);

  if (power.empty()) {
    std::cout << (1 << size) % mod;
  } else {
    matrix = Power(matrix, power, mod);
    long long result = 0;

    for (long long i = 0; i < 1 << size; ++i) {
      for (long long j = 0; j < 1 << size; ++j) {
        result = (result + matrix[i][j]) % mod;
      }
    }

    std::cout << result;
  }
}