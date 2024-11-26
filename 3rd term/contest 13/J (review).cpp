#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>

const long double cEps = 0.03;
const long double cPi = std::acos(-1);

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

struct Circle {
  long double x;
  long double y;
  long double r;

  Circle() = default;
  Circle(long double x, long double y, long double r) : x(x), y(y), r(r) {}

  long double Area() const { return cPi * r * r; }

  long double Perimeter() const { return 2 * cPi * r; }

  Point Center() const { return {x, y}; }
};

bool IsPointOnCircle(const Point& point, const Circle& circle) {
  return Equal(Distance(point, circle.Center()), circle.r);
}

Point RotatePoint(const Point& center, const Point& point, long double angle) {
  long double new_x = (point.x - center.x) * std::cos(angle) -
                      (point.y - center.y) * std::sin(angle) + center.x;
  long double new_y = (point.x - center.x) * std::sin(angle) +
                      (point.y - center.y) * std::cos(angle) + center.y;

  return {new_x, new_y};
}

std::istream& operator>>(std::istream& is, Circle& s) {
  is >> s.x >> s.y >> s.r;
  return is;
}

std::ostream& operator<<(std::ostream& os, const Line& line) {
  os << line.a << " " << line.b << " " << line.c;
  return os;
}

std::pair<Line, Line> Tangents(const Point& point, const Circle& circle) {
  assert(Greater(Distance(circle.Center(), point), circle.r));

  long double angle = std::asin(circle.r / Distance(circle.Center(), point));
  Point first = RotatePoint(point, circle.Center(), angle);
  Point second = RotatePoint(point, circle.Center(), -angle);

  return {{first, point}, {second, point}};
}

std::pair<Point, Point> TangentPoints(const Point& point,
                                      const Circle& circle) {
  assert(GreaterOrEqual(Distance(circle.Center(), point), circle.r));

  if (IsPointOnCircle(point, circle)) {
    return {point, point};
  }

  auto [first, second] = Tangents(point, circle);
  long double d = Distance(circle.Center(), point);
  long double len = std::sqrt(d * d - circle.r * circle.r);

  Point ans1 = point + Point(-first.b, first.a) * len;
  Point ans2 = point + Point(-second.b, second.a) * len;

  return {ans1, ans2};
}

std::vector<Point> LineCircleIntersection(const Line& line,
                                          const Circle& circle) {
  Point p = Project(circle.Center(), line);
  long double h = Distance(circle.Center(), p);

  if (Greater(h, circle.r)) {
    return {};
  }

  if (Equal(h, circle.r)) {
    return {p};
  }

  long double x = std::sqrt(circle.r * circle.r - h * h);
  Point a(-line.b, line.a);

  return {p + x * a, p - x * a};
}

std::vector<Point> SegmentCircleIntersection(const Segment& segment,
                                             const Circle& circle) {
  std::vector<Point> fake_ans = LineCircleIntersection(Line(segment), circle);
  std::vector<Point> real_ans;

  for (Point p : fake_ans) {
    if (IsPointOnSegment(p, segment)) {
      real_ans.push_back(p);
    }
  }

  return real_ans;
}

bool operator<(const Point& first, const Point& second) {
  return Less(first.x, second.x) ||
         (Equal(first.x, second.x) && Less(first.y, second.y));
}

std::vector<Point> ConvexHull(std::vector<Point>& points,
                              bool need_sort = true) {
  if (need_sort) {
    size_t min_index = 0;
    Point min_point = points[0];

    for (size_t i = 1; i < points.size(); ++i) {
      if (points[i] < min_point) {
        min_point = points[i];
        min_index = i;
      }
    }
    std::swap(points[0], points[min_index]);

    std::sort(
        points.begin() + 1, points.end(),
        [&](const Point& first, const Point& second) {
          long double d1 = Distance(points[0], first);
          long double d2 = Distance(points[0], second);
          if (d1 == 0) {
            return true;
          }
          if (d2 == 0) {
            return false;
          }
          long double angle1 =
              std::asin(CrossProduct(first - points[0], Point(1, 0)) / d1);
          long double angle2 =
              std::asin(CrossProduct(second - points[0], Point(1, 0)) / d2);
          return Less(angle1, angle2) ||
                 (Equal(angle1, angle2) && Less(d1, d2));
        });
  }

  std::vector<Point> ans = {points[0], points[1]};

  for (size_t i = 2; i < points.size(); ++i) {
    Point cur = points[i];
    while (ans.size() >= 2 &&
           (LessOrEqual(
               CrossProduct(ans.back() - cur, ans[ans.size() - 2] - cur), 0))) {
      ans.pop_back();
    }
    ans.push_back(cur);
  }

  return ans;
}

std::vector<Point> MinkowskiSum(std::vector<Point>& first,
                                std::vector<Point>& second) {
  size_t index1 = 0;
  Point min_point = first[0];
  for (size_t i = 1; i < first.size(); ++i) {
    Point cur = first[i];
    if (Less(cur.y, min_point.y) ||
        (Equal(cur.y, min_point.y) && Less(cur.x, min_point.x))) {
      index1 = i;
      min_point = cur;
    }
  }

  size_t index2 = 0;
  min_point = second[0];
  for (size_t i = 1; i < second.size(); ++i) {
    Point cur = second[i];
    if (Less(cur.y, min_point.y) ||
        (Equal(cur.y, min_point.y) && Less(cur.x, min_point.x))) {
      index2 = i;
      min_point = cur;
    }
  }

  std::vector<Point> ans = {first[index1] + second[index2]};
  size_t index1_copy = index1;
  size_t index2_copy = index2;
  bool flag1 = true;
  bool flag2 = true;

  while (index1 != index1_copy || index2 != index2_copy || (flag1 && flag2)) {
    Point cur1 = first[index1];
    Point cur2 = second[index2];
    Point next1 = first[(index1 + 1) % first.size()];
    Point next2 = second[(index2 + 1) % second.size()];
    Point v1 = next1 - cur1;
    Point v2 = next2 - cur2;

    if (index1 == index1_copy && !flag1) {
      ans.push_back(ans.back() + v2);
      index2 = (index2 + 1) % second.size();
    } else if (index2 == index2_copy && !flag2) {
      ans.push_back(ans.back() + v1);
      index1 = (index1 + 1) % first.size();
    } else {
      if (Greater(CrossProduct(v1, v2), 0)) {
        ans.push_back(ans.back() + v1);
        index1 = (index1 + 1) % first.size();
        flag1 = false;
      } else {
        ans.push_back(ans.back() + v2);
        index2 = (index2 + 1) % second.size();
        flag2 = false;
      }
    }
  }

  return ans;
}

bool IsPointInTriangle(Point a, Point b, Point c, Point point) {
  long double s1 = std::abs(CrossProduct(b - a, c - a));
  long double s2 = std::abs(CrossProduct(a - point, b - point)) +
                   std::abs(CrossProduct(b - point, c - point)) +
                   std::abs(CrossProduct(c - point, a - point));

  if (Greater(s1, 0)) {
    return Equal(s1, s2);
  }

  return IsPointOnSegment(point, {a, b}) || IsPointOnSegment(point, {c, b}) ||
         IsPointOnSegment(point, {a, c});
}

bool Solve(Point p, std::vector<Point>& polygon) {
  long long r = polygon.size() - 1;
  long long l = 1;

  if (Less(CrossProduct(polygon[l] - polygon[0], p - polygon[0]), 0) ||
      Less(CrossProduct(p - polygon[0], polygon[r] - polygon[0]), 0)) {
    return false;
  }

  while (r - l > 1) {
    long long m = (l + r) / 2;
    if (GreaterOrEqual(CrossProduct(polygon[l] - polygon[0], p - polygon[0]),
                       0) &&
        GreaterOrEqual(CrossProduct(p - polygon[0], polygon[m] - polygon[0]),
                       0)) {
      r = m;
    } else {
      l = m;
    }
  }

  return IsPointInTriangle(polygon[0], polygon[l], polygon[r], p);
}

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  long long n1;
  std::cin >> n1;
  std::vector<Point> polygon1(n1);
  for (long long i = 0; i < n1; ++i) {
    std::cin >> polygon1[i];
  }

  long long n2;
  std::cin >> n2;
  std::vector<Point> polygon2(n2);
  for (long long i = 0; i < n2; ++i) {
    std::cin >> polygon2[i];
  }

  long long n3;
  std::cin >> n3;
  std::vector<Point> polygon3(n3);
  for (long long i = 0; i < n3; ++i) {
    std::cin >> polygon3[i];
  }

  std::vector<Point> result = MinkowskiSum(polygon1, polygon2);
  result.pop_back();
  result = MinkowskiSum(result, polygon3);
  result.pop_back();

  std::vector<Point> real_result = {result[0], result[1]};
  for (size_t i = 2; i < result.size(); ++i) {
    if (!IsPointOnLine(result[i], {real_result.back(),
                                   real_result[real_result.size() - 2]})) {
      real_result.push_back(result[i]);
    } else {
      real_result.pop_back();
      real_result.push_back(result[i]);
    }
  }

  long long qst;
  std::cin >> qst;
  while (qst-- > 0) {
    Point point;
    std::cin >> point;
    point.x *= 3;
    point.y *= 3;
    std::cout << (Solve(point, real_result) ? "YES" : "NO") << std::endl;
  }
}