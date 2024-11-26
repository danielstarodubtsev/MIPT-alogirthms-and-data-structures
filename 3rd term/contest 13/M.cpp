#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>

const long double cEps = 1e-9;
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
  int number;

  long double Length() const {
    long double x_diff = start.x - end.x;
    long double y_diff = start.y - end.y;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
  }

  double GetYByX(long double x) const {
    if (Equal(start.x, end.x)) {
      return start.y;
    }

    return start.y + (end.y - start.y) * (x - start.x) / (end.x - start.x);
  }

  Segment() = default;
  Segment(const Point& start, const Point& end) : start(start), end(end) {}
  Segment(const Point& start, const Point& end, int n)
      : start(start), end(end), number(n) {}
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

Point DoSegmentsIntersect2(const Line& first, const Line& second) {
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

  p = DoSegmentsIntersect2(first, second);

  if (IsPointOnSegment(p, first) && IsPointOnSegment(p, second)) {
    return 0;
  }

  return possible_ans;
}

bool DoRayAndSegmentIntersect(const Segment& segment, const Point& ray_start,
                              const Point& ray_end) {
  Line first(segment);
  Line second(ray_start, ray_end);
  Point p = DoSegmentsIntersect2(first, second);

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
  ans.pop_back();
  return ans;
}

bool DoIntersect(long double l1, long double r1, long double l2,
                 long double r2) {
  if (Greater(l1, r1)) {
    std::swap(l1, r1);
  }

  if (Greater(l2, r2)) {
    std::swap(l2, r2);
  }

  return LessOrEqual(std::max(l1, l2), std::min(r1, r2));
}

int CrossProductSign(const Point& a, const Point& b, const Point& c) {
  long double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  return std::abs(s) < cEps ? 0 : s > 0 ? 1 : -1;
}

bool DoSegmentsIntersect2(const Segment& a, const Segment& b) {
  return DoIntersect(a.start.x, a.end.x, b.start.x, b.end.x) &&
         DoIntersect(a.start.y, a.end.y, b.start.y, b.end.y) &&
         CrossProductSign(a.start, a.end, b.start) *
                 CrossProductSign(a.start, a.end, b.end) <=
             0 &&
         CrossProductSign(b.start, b.end, a.start) *
                 CrossProductSign(b.start, b.end, a.end) <=
             0;
}

bool operator<(const Segment& a, const Segment& b) {
  long double x =
      std::max(std::min(a.start.x, a.end.x), std::min(b.start.x, b.end.x));
  return Less(a.GetYByX(x), b.GetYByX(x));
}

struct Event {
  long double x;
  int tp;
  int id;

  Event() {}
  Event(long double x, int tp, int id) : x(x), tp(tp), id(id) {}

  bool operator<(const Event& other) const {
    if (NotEqual(x, other.x)) {
      return x < other.x;
    }
    return tp > other.tp;
  }
};

std::set<Segment> s;
std::vector<std::set<Segment>::iterator> where;

std::set<Segment>::iterator PreviousIterator(std::set<Segment>::iterator it) {
  return it == s.begin() ? s.end() : --it;
}

std::set<Segment>::iterator NextIterator(std::set<Segment>::iterator it) {
  return ++it;
}

std::pair<int, int> Solve(std::vector<Segment>& a) {
  int n = a.size();
  std::vector<Event> e;

  for (int i = 0; i < n; ++i) {
    e.push_back(Event(std::min(a[i].start.x, a[i].end.x), 1, i));
    e.push_back(Event(std::max(a[i].start.x, a[i].end.x), -1, i));
  }

  std::sort(e.begin(), e.end());
  s.clear();
  where.resize(a.size());

  for (size_t i = 0; i < e.size(); ++i) {
    int id = e[i].id;

    if (e[i].tp == 1) {
      auto next = s.lower_bound(a[id]);
      auto prev = PreviousIterator(next);

      if (next != s.end() && DoSegmentsIntersect2(*next, a[id])) {
        return {next->number, id};
      }

      if (prev != s.end() && DoSegmentsIntersect2(*prev, a[id])) {
        return {prev->number, id};
      }

      where[id] = s.insert(next, a[id]);
    } else {
      auto next = NextIterator(where[id]);
      auto prev = PreviousIterator(where[id]);

      if (next != s.end() && prev != s.end() &&
          DoSegmentsIntersect2(*next, *prev)) {
        return {prev->number, next->number};
      }

      s.erase(where[id]);
    }
  }

  return {-1, -1};
}

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int n;
  std::cin >> n;
  std::vector<Segment> a(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
    a[i].number = i;
  }

  std::pair<int, int> res = Solve(a);

  if (res.first == -1) {
    std::cout << "NO";
  } else {
    std::cout << "YES\n" << res.first + 1 << " " << res.second + 1;
  }
}