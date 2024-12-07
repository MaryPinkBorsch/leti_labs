

// закомментированный код работает - пищит раз в 3 секунды (обработчик таймера подменен)
// // Аппаратное прерывание от таймера, генерация звука от 700 до 1200 Гц каждые 3 секунды
// #include <dos.h>
// #include <stdio.h>
// #include <conio.h>
// #include <bios.h>

// unsigned long far *count =  (unsigned long far*) MK_FP(0x0040, 0x006c); // тут храниться время

// void interrupt mybeep(...) // моё прерывание
// {
// 	int i;
// 	if (*count % 54L == 0) // берем каждую 3 секунду 18*3
// 	{
// 		for(i = 0; i <= 1200; i++)
// 			sound(700 + i/30);
// 		nosound();
// 	}
// }

// int main(void)
//  {
// 	clrscr();
// 	void interrupt (*oldvect)(...);
// 	oldvect = getvect(0x1c); // получаем значение вектора прерываний
// 	setvect(0x1c, mybeep); 	// создаем свое прерывание
// 	printf("Для выхода из тишины, нажмите любую клавишу...\n");
// 	getch();
// 	setvect(0x1c, oldvect);  // восстанавливаем значения
// 	return 0;
// }

// Аппаратное прерывание от таймера, генерация звука от 700 до 1200 Гц каждые 3 секунды
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <bios.h>

unsigned long far *myflag = (unsigned long far *)MK_FP(0x0040, 0x006c); // тут хранится сменился ли шрифт
void interrupt (*oldvect)(...);

void interrupt mykbhandler(...) // моё прерывание - выставляет myflag в 1
{
    *myflag = 1;
    // возможно здесь надо вызвать сохраненный обработчик прерывания от клавиатуры
}

int main(void)
{
    clrscr();
    // print symbol
    oldvect = getvect(0x9);    // получаем значение вектора прерываний
    setvect(0x9, mykbhandler); // создаем свое прерывание

    // sleep (10);
    // if (myflag == 1)
    //{
    // change font
    // print symbol
    // myflag = 0
    //}

    setvect(0x9, oldvect); // восстанавливаем значения
    return 0;
}