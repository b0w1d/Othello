#include <cstdint>
#include <cstdlib>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cassert>
using namespace std;

namespace s_106000135 {
const int NN = 10;
clock_t timer;
double elapsed();
uint64_t shift(uint64_t board, uint8_t dir);
uint64_t valid_moves(uint64_t fb, uint64_t sb);
int32_t evaluate(uint64_t fb, uint64_t sb);
uint64_t detect_flips(uint64_t fb, uint64_t sb, uint64_t mov);
int32_t negamax(uint64_t fb, uint64_t sb, int32_t alpha, int32_t beta, int8_t depth);
uint64_t decide_move(uint64_t fb, uint64_t sb);
}  // namespace s_106000135

double s_106000135::elapsed() {
  return (1.0 * clock() - timer) / CLOCKS_PER_SEC;
}

uint64_t s_106000135::shift(uint64_t board, uint8_t dir) {
  static const uint8_t rot[] = { 1, 9, 8, 7 }; // l; ul; u; ur
  return dir & 0x04 ? board >> rot[dir & 0x03] : board << rot[dir];
}
uint64_t s_106000135::valid_moves(uint64_t fb, uint64_t sb) {
  static const uint64_t masks[] = {
    0x7e7e7e7e7e7e7e7e,
    0x007e7e7e7e7e7e00,
    0x00ffffffffffff00,
    0x007e7e7e7e7e7e00,
  };
  uint64_t vm = 0;
  for (uint8_t d = 0; d < 8; ++d) {
    uint64_t s = sb & masks[d & 0x03];
    uint64_t t = s & shift(fb, d);
    for (uint8_t i = 0; i < 5; ++i) {
      t |= s & shift(t, d); // move accross foe iff exists
    }
    vm |= ~(fb | sb) & shift(t, d); // check blank instead at last shift
  }
  return vm;
}

int32_t s_106000135::evaluate(uint64_t fb, uint64_t sb) {
  uint64_t corner = 0x8100000000000081;
  uint64_t adj_corner = 0x42c300000000c342;
  int32_t res = 0;
  function<int32_t(uint64_t)> bc = [&](uint64_t b) { return __builtin_popcountll(b); };
  res += (bc(fb) - bc(sb)) * 1024;
  res += (bc(valid_moves(fb, sb)) - bc(valid_moves(sb, fb))) * 4096;
  res -= (bc(fb & adj_corner) - bc(sb & adj_corner)) * 16384;
  res += (bc(fb & corner) - bc(sb & corner)) * 65536;
  return res;
}

uint64_t s_106000135::detect_flips(uint64_t fb, uint64_t sb, uint64_t mov) {
  static const uint64_t masks[] = {
    0xfefefefefefefefe,
    0xfefefefefefefefe,
    0xffffffffffffffff,
    0x7f7f7f7f7f7f7f7f,
    0x7f7f7f7f7f7f7f7f,
    0x7f7f7f7f7f7f7f7f,
    0xffffffffffffffff,
    0xfefefefefefefefe
  };
  uint64_t ub = 0;  
  for (uint8_t d = 0; d < 8; ++d) {
    uint64_t s = sb & masks[d];
    uint64_t mov_t[8] = { mov, 0, 0, 0, 0, 0, 0, 0 };
    for (uint8_t i = 1; i < 8; ++i) {
      if (((mov_t[i] = shift(mov_t[i - 1], d)) & s) == 0) {
        if (mov_t[i] & fb) ub |= accumulate(mov_t + 1, mov_t + i, 0, [&](uint64_t x, uint64_t y) { return x | y; });
        break;
      }
    }
  }
  return ub;
}

int32_t s_106000135::negamax(uint64_t fb, uint64_t sb, int32_t alpha, int32_t beta, int8_t depth) {
  if (depth == 0 || elapsed() > 1.5) return evaluate(fb, sb);
  uint64_t vm = valid_moves(fb, sb);
  if (!vm) return __builtin_popcountll(fb) - __builtin_popcountll(sb);
  for ( ; vm && alpha < beta; vm &= vm - 1) {
    uint64_t mv = vm & -vm;
    uint64_t dflips = detect_flips(fb, sb, mv);
    int32_t new_alpha = -negamax(sb ^ dflips, fb | dflips | mv, -beta, -alpha, depth - 1);
    if (alpha < new_alpha) alpha = new_alpha;
  }
  return alpha;
}

uint64_t s_106000135::decide_move(uint64_t fb, uint64_t sb) {
  static const int8_t search_depth = 7;
  int32_t alpha = -(1 << 29), beta = 1 << 29;
  uint64_t opt;
  for (uint64_t vm = valid_moves(fb, sb); vm && alpha < beta; vm &= vm - 1) {
    uint64_t mv = vm & -vm;
    uint64_t dflips = detect_flips(fb, sb, mv);
    int32_t new_alpha = -negamax(sb ^ dflips, fb | dflips | mv, -beta, -alpha, search_depth);
    if (alpha < new_alpha) {
      alpha = new_alpha;
      opt = mv;
    }
  }
  return opt;
}

pair<int, int> I2P_106000135(int Board[][s_106000135::NN], int player, vector<pair<int, int>> ValidPoint) {
  s_106000135::timer = clock();
  uint64_t fb = 0, sb = 0;
  for (int i = 1; i <= 8; ++i) {
    for (int j = 1; j <= 8; ++j) {
      if (Board[i][j] == player) fb |= 1ULL << (j - 1) * 8 + i - 1;
      else if (Board[i][j] == 3 - player) sb |= 1ULL << (j - 1) * 8 + i - 1;
    }
  }
  uint64_t mov = s_106000135::decide_move(fb, sb);
  for (pair<int, int> p : ValidPoint) {
    if (mov >> (p.second - 1) * 8 + p.first - 1 & 1ULL) return p;
  }
  assert(s_106000135::elapsed() < 1.8);
  return make_pair(-1, -1);
}
