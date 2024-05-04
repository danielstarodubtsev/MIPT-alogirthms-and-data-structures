#include <iostream>
#include <vector>

const long long kMod = 1e9 + 9;

struct Matrix {
  long long a11 = 0;
  long long a12 = 0;
  long long a21 = 0;
  long long a22 = 0;
};

struct Airport {
  long long x_coord = 0;
  long long y_coord = 0;
  long long multiplier = 0;
};

Matrix Product(Matrix first, Matrix second) {
  return Matrix{(first.a11 * second.a11 + first.a12 * second.a21) % kMod,
                (first.a11 * second.a12 + first.a12 * second.a22) % kMod,
                (first.a21 * second.a11 + first.a22 * second.a21) % kMod,
                (first.a21 * second.a12 + first.a22 * second.a22) % kMod};
}

Matrix Power(Matrix matrix, long long pow) {
  if (pow == 1) {
    return matrix;
  }

  if (pow % 2 == 1) {
    return Product(matrix, Power(matrix, pow - 1));
  }

  return Power(Product(matrix, matrix), pow / 2);
}

long long Fib(long long index) {
  if (index < 0) {
    return 0;
  }

  if (index == 0) {
    return 1;
  }

  Matrix matrix{1, 1, 1, 0};
  matrix = Power(matrix, index);
  return matrix.a11;
}

int main() {
  int cnt;
  int qst;

  std::cin >> cnt >> qst;

  long long cur_x;
  long long cur_y;
  long long cur_mult;
  std::vector<Airport> airports;
  for (int i = 0; i < cnt; ++i) {
    std::cin >> cur_x >> cur_y >> cur_mult;
    airports.push_back(Airport{cur_x, cur_y, cur_mult % kMod});
  }

  long long cur_shift;
  for (int i = 0; i < qst; ++i) {
    std::cin >> cur_shift;

    long long answer = 0;
    for (Airport cur_air : airports) {
      answer += cur_air.multiplier *
                Fib(cur_air.y_coord - cur_air.x_coord - cur_shift);
      answer %= kMod;
    }

    std::cout << answer << "\n";
  }
}