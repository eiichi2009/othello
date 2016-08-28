#include <iostream>
#include "othello.hpp"

using namespace std;

char ban[91];
int te;

void set_ban()
{
  for (int i = 0; i < 91; ++i) {
    ban[i] = WALL;
  }

  for (int i = 1; i <= 8; ++i) {
    for (int j = 1; j <= 8; ++j) {
      ban[i + j*9] = BLANK;
    }
  }

  ban[D4] = ban[E5] = WHITE;
  ban[E4] = ban[D5] = BLACK;
}
  
void disp_ban()
{
  int black = 0;
  int white = 0;
  cout << "  a b c d e f g h" << endl;
  for (int i = 1; i <= 8; ++i) {
    cout << i;
    for (int j = 1; j <= 8; ++j) {
      char c = ban[i*9 + j];
      cout << " " << c;
      if (c == BLACK) {
	black++;
      }
      else if (c == WHITE) {
	white++;
      }
    }
    cout << endl;
  }
  cout << "  Black:" << black << "  White:" << white << endl;
  cout << endl;
  
  return;
}

void rev_ban()
{
  for (int i = 0; i < 91; ++i) {
    if (ban[i] == BLACK) {
      ban[i] = WHITE;
    }
    else if (ban[i] == WHITE) {
      ban[i] = BLACK;
    }
  }
  return;
}

int sekisa;
char yusen[] = {
  A1, H1, A8, H8,
  D3, E3, C4, C5, F4, F5, D6, E6,
  C3, F3, C6, F6,
  D2, E2, B4, B5, G4, G5, D7, E7,
  C1, F1, A3, A6, H3, H6, C8, F8,
  D1, E1, A4, A5, H4, H5, D8, E8,
  C2, F2, B3, B6, G3, G6, C7, F7,
  B1, G1, A2, A7, H2, H7, B8, G8,
  B2, G2, B7, G7,
  0
};

#define MAXCELL 7000
CELL mycell[MAXCELL];
CELL *myfree;
int nc;
void init_cell()
{
  for (int i = 0; i < MAXCELL-1; ++i) {
    mycell[i].cdr = &mycell[i+1];
  }

  mycell[MAXCELL-1].cdr = NULL;
  myfree = mycell;
  nc = MAXCELL;
}

int uscan3(char *bp, int vec)
{
  bp += vec;
  int num = 0;
  while (*bp == WHITE) {
    num++;
    bp += vec;
  }

  if (num != 0 && *bp == BLACK) {
    return num;
  }
  else {
    return 0;
  }
}

int uscan4(char *bp, int vec)
{
  bp += vec;
  int num = 0;
  while (*bp == BLACK) {
    num++;
    bp += vec;
  }

  if (num != 0 && *bp == WHITE) {
    return num;
  }
  else {
    return 0;
  }
}


int isable1(char *bp)
{
  if (uscan3(bp, VECT1) ||
      uscan3(bp, VECT2) ||
      uscan3(bp, VECT3) ||
      uscan3(bp, VECT4) ||
      uscan3(bp, VECT5) ||
      uscan3(bp, VECT6) ||
      uscan3(bp, VECT7) ||
      uscan3(bp, VECT8)) {
    return 1;
  }
  else {
    return 0;
  }
}

int isable2(char *bp)
{
  if (uscan4(bp, VECT1) ||
      uscan4(bp, VECT2) ||
      uscan4(bp, VECT3) ||
      uscan4(bp, VECT4) ||
      uscan4(bp, VECT5) ||
      uscan4(bp, VECT6) ||
      uscan4(bp, VECT7) ||
      uscan4(bp, VECT8)) {
    return 1;
  }
  else {
    return 0;
  }
} 

void com()
{
  int kuusho = 0;

  if (te) {
    return;
  }

  char **a;
  char *bp;
  for (int i = 0; i < 60; ++i) {
    bp = ban + yusen[i];
    if (*bp == BLANK) {
      *a = bp;
      a++;
      kuusho++;
    }
  }
  *a = 0;
  sekisa = 0;
  for (int i = 0; i <= 8; ++i) {
    for (int j = 0; j <= 8; ++j) {
      if (ban[i*9 + j] == BLACK) {
	sekisa++;
      }
      else if (ban[i*9 + j] == WHITE) {
	sekisa--;
      }
    }
  }

  init_cell();

}    


int stat()
{
  int n1 = 0;
  int n2 = 0;
  te = 0;

  for (int t = A1; t <= H8; ++t) {
    if (ban[t] != BLANK) {
      continue;
    }

    if (isable1(ban + t)) {
      te = t;
      n1++;
    }

    if (isable2(ban + t)) {
      n2++;
    }
  }

  if (n1) {
    return n1;
  }
  else if (n2) {
    return 0;
  }
  else {
    te = -1;
    return -1;
  }
}

int uscan1(char *bp, int vec)
{
  bp += vec;
  int num = 0;
  while (*bp == WHITE) {
    num++;
    bp += vec;
  }
  if (num == 0 || *bp != BLACK) {
    return 0;
  }

  int n = num;
  bp -= vec;
  while (n) {
    *bp = BLACK;
    n--;
  }

  return num;  
}

int uscan2(char *bp, int vec)
{
  bp += vec;
  int num = 0;
  while (*bp == BLACK) {
    num++;
    bp += vec;
  }
  if (num == 0 || *bp != WHITE) {
    return 0;
  }

  int n = num;
  bp -= vec;
  while (n) {
    *bp = WHITE;
    n--;
  }

  return num;  
}

int put1(char *bp)
{
  int turn = uscan1(bp, VECT1) +
    uscan1(bp, VECT2) +
    uscan1(bp, VECT3) +
    uscan1(bp, VECT4) +
    uscan1(bp, VECT5) +
    uscan1(bp, VECT6) +
    uscan1(bp, VECT7) +
    uscan1(bp, VECT8);
  if (turn == 0) {
    return 0;
  }

  *bp = BLACK;
  return turn;
}

int put2(char *bp)
{
  int turn = uscan2(bp, VECT1) +
    uscan2(bp, VECT2) +
    uscan2(bp, VECT3) +
    uscan2(bp, VECT4) +
    uscan2(bp, VECT5) +
    uscan2(bp, VECT6) +
    uscan2(bp, VECT7) +
    uscan2(bp, VECT8);
  if (turn == 0) {
    return 0;
  }

  *bp = WHITE;
  return turn;
}

#define TEBAN_BLACK 0
#define TEBAN_WHITE 1
void move(int teban)
{
  if (teban == TEBAN_WHITE) {
    rev_ban();
  }

  if (stat() > 1) {
    com();
  }
  
  if (teban == TEBAN_WHITE) {
    cout << "white:";
  }
  else {
    cout << "black:";
  }

  if (te > 0) {
    char c = (te % 9) + 96;
    char n = te / 9 + 48;
    cout << c << n;
  }
  else {
    cout << "PASS";
  }
  cout << endl;

  if (teban == TEBAN_WHITE) {
    rev_ban();
  }

  return;
}

int main(int ac, char **av)
{
  set_ban();

  int teban = 0;
  while (1) {
    disp_ban();
    move(teban);
    if (te == -1) {
      break;
    }

    if (te) {
      if (teban == TEBAN_BLACK) {
	put1(ban + te);
      }
      else {
	put2(ban + te);
      }
    }
    teban = 1 - teban;
  }
  
  return 0;
}
