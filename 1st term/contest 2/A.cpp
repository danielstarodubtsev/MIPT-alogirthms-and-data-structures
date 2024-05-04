#include <iostream>
#include <string>
#include <vector>

int main() {
  std::string sequence;
  std::cin >> sequence;

  std::vector<char> stack;
  char last_char;
  for (char current_char : sequence) {
    if (current_char == '[' || current_char == '(' || current_char == '{') {
      stack.push_back(current_char);
      continue;
    }
    if (stack.empty()) {
      std::cout << "NO";
      return 0;
    }
    last_char = stack[stack.size() - 1];
    stack.pop_back();
    if ((last_char == '{' && current_char == '}') ||
        (last_char == '[' && current_char == ']') ||
        (last_char == '(' && current_char == ')')) {
      continue;
    }
    std::cout << "NO";
    return 0;
  }
  if (stack.empty()) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
}