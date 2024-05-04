#include <iostream>
#include <string>
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
  int cnt;
  std::cin >> cnt;

  std::string input;
  std::string command;
  Queue queue;
  int argument;

  for (int i = 0; i < cnt; i++) {
    std::cin >> command;

    if (command == "dequeue") {
      if (queue.IsEmpty()) {
        std::cout << "error\n";
      } else {
        std::cout << queue.Pop() << "\n";
      }
    } else if (command == "front") {
      if (queue.IsEmpty()) {
        std::cout << "error\n";
      } else {
        std::cout << queue.Front() << "\n";
      }
    } else if (command == "size") {
      std::cout << queue.GetSize() << "\n";
    } else if (command == "clear") {
      std::cout << "ok\n";
      queue.Clear();
    } else if (command == "min") {
      if (queue.IsEmpty()) {
        std::cout << "error\n";
      } else {
        std::cout << queue.GetMin() << "\n";
      }
    } else {
      std::cin >> argument;
      queue.Push(argument);
      std::cout << "ok\n";
    }
  }
}