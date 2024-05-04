#include <cmath>
#include <iostream>
#include <vector>

const long long kDefaultCapacity = 30001;
const long long kFirstPrime = 7;
const long long kSecondPrime = 16777619;
const long long kMaxCoord = 1e7;

struct Point {
  long long x;
  long long y;

  Point() = default;
  Point(long long x_coord, long long y_coord) : x(x_coord), y(y_coord) {}
};

long long Hash(long long num) { return kFirstPrime * num + kSecondPrime; }

long long Hash(Point point) {
  long long num = point.x * (kMaxCoord + 1) + point.y;
  return Hash(num);
}

long long DistanceSquare(const Point& first, const Point& second) {
  long long x_offset = first.x - second.x;
  long long y_offset = first.y - second.y;
  return x_offset * x_offset + y_offset * y_offset;
}

struct Node {
  long long value = 0;
  size_t count = 1;
  Node* next = nullptr;

  Node() = default;
  Node(long long value) : value(value) {}
  Node(long long value, size_t count) : value(value), count(count) {}
  Node(long long value, Node* next) : value(value), next(next) {}
};

struct Set {
  size_t size = 0;
  size_t first_n_empty = 0;
  size_t capacity = kDefaultCapacity;
  std::vector<Node*> data;

  void Add(long long num) {
    ++size;

    long long hash = Hash(num) % capacity;
    Node* cur_node = data[hash];

    if (cur_node == nullptr) {
      data[hash] = new Node{num};
      return;
    }

    while (cur_node->next != nullptr) {
      if (cur_node->value == num) {
        ++cur_node->count;
        return;
      }

      cur_node = cur_node->next;
    }

    if (cur_node->value == num) {
      ++cur_node->count;
      return;
    }

    Node* new_node = new Node(num);
    cur_node->next = new_node;
  }

  size_t EraseAll(long long num) {
    long long hash = Hash(num) % capacity;
    Node* cur_node = data[hash];

    if (cur_node == nullptr) {
      return 0;
    }

    if (cur_node->value == num) {
      size_t deleted = cur_node->count;
      Node* new_next = cur_node->next;
      delete cur_node;
      data[hash] = new_next;
      size -= deleted;
      return deleted;
    }

    while (cur_node->next != nullptr) {
      if (cur_node->next->value == num) {
        size_t deleted = cur_node->next->count;
        Node* new_next = cur_node->next->next;
        delete cur_node->next;
        cur_node->next = new_next;
        size -= deleted;
        return deleted;
      }
      cur_node = cur_node->next;
    }

    return 0;
  }

  bool IsEmpty() const { return size == 0; }

  long long GetAny() {
    for (size_t i = first_n_empty; i < capacity; ++i) {
      if (data[i] != nullptr) {
        first_n_empty = i;
        return data[i]->value;
      }
    }

    return 0;
  }

  Set(size_t capacity) : capacity(capacity) { data.resize(capacity); }
  Set() : Set(kDefaultCapacity) {}

  ~Set() {
    for (size_t i = 0; i < capacity; ++i) {
      Node* cur_node = data[i];

      while (cur_node != nullptr) {
        Node* next = cur_node->next;
        delete cur_node;
        cur_node = next;
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  size_t points_num;
  int theory_num;

  std::cin >> points_num >> theory_num;
  std::vector<Point> points(points_num);

  long long x_coord;
  long long y_coord;

  for (size_t i = 0; i < points_num; ++i) {
    std::cin >> x_coord >> y_coord;
    points[i] = Point{x_coord, y_coord};
  }

  long long answer = 0;

  if (theory_num == 1) {
    for (size_t cur_index = 0; cur_index < points_num; ++cur_index) {
      Set distances;

      for (size_t other_index = 0; other_index < points_num; ++other_index) {
        if (cur_index == other_index) {
          continue;
        }

        distances.Add(DistanceSquare(points[cur_index], points[other_index]));
      }

      while (!distances.IsEmpty()) {
        int num = distances.EraseAll(distances.GetAny());
        answer += (num - 1) * num / 2;
      }
    }
  }

  std::cout << answer;
}
