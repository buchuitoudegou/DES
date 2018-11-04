#include <iostream>
#include <string>
#include "des.hpp"

using namespace std;

bitset<64> stringToBit(const string& s) {
  bitset<64> bits;
  for (int i = 0; i < s.length(); ++i) {
    for (int j = 0; j < 8; ++j) {
      bits[i * 8 + j] = ((s[i] >> j) & 1);
    }
  }
  return bits;
}

string bitToString(const bitset<64>& src) {
  bitset<8> temp;
  string result("");
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      temp[j] = src[i * 8 + j];
    }
    result.push_back(char(temp.to_ullong()));
  }
  return result;
}

int main(int argc, char const *argv[]) {
  string test;
  getline(cin, test);
  if (test.length() % 8 != 0) {
    int u = 8 - test.length() % 8;
    for (int i = 0; i < u; ++i) {
      test.push_back('#');
    }
  }
  bitset<64> key = stringToBit("12345678");
  Des* solution = new Des(key);
  solution->generateKeys();
  string plain("");
  string cipher("");
  for (int i = 0; i < test.length() / 8; ++i) {
    string temp = test.substr(i * 8, 8);
    bitset<64> src = stringToBit(temp);
    bitset<64> en_src = solution->encrypt(src);
    bitset<64> pre_src = solution->decrypt(en_src);
    cipher += bitToString(en_src);
    plain += bitToString(pre_src);
  }
  cout << cipher << endl;
  cout << plain << endl;
  delete solution;
  return 0;
}
