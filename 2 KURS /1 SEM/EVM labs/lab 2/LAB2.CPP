// #include <conio.h>
// #include <stdio.h>
// #include <dos.h>

// #define x1 25
// #define y1 10
// #define x2 55
// #define y2 20

// enum
// {
//   ENTIRE,
//   UP,
//   DOWN
// };

// void scroll(int direction, char l_row, char l_col, char r_row, char r_col, char attr, int lines)
// {
//   union REGS r;
//   if (direction == DOWN)
//     r.h.ah = 7;
//   else
//     r.h.ah = 6;
//   r.h.al = lines; // scroll lines
//   r.h.ch = l_row;
//   r.h.cl = l_col;
//   r.h.dh = r_row;
//   r.h.dl = r_col;
//   r.h.bh = attr;
//   int86(0x10, &r, &r);
// }
// void print(int i, int x, int y, int xx, int j)
// {
//   int k;
//   gotoxy(x, y);
//   printf("%d %d", i, j);
//   gotoxy(xx, y);
//   textbackground(0);
//   for (k = xx; k < xx + 26; k++)
//     putch(' ');
// }
// void clear(int y, int X1, int X2)
// {
//   gotoxy(X1, y);
//   clreol();
// }
// void main()
// {
//   int i = 0, j = 0;
//   window(x1, y2, x2, y1);
//   textcolor(i);
//   textbackground(j);
//   clrscr();
//   //int counter = 0;
//   for (j; j < 8; j++)
//   {
//     for (i = 0; i < 16; i++)
//     {
//       int linestoscroll = 1;
//       textcolor(i);
//       textbackground(j);
//       clear(y2, x1, x2);
//       print(i, x1, y2, x2, j);
//       if (i + 1 < 16)
//       {
//         ++linestoscroll;
//         textcolor(i + 1);
//         textbackground(j);
//         clear(y2 + 1, x1, x2);
//         print(i + 1, x1, y2 + 1, x2, j);
//         ++i;
//       }
//       if (i + 1 < 16)
//       {
//         ++linestoscroll;
//         textcolor(i + 1);
//         textbackground(j);
//         clear(y2 + 2, x1, x2);
//         print(i + 1, x1, y2 + 2, x2, j);
//         ++i;
//       }
//       //   if (++counter % 3 == 0)
//       delay(200);
//       scroll(UP, y1 - 1, x1 - 1, y2 + 3, x2, 1, linestoscroll);
//     }
//   }
//   getch();
// }