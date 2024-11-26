#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

const long long cMult = 31;
const long long cMod = 1e9 + 9;
const long long cMaxN = 1e6 + 7;

struct PairHash {
 public:
  template <typename T, typename U>
  size_t operator()(const std::pair<T, U>& x) const {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  int n;
  int k;
  std::cin >> n >> k;
  std::vector<std::vector<long long>> pref_hash(n);
  std::vector<std::vector<long long>> suf_hash(n);

  std::string cur;
  for (int i = 0; i < n; ++i) {
    std::cin >> cur;
    long long pref = 0;
    long long suf = 0;
    for (int j = 0; j < int(cur.size()); ++j) {
      pref_hash[i].push_back(pref);
      suf_hash[i].push_back(suf);
      pref = (pref * cMult + cur[j]) % cMod;
      suf = (suf * cMult + cur[cur.size() - 1 - j]) % cMod;
    }
    pref_hash[i].push_back(pref);
    suf_hash[i].push_back(suf);
  }

  std::vector<
      std::unordered_map<std::pair<long long, long long>, int, PairHash>>
      ans(cMaxN);
  std::vector<int> ans_real(cMaxN);
  for (int i = 0; i < n; ++i) {
    for (int l = 0; l < int(pref_hash[i].size()); ++l) {
      if (++ans[l][{pref_hash[i][l], suf_hash[i][l]}] == k) {
        ++ans_real[l];
      }
    }
  }

  int qst;
  std::cin >> qst;
  while (qst-- > 0) {
    int l;
    std::cin >> l;
    std::cout << ans_real[l] << "\n";
  }
}