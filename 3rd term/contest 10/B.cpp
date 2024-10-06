#include <iostream>
#include <string>
#include <vector>

const long long cB = 31;
const long long cMod = 1000000007;
const long long cMod2 = cMod * cMod;
const long long cMaxN = 3e5 + 7;

long long SubstrHash(long long begin, long long end,
                     std::vector<long long>& pref_hash,
                     std::vector<long long>& pow, int n) {
  if (end >= n) {
    return cMod;
  }
  return (pref_hash[end + 1] - pref_hash[begin] * pow[end - begin + 1] +
          cMod2) %
         cMod;
}

signed main() {
  std::string str;
  std::cin >> str;
  int n = str.size();

  std::vector<long long> pow(cMaxN, 1);
  for (long long i = 1; i < cMaxN; ++i) {
    pow[i] = (pow[i - 1] * cB) % cMod;
  }

  std::vector<long long> pref_hash(n + 1);
  for (long long i = 1; i <= n; ++i) {
    pref_hash[i] = (pref_hash[i - 1] * cB + str[i - 1]) % cMod;
  }
  std::vector<std::vector<int>> max_from_here(n + 1, std::vector<int>(n + 1));
  int ans = 0;
  for (long long begin = n - 1; begin >= 0; --begin) {
    for (long long size = 1; size <= n - begin; ++size) {
      int new_ans = 1;
      if (SubstrHash(begin, begin + size - 1, pref_hash, pow, n) ==
          SubstrHash(begin + size, begin + 2 * size - 1, pref_hash, pow, n)) {
        new_ans = max_from_here[begin + size][size] + 1;
      }
      max_from_here[begin][size] = new_ans;
      ans = std::max(ans, new_ans);
    }
  }
  std::cout << ans;
}