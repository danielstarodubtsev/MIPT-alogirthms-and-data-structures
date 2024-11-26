#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>

const long double cEps = 1e-9;
const long double cPi = std::acos(-1);
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
};

struct Segment {
  Point start;
  Point end;

  long double Length() const {
    long double x_diff = start.x - end.x;
    long double y_diff = start.y - end.y;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
  }

  Segment() = default;
  Segment(const Point& start, const Point& end) : start(start), end(end) {}
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

Point operator*(const Point& point, long double c) {
  Point ans = point;
  ans.x *= c;
  ans.y *= c;
  return ans;
}

Point operator*(long double c, const Point& point) {
  Point ans = point;
  ans.x *= c;
  ans.y *= c;
  return ans;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << p.x << " " << p.y;
  return os;
}

std::istream& operator>>(std::istream& is, Point& p) {
  is >> p.x >> p.y;
  return is;
}

long double DotProduct(const Point& first, const Point& second) {
  return first.x * second.x + first.y * second.y;
}

long double CrossProduct(const Point& first, const Point& second) {
  return first.x * second.y - second.x * first.y;
}

struct Line {
  long double a;
  long double b;
  long double c;

  Line(long double a, long double b, long double c) : a(a), b(b), c(c) {}
  Line(const Line& other) = default;
  Line(const Point& first, const Point& second) {
    assert(first != second);
    a = first.y - second.y;
    b = second.x - first.x;
    c = first.x * second.y - second.x * first.y;
    Normalize();
  }
  Line(const Segment& s) : Line(s.start, s.end) {}

  void Normalize() {
    long double coef = 1 / std::sqrt(a * a + b * b);
    a *= coef;
    b *= coef;
    c *= coef;
  }
};

bool IsPointOnLine(const Point& point, const Line& line) {
  return Equal(line.a * point.x + line.b * point.y + line.c, 0);
}

long double Distance(const Point& first, const Point& second) {
  long double x_diff = first.x - second.x;
  long double y_diff = first.y - second.y;
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

bool IsPointOnSegment(const Point& point, const Point& start,
                      const Point& end) {
  return Equal(Distance(point, start) + Distance(point, end),
               Distance(start, end));
}

bool IsPointOnSegment(const Point& point, const Segment& segment) {
  return IsPointOnSegment(point, segment.start, segment.end);
}

Point Project(const Point& point, const Line& line) {
  Point ans = point;
  ans = ans -
        Point(line.a, line.b) * (line.a * point.x + line.b * point.y + line.c);
  return ans;
}

Line PerpendicularThroughPoint(const Point& point, const Line& line) {
  Point second = point + Point(line.a, line.b);
  return {point, second};
}

bool IsPointOnRay(const Point& ray_start, const Point& ray_other,
                  const Point& point) {
  if (!IsPointOnLine(point, {ray_other, ray_start})) {
    return false;
  }

  return NotEqual(Distance(ray_start, ray_other) + Distance(ray_start, point),
                  Distance(ray_other, point));
}

long double PointLineDistance(const Point& point, const Line& line) {
  Point p = Project(point, line);
  return Distance(p, point);
}

long double PointRayDistance(const Point& ray_start, const Point& ray_other,
                             const Point& point) {
  Point p = Project(point, {ray_start, ray_other});

  if (IsPointOnRay(ray_start, ray_other, p)) {
    return Distance(point, p);
  }

  return Distance(point, ray_start);
}

std::istream& operator>>(std::istream& is, Segment& s) {
  is >> s.start >> s.end;
  return is;
}

long double PointSegmentDistance(const Point& start, const Point& end,
                                 const Point& point) {
  Point p = Project(point, {start, end});

  if (IsPointOnSegment(p, start, end)) {
    return Distance(point, p);
  }

  return std::min(Distance(point, start), Distance(point, end));
}

bool AreParallel(const Line& first, const Line& second) {
  long double delta = first.a * second.b - second.a * first.b;
  return Equal(delta, 0);
}

Point Intersect(const Line& first, const Line& second) {
  long double delta = first.a * second.b - second.a * first.b;
  long double delta1 = -first.c * second.b + second.c * first.b;
  long double delta2 = -first.a * second.c + second.a * first.c;
  return {delta1 / delta, delta2 / delta};
}

long double SegmentSegmentDistance(const Segment& first,
                                   const Segment& second) {
  Line first_line(first);
  Line second_line(second);

  long double possible_ans = std::min(Distance(first.start, second.start),
                                      Distance(first.start, second.end));
  possible_ans = std::min(possible_ans, Distance(first.end, second.start));
  possible_ans = std::min(possible_ans, Distance(first.end, second.end));

  Point p = Project(first.start, second_line);
  if (IsPointOnSegment(p, second)) {
    possible_ans = std::min(possible_ans, Distance(first.start, p));
  }
  p = Project(first.end, second_line);
  if (IsPointOnSegment(p, second)) {
    possible_ans = std::min(possible_ans, Distance(first.end, p));
  }
  p = Project(second.start, first_line);
  if (IsPointOnSegment(p, first)) {
    possible_ans = std::min(possible_ans, Distance(second.start, p));
  }
  p = Project(second.end, first_line);
  if (IsPointOnSegment(p, first)) {
    possible_ans = std::min(possible_ans, Distance(second.end, p));
  }

  if (AreParallel(first_line, second_line)) {
    return possible_ans;
  }

  p = Intersect(first, second);

  if (IsPointOnSegment(p, first) && IsPointOnSegment(p, second)) {
    return 0;
  }

  return possible_ans;
}

bool DoRayAndSegmentIntersect(const Segment& segment, const Point& ray_start,
                              const Point& ray_end) {
  Line first(segment);
  Line second(ray_start, ray_end);
  Point p = Intersect(first, second);

  return IsPointOnRay(ray_start, ray_end, p) && IsPointOnSegment(p, segment);
}

signed main() {
  std::cout << std::setprecision(cPrec);

  int n;
  Point p;
  std::cin >> n >> p;
  std::vector<Point> points(n);

  for (int i = 0; i < n; ++i) {
    std::cin >> points[i];
  }

  for (int i = 0; i < n; ++i) {
    if (IsPointOnSegment(p, {points[i], points[(i + 1) % n]})) {
      std::cout << "YES";
      return 0;
    }
  }

  int cnt = 0;

  for (int i = 0; i < n; ++i) {
    Segment cur(points[i], points[(i + 1) % n]);
    if (DoRayAndSegmentIntersect(cur, p, p + Point(1, cPi))) {
      ++cnt;
    }
  }

  std::cout << ((cnt % 2 == 0) ? "NO" : "YES");
}
