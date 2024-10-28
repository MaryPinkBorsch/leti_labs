// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <dos.h>
// const int SCALE = 30;
// const double PI = 3.14159;
// double func(double x)
// {
//     return pow(sin(x), 3) - pow(cos(x), 3);
// }
// void drawLine(int moveToX, int moveToY, int drawX, int drawY, int color)
// {
//     moveto(moveToX, moveToY);
//     setcolor(color);
//     lineto(drawX, drawY);
// }
// double scaleX(double x, int x_center)
// {
//     return x_center + int(x * SCALE);
// }
// double scaleY(double y, int y_center)
// {
//     return y_center + int(y * SCALE);
// }
// int main(void)
// {
//     // request auto detection
//     int gdriver = DETECT, gmode, errorcode;

//     // initialize graphics and local variables
//     initgraph(&gdriver, &gmode, "c:/turboc3/bgi");
//     // read result of initialization
//     errorcode = graphresult();
//     if (errorcode != grOk) /* an error occurred */
//     {
//         printf("Graphics error: %s\n", grapherrormsg(errorcode));
//         printf("Press any key to halt:");
//         getch();
//         exit(1); /* terminate with an error code */
//     }
//     int WIDTH = getmaxx(), HEIGHT = getmaxy();
//     setbkcolor(15);
//     cleardevice();
//     // Set center of axis
//     int center_x = 40, center_y = HEIGHT / 2;
//     // Set Axis
//     drawLine(center_x - 20, center_y, WIDTH - 20, center_y, 12); // x
//     drawLine(center_x, 20, center_x, HEIGHT - 20, 12);           // y
//     // Draw markup for x
//     float n = 0.5;
//     char temp_ar1[6];
//     for (int x_d = SCALE * (PI / 2) + center_x; x_d < WIDTH - 20; x_d += SCALE * (PI / 2))
//     {
//         drawLine(x_d, center_y + 5, x_d, center_y - 5, 12);
//         sprintf(temp_ar1, "%.1fpi", n);
//         outtextxy(x_d - 15, center_y + 7, temp_ar1);
//         n += 0.5;
//     }

//     // Draw markup for y
//     int v = 1, y_d;
//     char temp_ar2[2];
//     // Draw zero
//     int digit_height = textheight("0");
//     outtextxy(center_x - 13, center_y + (digit_height)-1, "0");
//     // From center to top
//     for (y_d = center_y - SCALE; y_d >= 20; y_d -= 1 * SCALE)
//     {
//         drawLine(center_x - 5, y_d, center_x + 5, y_d, 12);
//         sprintf(temp_ar2, "%i", v++);
//         outtextxy(center_x - 13, y_d - 4, temp_ar2);
//     }
//     // From center to bottom
//     int miunus_width = textwidth("-");
//     int z = 1;
//     for (y_d = center_y + SCALE; y_d <= HEIGHT - 20; y_d += 1 * SCALE)
//     {
//         drawLine(center_x - 5, y_d, center_x + 5, y_d, 12);
//         sprintf(temp_ar2, "-%i", z++);
//         outtextxy(center_x - (13 + miunus_width), y_d - 4, temp_ar2);
//     } // Drawing a function
//     for (double x = PI / 2; x <= PI * 8; x += 0.01)
//     {
//         double y = func(x);
//         putpixel(scaleX(x, center_x), scaleY(-y, center_y), 1);
//         delay(1);
//     }
//     // Printing a max value
//     setcolor(12);
//     setlinestyle(0, 1, 3);
//     rectangle(WIDTH - 300, HEIGHT - 100, WIDTH - (50), HEIGHT - 50);
//     settextstyle(0, HORIZ_DIR, 2);
//     outtextxy(WIDTH - 270, HEIGHT - 80, "f(x) max = 1");
//     // Очистка
//     getch();
//     closegraph();
//     return 0;
// }