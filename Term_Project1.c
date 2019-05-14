#include <stdio.h>
#include "curses.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#define COLOR_ORANGE (COLOR_RED | COLOR_YELLOW)
#define COLOR_BROWN (COLOR_ORANGE | COLOR_BLACK)


WINDOW *my_win;
typedef struct occur{
	int occur1;
	int occur2;
	int occur3;
	int occur4;
} occur;
void draw_rubber(int input_x0, int input_y0, int input_x, int input_y, char* string);
void erase_rubber(int input_x0, int input_y0, int input_x, int input_y);
void draw_Y(int dot_x, int dot_y);
void draw_O(int dot_x, int dot_y);
void draw_U(int dot_x, int dot_y);
void draw_L(int dot_x, int dot_y);
void draw_S(int dot_x, int dot_y);
void draw_E(int dot_x, int dot_y);
void draw_W(int dot_x, int dot_y);
void draw_I(int dot_x, int dot_y);
void draw_N(int dot_x, int dot_y);
int hit(int X, int Y, int i, int j);//���� �������� ���� ���� �Լ�
void explosion(int X, int Y);
void draw_win(int x, int y);
void draw_lose(int x, int y);
void draw_gaugebar(int position_x, int position_y, int* x, int* y, int* x0, int* y0, char* ch);//�������� ����Լ�
void draw_cloud(int x1, int y1, int x2, int y2); //���� ����Լ�
int getkey(void);//�ǽð� Ű���� �Է°��� �����ϴ� �Լ�

int main(void)
{

	int ch = 0;
	int x0 = 20, y0 = 9; ////////////////////////////////////////���� �������� ��ġ ��ǥ��. �ʱ���ġ(x0==10,y0==9)
	int x = 20, y = 9;///////////////////////////////////////////���� ������ ��ġ�� ��Ÿ���� ��ǥ��. ������ ��ܼ� �� ��ǥ���� ���� ��Ų��.
	int *xptr = &x, *yptr = &y, *x0ptr = &x0, *y0ptr = &y0;
	///////////////////////////////////////////////////////////////// **���ǻ���!**���⼭ x0,y0,x,y��ǥ���� ������������� ��ǥ���� �ƴ϶� ȭ���� �ǹظ��� y���� �ǵ��� ���� ��ǥ����� ��ǥ����
		//////////////////////////////////////////////////////////////////////////////mvwprintw(my_win, y��, x��, "����"); �Լ� ����ҋ��� 30-y -> y ��ȯ����!!

	int i, j;//////////////////////////////////////////////////////���� ������ �� x,y��//////���� ������ ������ �ʱ�ȭ �ʿ�
	int h; ////////////////////////////////////////////////////////���� �߻�� �����߻���� ������ ����ϴ� ���� ����
	int score = 0;/////////////////////////////////////////////////��������
	int remaining_bullet = 20;/////////////////////////////////////���� ź�˼�
	double sec = clock() / 1000.0;/////////////////////////////////����ð�(��)
	double theta = (M_PI / 6);
	double v0 = 30.0;
	double vy = 0.0;///////////////////////////////////////////////�����߻�� ����� �����ʱ�ӷ�
	int target1 = 0, target2 = 0, target3 = 0, target4 = 0;////////�������߿��� üũ����

	occur occur_explosion;//int occur1 = 0, occur2 = 0, occur3 = 0, occur4 = 0;////////////�̹� ������ ���������ƴ��� üũ�ϴ� ������ ����� ����ִ� ����ü
	occur* occur_explosion_ptr = &occur_explosion;

	int dot_x, dot_y;///////////��Ʈ ��¿� ����


	enum win_lose{lose = 0, win = 1, none = 2}win_lose_game = none;///////////////////////////////////////////������ ���� ���� win_lose_game

	//////////////////////////////////////////////////////////////��������

	occur_explosion_ptr->occur1 = 0;
	occur_explosion_ptr->occur2 = 0;
	occur_explosion_ptr->occur3 = 0;
	occur_explosion_ptr->occur4 = 0;

	initscr();

	my_win = newwin(30, 120, 0, 0);
	box(my_win, 0, 0);
	wrefresh(my_win);
	//////////////////////////////////////////////////////////////������ ����, ����������� ����30 ����120/////////////////////////////////////////////////////////////

	start_color();

	/*
	ȭ�� ��ü�� ���� ������ ���
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_MAGENTA);
	wbkgd(my_win, COLOR_PAIR(1));

	���ϴ� �κи� ���� ������ ���
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_MAGENTA);
	wattron(my_win, COLOR_PAIR(1));
	wattroff(my_win, COLOR_PAIR(1));
	*/

	draw_rubber(49, 23, 80, 23, "O");
	mvwprintw(my_win, 30 - 21, 52, "Angry Namgoong ver 1.0");
	mvwprintw(my_win, 30 - 19, 52, "Made by Young-soo Namgoong");
	draw_rubber(49,17,80,17,"O");

	mvwprintw(my_win, 30 - 15 ,49 , "LEFT_KEY : Pull rubber to left" );
	mvwprintw(my_win, 30 - 13, 49, "RIGHT_KEY : Pull rubber to right");
	mvwprintw(my_win, 30 -11, 49, "UP_KEY : Pull rubber to up");
	mvwprintw(my_win, 30 - 9, 49, "DOWN_KEY : Pull rubber to down");
	mvwprintw(my_win, 30 - 7, 49, " \'Q\' : FIre bullet");

	mvwprintw(my_win, 30-3, 50, "Press any key to Start game");
	wrefresh(my_win);


	while (ch = wgetch(my_win))
	{
		if (ch)
			break;
	}
	for (i = 1; i < 119; i++)
	{
		for (j = 1; j < 29; j++ )
		{
			mvwprintw(my_win, 30-j, i , " ");
		}
	}

	//////////////////////////////////////////////////////////////���� ����ȭ�� ����

	for (j = 24; j <= 30; j++)
	{
		for (i = 0; i <= 120; i++)
		{
			switch ((j >= 24) && (j <= 26) && (i >= 1) && (i <= 30) || (j >= 24) && (j <= 26) && (i >= 60) && (i <= 90))
			{
			case 1:
				mvwprintw(my_win, j, i, "#");
				break;
			default:
				break;
			}
			switch ((j >= 27) && (j <= 30) && (i >= 1) && (i <= 119))
			{
			case 1:
				mvwprintw(my_win, j, i, "#");
				break;
			default:
				break;
			}
		}
	}///////////////////////////////�� �ٴ� ���

	draw_cloud(10, 25, 15, 26);
	draw_cloud(15, 26, 18, 27);
	draw_cloud(8, 26, 10, 27);
	draw_cloud(7, 26, 7, 26);

	draw_cloud(87, 24, 91, 25);
	draw_cloud(84, 23, 87, 24);
	draw_cloud(88, 24, 89, 25);

	draw_cloud(97, 24, 101, 25);
	draw_cloud(102, 27, 104, 27);
	draw_cloud(108, 26, 110, 27);

	draw_cloud(68, 28, 72, 29);
	draw_cloud(65, 26, 69, 27);
	draw_cloud(58, 27, 62, 28);

	//////////////////////////////�������

	erase_rubber(14, 5, 28, 5);
	erase_rubber(14, 4, 28, 4);
	erase_rubber(14, 3, 28, 3);
	erase_rubber(14, 2, 28, 2);
	mvwprintw(my_win, 27, 15, "Pulling Power");
	mvwprintw(my_win, 25, 23, "ANGLE");
	mvwprintw(my_win, 26, 23, "%.2f", theta*(180 / M_PI));
	draw_gaugebar(27, 2, xptr, yptr, x0ptr, y0ptr, "P");

	////////////////////�������� ���(�ʱ�)

	for (j = 21; j <= 22; j++)
	{
		i = 18;
		mvwprintw(my_win, j, i, "#");
	}
	for (j = 21; j <= 22; j++)
	{
		i = 22;
		mvwprintw(my_win, j, i, "#");
	}
	for (i = 18; i <= 22; i++)
	{
		j = 22;
		mvwprintw(my_win, j, i, "#");
	}
	for (j = 22; j <= 23; j++)
	{
		i = 20;
		mvwprintw(my_win, j, i, "#");
	}///////////////�߻�� ���

	if (target1 == 0)//1������(�ʱ�)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 28) && (i <= 29) && (j >= 22) && (j <= 23))
				{
				case(1):
					init_pair(1, COLOR_BLACK, COLOR_GREEN);
					wattron(my_win, COLOR_PAIR(1));
					mvwprintw(my_win, j, i, "B");
					wattroff(my_win, COLOR_PAIR(1));
					break;
				default:
					break;
				}
			}
		}
	}
	if (target1 == 1)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 28) && (i <= 29) && (j >= 22) && (j <= 23))
				{
				case(1):
					wbkgd(my_win, COLOR_PAIR(6));
					mvwprintw(my_win, j, i, " ");
					break;
				default:
					break;
				}
			}
		}
	}
	if (target2 == 0)//2������(�ʱ�)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 57) && (i <= 58) && (j >= 25) && (j <= 26))
				{
				case(1):
					init_pair(1, COLOR_BLACK, COLOR_GREEN);
					wattron(my_win, COLOR_PAIR(1));
					mvwprintw(my_win, j, i, "B");
					wattroff(my_win, COLOR_PAIR(1));
					break;
				default:
					break;
				}
			}
		}
	}
	if (target2 == 1)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 57) && (i <= 58) && (j >= 25) && (j <= 26))
				{
				case(1):
					mvwprintw(my_win, j, i, " ");
					break;
				default:
					break;
				}
			}
		}
	}
	if (target3 == 0)//3������(�ʱ�)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 75) && (i <= 76) && (j >= 22) && (j <= 23))
				{
				case(1):
					init_pair(1, COLOR_BLACK, COLOR_GREEN);
					wattron(my_win, COLOR_PAIR(1));
					mvwprintw(my_win, j, i, "B");
					wattroff(my_win, COLOR_PAIR(1));
					break;
				default:
					break;
				}
			}
		}
	}
	if (target3 == 1)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 75) && (i <= 76) && (j >= 22) && (j <= 23))
				{
				case(1):
					mvwprintw(my_win, j, i, " ");
					break;
				default:
					break;
				}
			}
		}
	}
	if (target4 == 0)//4������(�ʱ�)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 93) && (i <= 94) && (j >= 25) && (j <= 26))
				{
				case(1):
					init_pair(1, COLOR_BLACK, COLOR_GREEN);
					wattron(my_win, COLOR_PAIR(1));
					mvwprintw(my_win, j, i, "B");
					wattroff(my_win, COLOR_PAIR(1));
					break;
				default:
					break;
				}
			}
		}
	}
	if (target4 == 1)
	{
		for (i = 0; i <= 120; i++)
		{
			for (j = 0; j <= 30; j++)
			{
				switch ((i >= 93) && (i <= 94) && (j >= 25) && (j <= 26))
				{
				case(1):
					mvwprintw(my_win, j, i, " ");
					break;
				default:
					break;
				}
			}
		}
	}

	////////////////////////////////<<<<<<<<<<<<����ڿ��� ������ ��»���<<<<<<<<<<<<<<<<<<<<<<���� ���� ��ġ�� ���� �������ڵ� ���߿� ���⿡�� ����!! ( )

	for (i = 62; i <= 88; i++)
	{
		for (j = 0; j <= 5; j++)
		{
			mvwprintw(my_win, 30 - j, i, " ");
		}
	}
	mvwprintw(my_win, 26, 63, "  Remaining Bullet : %d", remaining_bullet);
	mvwprintw(my_win, 27, 63, "  Score : %d", score);
	sec = clock() / 1000.0;
	mvwprintw(my_win, 28, 63, "  Time : %d : %d", (int)sec / 60, (int)sec - 60 * ((int)sec / 60));

	wrefresh(my_win);


	/////////////////////////////////////////////////////////////�ʱ� ȭ�� ����(�ʱ������_�þ�1)//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////�ʱ����  �Լ� ����Ʈ ��ġ//////////////////////////////////////////////////////////////////////////

	keypad(my_win, TRUE);
	while (win_lose_game == none)
	{

		while (1)
		{

			///�� ��� ������/////////�������� : �߻�ü �߻� ���Ŀ� �ʿ� ź���� �����ִ°� �����ֱ� ����

			for (i = 0; i <= 119; i++)
			{
				for (j = 0; j <= 20; j++)
				{
					mvwprintw(my_win, j, i, " ");
				}
			}
			for (i = 0; i <= 30; i++)
			{
				for (j = 0; j <= 26; j++)
				{
					mvwprintw(my_win, j, i, " ");
				}
			}//�߻�� �ֺ� ������

			//�� ��� ������

			//�� �ʱ�ȭ ��ġ////////�������� : ���� ��拚 ���� �������� ������ ���ֱ� ����
			for (j = 24; j <= 30; j++)
			{
				for (i = 0; i <= 120; i++)
				{
					switch ((j >= 24) && (j <= 26) && (i >= 1) && (i <= 30) || (j >= 24) && (j <= 26) && (i >= 60) && (i <= 90))
					{
					case 1:
						mvwprintw(my_win, j, i, "#");
						break;
					default:
						break;
					}
					switch ((j >= 27) && (j <= 30) && (i >= 1) && (i <= 119))
					{
					case 1:
						mvwprintw(my_win, j, i, "#");
						break;
					default:
						break;
					}
				}
			}
			////////////////////////////�� �ٴ� ���

			draw_cloud(10, 25, 15, 26);
			draw_cloud(15, 26, 18, 27);
			draw_cloud(8, 26, 10, 27);
			draw_cloud(7, 26, 7, 26);

			draw_cloud(87, 24, 91, 25);
			draw_cloud(84, 23, 87, 24);
			draw_cloud(88, 24, 89, 25);

			draw_cloud(97, 24, 101, 25);
			draw_cloud(102, 27, 104, 27);
			draw_cloud(108, 26, 110, 27);

			draw_cloud(68, 28, 72, 29);
			draw_cloud(65, 26, 69, 27);
			draw_cloud(58, 27, 62, 28);

			///////////////////////////���� ���
			erase_rubber(14, 5, 28, 5);
			erase_rubber(14, 4, 28, 4);
			erase_rubber(14, 3, 28, 3);
			erase_rubber(14, 2, 28, 2);
			mvwprintw(my_win, 27, 15, "Pulling Power");
			mvwprintw(my_win, 25, 23, "ANGLE");
			mvwprintw(my_win, 26, 23, "%.2f", theta*(180 / M_PI));
			draw_gaugebar(27, 2, xptr, yptr, x0ptr, y0ptr, "P");//��������


			////////////////////�������� ���
			for (j = 21; j <= 22; j++)
			{
				i = 18;
				mvwprintw(my_win, j, i, "#");
			}
			for (j = 21; j <= 22; j++)
			{
				i = 22;
				mvwprintw(my_win, j, i, "#");
			}
			for (i = 18; i <= 22; i++)
			{
				j = 22;
				mvwprintw(my_win, j, i, "#");
			}
			for (j = 22; j <= 23; j++)
			{
				i = 20;
				mvwprintw(my_win, j, i, "#");

			}///////////////�߻�� ���

			if (target1 == 0)//1������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 28) && (i <= 29) && (j >= 22) && (j <= 23))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target1 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 28) && (i <= 29) && (j >= 22) && (j <= 23))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			if (target2 == 0)//2������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 57) && (i <= 58) && (j >= 25) && (j <= 26))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target2 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 57) && (i <= 58) && (j >= 25) && (j <= 26))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			if (target3 == 0)//3������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 75) && (i <= 76) && (j >= 22) && (j <= 23))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target3 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 75) && (i <= 76) && (j >= 22) && (j <= 23))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			if (target4 == 0)//4������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 93) && (i <= 94) && (j >= 25) && (j <= 26))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target4 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 93) && (i <= 94) && (j >= 25) && (j <= 26))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}

			////////////////////////////////<<<<<<<<<<<<����ڿ��� ������ ��»���

			for (i = 62; i <= 88; i++)
			{
				for (j = 0; j <= 5; j++)
				{
					mvwprintw(my_win, 30 - j, i, " ");
				}
			}
			
			mvwprintw(my_win, 26, 63, "  Remaining Bullet : %d", remaining_bullet);
			score = 10 * (target1 + target2 + target3 + target4);
			mvwprintw(my_win, 27, 63, "  Score : %d", score);
			sec = clock() / 1000.0;
			mvwprintw(my_win, 28, 63, "  Time : %d : %d", (int)sec / 60, (int)sec - 60 * ((int)sec / 60));
			wrefresh(my_win);
			//////////////////////////////////////////////////////////////����ź�� ���� �ð� ��ġ1

			for (i = 20; i <= 30; i++)
			{
				theta = atan(((double)y0 - (double)y) / ((double)x0 - (double)x));
				v0 = (2.33) * sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
				//j = 21 - tan(theta)*(i - 20);//���� ���ؼ�
				j = 21 - (tan(theta))*(i - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*(i - 20)*(i - 20);//������ ���� ���ؼ�
				mvwprintw(my_win, j, i, "+");
				wrefresh(my_win);
			}
			wrefresh(my_win);////////////////////////////////////////////���ؼ� ���

		///�� �ʱ�ȭ ��ġ


			switch (ch)
			{

			case KEY_UP:
				y++;
				erase_rubber(x0, y0, x, y - 1);
				draw_rubber(x0, y0, x, y, "o");
				wrefresh(my_win);
				break;

			case KEY_DOWN:
				y--;
				erase_rubber(x0, y0, x, y + 1);
				draw_rubber(x0, y0, x, y, "o");
				wrefresh(my_win);
				break;

			case KEY_LEFT:
				x--;
				erase_rubber(x0, y0, x + 1, y);
				draw_rubber(x0, y0, x, y, "o");
				wrefresh(my_win);
				break;
			case KEY_RIGHT:
				if (x >= 20)
				{
					break;
				}
				x++;
				erase_rubber(x0, y0, x - 1, y);
				draw_rubber(x0, y0, x, y, "o");
				wrefresh(my_win);
				break;
			}
			wrefresh(my_win);

			if ((ch == 'q'))
			{
				ch = 0;
				break;
				//goto shoot;
			}
			while (1)
			{
				sec = clock() / 1000.0;
				mvwprintw(my_win, 28, 63, "  Time : %d : %d", (int)sec / 60, (int)sec - 60 * ((int)sec / 60));
				wrefresh(my_win);

				ch = getkey();

				if (ch != '\0')
				{
					break;
				}

				Sleep(50);
			}
		}

		/////////////////////////////////////////////////���� �۵�!!
	// shoot:
		wrefresh(my_win);
		if (x == x0)/////�߻簢�� ����� 90���� ��� �۵� ���ϵ���
		{
		}
		else//////�߻簢�� ������ �ƴѰ��(����߻�)
		{

			for (i = 20; j < 30; i++)
			{
				theta = atan(((double)y0 - (double)y) / ((double)x0 - (double)x));
				v0 = (2.33) * sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));////�ʱ�ӵ� ���
				j = 21 - (tan(theta))*(i - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*(i - 20)*(i - 20);//�߷°��ӵ��� 10���� ����
				Sleep((210) / ((2.33)*sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0))*cos(theta)));////////////////////////////////////////////////�ʱ� �ӷ¿� ���� �ð�����
				init_pair(5, COLOR_WHITE, COLOR_BLUE);
				wattron(my_win, COLOR_PAIR(5));
				mvwprintw(my_win, j, i, "@");
				wattroff(my_win, COLOR_PAIR(5));
				if ((i - 5) >= 20)
				{
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߰�..�߻�ü �� ���� ����
				}
				wrefresh(my_win);////////////////////ź�� ���ư��� �ִϸ��̼�

				if (hit(28, 23, i, j) == 1)
				{
					target1 = 1;
					if ((target1 == 1) && (occur_explosion_ptr->occur1 == 0))
					{
						explosion(i, j);
						occur_explosion_ptr->occur1 = 1;
					}
				}
				if (hit(57, 26, i, j) == 1)
				{
					target2 = 1;
					if ((target2 == 1) && (occur_explosion_ptr->occur2 == 0))
					{
						explosion(i, j);
						occur_explosion_ptr->occur2 = 1;
					}
				}
				if (hit(75, 23, i, j) == 1)
				{
					target3 = 1;
					if ((target3 == 1) && (occur_explosion_ptr->occur3 == 0))
					{
						explosion(i, j);
						occur_explosion_ptr->occur3 = 1;
					}
				}
				if (hit(93, 26, i, j) == 1)
				{
					target4 = 1;
					if ((target4 == 1) && (occur_explosion_ptr->occur4 == 0))
					{
						explosion(i, j);
						occur_explosion_ptr->occur4 = 1;
					}
				}/////////////////////////////////////���� ���� ���� üũ

				if (((i >= 0) && (i <= 30) && (j >= 24)) || ((i >= 30) && (i <= 60) && (j >= 27)) || ((i == 60) && (j >= 24) && (j <= 26)) || ((i >= 60) && (i <= 90) && (j >= 24)) || ((i >= 90) && (i <= 120) && (j >= 27)))/////�ٴڰ�輱 ���� ��� ź���� ���߰� ����
				{
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߻�ü �� ���� ���� ����
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߻�ü �� ���� ���� ����
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߻�ü �� ���� ���� ����
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߻�ü �� ���� ���� ����
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߻�ü �� ���� ���� ����
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////�߻�ü �� ���� ���� ����
					wrefresh(my_win);
					i++;
					break;
				}
			}


			remaining_bullet--;
			x = x0;
			y = y0;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////�߻����� �� ������ ����

			for (i = 0; i <= 119; i++)
			{
				for (j = 0; j <= 20; j++)
				{
					mvwprintw(my_win, j, i, " ");
				}
			}
			for (i = 0; i <= 30; i++)
			{
				for (j = 0; j <= 26; j++)
				{
					mvwprintw(my_win, j, i, " ");
				}
			}//�߻�� �ֺ� ������

			//�� ��� ������

			//�� �ʱ�ȭ ��ġ////////�������� : ���� ��涧 ���� �������� ������ ���ֱ� ����
			for (j = 24; j <= 30; j++)
			{
				for (i = 0; i <= 120; i++)
				{
					switch ((j >= 24) && (j <= 26) && (i >= 1) && (i <= 30) || (j >= 24) && (j <= 26) && (i >= 60) && (i <= 90))
					{
					case 1:
						mvwprintw(my_win, j, i, "#");
						break;
					default:
						break;
					}
					switch ((j >= 27) && (j <= 30) && (i >= 1) && (i <= 119))
					{
					case 1:
						mvwprintw(my_win, j, i, "#");
						break;
					default:
						break;
					}
				}
			}/////////////////�� �ٴ� ���

			draw_cloud(10, 25, 15, 26);
			draw_cloud(15, 26, 18, 27);
			draw_cloud(8, 26, 10, 27);
			draw_cloud(7, 26, 7, 26);

			draw_cloud(87, 24, 91, 25);
			draw_cloud(84, 23, 87, 24);
			draw_cloud(88, 24, 89, 25);

			draw_cloud(97, 24, 101, 25);
			draw_cloud(102, 27, 104, 27);
			draw_cloud(108, 26, 110, 27);

			draw_cloud(68, 28, 72, 29);
			draw_cloud(65, 26, 69, 27);
			draw_cloud(58, 27, 62, 28);

			////////////////////////////���� ���

			erase_rubber(14, 5, 28, 5);
			erase_rubber(14, 4, 28, 4);
			erase_rubber(14, 3, 28, 3);
			erase_rubber(14, 2, 28, 2);
			mvwprintw(my_win, 27, 15, "Pulling Power");
			mvwprintw(my_win, 25, 23, "ANGLE");
			mvwprintw(my_win, 26, 23, "%.2f", theta*(180 / M_PI));
			draw_gaugebar(27, 2, xptr, yptr, x0ptr, y0ptr, "P");

			////////////////////�������� ���
			for (j = 21; j <= 22; j++)
			{
				i = 18;
				mvwprintw(my_win, j, i, "#");
			}
			for (j = 21; j <= 22; j++)
			{
				i = 22;
				mvwprintw(my_win, j, i, "#");
			}
			for (i = 18; i <= 22; i++)
			{
				j = 22;
				mvwprintw(my_win, j, i, "#");
			}
			for (j = 22; j <= 23; j++)
			{
				i = 20;
				mvwprintw(my_win, j, i, "#");

			}///////////////�߻�� ���

			if (target1 == 0)//1������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 28) && (i <= 29) && (j >= 22) && (j <= 23))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target1 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 28) && (i <= 29) && (j >= 22) && (j <= 23))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			if (target2 == 0)//2������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 57) && (i <= 58) && (j >= 25) && (j <= 26))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target2 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 57) && (i <= 58) && (j >= 25) && (j <= 26))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			if (target3 == 0)//3������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 75) && (i <= 76) && (j >= 22) && (j <= 23))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target3 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 75) && (i <= 76) && (j >= 22) && (j <= 23))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			if (target4 == 0)//4������
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 93) && (i <= 94) && (j >= 25) && (j <= 26))
						{
						case(1):
							init_pair(1, COLOR_BLACK, COLOR_GREEN);
							wattron(my_win, COLOR_PAIR(1));
							mvwprintw(my_win, j, i, "B");
							wattroff(my_win, COLOR_PAIR(1));
							break;
						default:
							break;
						}
					}
				}
			}
			if (target4 == 1)
			{
				for (i = 0; i <= 120; i++)
				{
					for (j = 0; j <= 30; j++)
					{
						switch ((i >= 93) && (i <= 94) && (j >= 25) && (j <= 26))
						{
						case(1):
							mvwprintw(my_win, j, i, " ");
							break;
						default:
							break;
						}
					}
				}
			}
			////////////////////////////////<<<<<<<<<<<<����ڿ��� ������ ��»���

			for (i = 62; i <= 88; i++)
			{
				for (j = 0; j <= 5; j++)
				{
					mvwprintw(my_win, 30 - j, i, " ");
				}
			}
			
			mvwprintw(my_win, 26, 63, "  Remaining Bullet : %d", remaining_bullet);
			score = 10 * (target1 + target2 + target3 + target4);
			mvwprintw(my_win, 27, 63, "  Score : %d", score);
			sec = clock() / 1000.0;
			mvwprintw(my_win, 28, 63, "  Time : %d : %d", (int)sec / 60, (int)sec - 60 * ((int)sec / 60));
			wrefresh(my_win);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////�߻����� �� ������ ��

			wrefresh(my_win);

			score = 10 * (target1 + target2 + target3 + target4);
			if ((sec <= 600) && (score >= 40))
			{
				mvwprintw(my_win, 26, 63, "  Remaining Bullet : %d", remaining_bullet);
				score = 10 * (target1 + target2 + target3 + target4);
				mvwprintw(my_win, 27, 63, "  Score : %d", score);
				win_lose_game = win;
			}
			else if ((sec > 600) || (remaining_bullet <= 0))
			{
				mvwprintw(my_win, 26, 63, "  Remaining Bullet : %d", remaining_bullet);
				score = 10 * (target1 + target2 + target3 + target4);
				mvwprintw(my_win, 27, 63, "  Score : %d", score);
				win_lose_game = lose;
			}
			//////////////////////�¸� �й� ���� ��ġ
		}

		wrefresh(my_win);
		//x = 10, y = 9;
		i = 20, j = 21;/////////////////////////////////////////////////////////////����ü �ٽ� ������ ���� i,j�� �ʱⰪ���� �ǵ���

		/////////////////////////////////////////////////ź�� �߻�!!!!
		switch (win_lose_game)
		{
		case win:
			while (1)
			{
				start_color();
				init_pair(1, COLOR_YELLOW, COLOR_BLACK);
				wattron(my_win, COLOR_PAIR(1));
				draw_win(31, 10);
				wattroff(my_win, COLOR_PAIR(1));

				Sleep(400);
				for (i = 31; i <= 92; i++)
				{
					for (j = 10; j <= 20; j++)
					{
						mvwprintw(my_win, j, i, " ");
					}
				}

				wrefresh(my_win);
				Sleep(400);

			}
			break;
		case lose:
			while (1)
			{
				start_color();
				init_pair(1, COLOR_BLUE, COLOR_BLACK);
				wattron(my_win, COLOR_PAIR(1));
				draw_lose(28, 10);
				wattroff(my_win, COLOR_PAIR(1));

				Sleep(400);
				for (i = 27; i <= 97; i++)
				{
					for (j = 10; j <= 20; j++)
					{
						mvwprintw(my_win, j, i, " ");
					}
				}

				wrefresh(my_win);
				Sleep(400);

			}
			break;
		}/////////////////////////���� ����
	}



	system("pause");
	return 0;
}



void draw_rubber(int input_x0, int input_y0, int input_x, int input_y, char* string)
{
	int i, j;/////////////////////////////////////////////////////�ݺ��� ���ۿ� ����
	start_color();
	init_pair(3, COLOR_BROWN, COLOR_BROWN);


	if (input_x > input_x0)
	{
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////���Ⱑ 1���� Ŭ��, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y; j <= 30 - input_y0; j++)//for���ȿ� �ִ� j�������� �׳� �׋��׋� ��Ȳ���°� ���������� ū�������� ���j++���ְ�
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				//�ڼ�������, j���� mvwprintw�Լ��� �ֱ����� j��ſ� 30-j�� �ٲ��ָ� ��.
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////���Ⱑ -1���� ������, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y0; j <= 30 - input_y; j++)//for���ȿ� �ִ� j�������� �׳� �׋��׋� ��Ȳ���°� ���������� ū�������� ���j++���ְ�
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				//�ڼ�������, j���� mvwprintw�Լ��� �ֱ����� j��ſ� 30-j�� �ٲ��ָ� ��.
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}
		else
		{
			for (i = input_x0; i <= input_x; i++)
			{
				j = 30 - ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0))*(i - (double)input_x0) + ((double)input_y0));
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}

	}
	else if (input_x < input_x0)
	{
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////���Ⱑ 1���� Ŭ��, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y0; j <= 30 - input_y; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////���Ⱑ -1���� ������, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y; j <= 30 - input_y0; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}
		else
		{
			for (i = input_x; i <= input_x0; i++)
			{
				j = 30 - ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0))*(i - (double)input_x0) + ((double)input_y0));
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}
	}
	else if (input_y > input_y0)
	{
		for (j = 30 - input_y; j <= 30 - input_y0; j++)
		{
			i = input_x0;
			wattron(my_win, COLOR_PAIR(3));
			mvwprintw(my_win, j, i, string);
			wattroff(my_win, COLOR_PAIR(3));
			wrefresh(my_win);
		}
		return;
	}
	else if (input_y < input_y0)
	{
		for (j = 30 - input_y0; j <= 30 - input_y; j++)
		{
			i = input_x0;
			wattron(my_win, COLOR_PAIR(3));
			mvwprintw(my_win, j, i, string);
			wattroff(my_win, COLOR_PAIR(3));
			wrefresh(my_win);
		}
		return;
	}

	return;
}

void erase_rubber(int input_x0, int input_y0, int input_x, int input_y)
{
	int i, j;/////////////////////////////////////////////////////�ݺ��� ���ۿ� ����
	if (input_x > input_x0)
	{
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////���Ⱑ 1���� Ŭ��, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y; j <= 30 - input_y0; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////���Ⱑ -1���� ������, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y0; j <= 30 - input_y; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
		else
		{
			for (i = input_x0; i <= input_x; i++)
			{
				j = 30 - ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0))*(i - (double)input_x0) + ((double)input_y0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}

	}
	else if (input_x < input_x0)
	{
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////���Ⱑ 1���� Ŭ��, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y0; j <= 30 - input_y; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////���Ⱑ -1���� ������, �ȼ� ������ ���� �ذ� �̰ɷ�!
		{
			for (j = 30 - input_y; j <= 30 - input_y0; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
		else
		{
			for (i = input_x; i <= input_x0; i++)
			{
				j = 30 - ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0))*(i - (double)input_x0) + ((double)input_y0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
	}
	else if (input_y > input_y0)
	{
		for (j = 30 - input_y; j <= 30 - input_y0; j++)
		{
			i = input_x0;
			mvwprintw(my_win, j, i, " ");
			wrefresh(my_win);
		}
		return;
	}
	else if (input_y < input_y0)
	{
		for (j = 30 - input_y0; j <= 30 - input_y; j++)
		{
			i = input_x0;
			mvwprintw(my_win, j, i, " ");
			wrefresh(my_win);
		}
		return;
	}
	return;
}
int hit(int X, int Y, int i, int j)
{
	if (((X - 1) <= i) && ((X + 2) >= i) && ((Y - 2) <= j) && ((Y + 1) >= j))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void explosion(int X, int Y)
{
	int d;
	int i, j;

	start_color();
	init_pair(2, COLOR_YELLOW, COLOR_RED);
	wattron(my_win, COLOR_PAIR(2));
	wattroff(my_win, COLOR_PAIR(2));

	for (d = 0; d < 4; d++)
	{
		for (j = Y - d; j <= Y + d; j++)
		{
			for (i = X - d; i <= X + d; i++)
			{
				if ((abs(X - i) == d) || (abs(Y - j) == d))
				{
					wattron(my_win, COLOR_PAIR(2));
					mvwprintw(my_win, j, i, "&");
					wattroff(my_win, COLOR_PAIR(2));

				}

			}
		}
		for (j = Y - (d - 1); j <= Y + (d - 1); j++)
		{
			for (i = X - (d - 1); i <= X + (d - 1); i++)
			{
				if ((abs(X - i) == (d - 1)) || (abs(Y - j) == (d - 1)))
				{
					mvwprintw(my_win, j, i, " ");


				}
			}
		}
		wrefresh(my_win);
		printf("\a");//////////���� ���߽� ������ �����
		Sleep(200);

		if (d == 3)
		{
			for (j = Y - d; j <= Y + d; j++)
			{
				for (i = X - d; i <= X + d; i++)
				{
					if ((abs(X - i) == d) || (abs(Y - j) == d))
					{
						mvwprintw(my_win, j, i, " ");


					}

				}
			}
			wrefresh(my_win);
			Sleep(200);
		}

	}
	return;
}

void draw_Y(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x, "D");
	mvwprintw(my_win, dot_y, dot_x + 1, "D");
	mvwprintw(my_win, dot_y, dot_x + 5, "D");
	mvwprintw(my_win, dot_y, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");

	return;
}
void draw_O(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x + 2, "D");
	mvwprintw(my_win, dot_y, dot_x + 3, "D");
	mvwprintw(my_win, dot_y, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 2, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 3, dot_x, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 4, dot_x, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 5, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 4, "D");


	return;
}

void draw_U(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x, "D");
	mvwprintw(my_win, dot_y, dot_x + 1, "D");
	mvwprintw(my_win, dot_y, dot_x + 5, "D");
	mvwprintw(my_win, dot_y, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 2, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 3, dot_x, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 4, dot_x, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 5, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 6, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 5, "D");


	return;
}
void draw_L(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x, "D");
	mvwprintw(my_win, dot_y, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 2, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 3, dot_x, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 4, dot_x, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 5, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 6, "D");


	return;
}
void draw_S(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x + 1, "D");
	mvwprintw(my_win, dot_y, dot_x + 2, "D");
	mvwprintw(my_win, dot_y, dot_x + 3, "D");
	mvwprintw(my_win, dot_y, dot_x + 4, "D");
	mvwprintw(my_win, dot_y, dot_x + 5, "D");

	mvwprintw(my_win, dot_y + 1, dot_x + 0, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 2, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 3, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 4, "D");

	mvwprintw(my_win, dot_y + 4, dot_x + 5, "D");

	mvwprintw(my_win, dot_y + 5, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 6, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 5, "D");


	return;
}
void draw_E(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x, "D");
	mvwprintw(my_win, dot_y, dot_x + 1, "D");
	mvwprintw(my_win, dot_y, dot_x + 2, "D");
	mvwprintw(my_win, dot_y, dot_x + 3, "D");
	mvwprintw(my_win, dot_y, dot_x + 4, "D");
	mvwprintw(my_win, dot_y, dot_x + 5, "D");
	mvwprintw(my_win, dot_y, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x, "D");

	mvwprintw(my_win, dot_y + 3, dot_x, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 4, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x, "D");

	mvwprintw(my_win, dot_y + 6, dot_x, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 6, "D");


	return;
}
void draw_W(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x, "D");
	mvwprintw(my_win, dot_y, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 2, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 3, dot_x, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 4, dot_x, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 5, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 6, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 5, "D");


	return;
}
void draw_I(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x + 1, "D");
	mvwprintw(my_win, dot_y, dot_x + 2, "D");
	mvwprintw(my_win, dot_y, dot_x + 3, "D");
	mvwprintw(my_win, dot_y, dot_x + 4, "D");
	mvwprintw(my_win, dot_y, dot_x + 5, "D");

	mvwprintw(my_win, dot_y + 1, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 3, "D");

	mvwprintw(my_win, dot_y + 6, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 5, "D");


	return;
}
void draw_N(int dot_x, int dot_y)
{

	mvwprintw(my_win, dot_y, dot_x, "D");
	mvwprintw(my_win, dot_y, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 1, dot_x, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 1, "D");
	mvwprintw(my_win, dot_y + 1, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 2, dot_x, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 2, "D");
	mvwprintw(my_win, dot_y + 2, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 3, dot_x, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 3, "D");
	mvwprintw(my_win, dot_y + 3, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 4, dot_x, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 4, "D");
	mvwprintw(my_win, dot_y + 4, dot_x + 6, "D");


	mvwprintw(my_win, dot_y + 5, dot_x, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 5, "D");
	mvwprintw(my_win, dot_y + 5, dot_x + 6, "D");

	mvwprintw(my_win, dot_y + 6, dot_x, "D");
	mvwprintw(my_win, dot_y + 6, dot_x + 6, "D");


	return;
}
void draw_win(int x, int y)
{

	draw_Y(x, y);
	draw_O(x + 9, y);
	draw_U(x + 18, y);
	draw_W(x + 36, y);
	draw_I(x + 45, y);
	draw_N(x + 54, y);
	mvwprintw(my_win, 0, 0, " ");
	wrefresh(my_win);

	return;
}
void draw_lose(int x, int y)
{
	draw_Y(x, y);
	draw_O(x + 9, y);
	draw_U(x + 18, y);
	draw_L(x + 36, y);
	draw_O(x + 45, y);
	draw_S(x + 54, y);
	draw_E(x + 63, y);
	mvwprintw(my_win, 0, 0, " ");
	wrefresh(my_win);

	return;
}
void draw_gaugebar(int position_x, int position_y, int* x, int* y, int* x0, int* y0, char* ch)
{
	draw_rubber(position_x, position_y, position_x - (1.0)*sqrt((*x - *x0)*(*x - *x0) + (*y - *y0)*(*y - *y0)), position_y, ch);
	return;
}

void draw_cloud(int x1, int y1, int x2, int y2)
{
	int i, j;

	start_color();
	init_pair(7, COLOR_WHITE, COLOR_CYAN);

	for (i = x1; i <= x2; i++)
	{
		for (j = y1; j <= y2; j++)
		{
			wattron(my_win, COLOR_PAIR(7));
			mvwprintw(my_win, 30 - j, i, "$");
			wattroff(my_win, COLOR_PAIR(7));
		}
	}

	return;
}
int getkey(void)
{
	if (kbhit())// kbhit()�� �̿��� �Է°��� �ִ��� Ȯ��
	{
		return wgetch(my_win);//�Է°��� wgetch()�� char�� ��������
	}
	return '\0';//�Է°��� ������ �ι��� ����
}
