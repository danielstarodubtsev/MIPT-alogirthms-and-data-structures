#include <iostream>
#include <vector>

const int cAlphabetSize = 256;
const int cFirstChar = 97;
const int cLastChar = 127;

std::vector<int> SortCyclicShifts(std::string s) {
  // ! списано с https://cp-algorithms.com/string/suffix-array.html
  int n = s.size();
  std::vector<int> p(n);
  std::vector<int> c(n);
  std::vector<int> cnt(std::max(cAlphabetSize, n), 0);
  for (int i = 0; i < n; i++) {
    cnt[s[i]]++;
  }
  for (int i = 1; i < cAlphabetSize; i++) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = 0; i < n; i++) {
    p[--cnt[s[i]]] = i;
  }
  c[p[0]] = 0;
  int classes = 1;
  for (int i = 1; i < n; i++) {
    if (s[p[i]] != s[p[i - 1]]) {
      classes++;
    }
    c[p[i]] = classes - 1;
  }
  std::vector<int> new_p(n);
  std::vector<int> new_c(n);
  for (int h = 0; (1 << h) < n; ++h) {
    for (int i = 0; i < n; i++) {
      new_p[i] = p[i] - (1 << h);
      if (new_p[i] < 0) {
        new_p[i] += n;
      }
    }
    std::fill(cnt.begin(), cnt.begin() + classes, 0);
    for (int i = 0; i < n; i++) {
      cnt[c[new_p[i]]]++;
    }
    for (int i = 1; i < classes; i++) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
      p[--cnt[c[new_p[i]]]] = new_p[i];
    }
    new_c[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      std::pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
      std::pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
      if (cur != prev) {
        ++classes;
      }
      new_c[p[i]] = classes - 1;
    }
    c.swap(new_c);
  }
  return p;
}

std::string BWT(std::string s) {
  std::vector<int> p = SortCyclicShifts(s);
  std::string res;

  for (size_t i = 0; i < p.size(); ++i) {
    res.push_back(s[(p[i] - 1 + s.size()) % s.size()]);
  }

  return res;
}

std::string InverseBWT(std::string s, int size) {
  std::vector<std::vector<int>> table(cAlphabetSize);

  for (size_t i = 0; i < s.size(); ++i) {
    table[s[i]].push_back(i);
  }
  std::vector<int> shift;
  for (size_t i = cFirstChar; i < cLastChar; ++i) {
    for (int jj : table[i]) {
      shift.push_back(jj);
    }
  }
  std::string ans;
  for (size_t i = 0; i < s.size(); ++i) {
    size = shift[size];
    ans += s[size];
  }
  return ans;
}

std::string MTF(std::string s) {
  std::vector<char> dictionary(cAlphabetSize);
  for (int i = 0; i < cAlphabetSize; ++i) {
    dictionary[i] = i;
  }
  std::string mtf;
  char rank = 0;
  for (char c : s) {
    for (unsigned char i = cFirstChar; i < cLastChar; ++i) {
      if (dictionary[i] == c) {
        rank = i;
        break;
      }
    }
    mtf += rank;
    dictionary.erase(dictionary.begin() + rank);
    dictionary.insert(dictionary.begin() + cFirstChar, c);
  }
  return mtf;
}

std::string InverseMTF(std::string s) {
  std::vector<char> dictionary(cAlphabetSize);
  for (int i = 0; i < cAlphabetSize; ++i) {
    dictionary[i] = i;
  }
  std::string ans;
  for (char c : s) {
    ans += dictionary[c];
    char hah = dictionary[c];
    dictionary.erase(dictionary.begin() + c);
    dictionary.insert(dictionary.begin() + cFirstChar, hah);
  }
  return ans;
}

std::string RLE(std::string s) {
  std::string ans;
  int count = 1;
  char last = s[0];
  for (size_t i = 1; i < s.size(); ++i) {
    if (s[i] != last) {
      ans += last + std::to_string(count);
      count = 1;
      last = s[i];
    } else {
      ++count;
    }
  }
  ans += last + std::to_string(count);
  return ans;
}

std::string InverseRLE(std::string s) {
  std::string ans;
  char last = s[0];
  std::string num;
  for (size_t i = 1; i < s.size(); ++i) {
    if (std::isalpha(s[i]) != 0) {
      for (int j = 0; j < std::stoi(num); ++j) {
        ans += last;
      }
      num.clear();
      last = s[i];
    } else {
      num += s[i];
    }
  }
  for (int j = 0; j < std::stoi(num); ++j) {
    ans += last;
  }
  return ans;
}

int main() {
  int m;
  std::cin >> m;
  std::string s;
  std::cin >> s;
  if (m == 1) {
    std::cout << RLE(MTF(BWT(s))) << '\n';
  } else {
    int n;
    std::cin >> n;
    std::cout << InverseBWT(InverseMTF(InverseRLE(s)), n) << '\n';
  }
}