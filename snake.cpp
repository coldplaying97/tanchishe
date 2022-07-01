#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <string>


const int MAX_SIZE = 100; //蛇最长有多少节
const int WIDTH = 640; //屏幕宽度
const int HEIGHT = 480; //屏幕高度
const int LENGTH_UNIT = 10; //蛇每节的长度
const int SCORE_UNIT = 10; //每次吃到食物得到的分数

//坐标x, y
struct point
{
	int x, y;
};

//蛇
struct Snake {
	int size; //长度
	point xy[MAX_SIZE]; //蛇身的坐标
	char position; //当前移动方向
	int score; //得分
}snake;

//食物
struct Food {
	point xy; //食物坐标
	bool exist; //是否存在
}food;

//移动方向
enum movePosition {
	right = 77,
	left = 75,
	down = 80,
	up = 72
};

//窗口句柄
HWND hwnd = NULL;

void initSnake() {
	//蛇初始长度为3
	snake.size = 3;
	snake.xy[0].x = WIDTH / 2;
	snake.xy[0].y = HEIGHT / 2;
	snake.xy[1].x = snake.xy[0].x - LENGTH_UNIT;
	snake.xy[1].y = snake.xy[0].y;
	snake.xy[2].x = snake.xy[0].x - 2 * LENGTH_UNIT;
	snake.xy[2].y = snake.xy[0].y;
	snake.position = right;
	snake.score = 0;
}

void drawSnake() {
	setlinecolor(BLACK); //边框黑色
	setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255)); //填充绿色
	for (int i = 0; i < snake.size; i++) {
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}

void moveSnake() {
	//除了蛇首，每节移动到前一节的位置
	for (int i = snake.size - 1; i > 0; i--) {
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}

	if (snake.position == left) {
		snake.xy[0].x -= 10;
	}
	else if (snake.position == right) {
		snake.xy[0].x += 10;
	}
	else if (snake.position == up) {
		snake.xy[0].y -= 10;
	}
	else {
		snake.xy[0].y += 10;
	}
}

void keyDown() {
	char userKey = _getch();
	//当方向不是目前方向及反方向时,才需要改变方向
	if (userKey == left) {
		if (snake.position != left && snake.position != right)	snake.position = left;
	}
	else if (userKey == right) {
		if (snake.position != right && snake.position != left) snake.position = right;
	} 
	else if (userKey == up) {
		if (snake.position != up && snake.position != down) snake.position = up;
	}
	else if (userKey == down) {
		if (snake.position != down && snake.position != up) snake.position = down;
	}
}

void initFood() {
	while (1) {
		//食物的坐标必须是单位长度的整数倍(因为蛇首的坐标是单位长度的整数倍)
		food.xy.x = rand() % (WIDTH / LENGTH_UNIT) * LENGTH_UNIT;
		food.xy.y = rand() % (HEIGHT / LENGTH_UNIT) * LENGTH_UNIT;

		bool success = true;
		//食物不能出现在蛇的身上
		for (int i = 0; i < snake.size; i++) {
			if (food.xy.x == snake.xy[i].x && food.xy.y == snake.xy[i].y) {
				success = false;
				break;
			}
		}
		if (success) break;
	}
	food.exist = true;
}

void drawFood() {
	setlinecolor(BLACK); //边框黑色
	setfillcolor(RED); //填充红色
	fillrectangle(food.xy.x, food.xy.y, food.xy.x + 10, food.xy.y + 10);
}

void eatFood() {
	if (snake.xy[0].x == food.xy.x && snake.xy[0].y == food.xy.y) {
		snake.size++; //节数增加
		snake.score += SCORE_UNIT; //得分增加
		food.exist = false; //为了重新产生食物
	}
}


bool snakeAlive() {
	//撞墙
	if (snake.xy[0].x > WIDTH || snake.xy[0].x < 0 || snake.xy[0].y > HEIGHT || snake.xy[0].y < 0) {
		return false;
	}

	//撞到自己
	for (int i = 1; i < snake.size; i++) {
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y) {
			return false;
		}
	}

	return true;
}

void showGrade() {
	settextcolor(TRANSPARENT);
	outtextxy(500, 20, _T("您的分数为: "));
	TCHAR s[100];
	_stprintf(s, _T("%d"), snake.score);
	outtextxy(600, 20, s);
}
int maiin() {
	srand((unsigned int)time(NULL));
	hwnd = initgraph(WIDTH, HEIGHT); //初始化屏幕
	setbkcolor(WHITE); //设置背景颜色
	cleardevice(); //需要刷新，上一步设置的背景颜色才会生效
	initSnake();
	
	while (1) {
		cleardevice();
		showGrade();
		if (!food.exist)
			initFood();
		drawFood();
		drawSnake();
		if (!snakeAlive()) {
			outtextxy(WIDTH / 2 - 50, HEIGHT / 2, _T("GAME OVER! 按任意键退出!"));
			break;
		}
		eatFood();
		moveSnake();
		if (_kbhit()) {
			keyDown();
		}
		Sleep(50);
	}

	getchar();
	closegraph();
	return 0;
}