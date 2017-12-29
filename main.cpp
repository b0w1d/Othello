#define CONCAT(func, name) func##name
#define HAND(name) CONCAT(I2P_, name)

#define FIRST RANDOM  // computer
#define SECOND 106000135    // change to your student ID (ex: 105062999)

#define _CRT_SECURE_NO_WARNINGS

#ifndef FIRST
#error you have to specify macro FIRST
#endif

#ifndef SECOND
#error you have to specify macro SECOND
#endif

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
const int N = 10;
extern pair<int, int> HAND(FIRST)(int Board[][N], int player,
                                  vector<pair<int, int>> ValidPoint);
extern pair<int, int> HAND(SECOND)(int Board[][N], int player,
                                   vector<pair<int, int>> ValidPoint);

void InitialBoard(int a[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) a[i][j] = 0;

  a[4][4] = 2;
  a[5][5] = 2;
  a[4][5] = 1;
  a[5][4] = 1;

  for (int j = 0; j < N; j++) a[0][j] = -1;
  for (int j = 0; j < N; j++) a[N - 1][j] = -1;

  for (int i = 0; i < N; i++) a[i][0] = -1;
  for (int i = 0; i < N; i++) a[i][N - 1] = -1;
}

void BoardShow(int Board[N][N]) {
  cout << "  |-+-+-+-+-+-+-+-|\n";
  for (int i = 1; i < N - 1; i++) {
    cout << " ";
    cout << i;
    cout << "|";
    for (int j = 1; j < N; j++) {
      if (Board[j][i] == 1)
        cout << "x"
             << "|";
      else if (Board[j][i] == 2)
        cout << "o"
             << "|";
      else if (Board[j][i] == 0)
        cout << " "
             << "|";
    }
    cout << "\n";
    if (i != N - 2) cout << "  |-+-+-+-+-+-+-+-|\n";
  }
  cout << "  |-+-+-+-+-+-+-+-|\n";
  cout << "   1 2 3 4 5 6 7 8\n";
  cout << endl;
}

// player=1: black number
// player=2: white number

// valid: return whether the chess can be put at this place or not
bool PointValid(int board[N][N],  // board 10*10
                int player, pair<int, int> point) {
  int opponent = 3 - player;
  vector<pair<int, int>> mydirection;

  mydirection = {{-1, 1}, {0, 1},  {1, 1},   {1, 0},
                 {1, -1}, {0, -1}, {-1, -1}, {-1, 0}};

  for (int i = 0; i < 8; i++) {
    bool lock = false;
    int px = point.first;
    int py = point.second;
    for (;;) {
      px = px + mydirection[i].first;
      py = py + mydirection[i].second;

      if (board[px][py] == -1)
        break;
      else if (board[px][py] == 0)
        break;
      else if (board[px][py] == opponent) {
        lock = true;
        continue;
      } else if (board[px][py] == player) {
        if (lock == true)
          return true;
        else
          break;
      }
    }
  }
  return false;
}

vector<pair<int, int>> BoardPointValid(int board[N][N], int player) {
  vector<pair<int, int>> ans;

  for (int i = 1; i < N - 1; i++) {
    for (int j = 1; j < N - 1; j++) {
      pair<int, int> point(i, j);

      if (board[i][j] == 0) {
        if (PointValid(board, player, point) == true) {
          ans.push_back(point);
        } else {
          continue;
        }
      }
    }
  }
  return ans;
}

void BoardCopy(int boardA[N][N], int boardB[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      boardA[i][j] = boardB[i][j];
    }
  }
}

void show_ValidPoint(vector<pair<int, int>> ValidPoint) {
  cout << " Next point you can put: ";
  for (int i = 0; i < ValidPoint.size(); i++) {
    cout << "(" << ValidPoint[i].first << "," << ValidPoint[i].second << ")"
         << " ";
  }
  cout << endl;
}

bool PlayReversi(int MBoard[N][N], int player, pair<int, int> point,
                 vector<pair<int, int>> ValidPoint) {
  bool ValidLock = false;
  for (int i = 0; i < ValidPoint.size(); i++) {
    if (point == ValidPoint[i])
      ValidLock = true;
    else
      continue;
  }
  if (ValidLock == false) return false;  // invalid

  int px = point.first;
  int py = point.second;

  MBoard[px][py] = player;

  int opponent = 3 - player;

  vector<pair<int, int>> mydirection = {
      pair<int, int>(-1, 1),  pair<int, int>(0, 1),  pair<int, int>(1, 1),
      pair<int, int>(1, 0),   pair<int, int>(1, -1), pair<int, int>(0, -1),
      pair<int, int>(-1, -1), pair<int, int>(-1, 0),
  };

  for (int i = 0; i < 8; i++) {
    bool lock = false;
    int px = point.first;
    int py = point.second;
    for (int in = 0;; in++) {
      px = px + mydirection[i].first;
      py = py + mydirection[i].second;

      if (MBoard[px][py] == -1)
        break;
      else if (MBoard[px][py] == 0)
        break;
      else if (MBoard[px][py] == opponent) {
        lock = true;
        continue;
      } else if (MBoard[px][py] == player) {
        if (lock == true) {
          int ppx = point.first;
          int ppy = point.second;

          for (int j = 0; j < in; j++) {
            ppx = ppx + mydirection[i].first;
            ppy = ppy + mydirection[i].second;
            MBoard[ppx][ppy] = player;
          }
          break;
        }

        else
          break;
      }
    }
  }
  return true;
}

int main() {
  if (freopen("Chess.txt", "w", stdout) == NULL) {
    cout << "fail to read file" << endl;
    return 1;
  }

  srand(time(NULL));

  int MainBoard[N][N];
  int ModeBlackWhite = 0;
  int GP0_win = 0;  // computer win
  int GP1_win = 0;  // student win
  int tie = 0;
  ModeBlackWhite = rand() % 2;  // random first
  // if(ModeBlackWhite==0){
  // 	cout << "GP1 First" << endl;
  // }
  // else{
  // 	cout << "GP0 First" << endl;
  // }
  for (int game = 0; game < 3; ++game) {
    InitialBoard(MainBoard);
    cout << "GAME: " << game << endl;
    if (ModeBlackWhite == 0)
      cout << "Student: x" << endl << "Computer: o" << endl;
    else
      cout << "Student: o" << endl << "Computer: x" << endl;

    for (int i = 1, k = 0; k < 2; i = i % 2 + 1) {
      // cout << "i = " << i << ", mode = " << ModeBlackWhite << endl;
      int CounterpartBoard[N][N];
      BoardCopy(CounterpartBoard, MainBoard);
      BoardShow(MainBoard);

      vector<pair<int, int>> ValidPoint;

      ValidPoint = BoardPointValid(CounterpartBoard, i);

      if (i == 1)
        cout << " (x black)" << endl;
      else if (i == 2)
        cout << " (o white)" << endl;

      show_ValidPoint(ValidPoint);
      if (ValidPoint.size() == 0) {
        k++;
      } else {
        k = 0;
        pair<int, int> point;

        // if(ModeBlackWhite==0)
        // {
        // 	if(i==1)
        // 		point=HAND(SECOND)(CounterpartBoard,i,ValidPoint);
        // 	else
        // 		point=HAND(FIRST)(CounterpartBoard,3-i,ValidPoint);
        // }
        // else
        // {
        // 	if(i==1)
        // 		point=HAND(FIRST)(CounterpartBoard,3-i,ValidPoint);
        // 	else
        // 		point=HAND(SECOND)(CounterpartBoard,i,ValidPoint);
        // }

        if ((ModeBlackWhite + i) != 2) {
          cout << " (Student): ";
          point = HAND(SECOND)(CounterpartBoard, i, ValidPoint);
        } else {
          cout << " (Computer): ";
          point = HAND(FIRST)(CounterpartBoard, i, ValidPoint);
        }

        bool boolplay = PlayReversi(MainBoard, i, point, ValidPoint);

        int px = point.first;
        int py = point.second;

        if (i == 1)
          cout << " x";
        else
          cout << " o";
        cout << "(" << px << "," << py << ")";

        if (boolplay == false) {
          cout << " Error!";
          break;
        }
      }
      cout << endl;
    }

    int black_num = 0;
    int white_num = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (MainBoard[i][j] == 1) black_num++;
        if (MainBoard[i][j] == 2) white_num++;
      }
    }

    if (ModeBlackWhite == 0) {
      if (black_num > white_num)
        GP1_win++;
      else if (black_num < white_num)
        GP0_win++;
      else
        tie++;
    } else {
      if (black_num > white_num)
        GP0_win++;
      else if (black_num < white_num)
        GP1_win++;
      else
        tie++;
    }
    cout << "black num: " << black_num << " white num: " << white_num << endl;
    // if(black_num > white_num)
    // 	cout << "1" << endl;
    // else if(black_num < white_num)
    // 	cout << "0" << endl;
    // else
    // 	cout << "2" << endl;
    // system("pause");
    ModeBlackWhite = !ModeBlackWhite;
  }
  if (GP1_win > GP0_win) {
    cout << "Student Win" << endl;
    // cout<<"1"<<endl;
  } else if (GP1_win < GP0_win) {
    cout << "Computer Win" << endl;
    // cout<<"0"<<endl;
  } else {
    cout << "Tie" << endl;
    // cout<<"2"<<endl;
  }
  return 0;
}
