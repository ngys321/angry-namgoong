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
int hit(int X, int Y, int i, int j);//과녁 맞혔는지 여부 판정 함수
void explosion(int X, int Y);
void draw_win(int x, int y);
void draw_lose(int x, int y);
void draw_gaugebar(int position_x, int position_y, int* x, int* y, int* x0, int* y0, char* ch);//게이지바 출력함수
void draw_cloud(int x1, int y1, int x2, int y2); //구름 출력함수
int getkey(void);//실시간 키보드 입력값을 리턴하는 함수

int main(void)
{

	int ch = 0;
	int x0 = 20, y0 = 9; ////////////////////////////////////////고무줄 시작점의 위치 좌표값. 초기위치(x0==10,y0==9)
	int x = 20, y = 9;///////////////////////////////////////////고무줄 끝점의 위치를 나타내는 좌표값. 고무줄을 당겨서 이 좌표값을 변경 시킨다.
	int *xptr = &x, *yptr = &y, *x0ptr = &x0, *y0ptr = &y0;
	///////////////////////////////////////////////////////////////// **주의사항!**여기서 x0,y0,x,y좌표값은 실제윈도우상의 좌표값이 아니라 화면의 맨밑면이 y축이 되도록 잡은 좌표평면의 좌표값임
		//////////////////////////////////////////////////////////////////////////////mvwprintw(my_win, y값, x값, "문자"); 함수 사용할떄만 30-y -> y 변환해줘!!

	int i, j;//////////////////////////////////////////////////////루프 돌릴떄 쓸 x,y값//////루프 돌리기 직전에 초기화 필요
	int h; ////////////////////////////////////////////////////////수직 발사시 수직발사루프 돌리때 사용하는 높이 변수
	int score = 0;/////////////////////////////////////////////////점수변수
	int remaining_bullet = 20;/////////////////////////////////////남은 탄알수
	double sec = clock() / 1000.0;/////////////////////////////////경과시간(초)
	double theta = (M_PI / 6);
	double v0 = 30.0;
	double vy = 0.0;///////////////////////////////////////////////수직발사시 사용할 수직초기속력
	int target1 = 0, target2 = 0, target3 = 0, target4 = 0;////////과녁적중여부 체크변수

	occur occur_explosion;//int occur1 = 0, occur2 = 0, occur3 = 0, occur4 = 0;////////////이미 맞혔던 과녁인지아닌지 체크하는 변수를 멤버로 담고있는 구조체
	occur* occur_explosion_ptr = &occur_explosion;

	int dot_x, dot_y;///////////도트 출력용 변수


	enum win_lose{lose = 0, win = 1, none = 2}win_lose_game = none;///////////////////////////////////////////열거형 승패 변수 win_lose_game

	//////////////////////////////////////////////////////////////변수선언

	occur_explosion_ptr->occur1 = 0;
	occur_explosion_ptr->occur2 = 0;
	occur_explosion_ptr->occur3 = 0;
	occur_explosion_ptr->occur4 = 0;

	initscr();

	my_win = newwin(30, 120, 0, 0);
	box(my_win, 0, 0);
	wrefresh(my_win);
	//////////////////////////////////////////////////////////////윈도우 생성, 윈도우사이즈 세로30 가로120/////////////////////////////////////////////////////////////

	start_color();

	/*
	화면 전체의 색깔 입히는 방법
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_MAGENTA);
	wbkgd(my_win, COLOR_PAIR(1));

	원하는 부분만 색깔 입히는 방법
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

	//////////////////////////////////////////////////////////////게임 시작화면 제작

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
	}///////////////////////////////땅 바닥 출력

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

	//////////////////////////////구름출력

	erase_rubber(14, 5, 28, 5);
	erase_rubber(14, 4, 28, 4);
	erase_rubber(14, 3, 28, 3);
	erase_rubber(14, 2, 28, 2);
	mvwprintw(my_win, 27, 15, "Pulling Power");
	mvwprintw(my_win, 25, 23, "ANGLE");
	mvwprintw(my_win, 26, 23, "%.2f", theta*(180 / M_PI));
	draw_gaugebar(27, 2, xptr, yptr, x0ptr, y0ptr, "P");

	////////////////////게이지바 출력(초기)

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
	}///////////////발사대 출력

	if (target1 == 0)//1번과녁(초기)
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
	if (target2 == 0)//2번과녁(초기)
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
	if (target3 == 0)//3번과녁(초기)
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
	if (target4 == 0)//4번과녁(초기)
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

	////////////////////////////////<<<<<<<<<<<<여기뒤에다 점수판 출력삽입<<<<<<<<<<<<<<<<<<<<<<고무줄 조절 위치에 넣은 점수판코드 나중에 여기에다 복붙!! ( )

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


	/////////////////////////////////////////////////////////////초기 화면 구성(초기디자인_시안1)//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////초기상태  함수 데스트 위치//////////////////////////////////////////////////////////////////////////

	keypad(my_win, TRUE);
	while (win_lose_game == none)
	{

		while (1)
		{

			///맵 허공 리셋팅/////////만든이유 : 발사체 발사 이후에 맵에 탄알이 남아있는걸 없애주기 위함

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
			}//발사대 주변 리셋팅

			//맵 허공 리셋팅

			//맵 초기화 위치////////만든이유 : 고무줄 당길떄 맵이 지워지는 문제를 없애기 위함
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
			////////////////////////////땅 바닥 출력

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

			///////////////////////////구름 출력
			erase_rubber(14, 5, 28, 5);
			erase_rubber(14, 4, 28, 4);
			erase_rubber(14, 3, 28, 3);
			erase_rubber(14, 2, 28, 2);
			mvwprintw(my_win, 27, 15, "Pulling Power");
			mvwprintw(my_win, 25, 23, "ANGLE");
			mvwprintw(my_win, 26, 23, "%.2f", theta*(180 / M_PI));
			draw_gaugebar(27, 2, xptr, yptr, x0ptr, y0ptr, "P");//게이지바


			////////////////////게이지바 출력
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

			}///////////////발사대 출력

			if (target1 == 0)//1번과녁
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
			if (target2 == 0)//2번과녁
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
			if (target3 == 0)//3번과녁
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
			if (target4 == 0)//4번과녁
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

			////////////////////////////////<<<<<<<<<<<<여기뒤에다 점수판 출력삽입

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
			//////////////////////////////////////////////////////////////남은탄알 점수 시간 위치1

			for (i = 20; i <= 30; i++)
			{
				theta = atan(((double)y0 - (double)y) / ((double)x0 - (double)x));
				v0 = (2.33) * sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
				//j = 21 - tan(theta)*(i - 20);//직선 조준선
				j = 21 - (tan(theta))*(i - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*(i - 20)*(i - 20);//포물선 궤적 조준선
				mvwprintw(my_win, j, i, "+");
				wrefresh(my_win);
			}
			wrefresh(my_win);////////////////////////////////////////////조준선 출력

		///맵 초기화 위치


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

		/////////////////////////////////////////////////고무줄 작동!!
	// shoot:
		wrefresh(my_win);
		if (x == x0)/////발사각이 지면과 90도인 경우 작동 안하도록
		{
		}
		else//////발사각이 수직이 아닌경우(정상발사)
		{

			for (i = 20; j < 30; i++)
			{
				theta = atan(((double)y0 - (double)y) / ((double)x0 - (double)x));
				v0 = (2.33) * sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));////초기속도 계산
				j = 21 - (tan(theta))*(i - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*(i - 20)*(i - 20);//중력가속도는 10으로 잡음
				Sleep((210) / ((2.33)*sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0))*cos(theta)));////////////////////////////////////////////////초기 속력에 따라 시간지연
				init_pair(5, COLOR_WHITE, COLOR_BLUE);
				wattron(my_win, COLOR_PAIR(5));
				mvwprintw(my_win, j, i, "@");
				wattroff(my_win, COLOR_PAIR(5));
				if ((i - 5) >= 20)
				{
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////추가..발사체 뒤 꼬리 삭제
				}
				wrefresh(my_win);////////////////////탄알 날아가는 애니메이션

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
				}/////////////////////////////////////과녁 적중 판정 체크

				if (((i >= 0) && (i <= 30) && (j >= 24)) || ((i >= 30) && (i <= 60) && (j >= 27)) || ((i == 60) && (j >= 24) && (j <= 26)) || ((i >= 60) && (i <= 90) && (j >= 24)) || ((i >= 90) && (i <= 120) && (j >= 27)))/////바닥경계선 닿은 경우 탄알이 멈추게 만듬
				{
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////발사체 뒤 꼬리 마저 삭제
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////발사체 뒤 꼬리 마저 삭제
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////발사체 뒤 꼬리 마저 삭제
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////발사체 뒤 꼬리 마저 삭제
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////발사체 뒤 꼬리 마저 삭제
					wrefresh(my_win);
					i++;
					mvwprintw(my_win, 21 - (tan(theta))*((i - 5) - 20) + (10 / (2 * (v0)*(v0)*(cos(theta))*(cos(theta))))*((i - 5) - 20)*((i - 5) - 20), i - 5, " ");////발사체 뒤 꼬리 마저 삭제
					wrefresh(my_win);
					i++;
					break;
				}
			}


			remaining_bullet--;
			x = x0;
			y = y0;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////발사직후 맵 리셋팅 시작

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
			}//발사대 주변 리셋팅

			//맵 허공 리셋팅

			//맵 초기화 위치////////만든이유 : 고무줄 당길때 맵이 지워지는 문제를 없애기 위함
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
			}/////////////////땅 바닥 출력

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

			////////////////////////////구름 출력

			erase_rubber(14, 5, 28, 5);
			erase_rubber(14, 4, 28, 4);
			erase_rubber(14, 3, 28, 3);
			erase_rubber(14, 2, 28, 2);
			mvwprintw(my_win, 27, 15, "Pulling Power");
			mvwprintw(my_win, 25, 23, "ANGLE");
			mvwprintw(my_win, 26, 23, "%.2f", theta*(180 / M_PI));
			draw_gaugebar(27, 2, xptr, yptr, x0ptr, y0ptr, "P");

			////////////////////게이지바 출력
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

			}///////////////발사대 출력

			if (target1 == 0)//1번과녁
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
			if (target2 == 0)//2번과녁
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
			if (target3 == 0)//3번과녁
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
			if (target4 == 0)//4번과녁
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
			////////////////////////////////<<<<<<<<<<<<여기뒤에다 점수판 출력삽입

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
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////발사직후 맵 리셋팅 끝

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
			//////////////////////승리 패배 조건 위치
		}

		wrefresh(my_win);
		//x = 10, y = 9;
		i = 20, j = 21;/////////////////////////////////////////////////////////////포물체 다시 던지기 위해 i,j값 초기값으로 되돌림

		/////////////////////////////////////////////////탄알 발사!!!!
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
		}/////////////////////////게임 오버
	}



	system("pause");
	return 0;
}



void draw_rubber(int input_x0, int input_y0, int input_x, int input_y, char* string)
{
	int i, j;/////////////////////////////////////////////////////반복문 동작용 변수
	start_color();
	init_pair(3, COLOR_BROWN, COLOR_BROWN);


	if (input_x > input_x0)
	{
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////기울기가 1보다 클때, 픽셀 깨지는 문제 해결 이걸로!
		{
			for (j = 30 - input_y; j <= 30 - input_y0; j++)//for문안에 있는 j값범위는 그냥 그떄그떄 상황에맞게 작은값부터 큰값까지로 잡고j++해주고
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				//자세히보면, j값만 mvwprintw함수에 넣기전에 j대신에 30-j로 바꿔주면 돼.
				wattron(my_win, COLOR_PAIR(3));
				mvwprintw(my_win, j, i, string);
				wattroff(my_win, COLOR_PAIR(3));
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////기울기가 -1보다 작을때, 픽셀 깨지는 문제 해결 이걸로!
		{
			for (j = 30 - input_y0; j <= 30 - input_y; j++)//for문안에 있는 j값범위는 그냥 그떄그떄 상황에맞게 작은값부터 큰값까지로 잡고j++해주고
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				//자세히보면, j값만 mvwprintw함수에 넣기전에 j대신에 30-j로 바꿔주면 돼.
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
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////기울기가 1보다 클때, 픽셀 깨지는 문제 해결 이걸로!
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
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////기울기가 -1보다 작을때, 픽셀 깨지는 문제 해결 이걸로!
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
	int i, j;/////////////////////////////////////////////////////반복문 동작용 변수
	if (input_x > input_x0)
	{
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////기울기가 1보다 클때, 픽셀 깨지는 문제 해결 이걸로!
		{
			for (j = 30 - input_y; j <= 30 - input_y0; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////기울기가 -1보다 작을때, 픽셀 깨지는 문제 해결 이걸로!
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
		if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) > 1)//////////////기울기가 1보다 클때, 픽셀 깨지는 문제 해결 이걸로!
		{
			for (j = 30 - input_y0; j <= 30 - input_y; j++)
			{
				i = ((((double)input_x - (double)input_x0) / ((double)input_y - (double)input_y0))*((30 - j) - (double)input_y0) + ((double)input_x0));
				mvwprintw(my_win, j, i, " ");
				wrefresh(my_win);
			}
			return;
		}
		else if ((((double)input_y - (double)input_y0) / ((double)input_x - (double)input_x0)) < -1)//////////////기울기가 -1보다 작을때, 픽셀 깨지는 문제 해결 이걸로!
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
		printf("\a");//////////과녁 명중시 윈도우 경고음
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
	if (kbhit())// kbhit()를 이용해 입력값이 있는지 확인
	{
		return wgetch(my_win);//입력값이 wgetch()로 char를 리턴해줌
	}
	return '\0';//입력값이 없으면 널문자 리턴
}
