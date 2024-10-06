#include <iostream>
#include <string>
#include <vector>

const long long cB = 31;
const long long cMod = 1000000007;
const long long cMaxN = 1e6 + 7;

void Combine(std::string& cur, std::string& next, std::vector<long long>& pow) {
  long long hash1 = 0;
  long long hash2 = 0;
  long long max_overlap = 0;
  for (size_t overlap = 1; overlap <= std::min(cur.size(), next.size());
       ++overlap) {
    hash1 =
        ((pow[overlap - 1] * cur[cur.size() - overlap]) % cMod + hash1) % cMod;
    hash2 = ((cB * hash2) % cMod + next[overlap - 1]) % cMod;
    if (hash1 == hash2) {
      max_overlap = overlap;
    }
  }
  for (long long i = 0; i < max_overlap; ++i) {
    cur.pop_back();
  }
  cur += next;
}

signed main() {
  std::vector<long long> pow(cMaxN, 1);
  for (long long i = 1; i < cMaxN; ++i) {
    pow[i] = (pow[i - 1] * cB) % cMod;
  }
  long long n;
  std::cin >> n;
  std::vector<std::string> strings(n);
  for (long long i = 0; i < n; ++i) {
    std::cin >> strings[i];
  }

  std::string cur = strings[0];
  for (long long i = 1; i < n; ++i) {
    Combine(cur, strings[i], pow);
  }
  std::cout << cur;
}