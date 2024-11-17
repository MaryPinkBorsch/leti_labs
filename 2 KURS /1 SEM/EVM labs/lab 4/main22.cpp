// #include <stdio.h>
// #include <conio.h>
// #include <bios.h>
// #include <dos.h>

// // референс по дос расширениям
// // http://www.techhelpmanual.com/27-dos__bios___extensions_service_index.html

// // использование int 21h для блокирующего чтения с консоли без вывода на экран
// // http://www.techhelpmanual.com/380-dos_fn_07h__no_echo_unfiltered_console_input.html

// #define ESC 27

// #define F7 65
// #define F8 66

// char read_char()
// {
//     union REGS in, out;
//     in.h.ah = 0x08;  // Функция считывания символа
//     int86(0x21, &in, &out);  // вызов прерывания с регистрами in & out
//     if (out.h.al == 0) // возвращенный ноль означает что есть расширенный код и надо повторить
//     {
//         int86(0x21, &in, &out); // вызов прерывания с регистрами in & out
//         return out.h.al + 256; // расширенный код
//     }
//     return out.h.al; // результат в AL
// }

// int main()
// {
//     int exit = 0; // переменная для выхода
//     int ch,
// 	x = 8,
// 	y = 5;
//     const int x1 = 25,
// 	      y1 = 10,
// 	      x2 = 55,
// 	      y2 = 20;
//     textbackground(0);
//     textcolor(0);
//     clrscr();
//     window(x1,y1,x2,y2);
//     textbackground(5);
//     clrscr();
//     textcolor(0);

//     do{
// 	clrscr();
// 	gotoxy(x,y); //Переводим курсор в положение x,y
// 	putch('*');
// 	ch = read_char();
// 	switch (ch)
// 	{
// 	    case F8: //Вправо - F8
// 		if (x < x2-x1+1)
// 		    x++;
// 		break;
// 	    case F7: //Влево - F7
// 		if (x > 1)
// 		    x--;
// 		break;
// 	    case ESC:  //Выход - ESC
// 		exit = 1;
// 		break;
// 	    default:
// 	    break;
// 	}
//     }
//     while(exit!=1);
//     return 0;
// }
