#include <iostream>
#include <cstdint>
#include <stack>

#include "othello.hpp"

using namespace std;

void mr2(CELL *p, int tesu, int beta);

char ban[91];
int te;
char *open_space_list[61];
stack<uint64_t> stk;

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

  // white
  ban[E4] = ban[D5] = WHITE;
  ban[C1] = ban[D1] = ban[E1] = ban[F1] = WHITE;
  ban[A2] = ban[C2] = ban[D2] = ban[E2] = ban[F2] = WHITE;
  ban[B3] = ban[D3] = ban[E3] = ban[F3] = ban[G3] = ban[H3] = WHITE;
  ban[C4] = ban[D4] = ban[E4] = ban[F4] = ban[G4] = ban[H4] = WHITE;
  ban[B5] = ban[D5] = ban[E5] = ban[F5] = ban[G5] = ban[H5] = WHITE;
  ban[A6] = ban[B6] = ban[C6] = ban[E6] = ban[F6] = ban[G6] = ban[H6] = WHITE;
  ban[C7] = ban[D7] = ban[F7] = ban[G7] = WHITE;
  ban[C8] = ban[D8] = ban[H8] = WHITE;

  ban[C3] = ban[B4] = ban[A5] = ban[C5] = ban[D6] = ban[E7] = ban[F8] = BLACK;
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

CELL *get_cell()
{
  CELL *p = myfree;
  myfree = p->cdr;
  return p;
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

void mkchild1(CELL *p)
{
  CELL ltop;
  CELL *p1 = &ltop;
  char *bp;
  for (char **ap = open_space_list; *ap; ap++) {
    bp = *ap;
    if (*bp != BLANK) {
      continue;
    }

    if (isable1(bp)) {
      p1 = p1->cdr = get_cell();
      p1->child = NULL;
      p1->t = bp - ban;
    }
  }

  p1->cdr = NULL;

  if (ltop.cdr) {
    p->child = ltop.cdr;
  }
  else {
    p1 = p->child = get_cell();
    p1->child = NULL;
    p1->cdr = NULL;
    p1->t = 0;
  }

  return;
}

void print_cell(CELL *p)
{
  cout << "printing cell" << endl;
  cout << "self = " << p << endl;
  cout << "*child = " << p->child << endl;
  cout << "*cdr = " << p->cdr << endl;
  char c = p->t % 9 + 96;
  char n = p->t / 9 + 48;
  cout << "t = " << c << n << endl;
  cout << "h = " << p->h << endl;
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

void com()
{
  int open_space = 0;

  char **a = open_space_list;
  char *bp;
  for (int i = 0; i < 60; ++i) {
    bp = ban + yusen[i];
    if (*bp == BLANK) {
      *a = bp;
      a++;
      open_space++;
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

  CELL *p = get_cell();
  p->child = NULL;
  p->cdr = NULL;

  if (open_space <= 16) {
    p->h = -64;
    mr2(p, open_space, 64); // perfect read
  }
#if 0
  else {
    int search_depth = 4;
    p->h = -10000;
    //    mr1(p, open_space, 10000); // general read
  }
#endif
  te = p->t;

  return;
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

  int rev = num;
  bp -= vec;
  while (rev) {
    *bp = BLACK;
    stk.push((uint64_t)bp);
    bp -= vec;
    rev--;
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

  int rev = num;
  bp -= vec;
  while (rev) {
    *bp = WHITE;
    stk.push((uint64_t)bp);
    bp -= vec;
    rev--;
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

  sekisa += turn + turn + 1;
  stk.push((uint64_t)bp);
  stk.push((uint64_t)turn);
  
  return turn;
}

void tup1()
{
  int turn = (int)stk.top();
  stk.pop();
  char *bp = (char *)stk.top();
  stk.pop();

  sekisa -= turn + turn + 1;
  *bp = BLANK;

  while (turn) {
    bp = (char *)stk.top();
    stk.pop();
    *bp = WHITE;
    turn--;
  }
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

void tup2()
{
  int turn = (int)stk.top();
  stk.pop();
  char *bp = (char *)stk.top();
  stk.pop();

  sekisa -= turn + turn + 1;
  *bp = BLANK;

  while (turn) {
    bp = (char *)stk.top();
    stk.pop();
    *bp = BLACK;
    turn--;
  }
}

int eval1()
{
  int hyo = 0;
  for (int row = 1; row <= 8; ++row) {
    for (int col = 1; col <= 8; ++col) {
      int loc = row + col*9;
      int sign = 0;
      if (ban[loc] == BLACK) {
	hyo++;
	sign = 1;
      }
      else if (ban[loc] == WHITE) {
	hyo--;
	sign = -1;
      }
      else {
	continue;
      }
      
      if (row == 1 || row == 8 || col == 1 || col == 8) {
	hyo += sign * 2;
      }

      if ((row == 1 && col == 1) ||
	  (row == 1 && col == 8) ||
	  (row == 8 && col == 1) ||
	  (row == 8 && col == 8)) {
	hyo += sign * 4;
      }
    }
  }
  
  return hyo;
}

int eval2()
{
  rev_ban();
  int hyo = eval1();
  rev_ban();
  return -hyo;
}

void sort(CELL *p)
{
  int cnt = 0;
  for (CELL *cur = p->child; cur; cur = cur->cdr) {
    cnt++;
  }
  if (cnt == 1) {
    return;
  }
  
  while (cnt) {
    CELL *p1 = p->child;
    CELL *p2 = p->child->cdr;
    while (p2) {
      if (p1->h < p2->h) {
	int h = p1->h;
	int t = p1->t;
	p1->h = p2->h;
	p1->t = p2->t;
	p2->h = h;
	p2->t = t;
      }

      p1 = p2;
      p2 = p2->cdr;
    }
    cnt--;
  }
}

void mr2(CELL *p, int tesu, int beta)
{
  mkchild1(p);

  for (CELL *son = p->child; son; son = son->cdr) {
    put1(son->t + ban);
    son->h = eval2();
    tup1();
  }

  sort(p);

  p->t = p->child->t;
  
  for (CELL *son = p->child; son; son = son->cdr) {
    son->h = beta;
  }
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
