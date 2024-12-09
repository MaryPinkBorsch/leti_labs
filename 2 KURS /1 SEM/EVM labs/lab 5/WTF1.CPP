// #include <dos.h>
// #include <conio.h>
// #include <iostream.h>
// #include <graphics.h>
// #include <stdio.h>

// int tmp = 0;
// int ch = 0;

// // Pointer to hold the address of the old ISR
// void interrupt (*oldISR)(...);

// // Custom ISR
// void interrupt newISR(...)
// {
//     // Call the original ISR to ensure default behavior
//     if (ch == 61)
//     {
//         tmp = (tmp + 1) % 10;
//         // textbackground(7);
//         clrscr();
//         outtextxy(20, 20, "M");
//         settextstyle(tmp, HORIZ_DIR, 4);
//     }
//     (*oldISR)();
// }

// int main()
// {
//     int gd = DETECT, gm;
//     initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
//     // Save the old ISR for INT 9h
//     oldISR = getvect(0x09);

//     // Set the new ISR for INT 9h
//     setvect(0x09, newISR);

//     // Keep the program running until ESC is pressed
//     while (1)
//     {
//         if (kbhit())
//         {
//             ch = getch();
//             if (ch == 27)
//                 break;
//         }
//     }

//     // Restore the old ISR before exiting
//     setvect(0x09, oldISR);
//     closegraph();
//     return 0;
// }