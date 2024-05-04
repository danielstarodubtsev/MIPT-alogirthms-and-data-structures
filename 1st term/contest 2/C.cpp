#include <deque>
#include <iostream>
#include <string>

int main() {
  size_t cnt;
  std::cin >> cnt;

  std::deque<int> first_deque;
  std::deque<int> second_deque;

  std::string query;
  int num;

  for (size_t i = 0; i < cnt; i++) {
    std::cin >> query;

    if (query == "+") {
      std::cin >> num;
      if (first_deque.size() == second_deque.size()) {
        if (first_deque.empty()) {
          first_deque.push_back(num);
        } else {
          first_deque.push_back(second_deque.front());
          second_deque.pop_front();
          second_deque.push_back(num);
        }
      } else {
        second_deque.push_back(num);
      }
    } else if (query == "*") {
      std::cin >> num;
      if (first_deque.size() == second_deque.size()) {
        first_deque.push_back(num);
      } else {
        second_deque.push_front(num);
      }
    } else {
      std::cout << first_deque.front() << "\n";
      first_deque.pop_front();
      if (first_deque.size() < second_deque.size()) {
        first_deque.push_back(second_deque.front());
        second_deque.pop_front();
      }
    }
  }
}