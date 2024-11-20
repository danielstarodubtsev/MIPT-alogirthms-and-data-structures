#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

constexpr long long cMod = 7340033;
constexpr long long cBaseW = 5;
constexpr long long cSmall = 256;
constexpr long long cRealMod = 7;
std::vector<long long> w_powers;

long long Power(long long x, long long pow, long long mod) {
  if (pow == 0) {
    return 1;
  }

  if (pow == 1) {
    return x;
  }

  if (pow % 2 == 0) {
    return Power((x * x) % mod, pow / 2, mod);
  }

  return (x * Power(x, pow - 1, mod)) % mod;
}

void MakeCurW(long long k) {
  w_powers.resize(k);
  w_powers[0] = 1;
  long long base = Power(cBaseW, (cMod - 1) / (k << 1), cMod);

  for (long long i = 1; i < k; ++i) {
    w_powers[i] = (w_powers[i - 1] * base) % cMod;
  }
}

void FFT(std::vector<long long>& a) {
  long long n = a.size();

  for (long long k = n >> 1; k > 0; k >>= 1) {
    MakeCurW(k);

    for (long long i = 0; i < n; i += k << 1) {
      for (long long j = 0; j < k; ++j) {
        long long y = a[i + j + k];
        a[i + j + k] = ((a[i + j] - y + cMod) * w_powers[j]) % cMod;
        a[i + j] = (a[i + j] + y) % cMod;
      }
    }
  }
}

void IFFT(std::vector<long long>& a) {
  long long n = a.size();

  for (long long k = 1; k < n; k <<= 1) {
    MakeCurW(k);

    for (long long i = 0; i < n; i += k << 1) {
      for (long long j = 0; j < k; ++j) {
        long long x = a[i + j];
        long long y = (a[i + j + k] * w_powers[j]) % cMod;
        a[i + j] = (x + y) % cMod;
        a[i + j + k] = (x - y + cMod) % cMod;
      }
    }
  }

  for (long long i = 0; i < n; i++) {
    a[i] = (a[i] * (cMod - (cMod - 1) / n)) % cMod;
  }

  std::reverse(a.begin() + 1, a.end());
}

std::vector<long long> MultiplyNaive(std::vector<long long> a,
                                     std::vector<long long> b) {
  long long p = a.size() + b.size() - 1;
  std::vector<long long> ans(p, 0);

  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = 0; j < b.size(); ++j) {
      ans[i + j] = (ans[i + j] + a[i] * b[j]) % cMod;
    }
  }

  return ans;
}

void MultiplyInPlace(std::vector<long long>& a, std::vector<long long> b) {
  if (a.size() < cSmall || b.size() < cSmall) {
    a = MultiplyNaive(a, b);
    return;
  }

  long long len = a.size() + b.size() - 1;
  long long k = (1LL << ((long long)std::log2(len - 1) + 2LL));
  a.resize(k, 0);
  b.resize(k, 0);

  FFT(a);
  FFT(b);

  for (long long i = 0; i < k; ++i) {
    a[i] = (a[i] * b[i]) % cMod;
  }

  IFFT(a);
  a.resize(len);
}

std::vector<long long> InvertPolynom(std::vector<long long> a) {
  if (a.size() == 1) {
    return {Power(a[0], cRealMod - 2, cRealMod)};
  }

  long long m = ((a.size() + 1) >> 1);
  std::vector<long long> new_a(m);

  for (long long i = 0; i < m; ++i) {
    new_a[i] = a[i];
  }

  std::vector<long long> new_b = InvertPolynom(new_a);

  for (size_t i = 0; i < new_b.size(); ++i) {
    new_b[i] %= cRealMod;
  }

  std::vector<long long> r1 = new_b;
  MultiplyInPlace(r1, new_b);

  for (size_t i = 0; i < r1.size(); ++i) {
    r1[i] %= cRealMod;
  }

  std::vector<long long> res = a;
  MultiplyInPlace(res, r1);

  for (size_t i = 0; i < res.size(); ++i) {
    res[i] %= cRealMod;
  }

  for (long long i = 0; i < (long long)res.size(); ++i) {
    res[i] = cRealMod - res[i];

    if (i < m) {
      res[i] += new_b[i] + new_b[i];
    }

    res[i] %= cRealMod;
  }

  res.resize(a.size());
  return res;
}

std::vector<long long> Divide(std::vector<long long> a,
                              std::vector<long long> b) {
  long long n = a.size();
  long long m = b.size();

  if (n < m) {
    return {0};
  }

  long long k = n == m ? 1 : 1LL << ((long long)std::log2(n - m) + 1);
  std::reverse(a.begin(), a.end());
  std::reverse(b.begin(), b.end());
  a.resize(k);
  b.resize(k);
  b = InvertPolynom(b);

  for (size_t i = 0; i < b.size(); ++i) {
    b[i] %= cRealMod;
  }

  MultiplyInPlace(a, b);

  for (size_t i = 0; i < a.size(); ++i) {
    a[i] %= cRealMod;
  }

  a.resize(n - m + 1);
  std::reverse(a.begin(), a.end());
  return a;
}

std::pair<std::vector<long long>, std::vector<long long>> DivideWithRemainder(
    std::vector<long long> a, std::vector<long long> b) {
  long long m = b.size();
  std::vector<long long> c = Divide(a, b);

  for (size_t i = 0; i < c.size(); ++i) {
    c[i] %= cRealMod;
  }

  MultiplyInPlace(b, c);

  for (size_t i = 0; i < b.size(); ++i) {
    b[i] %= cRealMod;
  }

  a.resize(m - 1);

  for (long long i = 0; i < m; ++i) {
    a[i] = (a[i] - b[i] + cRealMod) % cRealMod;
  }

  while (!c.empty() && c.back() == 0) {
    c.pop_back();
  }

  while (!a.empty() && a.back() == 0) {
    a.pop_back();
  }

  if (c.empty()) {
    c.push_back(0);
  }

  if (a.empty()) {
    a.push_back(0);
  }

  return {c, a};
}

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  long long n;
  std::cin >> n;
  std::vector<long long> a(n + 1);

  for (long long i = n; i >= 0; --i) {
    std::cin >> a[i];
  }

  long long m;
  std::cin >> m;
  std::vector<long long> b(m + 1);

  for (long long i = m; i >= 0; --i) {
    std::cin >> b[i];
  }

  auto [first, second] = DivideWithRemainder(a, b);
  std::reverse(first.begin(), first.end());
  std::reverse(second.begin(), second.end());

  std::cout << first.size() - 1 << " ";

  for (size_t i = 0; i < first.size(); ++i) {
    std::cout << first[i] << " ";
  }

  std::cout << "\n" << second.size() - 1 << " ";

  for (size_t i = 0; i < second.size(); ++i) {
    std::cout << second[i] << " ";
  }
}