#include <iostream>
#include <vector>

class Stack {
 private:
  std::vector<int> data_;
  int size_ = 0;

 public:
  void Push(int num) {
    data_.push_back(num);
    size_++;
  }
  int Top() { return data_[size_ - 1]; }
  int Pop() {
    int popped_elem = data_[size_ - 1];
    data_.pop_back();
    size_--;
    return popped_elem;
  }
  int GetSize() const { return size_; }
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
  void Push(int num) {
    data_.Push(num);
    if (minimums_.IsEmpty()) {
      minimums_.Push(num);
    } else {
      minimums_.Push(std::min(num, minimums_.Top()));
    }
  }
  int Top() { return data_.Top(); }
  int Pop() {
    minimums_.Pop();
    return data_.Pop();
  }
  int GetSize() { return data_.GetSize(); }
  bool IsEmpty() { return data_.IsEmpty(); }
  int GetMin() { return minimums_.Top(); }
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
  void Push(int num) { first_stack_.Push(num); }
  int Front() {
    if (second_stack_.IsEmpty()) {
      while (!first_stack_.IsEmpty()) {
        second_stack_.Push(first_stack_.Pop());
      }
    }
    return second_stack_.Top();
  }
  int Pop() {
    if (second_stack_.IsEmpty()) {
      while (!first_stack_.IsEmpty()) {
        second_stack_.Push(first_stack_.Pop());
      }
    }
    return second_stack_.Pop();
  }
  int GetSize() { return first_stack_.GetSize() + second_stack_.GetSize(); }
  int GetMin() {
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

int main() {
  int size;
  int target_size;
  std::cin >> size >> target_size;

  std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      std::cin >> matrix[i][j];
    }
  }

  Queue queue;
  int elem;

  for (int column = 0; column < size; column++) {
    for (int row = size - 1; row >= 0; row--) {
      elem = matrix[row][column];
      queue.Push(elem);
      if (queue.GetSize() > target_size) {
        queue.Pop();
      }
      matrix[row][column] = queue.GetMin();
    }
  }

  queue.Clear();

  for (int row = 0; row < size; row++) {
    for (int column = size - 1; column >= 0; column--) {
      elem = matrix[row][column];
      queue.Push(elem);
      if (queue.GetSize() > target_size) {
        queue.Pop();
      }
      matrix[row][column] = queue.GetMin();
    }
  }

  // output
  for (int i = 0; i <= size - target_size; i++) {
    for (int j = 0; j <= size - target_size; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }
}