#include <iostream>
#include <iomanip>
//#include <cmath>
#include <fstream>
#include <math.h>
using namespace std;

const int N = 100;
const int m = 2;
const double EPS = 1e-5;
int a = 0;



void outf2(float P[m], ofstream& f3) {
	for (unsigned i = 0; i < 2; i++) f3 << left << setw(7) << P[i];
	f3 << "\n";
}

void outf(float P[N][m], unsigned a, ofstream& f3) {
	f3 << "\n\nГотовый набор точек : \n";
	for (unsigned i = 0; i < a; i++) outf2(P[i], f3);
}

void inpf(ifstream& f1, float P[N][m], ofstream& f3) {
	int c = 0;
	int koli = 0;
	float num;
	while (true)
	{
		if (f1.eof()) break;
		f1 >> num;

		if (c < N)
		{
			P[a][c] = num;
			koli++;
			f3 << P[a][c] << " ";
			c++;
		}

		if (f1.peek() == '\n' || f1.eof())
		{
			if (c > 1) {
				for (int k = 2; k < c; k++) {
					f3 << "Лишнее значение в строке: " << P[a][k] << ". ";
				}
			}
			if (c == 1) {
				f3 << "Строка содержит 1 координату";
				a--;

			}
			a++;

			if (f1.eof())
			{
				break;
			}
			c = 0;
			f3 << "\n";
		}
	}
}



float process(ofstream& f3, float P[N][m], int a, ofstream& f4, float Figuri[N][15], int& kolvo_figuri, float TOCHKI[N][2], int& popali_tochki, int& max_popali_tochki, int& max_index_figura)
{
	float FIGURI_MAX[N][15];// массив для окончательного вывода в файл результата
	Figuri[N][15];
	int const T = 12;
	float Ax, Ay, Bx, By, Cx, Cy, Dx, Dy, Ex, Ey, Fx, Fy; //точки фигуры 
	int i, j, k, p, q, w, e = 0; //переменные для перебора
	float AC = 0, CB, AB, CD, DE, EF = 0; //длины отрезков  
	float Kab, Kbc, Kcd, Kde, Kef = 0; //коэффициенты прямых
	int kolvo = 0;
	float Perimetrs[20] = { 0 };
	float Pr = 0;
	float Prmax = -1;
	for (i = 0; i < a; i++) {
		Ax = P[i][0];
		Ay = P[i][1];
		f3 << "\n\n   НАЧАЛО ПОСТРОЕНИЯ НОВОЙ ФИГУРЫ  \n ";
		f3 << "взяли точку A(" << Ax << ";" << Ay << ")\n";
		for (j = 0; j < a; j++)
		{
			if ((P[j][0] == Ax) && (P[j][1] == Ay))
			{
				f3 << "Ищем точку B\nТочка B (" << P[j][0] << ";" << P[j][1] << ") не подходит. Координаты совпали с точкой A\n";
				continue;
			}
			else
			{
				Bx = P[j][0];
				By = P[j][1];
				if (Ax == Bx || Ay == By)
				{
					f3 << "Ищем точку B\nТочка B (" << Bx << ";" << By << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
					continue;
				}
				else
				{
					f3 << "Ищем точку B\nТочка B (" << Bx << ";" << By << ") подходит. Найденные 2 точки A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")\n";
					for (k = 0; k < a; k++)
					{
						if (((P[k][0] == Ax) && (P[k][1] == Ay)) || (((P[k][0] == Bx) && (P[k][1] == By))))
						{
							f3 << "Ищем точку C\nТочка C (" << P[k][0] << ";" << P[k][1] << ") не подходит. Координаты совпали с координатами других точек\n";
							continue;
						}
						else
						{
							Cx = P[k][0];
							Cy = P[k][1];
							if ((((Cx - Ax) * (By - Ay)) - ((Cy - Ay) * (Bx - Ax))) < 0) {
								f3 << "Ищем точку C\nТочка C (" << Cx << ";" << Cy << ") не подходит. Она лежит не там, где нужно, фигуру построить нельзя\n";
								continue;
							}

							if (Cx == Bx || Cy == By)
							{
								f3 << "Ищем точку C\nТочка C (" << Cx << ";" << Cy << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
								continue;
							}
							Kab = (By - Ay) / (Bx - Ax);
							Kbc = (By - Cy) / (Bx - Cx);
							if (Ay == Cy || Ax == Cx) {
								if (Kab != -Kbc)
								{
									f3 << "Ищем точку C\nТочка C (" << Cx << ";" << Cy << ") не подходит. НЕ те коэффициенты, фигуру построить нельзя\n";
									continue;
								}
							}
							if (Ay != Cy && Ax != Cx) {
								if (Kab != (1 / Kbc)) {

									f3 << "Ищем точку C\nТочка C (" << Cx << ";" << Cy << ") не подходит. НЕ те коэффициенты, фигуру построить нельзя\n";
									continue;
								}
							}
							CB = (sqrt(((Cx - Bx) * (Cx - Bx)) + ((Cy - By) * (Cy - By))));
							AB = (sqrt(((Bx - Ax) * (Bx - Ax)) + ((By - Ay) * (By - Ay))));
							if (abs(AB - CB) > EPS)
							{
								f3 << "Ищем точку C\nТочка C (" << Cx << ";" << Cy << ") не подходит. НЕ одинаковая длина, фигуру построить нельзя\n";
								continue;
							}
							else
							{
								f3 << "Точка C(" << Cx << ";" << Cy << ") подходит. Найденные 3 точки A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << "\n";
								for (q = 0; q < a; q++)
								{
									if (((P[q][0] == Ax) && (P[q][1] == Ay)) || ((P[q][0] == Bx) && (P[q][1] == By)) || ((P[q][0] == Cx) && (P[q][1] == Cy)))
									{
										f3 << "Ищем точку D\nТочка D (" << P[q][0] << ";" << P[q][1] << ") не подходит. Координаты совпали с координатами других точек\n";
										continue;
									}
									else
									{
										Dx = P[q][0];
										Dy = P[q][1];
										if (Cx == Dx || Cy == Dy)
										{
											f3 << "Ищем точку D\nТочка D (" << Dx << ";" << Dy << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
											continue;
										}
										Kcd = ((Dy - Cy) / (Dx - Cx));
										if (fabs(Kcd - Kab) > EPS)
										{
											f3 << "Ищем точку D\nТочка D (" << Dx << ";" << Dy << ") не подходит. АБ не параллельно СД, фигуру построить нельзя\n";
											continue;
										}
										CD = (sqrt(((Cx - Dx) * (Cx - Dx)) + ((Cy - Dy) * (Cy - Dy))));
										float AD = (sqrt(((Ax - Dx) * (Ax - Dx)) + ((Ay - Dy) * (Ay - Dy))));
										if (fabs(CD - CB) > EPS || fabs(CD - AB) > EPS || fabs(CD - AD) > EPS || fabs(AD - CB) > EPS || fabs(AD - AB) > EPS)
										{
											f3 << "Ищем точку D\nТочка D (" << Dx << ";" << Dy << ") не подходит. Отрезки не равны, фигуру построить нельзя\n";
											continue;
										}
										float O1x = (Ax + Bx + Cx + Dx) / 4; //центр первого ромба коорд-ты по х и у
										float O1y = (Ay + By + Cy + Dy) / 4;
										float AO1 = (sqrt(((Ax - O1x) * (Ax - O1x)) + ((Ay - O1y) * (Ay - O1y))));
										float BO1 = (sqrt(((Bx - O1x) * (Bx - O1x)) + ((By - O1y) * (By - O1y))));
										float CO1 = (sqrt(((Cx - O1x) * (Cx - O1x)) + ((Cy - O1y) * (Cy - O1y))));
										float DO1 = (sqrt(((Dx - O1x) * (Dx - O1x)) + ((Dy - O1y) * (Dy - O1y))));
										if (fabs(AO1 - CO1) > EPS || fabs(BO1 - DO1) > EPS)
										{
											f3 << "Ищем точку D\nТочка D (" << Dx << ";" << Dy << ") не подходит. Отрезки меж центром не равны, фигуру построить нельзя\n";
											continue;
										}
										else
										{
											f3 << "Точка D(" << Dx << ";" << Dy << ") подходит. Найденные 4 точки A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << " D(" << Dx << ";" << Dy << ")" << "\n";
										}
										for (p = 0; p < a; p++)
										{
											if (((P[p][0] == Ax) && (P[p][1] == Ay)) || ((P[p][0] == Bx) && (P[p][1] == By)) || ((P[p][0] == Cx) && (P[p][1] == Cy)) || ((P[p][0] == Dx) && (P[p][1] == Dy)) || ((P[p][0] == Dx) && (P[p][1] == Dy))) {
												f3 << "Ищем точку E\nТочка E (" << P[p][0] << ";" << P[p][1] << ") не подходит. Координаты совпали с координатами других точек\n";
												continue;
											}
											else
											{
												Ex = P[p][0];
												Ey = P[p][1];
												if (Ey > Cy) {
													f3 << "Ищем точку E\nТочка E (" << Ex << ";" << Ey << ") не подходит. Она лежит сверху, а должна лежать снизу, фигуру построить нельзя\n";
													continue;
												}
												if (Ey == Cy) {
													if (Ex > Cx) {
														f3 << "Ищем точку E\nТочка E (" << Ex << ";" << Ey << ") не подходит. Она лежит не с той стороны, фигуру построить нельзя\n";
														continue;
													}
												}
												if (Ex == Dx || Ey == Dy)
												{
													f3 << "Ищем точку E\nТочка E (" << Ex << ";" << Ey << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
													continue;
												}
												Kde = ((Ey - Dy) / (Ex - Dx));
												if (fabs(Kde - Kbc) > EPS)
												{
													f3 << "Ищем точку E\nТочка E (" << Ex << ";" << Ey << ") не подходит. DE не параллельно СB, фигуру построить нельзя\n";
													continue;
												}
												DE = (sqrt(((Dx - Ex) * (Dx - Ex)) + ((Dy - Ey) * (Dy - Ey))));
												if (fabs(DE - AB) > EPS)
												{
													f3 << "Ищем точку E\nТочка E (" << Ex << ";" << Ey << ") не подходит. DE не равно остальным сторонам обломитесь, фигуру построить нельзя\n";
													continue;
												}
												else
												{
													f3 << "Точка E(" << Ex << ";" << Ey << ") подходит. Найденные 5 точек A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << " D(" << Dx << ";" << Dy << ")" << " E(" << Ex << ";" << Ey << ")" << "\n";
													for (w = 0; w < a; w++)
													{
														if (((P[w][0] == Ax) && (P[w][1] == Ay)) || ((P[w][0] == Bx) && (P[w][1] == By)) || ((P[w][0] == Cx) && (P[w][1] == Cy)) || ((P[w][0] == Dx) && (P[w][1] == Dy)))
														{
															f3 << "Ищем точку F\nТочка F (" << P[w][0] << ";" << P[w][1] << ") не подходит. Координаты совпали с координатами других точек\n";
															continue;
														}
														else
														{
															Fx = P[w][0];
															Fy = P[w][1];
															if (Fx == Ex || Fy == Ey)
															{
																f3 << "Ищем точку F\nТочка F (" << Fx << ";" << Fy << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
																continue;
															}
															float Kfe = (Ey - Fy) / (Ex - Fx);
															if (fabs(Kfe - Kab) > EPS)
															{
																f3 << "Ищем точку F\nТочка F (" << Fx << ";" << Fy << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
																continue;
															}
															float FE = (sqrt(((Fx - Ex) * (Fx - Ex)) + ((Fy - Ey) * (Fy - Ey))));
															if (fabs(FE - AB) > EPS)
															{
																f3 << "Ищем точку F\nТочка F (" << Fx << ";" << Fy << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
																continue;
															}
															else
															{
																f3 << "Точка F(" << Fx << ";" << Fy << ") подходит. Найденные 6 точек A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << " D(" << Dx << ";" << Dy << ")" << " E(" << Ex << ";" << Ey << ")" << " F(" << Fx << ";" << Fy << ")" << "\n";
																/// <summary>
																/// //
																/// </summary>

																for (int M = 0; M < a; M++)
																{
																	if (((P[M][0] == Ax) && (P[M][1] == Ay)) || ((P[M][0] == Bx) && (P[M][1] == By)) || ((P[M][0] == Cx) && (P[M][1] == Cy)) || ((P[M][0] == Dx) && (P[M][1] == Dy)) || ((P[M][0] == Ex) && (P[M][1] == Ey)))
																	{
																		f3 << "Ищем точку J\nТочка J (" << P[M][0] << ";" << P[M][1] << ") не подходит. Координаты совпали с координатами других точек\n";
																		continue;
																	}
																	else
																	{
																		float Jx = P[M][0];
																		float Jy = P[M][1];
																		if (Jx == Fx || Fy == Jy)
																		{
																			f3 << "Ищем точку J\nТочка J (" << Jx << ";" << Jy << ") не подходит. Отрезок параллеелн одной из осей, фигуру построить нельзя\n";
																			continue;
																		}
																		float Kfj = (Jy - Fy) / (Jx - Fx);
																		if (fabs(Kfj - Kbc) > EPS)
																		{
																			f3 << "Ищем точку J\nТочка J (" << Jx << ";" << Jy << ") не подходит. Не паралелльна я хочу спать, фигуру построить нельзя\n";
																			continue;
																		}
																		float FJ = (sqrt(((Fx - Jx) * (Fx - Jx)) + ((Fy - Jy) * (Fy - Jy))));
																		if (fabs(FJ - AB) > EPS)
																		{
																			f3 << "Ищем точку J\nТочка J (" << Jx << ";" << Jy << ") не подходит. Отрезок NE RAVEN ostalnim otreskam, фигуру построить нельзя\n";
																			continue;
																		}
																		float JD = (sqrt(((Dx - Jx) * (Dx - Jx)) + ((Dy - Jy) * (Dy - Jy))));
																		if (fabs(JD - AB) > EPS)
																		{
																			f3 << "Ищем точку J\nТочка J (" << Jx << ";" << Jy << ") не подходит. Отрезок NE RAVEN ostalnim otreskam, фигуру построить нельзя\n";
																			continue;
																		}
																		float O2x = (Ex + Fx + Jx + Dx) / 4; //центр VTOROGO ромба коорд-ты по х и у
																		float O2y = (Ey + Fy + Jy + Dy) / 4;
																		float EO2 = (sqrt(((Ex - O2x) * (Ex - O2x)) + ((Ey - O2y) * (Ey - O2y))));
																		float FO2 = (sqrt(((Fx - O2x) * (Fx - O2x)) + ((Fy - O2y) * (Fy - O2y))));
																		float JO2 = (sqrt(((Jx - O2x) * (Jx - O2x)) + ((Jy - O2y) * (Jy - O2y))));
																		float DO2 = (sqrt(((Dx - O2x) * (Dx - O2x)) + ((Dy - O2y) * (Dy - O2y))));
																		if (fabs(JO2 - EO2) > EPS || fabs(FO2 - DO2) > EPS)
																		{
																			f3 << "Ищем точку J\nТочка J (" << Jx << ";" << Jy << ") не подходит. Отрезки меж центром не равны, фигуру построить нельзя\n";
																			continue;
																		}
																		else
																		{
																			f3 << endl << "Точка J(" << Jx << ";" << Jy << ") подходит. Найденные 7 точек A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << " D(" << Dx << ";" << Dy << ")" << " E(" << Ex << ";" << Ey << ")" << " F(" << Fx << ";" << Fy << ")" << " J(" << Jx << ";" << Jy << ")""\n" << endl;
																			cout << endl << " ФИГУРА НАШЛАСЬ !!! Найденные 7 точек A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << " D(" << Dx << ";" << Dy << ")" << " E(" << Ex << ";" << Ey << ")" << " F(" << Fx << ";" << Fy << ")" << " J(" << Jx << ";" << Jy << ")""\n" << endl;
																			f4 << endl << " ФИГУРА НАШЛАСЬ !!! Найденные 7 точек A(" << Ax << ";" << Ay << ")" << " B(" << Bx << "; " << By << ")" << " C(" << Cx << ";" << Cy << ")" << " D(" << Dx << ";" << Dy << ")" << " E(" << Ex << ";" << Ey << ")" << " F(" << Fx << ";" << Fy << ")" << " J(" << Jx << ";" << Jy << ")""\n" << endl;
																			popali_tochki = 0;

																			Figuri[kolvo_figuri][0] = Ax;
																			Figuri[kolvo_figuri][1] = Ay;
																			Figuri[kolvo_figuri][2] = Bx;
																			Figuri[kolvo_figuri][3] = By;
																			Figuri[kolvo_figuri][4] = Cx;
																			Figuri[kolvo_figuri][5] = Cy;
																			Figuri[kolvo_figuri][6] = Dx;
																			Figuri[kolvo_figuri][7] = Dy;
																			Figuri[kolvo_figuri][8] = Ex;
																			Figuri[kolvo_figuri][9] = Ey;
																			Figuri[kolvo_figuri][10] = Fx;
																			Figuri[kolvo_figuri][11] = Fy;
																			Figuri[kolvo_figuri][12] = Jx;
																			Figuri[kolvo_figuri][13] = Jy;
																			//point_in_figure
																			TOCHKI[N][2];
																			for (int aa = 0; aa < a; aa++)
																			{
																				float testx = P[aa][0];
																				float testy = P[aa][1];
																				// abc
																				float d1, d2, d3;
																				bool neg, pos;
																				d1 = ((testx - Bx) * (Ay - By) - (Ax - Bx) * (testy - By));
																				d2 = ((testx - Cx) * (By - Cy) - (Bx - Cx) * (testy - Cy));
																				d3 = ((testx - Ax) * (Cy - Ay) - (Cx - Ax) * (testy - Ay));
																				neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
																				pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

																				if (!(neg && pos))
																				{
																					bool already_exists = false;
																					for (int i = 0; i < popali_tochki; ++i)
																					{
																						if (TOCHKI[i][0] == testx && TOCHKI[i][1] == testy)
																						{
																							already_exists = true;
																							break;
																						}
																					}
																					if (!already_exists)
																					{
																						TOCHKI[popali_tochki][0] = testx;
																						TOCHKI[popali_tochki][1] = testy;

																						f3 << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																						cout << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																						f4 << "точка попала" << " ее Х = " << testx << " ее Y = " << testy << endl;
																						popali_tochki++;
																					}
																				}

																				// adc

																				d1 = ((testx - Dx) * (Ay - Dy) - (Ax - Dx) * (testy - Dy));
																				d2 = ((testx - Cx) * (Dy - Cy) - (Dx - Cx) * (testy - Cy));
																				d3 = ((testx - Ax) * (Cy - Ay) - (Cx - Ax) * (testy - Ay));
																				neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
																				pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

																				if (!(neg && pos))
																				{
																					bool already_exists = false;
																					for (int i = 0; i < popali_tochki; ++i)
																					{
																						if (TOCHKI[i][0] == testx && TOCHKI[i][1] == testy)
																						{
																							already_exists = true;
																							break;
																						}
																					}
																					if (!(neg && pos))
																					{
																						bool already_exists = false;
																						for (int i = 0; i < popali_tochki; ++i)
																						{
																							if (TOCHKI[i][0] == testx && TOCHKI[i][1] == testy)
																							{
																								already_exists = true;
																								break;
																							}
																						}
																						if (!already_exists)
																						{
																							TOCHKI[popali_tochki][0] = testx;
																							TOCHKI[popali_tochki][1] = testy;

																							f3 << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																							cout << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																							f4 << "точка попала" << " ее Х = " << testx << " ее Y = " << testy << endl;
																							popali_tochki++;
																						}
																					}
																				}
																				// DJE

																				d1 = ((testx - Dx) * (Jy - Dy) - (Jx - Dx) * (testy - Dy));
																				d2 = ((testx - Ex) * (Dy - Ey) - (Dx - Ex) * (testy - Ey));
																				d3 = ((testx - Jx) * (Ey - Jy) - (Ex - Jx) * (testy - Jy));
																				neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
																				pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

																				if (!(neg && pos))
																				{
																					bool already_exists = false;
																					for (int i = 0; i < popali_tochki; ++i)
																					{
																						if (TOCHKI[i][0] == testx && TOCHKI[i][1] == testy)
																						{
																							already_exists = true;
																							break;
																						}
																					}
																					if (!already_exists)
																					{
																						TOCHKI[popali_tochki][0] = testx;
																						TOCHKI[popali_tochki][1] = testy;

																						f3 << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																						cout << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																						f4 << "точка попала" << " ее Х = " << testx << " ее Y = " << testy << endl;
																						popali_tochki++;
																					}
																				}
																				// FJE

																				d1 = ((testx - Fx) * (Jy - Fy) - (Jx - Fx) * (testy - Fy));
																				d2 = ((testx - Ex) * (Fy - Ey) - (Fx - Ex) * (testy - Ey));
																				d3 = ((testx - Jx) * (Ey - Jy) - (Ex - Jx) * (testy - Jy));
																				neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
																				pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

																				if (!(neg && pos))
																				{
																					bool already_exists = false;
																					for (int i = 0; i < popali_tochki; ++i)
																					{
																						if (TOCHKI[i][0] == testx && TOCHKI[i][1] == testy)
																						{
																							already_exists = true;
																							break;
																						}
																					}
																					if (!already_exists)
																					{
																						TOCHKI[popali_tochki][0] = testx;
																						TOCHKI[popali_tochki][1] = testy;

																						f3 << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																						cout << "ТОЧКА ПОПАЛА" << " ее Х: " << testx << " ее Y: " << testy << endl;
																						f4 << "точка попала" << " ее Х = " << testx << " ее Y = " << testy << endl;
																						popali_tochki++;
																					}
																				}
																			}

																			cout << popali_tochki;
																			Figuri[kolvo_figuri][14] = popali_tochki;


																			for (int i = 0; i < kolvo_figuri + 1; i++)
																			{
																				if (Figuri[i][14] > Figuri[i + 1][14])
																				{
																					max_index_figura = i;
																					max_popali_tochki = Figuri[i][14];
																				}
																			}

																			f4 << " индекс фигуры: " << max_index_figura << endl;
																			/*for (int i = 0; i < 14; i++)
																			{
																				f4 << Figuri[max_index_figura][i] << " ";
																			}
																			*/

																			f4 << " кол-во попавших точек: " << max_popali_tochki << endl;
																			// сюда вывод	
																			kolvo_figuri++;
																		}


																	}
																}
																// тут?


															}
														}
													}
												}
											}
										}
									}
								}
							}

						}
					}

				}
			}
		}
	}
	f4 << endl << endl << endl;
	bool sorted = false;
	while (sorted == false) //! sorted
	{
		sorted = true; // надеемся на то что оно уже отсортированно
		for (int j = 0; j < kolvo_figuri - 1; j++)
		{
			if (Figuri[j][14] < Figuri[j + 1][14])
			{
				float tmp = Figuri[j][14];
				Figuri[j][14] = Figuri[j + 1][14];
				Figuri[j + 1][14] = tmp;
				sorted = false;
			}
		}
	}
	int MAX_TOHCKI_POPALI = Figuri[0][14];
	for (int i = 0; i < kolvo_figuri; i++)
	{
		if (Figuri[i][14] == MAX_TOHCKI_POPALI)
		{
			f4 << "ФИГУРА C МАКС. КОЛ_ВОМ ТОЧЕК, ее индекс: " << i << endl;
			f4 << " max. КОЛ_ВО ПОПАВШИХ ТОЧЕК: " << MAX_TOHCKI_POPALI << endl;
			f3 << "ФИГУРА C МАКС. КОЛ_ВОМ ТОЧЕК, ее индекс: " << i << endl;
			f3 << " max. КОЛ_ВО ПОПАВШИХ ТОЧЕК: " << MAX_TOHCKI_POPALI << endl;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	ifstream f1("in.txt", ios::in);
	if (!f1.is_open())
	{
		cout << "Ошибка открытия исходного файла" << std::endl;
		return 0;
	}
	float P[N][m];
	float Figuri[N][15]; //14 na koordinati tochek sostaavlyauschich odnu figuru i odna 15-ya - na kol-vo popavchich tochek
	int kolvo_figuri = 0;
	float TOCHKI[N][2];
	int popali_tochki = 0;
	int max_popali_tochki = 0;
	int max_index_figura = 0;
	ofstream f3("protocol.txt", ios::out);
	if (!f3.is_open())
	{
		cout << "Ошибка открытия файла записи" << "\n";
		return 0;
	}
	ofstream f4("out.txt", ios::out);
	if (!f4.is_open())
	{
		cout << "Ошибка открытия файла записи" << "\n";
		return 0;
	}

	inpf(f1, P, f3);
	f1.close();
	outf(P, a, f3);
	process(f3, P, a, f4, Figuri, kolvo_figuri, TOCHKI, popali_tochki, max_popali_tochki, max_index_figura);
	f4 << endl << endl;


	/*
	for (int i = 0; i < kolvo_figuri-1; i++)
		if (Figuri[i][14] > Figuri[i + 1][14])
		{
			max_index_figura = i;
			max_popali_tochki = Figuri[i][14];
		}

	f4 << "ФИГУРА С МАКСИМАЛЬНЫМ КОЛИЧЕСТВОМ ТОЧЕК, ее индекс: " << max_index_figura << endl;
	for (int i = 0; i < 14; i++)
	{
		f4 << Figuri[max_index_figura][i] << " ";
	}
	f4 << " КОЛ_ВО ПОПАВШИХ ТОЧЕК: " << max_popali_tochki;
	*/



	f3.close();
	return 0;
}




