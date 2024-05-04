#include <iostream>
#include <string>
#include <vector>

class BinaryHeap {
 private:
  std::vector<std::pair<int, int>> data_;
  int size_ = 0;
  int inserts_num_ = 0;
  bool is_max_ = false;

  void SiftUp(int index) {
    if (index == 0) {
      return;
    }
    int parent_index = (index - 1) / 2;
    if ((data_[index].first < data_[parent_index].first && !is_max_) ||
        (data_[index].first > data_[parent_index].first && is_max_)) {
      Swap(index, parent_index);
      SiftUp(parent_index);
    }
  }

  void SiftDown(int index) {
    if (2 * index + 1 >= size_) {
      return;
    }
    int child_index = 2 * index + 1;
    if ((2 * index + 2 < size_ &&
         data_[2 * index + 2].first < data_[2 * index + 1].first && !is_max_) ||
        (2 * index + 2 < size_ &&
         data_[2 * index + 2].first > data_[2 * index + 1].first && is_max_)) {
      child_index++;
    }
    if ((data_[index].first > data_[child_index].first && !is_max_) ||
        (data_[index].first < data_[child_index].first && is_max_)) {
      Swap(index, child_index);
      SiftDown(child_index);
    }
  }

  void Swap(int index1, int index2) { std::swap(data_[index1], data_[index2]); }

 public:
  BinaryHeap() {}
  BinaryHeap(bool is_max) : is_max_(is_max) {}

  std::pair<int, int> GetRoot() { return data_[0]; }
  void ExtractRoot() {
    Swap(0, size_ - 1);
    size_--;
    data_.pop_back();
    SiftDown(0);
  }
  void Insert(int value) {
    size_++;
    data_.push_back({value, inserts_num_});
    inserts_num_++;
    SiftUp(size_ - 1);
  }
  void Clear() {
    data_.clear();
    size_ = 0;
    inserts_num_ = 0;
  }
};

class MySmartStruct {
 private:
  BinaryHeap heap_min_;
  BinaryHeap heap_max_{true};
  std::vector<int> is_deleted_;
  int size_ = 0;

 public:
  void Insert(int value) {
    heap_min_.Insert(value);
    heap_max_.Insert(value);
    is_deleted_.push_back(0);
    size_++;
  }
  int GetMin() {
    while (is_deleted_[heap_min_.GetRoot().second] == 1) {
      heap_min_.ExtractRoot();
    }
    return heap_min_.GetRoot().first;
  }
  int ExtractMin() {
    int old_min;
    old_min = GetMin();
    is_deleted_[heap_min_.GetRoot().second] = 1;
    size_--;
    return old_min;
  }
  int GetMax() {
    while (is_deleted_[heap_max_.GetRoot().second] == 1) {
      heap_max_.ExtractRoot();
    }
    return heap_max_.GetRoot().first;
  }
  int ExtractMax() {
    int old_max;
    old_max = GetMax();
    is_deleted_[heap_max_.GetRoot().second] = 1;
    size_--;
    return old_max;
  }
  int GetSize() const { return size_; }
  void Clear() {
    heap_min_.Clear();
    heap_max_.Clear();
    is_deleted_.clear();
    size_ = 0;
  }
  bool IsEmpty() const { return size_ == 0; }
};

int main() {
  int cnt;
  std::cin >> cnt;

  MySmartStruct my_struct;

  std::string command;
  int argument;

  for (int i = 0; i < cnt; i++) {
    std::cin >> command;

    if (command == "insert") {
      std::cin >> argument;
      my_struct.Insert(argument);
      std::cout << "ok\n";
    } else if (command == "extract_min") {
      if (my_struct.IsEmpty()) {
        std::cout << "error\n";
        continue;
      }
      std::cout << my_struct.ExtractMin() << "\n";
    } else if (command == "get_min") {
      if (my_struct.IsEmpty()) {
        std::cout << "error\n";
        continue;
      }
      std::cout << my_struct.GetMin() << "\n";
    } else if (command == "extract_max") {
      if (my_struct.IsEmpty()) {
        std::cout << "error\n";
        continue;
      }
      std::cout << my_struct.ExtractMax() << "\n";
    } else if (command == "get_max") {
      if (my_struct.IsEmpty()) {
        std::cout << "error\n";
        continue;
      }
      std::cout << my_struct.GetMax() << "\n";
    } else if (command == "size") {
      std::cout << my_struct.GetSize() << "\n";
    } else if (command == "clear") {
      my_struct.Clear();
      std::cout << "ok\n";
    }
  }
}