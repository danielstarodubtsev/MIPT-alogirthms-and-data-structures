#include <algorithm>
#include <iostream>
#include <vector>

struct Triplet {
  long long first = 0;
  long long second = 0;
  long long third = 0;

  void ShowSelf() const {
    std::cout << "(" << first << " " << second << " " << third << ")\n";
  }
};

struct Compare {
  bool operator()(Triplet const& first, Triplet const& second) const {
    return first.first < second.first;
  }
};

class Stack {
 private:
  std::vector<long long> data_;
  long long size_ = 0;

 public:
  void Push(long long num) {
    data_.push_back(num);
    size_++;
  }
  long long Top() { return data_.at(size_ - 1); }
  long long Pop() {
    long long popped_elem = data_.at(size_ - 1);
    data_.pop_back();
    size_--;
    return popped_elem;
  }
  long long GetSize() const { return size_; }
  bool IsEmpty() const { return size_ == 0; }
  void Clear() {
    data_.clear();
    size_ = 0;
  }
};

class StackWithMin {
 private:
  Stack data_;
  Stack minimums_;

 public:
  void Push(long long num) {
    data_.Push(num);
    if (minimums_.IsEmpty()) {
      minimums_.Push(num);
    } else {
      minimums_.Push(std::min(num, minimums_.Top()));
    }
  }
  long long Top() { return data_.Top(); }
  long long Pop() {
    minimums_.Pop();
    return data_.Pop();
  }
  long long GetSize() { return data_.GetSize(); }
  bool IsEmpty() { return data_.IsEmpty(); }
  long long GetMin() { return minimums_.Top(); }
  void Clear() {
    data_.Clear();
    minimums_.Clear();
  }
};

class Queue {
 private:
  StackWithMin first_stack_;
  StackWithMin second_stack_;

 public:
  void Push(long long num) { first_stack_.Push(num); }
  long long Front() {
    if (second_stack_.IsEmpty()) {
      while (!first_stack_.IsEmpty()) {
        second_stack_.Push(first_stack_.Pop());
      }
    }
    return second_stack_.Top();
  }
  long long Pop() {
    if (second_stack_.IsEmpty()) {
      while (!first_stack_.IsEmpty()) {
        second_stack_.Push(first_stack_.Pop());
      }
    }
    return second_stack_.Pop();
  }
  long long GetSize() {
    return first_stack_.GetSize() + second_stack_.GetSize();
  }
  long long GetMin() {
    if (first_stack_.IsEmpty()) {
      return second_stack_.GetMin();
    }
    if (second_stack_.IsEmpty()) {
      return first_stack_.GetMin();
    }
    return std::min(first_stack_.GetMin(), second_stack_.GetMin());
  }
  void Clear() {
    first_stack_.Clear();
    second_stack_.Clear();
  }
  bool IsEmpty() { return first_stack_.IsEmpty() && second_stack_.IsEmpty(); }
};

std::pair<int, int> IntersectNormal(int current_start, int current_end,
                                    int new_start, int new_end) {
  if (new_start > current_end || current_start > new_end) {
    return {-2, -2};
  }
  if (current_start >= new_start && current_end <= new_end) {
  } else if (current_start <= new_start && current_end >= new_end) {
    current_start = new_start;
    current_end = new_end;
  } else if (current_start <= new_end && current_start >= new_start) {
    current_end = new_end;
  } else if (current_end >= new_start && current_end <= new_end) {
    current_start = new_start;
  }
  return {current_start, current_end};
}

std::pair<int, int> IntersectSecond(int current_start, int current_end,
                                    int new_start, int new_end) {
  if (current_start > new_end && current_end < new_start) {
    return {-2, -2};
  }
  if (current_start <= new_end && current_end <= new_end) {
  } else if (current_start >= new_start) {
  } else if (current_start > new_end && current_end >= new_start) {
    current_start = new_start;
  } else if (current_start <= new_end && current_end > new_end) {
    current_end = new_end;
  }
  return {current_start, current_end};
}

std::pair<int, int> IntersectThird(int current_start, int current_end,
                                   int new_start, int new_end) {
  if (new_start > current_end && new_end < current_start) {
    return {-2, -2};
  }
  if (new_start <= current_end && new_end <= current_end) {
  } else if (new_start >= current_start) {
  } else if (new_start > current_end && new_end >= current_start) {
    new_start = current_start;
  } else if (new_start <= current_end && new_end > current_end) {
    new_end = current_end;
  }
  return {new_start, new_end};
}

std::pair<int, int> Intersect(int current_start, int current_end, int new_start,
                              int new_end) {
  if (current_end >= current_start && new_end >= new_start) {
    return IntersectNormal(current_start, current_end, new_start, new_end);
  }
  if (current_end >= current_start && new_end < new_start) {
    return IntersectSecond(current_start, current_end, new_start, new_end);
  }
  if (new_end >= new_start && current_end < current_start) {
    return IntersectThird(current_start, current_end, new_start, new_end);
  }

  return {std::max(current_start, new_start), std::min(current_end, new_end)};
}

void Solve(long long cnt, std::vector<Triplet> curves, long long total_sum) {
  long long current_start;
  long long current_end;
  long long new_start;
  long long new_end;
  long long result = cnt;
  current_start = curves.back().second % cnt;
  current_end = (current_start + curves.back().third - 1 + 3 * cnt) % cnt;
  for (long long i = cnt - 2; i >= 0; i--) {
    new_start = curves.at(i).second % cnt;
    new_end = (new_start + curves.at(i).third - 1 + 3 * cnt) % cnt;

    std::pair<int, int> intersection;
    intersection = Intersect(current_start, current_end, new_start, new_end);
    current_start = intersection.first;
    current_end = intersection.second;

    if (current_end == -2) {
      result = i;
      break;
    }
  }
  std::cout << total_sum - curves.at(result).first << " "
            << curves.at(result).first;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  long long cnt;
  std::cin >> cnt;

  long long x;
  x = cnt / 2;

  std::vector<long long> data(cnt);
  long long total_sum = 0;

  for (long long i = 0; i < cnt; i++) {
    std::cin >> data.at(i);
    total_sum += data.at(i);
  }

  long long current_sum = 0;
  Queue queue;

  std::vector<Triplet> curves(cnt);

  for (long long i = 0; i < x; i++) {
    current_sum += data.at(i);
    queue.Push(data.at(i));
  }
  for (long long i = x; i < cnt + x; i++) {
    curves.at((i - x) % cnt) = {current_sum, (i - x) % cnt, x};
    current_sum -= queue.Pop();
    current_sum += data.at(i % cnt);
    queue.Push(data.at(i % cnt));
  }

  std::sort(curves.begin(), curves.end(), Compare());

  Solve(cnt, curves, total_sum);
}