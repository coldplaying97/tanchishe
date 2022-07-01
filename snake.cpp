#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <string>


const int MAX_SIZE = 100; //����ж��ٽ�
const int WIDTH = 640; //��Ļ���
const int HEIGHT = 480; //��Ļ�߶�
const int LENGTH_UNIT = 10; //��ÿ�ڵĳ���
const int SCORE_UNIT = 10; //ÿ�γԵ�ʳ��õ��ķ���

//����x, y
struct point
{
	int x, y;
};

//��
struct Snake {
	int size; //����
	point xy[MAX_SIZE]; //���������
	char position; //��ǰ�ƶ�����
	int score; //�÷�
}snake;

//ʳ��
struct Food {
	point xy; //ʳ������
	bool exist; //�Ƿ����
}food;

//�ƶ�����
enum movePosition {
	right = 77,
	left = 75,
	down = 80,
	up = 72
};

//���ھ��
HWND hwnd = NULL;

void initSnake() {
	//�߳�ʼ����Ϊ3
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
	setlinecolor(BLACK); //�߿��ɫ
	setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255)); //�����ɫ
	for (int i = 0; i < snake.size; i++) {
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}

void moveSnake() {
	//�������ף�ÿ���ƶ���ǰһ�ڵ�λ��
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
	//��������Ŀǰ���򼰷�����ʱ,����Ҫ�ı䷽��
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
		//ʳ�����������ǵ�λ���ȵ�������(��Ϊ���׵������ǵ�λ���ȵ�������)
		food.xy.x = rand() % (WIDTH / LENGTH_UNIT) * LENGTH_UNIT;
		food.xy.y = rand() % (HEIGHT / LENGTH_UNIT) * LENGTH_UNIT;

		bool success = true;
		//ʳ�ﲻ�ܳ������ߵ�����
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
	setlinecolor(BLACK); //�߿��ɫ
	setfillcolor(RED); //����ɫ
	fillrectangle(food.xy.x, food.xy.y, food.xy.x + 10, food.xy.y + 10);
}

void eatFood() {
	if (snake.xy[0].x == food.xy.x && snake.xy[0].y == food.xy.y) {
		snake.size++; //��������
		snake.score += SCORE_UNIT; //�÷�����
		food.exist = false; //Ϊ�����²���ʳ��
	}
}


bool snakeAlive() {
	//ײǽ
	if (snake.xy[0].x > WIDTH || snake.xy[0].x < 0 || snake.xy[0].y > HEIGHT || snake.xy[0].y < 0) {
		return false;
	}

	//ײ���Լ�
	for (int i = 1; i < snake.size; i++) {
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y) {
			return false;
		}
	}

	return true;
}

void showGrade() {
	settextcolor(TRANSPARENT);
	outtextxy(500, 20, _T("���ķ���Ϊ: "));
	TCHAR s[100];
	_stprintf(s, _T("%d"), snake.score);
	outtextxy(600, 20, s);
}
int maiin() {
	srand((unsigned int)time(NULL));
	hwnd = initgraph(WIDTH, HEIGHT); //��ʼ����Ļ
	setbkcolor(WHITE); //���ñ�����ɫ
	cleardevice(); //��Ҫˢ�£���һ�����õı�����ɫ�Ż���Ч
	initSnake();
	
	while (1) {
		cleardevice();
		showGrade();
		if (!food.exist)
			initFood();
		drawFood();
		drawSnake();
		if (!snakeAlive()) {
			outtextxy(WIDTH / 2 - 50, HEIGHT / 2, _T("GAME OVER! ��������˳�!"));
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