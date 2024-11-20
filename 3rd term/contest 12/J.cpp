#include <time.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

constexpr long long cSmallRange = 1e6;
constexpr __int128_t cMagicNumber = 41;
constexpr long long cStart = 7;
constexpr int cIterationsWithNoGcd = 2047;
const std::vector<int> cBases = {2, 3, 9931, 81929, 904733};
std::unordered_set<long long> small_primes = {2};

long long StepFunction(long long x, long long n) {
  __int128_t new_x = (__int128_t)x;
  return (new_x * new_x + cMagicNumber) % n;
}

bool IsPrimeNaive(long long x) {
  for (long long i = 2; i * i <= x; ++i) {
    if (x % i == 0) {
      return false;
    }
  }

  return true;
}

long long Power(long long x, long long pow, long long mod) {
  if (pow == 1) {
    return x;
  }

  if (pow % 2 == 0) {
    return Power(((__int128_t)x * (__int128_t)x) % mod, pow / 2, mod);
  }

  return ((__int128_t)x * (__int128_t)Power(x, pow - 1, mod)) % mod;
}

bool IsPrimeFermat(long long a, long long x) { return Power(a, x - 1, x) == 1; }

bool IsPrimeDetermenistic(long long x) {
  if (x < cSmallRange) {
    return small_primes.find(x) != small_primes.end();
  }

  for (size_t i = 0; i < cBases.size(); ++i) {
    if (!IsPrimeFermat(cBases[i], x)) {
      return false;
    }
  }

  return true;
}

std::vector<long long> FactorPollard(long long x,
                                     bool check_small_primes = true) {
  std::vector<long long> ans;

  if (check_small_primes) {
    for (long long cur : small_primes) {
      while (x % cur == 0) {
        x /= cur;
        ans.push_back(cur);
      }
    }
  }

  if (x == 1) {
    return ans;
  }

  if (IsPrimeDetermenistic(x)) {
    ans.push_back(x);
    return ans;
  }

  long long h = cStart;
  long long t = cStart;
  __int128_t cur = 1;
  __int128_t new_cur;
  int iter = 0;
  long long g;
  while (true) {
    ++iter;
    h = StepFunction(h, x);
    t = StepFunction(StepFunction(t, x), x);
    // std::cout << h << " " << t << "\n";

    new_cur = (std::abs(h - t) * cur) % x;

    if (new_cur == 0 || (iter & cIterationsWithNoGcd) == 0) {
      g = std::gcd((long long)x, ((long long)(new_cur > 0 ? new_cur : cur)));
      cur = 1;
      if (g > 1 && g < x) {
        std::vector<long long> part1 = FactorPollard(g, false);
        std::vector<long long> part2 = FactorPollard(x / g, false);
        for (long long i : part1) {
          ans.push_back(i);
        }
        for (long long i : part2) {
          ans.push_back(i);
        }
        return ans;
      }
    } else {
      cur = new_cur;
    }
  }

  return ans;
}

signed main() {
  for (long long i = 3; i < cSmallRange; i += 2) {
    if (IsPrimeNaive(i)) {
      small_primes.insert(i);
    }
  }

  int test_case;
  std::cin >> test_case;

  // double time1 = 0;

  while (test_case-- > 0) {
    long long x;
    std::cin >> x;
    // time1 = (double)(clock()) / CLOCKS_PER_SEC;

    if (x == 1) {
      std::cout << "0\n";
      continue;
    }

    // for (int i = 0; i < 100; ++i) {

    std::vector<long long> res = FactorPollard(x);
    std::sort(res.begin(), res.end());
    std::cout << res.size() << " ";
    for (long long i : res) {
      std::cout << i << " ";
    }
    std::cout << "\n";

    // }
  }

  // std::cout << "run time: " << (double)(clock()) / CLOCKS_PER_SEC - time1;
}

// 282449735347077428