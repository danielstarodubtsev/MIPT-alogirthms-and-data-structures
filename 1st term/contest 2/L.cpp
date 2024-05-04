#include <iostream>
#include <vector>

struct Quadro {
  long long first = 0;
  long long second = 0;
  long long third = 0;
  long long fourth = 0;

  void ShowSelf() const {
    std::cout << "(" << first << " " << second << " " << third << " " << fourth
              << ") ";
  }
};

struct Triplet {
  long long first = 0;
  long long second = 0;
  long long third = 0;
};

class Stack {
 private:
  std::vector<Quadro> data_;
  long long size_ = 0;

 public:
  void Push(Quadro elem) {
    data_.push_back(elem);
    size_++;
  }
  Quadro Top() { return data_.at(size_ - 1); }
  Quadro Pop() {
    Quadro popped_elem = data_.at(size_ - 1);
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

class StackWithMax {
 private:
  Stack data_;
  Stack maximums_;

 public:
  void Push(Quadro elem) {
    data_.Push(elem);
    if (maximums_.IsEmpty()) {
      maximums_.Push(elem);
    } else {
      if (maximums_.Top().first >= elem.first) {
        maximums_.Push(maximums_.Top());
      } else {
        maximums_.Push(elem);
      }
    }
  }
  Quadro Top() { return data_.Top(); }
  Quadro Pop() {
    maximums_.Pop();
    return data_.Pop();
  }
  long long GetSize() { return data_.GetSize(); }
  bool IsEmpty() { return data_.IsEmpty(); }
  Quadro GetMax() { return maximums_.Top(); }
  void Clear() {
    data_.Clear();
    maximums_.Clear();
  }
};

class Queue {
 private:
  StackWithMax first_stack_;
  StackWithMax second_stack_;

 public:
  void Push(Quadro num) { first_stack_.Push(num); }
  Quadro Front() {
    if (second_stack_.IsEmpty()) {
      while (!first_stack_.IsEmpty()) {
        second_stack_.Push(first_stack_.Pop());
      }
    }
    return second_stack_.Top();
  }
  Quadro Pop() {
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
  Quadro GetMax() {
    if (first_stack_.IsEmpty()) {
      return second_stack_.GetMax();
    }
    if (second_stack_.IsEmpty()) {
      return first_stack_.GetMax();
    }
    if (first_stack_.GetMax().first > second_stack_.GetMax().first) {
      return first_stack_.GetMax();
    }
    return second_stack_.GetMax();
  }
  void Clear() {
    first_stack_.Clear();
    second_stack_.Clear();
  }
  bool IsEmpty() { return first_stack_.IsEmpty() && second_stack_.IsEmpty(); }
};

void FindSubmatrixMaxFirst(std::vector<std::vector<std::vector<Quadro>>>& field,
                           Triplet size, Triplet range) {
  Queue queue;
  Quadro current_elem;
  Quadro max;

  for (long long i = 0; i < size.first; i++) {
    for (long long j = 0; j < size.second; j++) {
      queue.Clear();
      for (long long k = size.third - 1; k >= 0; k--) {
        current_elem = field.at(i).at(j).at(k);

        queue.Push(current_elem);
        if (queue.GetSize() > range.third) {
          queue.Pop();
        }

        max = queue.GetMax();
        field.at(i).at(j).at(k).first = max.first;
        field.at(i).at(j).at(k).second = max.second;
        field.at(i).at(j).at(k).third = max.third;
        field.at(i).at(j).at(k).fourth = max.fourth;
      }
    }
  }
}

void FindSubmatrixMaxSecond(
    std::vector<std::vector<std::vector<Quadro>>>& field, Triplet size,
    Triplet range) {
  Queue queue;
  Quadro current_elem;
  Quadro max;

  for (long long i = 0; i < size.first; i++) {
    for (long long k = 0; k < size.third; k++) {
      queue.Clear();
      for (long long j = size.second - 1; j >= 0; j--) {
        current_elem = field.at(i).at(j).at(k);

        queue.Push(current_elem);
        if (queue.GetSize() > range.second) {
          queue.Pop();
        }

        max = queue.GetMax();
        field.at(i).at(j).at(k).first = max.first;
        field.at(i).at(j).at(k).second = max.second;
        field.at(i).at(j).at(k).third = max.third;
        field.at(i).at(j).at(k).fourth = max.fourth;
      }
    }
  }
}

void FindSubmatrixMaxThird(std::vector<std::vector<std::vector<Quadro>>>& field,
                           Triplet size, Triplet range) {
  Queue queue;
  Quadro current_elem;
  Quadro max;

  for (long long k = 0; k < size.third; k++) {
    for (long long j = 0; j < size.second; j++) {
      queue.Clear();
      for (long long i = size.first - 1; i >= 0; i--) {
        current_elem = field.at(i).at(j).at(k);

        queue.Push(current_elem);
        if (queue.GetSize() > range.first) {
          queue.Pop();
        }

        max = queue.GetMax();
        field.at(i).at(j).at(k).first = max.first;
        field.at(i).at(j).at(k).second = max.second;
        field.at(i).at(j).at(k).third = max.third;
        field.at(i).at(j).at(k).fourth = max.fourth;
      }
    }
  }
}

void FindSubmatrixMax(std::vector<std::vector<std::vector<Quadro>>>& field,
                      Triplet size, Triplet range) {
  FindSubmatrixMaxFirst(field, size, range);
  FindSubmatrixMaxSecond(field, size, range);
  FindSubmatrixMaxThird(field, size, range);
}

void PreprocessField(std::vector<std::vector<std::vector<Quadro>>>& field,
                     Triplet size, Triplet range) {
  long long next_x;
  long long next_y;
  long long next_z;

  FindSubmatrixMax(field, size, range);

  /*
  std::cout << "\n";
  for (int i = 0; i < size.first; i++) {
    for (int j = 0; j < size.second; j++) {
      for (int k = 0; k < size.third; k++) {
        field[i][j][k].ShowSelf();
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
  */

  for (long long i = size.first - 1; i >= 0; i--) {
    for (long long j = size.second - 1; j >= 0; j--) {
      for (long long k = size.third - 1; k >= 0; k--) {
        next_x = field.at(i).at(j).at(k).second;
        next_y = field.at(i).at(j).at(k).third;
        next_z = field.at(i).at(j).at(k).fourth;

        field.at(i).at(j).at(k).second =
            field.at(next_x).at(next_y).at(next_z).second;
        field.at(i).at(j).at(k).third =
            field.at(next_x).at(next_y).at(next_z).third;
        field.at(i).at(j).at(k).fourth =
            field.at(next_x).at(next_y).at(next_z).fourth;
      }
    }
  }
}

int main() {
  long long x_size;
  long long y_size;
  long long z_size;
  std::cin >> x_size >> y_size >> z_size;

  long long x_range;
  long long y_range;
  long long z_range;
  std::cin >> x_range >> y_range >> z_range;

  std::vector<std::vector<std::vector<Quadro>>> field(
      x_size,
      std::vector<std::vector<Quadro>>(y_size, std::vector<Quadro>(z_size)));

  for (long long i = 0; i < x_size; i++) {
    for (long long j = 0; j < y_size; j++) {
      for (long long k = 0; k < z_size; k++) {
        std::cin >> field.at(i).at(j).at(k).first;
        field.at(i).at(j).at(k).second = i;
        field.at(i).at(j).at(k).third = j;
        field.at(i).at(j).at(k).fourth = k;
      }
    }
  }

  Triplet size{x_size, y_size, z_size};
  Triplet range{x_range, y_range, z_range};

  PreprocessField(field, size, range);

  long long quest;
  std::cin >> quest;

  long long x;
  long long y;
  long long z;

  Quadro elem;

  for (long long i = 0; i < quest; i++) {
    std::cin >> x >> y >> z;
    elem = field.at(x).at(y).at(z);
    std::cout << elem.second << " " << elem.third << " " << elem.fourth << "\n";
  }
}