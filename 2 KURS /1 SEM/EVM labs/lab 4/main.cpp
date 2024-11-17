// #include <stdio.h>
// #include <conio.h>
// #include <bios.h>
// #include <dos.h>

// // референс по дос расширениям
// // http://www.techhelpmanual.com/27-dos__bios___extensions_service_index.html

// // использование int 21h для блокирующего чтения с консоли без вывода на экран
// // http://www.techhelpmanual.com/380-dos_fn_07h__no_echo_unfiltered_console_input.html


// #define ESC 283

// #define F7 16640
// #define F8 16896

// int main()
// {
//     int exit = 0;
//     int ch,
//         x = 8,
//         y = 5;
//     const int x1 = 25,
//               y1 = 10,
//               x2 = 55,
//               y2 = 20;
//     textbackground(0);
//     textcolor(0);
//     clrscr();
//     window(x1,y1,x2,y2);
//     textbackground(5);
//     clrscr();
//     textcolor(0);

//     do{
//         clrscr();
//         gotoxy(x,y); //Переводим курсор в положение x,y
//         putch('*');
//         while (bioskey(1) == 0);  //Использование прерывания INT 16h
//         ch = bioskey(0);
//         switch (ch)
//     	{
//             case F8: //Вправо - F8
//                 if (x < x2-x1+1)
//                     x++;
//                 break;
//             case F7: //Влево - F7
//                 if (x > 1)
//                     x--;
//                 break;
//             case ESC:  //Выход - ESC
//                 exit = 1;
//                 break;
//             default:
//             break;
//     	}
//     }
//     while(exit!=1);
//     return 0;
// }