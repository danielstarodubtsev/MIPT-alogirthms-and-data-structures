#include <cassert>
#include <iostream>

long long Power(long long x, long long pow, long long mod) {
  if (pow == 1) {
    return x;
  }

  if (pow % 2 == 0) {
    return Power((x * x) % mod, pow / 2, mod);
  }

  return (x * Power(x, pow - 1, mod)) % mod;
}

long long SquareTimes(long long x, long long times, long long mod) {
  if (times == 0) {
    return x;
  }

  return SquareTimes(Power(x, 2, mod), times - 1, mod);
}

long long LegendreSymbol(long long a, long long p) {
  if (a == 0) {
    return 1;
  }

  return Power(a, p / 2, p);
}

long long Solve(long long n, long long p) {
  if (LegendreSymbol(n, p) != 1) {
    return -1;
  }

  if (n == 0) {
    return 0;
  }

  if (p == 2) {
    return 1;
  }

  long long s = 0;
  long long q = p - 1;
  while ((q & 1) == 0) {
    q >>= 1;
    ++s;
  }

  if (s == 1) {
    return Power(n, (p + 1) / 4, p);
  }

  long long z = 2;
  while (LegendreSymbol(z, p) != p - 1) {
    ++z;
  }

  long long c = Power(z, q, p);
  long long r = Power(n, (q + 1) / 2, p);
  long long t = Power(n, q, p);
  long long m = s;
  while (true) {
    if (t == 1) {
      return r;
    }

    long long i = 0;
    long long cur_t = t;
    while (cur_t != 1) {
      ++i;
      cur_t = Power(cur_t, 2, p);
    }

    long long b = SquareTimes(c, m - i - 1, p);
    r = (r * b) % p;
    t = (t * b) % p;
    t = (t * b) % p;
    c = (b * b) % p;
    m = i;
  }
}

signed main() {
  long long t;
  std::cin >> t;

  while (t-- > 0) {
    long long n;
    long long p;
    std::cin >> n >> p;
    n %= p;

    long long res = Solve(n, p);
    if (res >= 0) {
      std::cout << Solve(n, p) << "\n";
    } else {
      std::cout << "IMPOSSIBLE\n";
    }
  }
}