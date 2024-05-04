#include <iostream>
#include <vector>

const long long kMaxValue = 1e6 + 7;
const long long kMod = 1e9 + 7;

int main() {
  long long cnt;
  std::cin >> cnt;

  std::vector<long long> sequence(cnt);
  for (long long i = 0; i < cnt; ++i) {
    std::cin >> sequence[i];
  }

  std::vector<long long> last_index_met(kMaxValue, -1);
  std::vector<long long> dp(cnt, 1);
  std::vector<long long> dp_pref_sum(cnt + 1, 0);
  dp_pref_sum[1] = 1;
  last_index_met[sequence[0]] = 0;

  for (long long i = 1; i < cnt; ++i) {
    long long last_met = last_index_met[sequence[i]];
    last_index_met[sequence[i]] = i;
    dp[i] = (dp_pref_sum[i] - dp_pref_sum[std::max(last_met, 0LL)] + kMod +
             (last_met != -1 ? 0 : 1)) %
            kMod;
    dp_pref_sum[i + 1] = (dp_pref_sum[i] + dp[i]) % kMod;
  }

  long long result = 0;
  for (long long elem : dp) {
    result += elem;
    result %= kMod;
  }

  std::cout << result;
}