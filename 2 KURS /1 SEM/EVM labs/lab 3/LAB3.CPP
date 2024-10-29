// #include <graphics.h>
// #include <conio.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <dos.h>
// const int SCALE = 20; //масштаб по оси Х
// const double PI = 3.14159;
// double func(double x)
// {
//     return pow(sin(x), 3) - pow(cos(x), 2);
// }
// void drawLine(int moveToX, int moveToY, int drawX, int drawY, int color)
// {
//     moveto(moveToX, moveToY);
//     setcolor(color);
//     lineto(drawX, drawY);
// }
// double scaleX(double x, int x_center) //масштаб координаты относительно центра
// {
//     return x_center + int(x * SCALE);
// }
// double scaleY(double y, int y_center)
// {
//     return y_center + int(y * SCALE);
// }
// int main(void)
// {
//     // запрос на граф параметры по умолчанию
//     int gdriver = DETECT, gmode, errorcode;

//     // иннициализация графической сис-мы
//     initgraph(&gdriver, &gmode, "c:/turboc3/bgi");
//     // считывание результата инициализации
//     errorcode = graphresult();
//     if (errorcode != grOk) //если случилась ошибка
//     {
//         printf("Graphics error: %s\n", grapherrormsg(errorcode));
//         printf("Press any key to halt:");
//         getch();
//         exit(1); 
//     }
//     int WIDTH = getmaxx(), HEIGHT = getmaxy();
//     setbkcolor(15);
//     cleardevice();
//     // Set center of axis
//     int center_x = 40, center_y = HEIGHT / 2;
//     // отрисовка осей
//     drawLine(center_x - 20, center_y, WIDTH - 20, center_y, 12); // x
//     drawLine(center_x, 20, center_x, HEIGHT - 20, 12);           // y
//     // Draw markup for x
//     float n = 0.5;
//     char temp_ar1[6];
//     for (int x_d = SCALE * (PI / 2) + center_x; x_d < WIDTH - 20; x_d += SCALE * (PI / 2)) 
//     // рисуем маленькие полоски на оси Х + координаты
//     {
//         drawLine(x_d, center_y + 5, x_d, center_y - 5, 12);
//         sprintf(temp_ar1, "%.1f", n);
//         outtextxy(x_d - 15, center_y + 7, temp_ar1);
//         sprintf(temp_ar1, "Pi");
//         outtextxy(x_d - 13, center_y + 16, temp_ar1);
//         n += 0.5;
//     }

//     // Draw markup for y
//     int v = 1, y_d;
//     char temp_ar2[2];
//     // Draw zero
//     int digit_height = textheight("0");
//     outtextxy(center_x - 13, center_y + (digit_height)-1, "0");
//     // From center to top
//     for (y_d = center_y - SCALE; y_d >= 20; y_d -= 1 * SCALE) // рисуем маленькие полоски на оси ОУ + координаты
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
//     } // Drawing a function (рисует график)
//     double max_y = -1000.0;
//     for (double x = PI / 2; x <= PI * 8; x += 0.01)
//     {
//         double y = func(x);
//         if (max_y < y)
//             max_y = y;
//         putpixel(scaleX(x, center_x), scaleY(-y, center_y), 1);
//         delay(1);
//     }
//     // Printing a max value
//     setcolor(12);
//     setlinestyle(0, 1, 3);
//     rectangle(WIDTH - 300, HEIGHT - 100, WIDTH - (40), HEIGHT - 50); // прямоугольник
//     settextstyle(0, HORIZ_DIR, 2);

//     char temp_ar3[20];
//     sprintf(temp_ar3, "f(x) max = %.1f", max_y);
//     outtextxy(WIDTH - 280, HEIGHT - 80, temp_ar3);
//     // Очистка
//     getch();
//     closegraph();
//     return 0;
// }