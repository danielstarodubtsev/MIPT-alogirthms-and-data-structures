#include <iostream>
#include <string>
#include <vector>

class BinaryHeap {
 private:
  const int kBigNum = 1e6 + 5;
  std::vector<std::pair<long long, int>> data_;
  std::vector<int> indexes_;
  int requests_ = 0;
  int size_ = 0;

  void SiftUp(int index) {
    if (index == 0) {
      return;
    }
    int parent_index = (index - 1) / 2;
    if (data_.at(index).first < data_.at(parent_index).first) {
      Swap(index, parent_index);
      SiftUp(parent_index);
    }
  }

  void SiftDown(int index) {
    if (2 * index + 1 >= size_) {
      return;
    }
    int child_index = 2 * index + 1;
    if (2 * index + 2 < size_ &&
        data_.at(2 * index + 2) < data_.at(2 * index + 1)) {
      child_index++;
    }
    if (data_.at(index).first > data_.at(child_index).first) {
      Swap(index, child_index);
      SiftDown(child_index);
    }
  }

  void Swap(int index1, int index2) {
    std::swap(indexes_.at(data_.at(index1).second),
              indexes_.at(data_.at(index2).second));
    std::swap(data_.at(index1), data_.at(index2));
  }

 public:
  long long GetMin() {
    requests_++;
    return data_.at(0).first;
  }
  void ExtractMin() {
    requests_++;
    Swap(0, size_ - 1);
    size_--;
    data_.pop_back();
    SiftDown(0);
  }
  void Insert(long long value) {
    if (requests_ == 0) {
      data_.resize(kBigNum);
      indexes_.resize(kBigNum);
    }
    requests_++;
    size_++;
    data_.at(size_ - 1) = {value, requests_};
    indexes_.at(requests_) = size_ - 1;
    SiftUp(size_ - 1);
  }
  void DecreaseKey(int request_num, int delta) {
    requests_++;
    int index = indexes_.at(request_num);
    data_.at(index).first -= delta;
    SiftUp(index);
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);

  BinaryHeap heap;

  int cnt;
  std::cin >> cnt;
  std::string command;

  int elem = 0;
  int request_num = 0;
  int delta = 0;

  for (int i = 0; i < cnt; i++) {
    std::cin >> command;
    if (command == "insert") {
      std::cin >> elem;
      heap.Insert(elem);
    } else if (command == "getMin") {
      std::cout << heap.GetMin() << "\n";
    } else if (command == "extractMin") {
      heap.ExtractMin();
    } else if (command == "decreaseKey") {
      std::cin >> request_num >> delta;
      heap.DecreaseKey(request_num, delta);
    }
  }
}