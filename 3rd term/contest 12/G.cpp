#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

constexpr long long cMod = 7340033;
constexpr long long cBaseW = 5;
constexpr long long cSmall = 256;
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

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  long long m;
  long long n;
  std::cin >> m >> n;
  std::vector<long long> p(n + 1);
  for (long long i = 0; i <= n; ++i) {
    std::cin >> p[i];
  }

  if (p[0] == 0) {
    std::cout << "The ears of a dead donkey";
    return 0;
  }

  std::vector<long long> q = {Power(p[0], cMod - 2, cMod)};

  long long cur_m = 1;
  while (cur_m < m) {
    std::vector<long long> p0;
    std::vector<long long> p1;
    for (long long i = 0; i <= n; ++i) {
      if (i < cur_m) {
        p0.push_back(p[i]);
      } else if (i < cur_m * 2) {
        p1.push_back(p[i]);
      }
    }
    p0.resize(cur_m);
    p1.resize(cur_m);
    MultiplyInPlace(p0, q);
    p0.resize(2 * cur_m);
    std::vector<long long> r(cur_m);
    for (long long i = cur_m; i < 2 * cur_m; ++i) {
      r[i - cur_m] = p0[i];
    }
    MultiplyInPlace(p1, q);
    p1.resize(cur_m);
    for (long long i = 0; i < cur_m; ++i) {
      r[i] = (r[i] + p1[i]) % cMod;
    }
    MultiplyInPlace(r, q);
    r.resize(cur_m);
    for (long long i = 0; i < cur_m; ++i) {
      r[i] = (cMod - r[i]) % cMod;
    }
    q.resize(cur_m * 2);
    for (long long i = 0; i < cur_m; ++i) {
      q[i + cur_m] = (q[i + cur_m] + r[i]) % cMod;
    }

    cur_m <<= 1;
  }

  for (long long i = 0; i < m; ++i) {
    std::cout << q[i] << " ";
  }
}