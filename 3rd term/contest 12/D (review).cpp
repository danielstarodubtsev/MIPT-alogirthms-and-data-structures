#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

const long long cInf = (1LL << 31) + 7;

long long Power(long long x, long long pow, long long mod) {
  if (pow == 1) {
    return x;
  }

  if (pow % 2 == 0) {
    return Power((x * x) % mod, pow / 2, mod);
  }

  return (x * Power(x, pow - 1, mod)) % mod;
}

signed main() {
  for (std::string cur; std::getline(std::cin, cur);) {
    long long spaces = 0;
    std::string p_str;
    std::string a_str;
    std::string b_str;
    for (char c : cur) {
      if (c == ' ') {
        ++spaces;
        continue;
      }
      if (spaces == 0) {
        p_str.push_back(c);
      } else if (spaces == 1) {
        a_str.push_back(c);
      } else {
        b_str.push_back(c);
      }
    }

    long long p = std::stoi(p_str);
    long long a = std::stoi(a_str);
    long long b = std::stoi(b_str);

    long long h = (long long)std::sqrt(p) + 1;
    long long c = Power(a, h, p);

    std::set<std::pair<long long, long long>> table1;
    std::set<std::pair<long long, long long>> table2;

    long long cur_c = c;
    long long cur_a = b;

    for (long long u = 1; u <= h; ++u) {
      table1.insert({cur_c, u});
      cur_c = (cur_c * c) % p;
    }
    for (long long v = 0; v <= h; ++v) {
      table2.insert({cur_a, v});
      cur_a = (cur_a * a) % p;
    }

    long long best = cInf;
    while (!table1.empty() && !table2.empty()) {
      long long top1 = (*table1.begin()).first;
      long long u = (*table1.begin()).second;
      long long top2 = (*table2.begin()).first;
      if (top1 < top2) {
        table1.erase(table1.begin());
      } else if (top2 < top1) {
        table2.erase(table2.begin());
      } else {
        auto cur = table2.begin();
        while (cur != table2.end() && (*cur).first == top1) {
          long long v = (*cur).second;
          best = std::min(best, h * u - v);
          ++cur;
        }

        table1.erase(table1.begin());
      }
    }
    if (best == cInf) {
      std::cout << "no solution\n";
    } else {
      std::cout << best << "\n";
    }
  }
}