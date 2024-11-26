#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

const long double cEps = 1e-9;
const int cPrec = 9;

bool Equal(long double first, long double second) {
  return std::abs(first - second) < cEps;
}

bool NotEqual(long double first, long double second) {
  return !Equal(first, second);
}

bool GreaterOrEqual(long double first, long double second) {
  return first > second - cEps;
}

bool LessOrEqual(long double first, long double second) {
  return second > first - cEps;
}

bool Greater(long double first, long double second) {
  return first > second + cEps;
}

bool Less(long double first, long double second) {
  return second > first + cEps;
}

struct Point {
  long double x;
  long double y;

  long double RadiusVectorLength() const { return std::sqrt(x * x + y * y); }

  Point() = default;
  Point(long double x, long double y) : x(x), y(y) {}
  Point(std::pair<long double, long double> p) : x(p.first), y(p.second) {}
  Point(long double start_x, long double start_y, long double end_x,
        long double end_y)
      : x(end_x - start_x), y(end_y - start_y) {}
  Point(const Point&) = default;
};

bool operator==(const Point& first, const Point& second) {
  return Equal(first.x, second.x) && Equal(first.y, second.y);
}

bool operator!=(const Point& first, const Point& second) {
  return !(first == second);
}

Point operator+(const Point& first, const Point& second) {
  return {first.x + second.x, first.y + second.y};
}

Point operator-(const Point& first, const Point& second) {
  return {first.x - second.x, first.y - second.y};
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << p.x << " " << p.y;
  return os;
}

long double DotProduct(const Point& first, const Point& second) {
  return first.x * second.x + first.y * second.y;
}

long double CrossProduct(const Point& first, const Point& second) {
  return first.x * second.y - second.x * first.y;
}

signed main() {
  std::cout << std::setprecision(cPrec);

  long double a;
  long double b;
  long double c;
  long double d;
  std::cin >> a >> b >> c >> d;
  Point x(a, b, c, d);
  std::cin >> a >> b >> c >> d;
  Point y(a, b, c, d);

  std::cout << x.RadiusVectorLength() << " " << y.RadiusVectorLength() << "\n";
  std::cout << x + y << "\n";
  std::cout << DotProduct(x, y) << " " << CrossProduct(x, y) << "\n";
  std::cout << std::abs(CrossProduct(x, y)) / 2;
}