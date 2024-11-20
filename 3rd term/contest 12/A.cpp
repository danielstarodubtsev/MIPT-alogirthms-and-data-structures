#include <fstream>
#include <iostream>

constexpr long long cMod = 1e9 + 7;

long long Power(long long x, long long pow, long long mod) {
  if (pow == 1) {
    return x;
  }

  if (pow % 2 == 0) {
    return Power((x * x) % mod, pow / 2, mod);
  }

  return (x * Power(x, pow - 1, mod)) % mod;
}

template <long long mod>
class Mint {
 private:
  long long value_ = 0;

  Mint Inverse() const { return {Power(value_, mod - 2, mod)}; }

 public:
  Mint() = default;

  Mint(long long v) : value_(v >= 0 ? v % mod : (mod + (v % mod)) % mod) {}

  Mint(const Mint& other) = default;

  Mint& operator=(const Mint& other) = default;

  Mint operator-() const {
    if (value_ == 0) {
      return {};
    }

    return {mod - value_};
  }

  Mint& operator+=(const Mint& other) {
    value_ = (value_ + other.value_) % mod;
    return *this;
  }

  Mint& operator-=(const Mint& other) {
    value_ = (value_ - other.value_ + mod) % mod;
  }

  Mint& operator++() { return *this += 1; }

  Mint& operator--() { return *this -= 1; }

  Mint operator++(int) {
    Mint temp = *this;
    *this += 1;
    return temp;
  }

  Mint operator--(int) {
    Mint temp = *this;
    *this -= 1;
    return temp;
  }

  Mint& operator*=(const Mint& other) {
    value_ = (value_ * other.value_) % mod;
    return *this;
  }

  Mint& operator/=(const Mint& other) { return *this *= other.Inverse(); }

  template <long long other_mod>
  friend std::ostream& operator<<(std::ostream&, const Mint<other_mod>&);

  template <long long other_mod>
  friend std::istream& operator>>(std::istream&, Mint<other_mod>&);

  template <long long other_mod>
  friend Mint<other_mod> operator+(const Mint<other_mod>&,
                                   const Mint<other_mod>&);

  template <long long other_mod>
  friend Mint<other_mod> operator-(const Mint<other_mod>&,
                                   const Mint<other_mod>&);

  template <long long other_mod>
  friend Mint<other_mod> operator*(const Mint<other_mod>&,
                                   const Mint<other_mod>&);

  template <long long other_mod>
  friend Mint<other_mod> operator/(const Mint<other_mod>&,
                                   const Mint<other_mod>&);
};

template <long long mod>
Mint<mod> operator+(const Mint<mod>& first, const Mint<mod>& second) {
  return {first.value_ + second.value_};
}

template <long long mod>
Mint<mod> operator-(const Mint<mod>& first, const Mint<mod>& second) {
  return {first.value_ - second.value_};
}

template <long long mod>
Mint<mod> operator*(const Mint<mod>& first, const Mint<mod>& second) {
  return {first.value_ * second.value_};
}

template <long long mod>
Mint<mod> operator/(const Mint<mod>& first, const Mint<mod>& second) {
  Mint<mod> ans = first;
  ans /= second;
  return ans;
}

template <long long mod>
std::ostream& operator<<(std::ostream& os, const Mint<mod>& x) {
  os << x.value_;
  return os;
}

template <long long mod>
std::istream& operator>>(std::istream& is, Mint<mod>& x) {
  is >> x.value_;
  return is;
}

signed main() {
  Mint<cMod> a;
  Mint<cMod> b;
  Mint<cMod> c;
  Mint<cMod> d;
  std::cin >> a >> b >> c >> d;
  std::cout << ((a * d) + (b * c)) / (b * d);
}