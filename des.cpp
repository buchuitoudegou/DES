#include "des.hpp"

bitset<32> Des::Feistel(const bitset<32> R, const bitset<48> K) {
  bitset<48> extR;
  for (int i = 0; i < 48; ++i) {
    extR[i] = R[E[i] - 1];
  }
  extR = extR ^ K;
  bitset<32> result;
  for (int group = 0; group < 8; ++group) {
    int row = extR[group * 6] * 2 + extR[group * 6 + 5];
    int col = extR[group * 6 + 1] * 8 + extR[group * 6 + 2] * 4 + extR[group * 6 + 3] * 2
            + extR[group * 6 + 4];
    int snum = S_BOX[group][row][col];
    bitset<4> sresult(snum);
    for (int i = 0; i < 4; ++i) {
      result[group * 4 + i] = sresult[i];
    }
  }
  return result;
}
void Des::generateKeys() {
  bitset<56> smallKey;
  for (int i = 0; i < 56; ++i) {
    smallKey[i] = Key[PC_1[i] - 1];
  }
  bitset<28> preL, preR;
  for (int j = 0; j < 56; ++j) {
    if (j < 28) {
      preL[j] = smallKey[j];
    } else {
      preR[j - 28] = smallKey[j];
    }
  }
  for (int i = 0; i < 16; ++i) {
    bitset<28> shiftL, shiftR;
    for (int j = 0; j < 28; ++j) {
      int index = j - shiftBits[i];
      if (index < 0) {
        index += 28;
      }
      shiftL[j] = preL[index];
      shiftR[j] = preR[index];
    }
    preL = shiftL;
    preR = shiftR;
    bitset<56> Ki;
    bitset<48> compressKi;
    for (int j = 0; j < 56; ++j) {
      if (j < 28) {
        Ki[j] = shiftL[j];
      } else {
        Ki[j] = shiftR[j - 28];
      }
    }
    for (int j = 0; j < 48; ++j) {
      compressKi[j] = Ki[PC_2[j] - 1];
    }
    subKey[i] = compressKi;
  }
}
bitset<64> Des::encrypt(const bitset<64>& src) {
  bitset<64> IP_src;
  for (int i = 0; i < 64; ++i) {
    IP_src[i] = src[IP[i] - 1];
  }
  bitset<32> L, R;
  for (int i = 0; i < 64; ++i) {
    if (i < 32) {
      L[i] = IP_src[i];
    } else {
      R[i - 32] = IP_src[i];
    }
  }
  for (int i = 0; i < 16; ++i) {
    bitset<32> temp = L;
    L = R;
    R = temp ^ Feistel(R, subKey[i]);
  }
  bitset<64> result;
  for (int i = 0; i < 64; ++i) {
    if (i < 32) {
      result[i] = R[i];
    } else {
      result[i] = L[i - 32];
    }
  }
  bitset<64> ip_result;
  for (int i = 0; i < 64; ++i) {
    ip_result[i] = result[IP_T[i] - 1];
  }
  return ip_result;
}
bitset<64> Des::decrypt(const bitset<64>& src) {
  bitset<64> IP_src;
  for (int i = 0; i < 64; ++i) {
    IP_src[i] = src[IP[i] - 1];
  }
  bitset<32> L, R;
  for (int i = 0; i < 64; ++i) {
    if (i < 32) {
      L[i] = IP_src[i];
    } else {
      R[i - 32] = IP_src[i];
    }
  }
  for (int i = 0; i < 16; ++i) {
    bitset<32> temp = L;
    L = R;
    R = temp ^ Feistel(R, subKey[15 - i]);
  }
  bitset<64> result;
  for (int i = 0; i < 64; ++i) {
    if (i < 32) {
      result[i] = R[i];
    } else {
      result[i] = L[i - 32];
    }
  }
  bitset<64> ip_result;
  for (int i = 0; i < 64; ++i) {
    ip_result[i] = result[IP_T[i] - 1];
  }
  return ip_result;
}