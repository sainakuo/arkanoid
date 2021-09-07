#include <windows.h>
#include <vector>

#define MILLIS_IN_SECOND 1000
#define SECONDS_IN_MINUTE 60
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24

#define WIDTH 80
#define HEIGHT 40


//������ ������ ��� - ������ �������� ������, ����� �������� ��������� � ����������� ���� � ������
UINT eraTimeToStartMonth(SYSTEMTIME systemTime) {
	UINT hoursInMonth = systemTime.wDay * HOURS_IN_DAY + systemTime.wHour;
	UINT minutesInMonth = hoursInMonth * MINUTES_IN_HOUR + systemTime.wMinute;
	UINT secondsInMonth = minutesInMonth * SECONDS_IN_MINUTE + systemTime.wSecond;
	UINT millisInMonth = secondsInMonth * MILLIS_IN_SECOND + systemTime.wMilliseconds;

	return millisInMonth;
}

COORD makeCoord(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	return coord;
}


//�����
class Stick { 
public:
	int x, y; //���������� ������ �������� ���� �����
	int a, b; //����� � ������
	bool dir;  //����������� 0 �����, 1 ������
	double vt; //��������*�����

	Stick() 
	{
		a = 20;
		b = 2;
		x = 0;
		y = HEIGHT - b;
		dir = 0;
		vt = 3;
	}

	
	void draw(HANDLE hCon)
	{
		COORD topLeftCorner;
		
		for (int i = 0; i < b; i++)
		{
			topLeftCorner = makeCoord(x, y + i);
			SetConsoleCursorPosition(hCon, topLeftCorner);
			for (int j = 0; j < a; j++)
			{
				std::cout << "|";
			}
		}
	}
	

	void position(int d) {

		dir = d;
		static int key = 0;

		if ((x < WIDTH - a && x > 0) || key == 1 || key == 2)
		{
			if (dir == 0 && key != 2)
			{
				x -= vt;
				key = 0;
				return;
			}
			else if (dir == 1 && key != 1)
			{
				x += vt;
				key = 0;
				return;
			}
			
		}

		if (x == WIDTH - a)
		{
			key = 1;
			
		}
		else if (x == 0)
		{
			key = 2;
		}

	}

};


//������
class Brick {
public:
	int x, y; //���������� ������ �������� ���� �������
	int a, b; //����� � ������

	Brick() 
	{
		x = 0;
		y = 0;
		a = 12;
		b = 1;
	}

	void draw(HANDLE hCon) {

		COORD topLeftCorner;

		for (int i = 0; i < b; i++)
		{
			topLeftCorner = makeCoord(x, y + i);
			SetConsoleCursorPosition(hCon, topLeftCorner);
			for (int j = 0; j < a; j++)
			{
				std::cout << "|";
			}
		}
	}
};

//���
class Ball {
public:
	int x, y; //���������� �����
	int dx, dy;  //����������� ��� 45 ��������: ����� ������ (1,1), ���� ������ (1, -1), ���� ����� (-1, -1), ����� ����� (-1, 1)
	int vt; //��������*�����

	Ball(int sy) {
		x = 10;
		y = sy - 1; //��� ������� ���� ��� �� �����
		dx = -1;
		dy = 1;
		vt = 4;
	}

	//��������� ����������� ��� ����� �� ������
	int crashBrick(Brick brick)
	{
		if ((x >= brick.x && x <= (brick.x + brick.a) && y <= brick.y + 1))
		{
			y = brick.y + 1;
			dy = -1;
			return 1;
		}
		else 
			return 0;
	}

	//��������� ����������� ��� ����� �� �����
	void crashStick(Stick stick)
	{
		if ((x >= stick.x && x <= (stick.x + stick.a) && y >= stick.y - 1))
		{
			y = stick.y - 1;
			dy = 1;
		}
	}

	

	void draw(HANDLE hCon)
	{
		//����������� �� �����
		if (x >= WIDTH - 1)
		{
			x = WIDTH - 1;
		}
		else if (x < 0)
		{
			x = 0;
		}

		if (y >= HEIGHT - 1)
		{
			y = HEIGHT - 1;
		}
		else if (y < 0)
		{
			y = 0;
		}

		COORD topLeftCorner = makeCoord(x, y);
		SetConsoleCursorPosition(hCon, topLeftCorner);
		std::cout << "O";
	}

	void position() {


		//����� ������ (1,1)
		if (dx == 1 && dy == 1)
		{
			x += vt;
			y -= vt;
		}
		//����� ����� (-1, 1)
		else if (dx == -1 && dy == 1)
		{
			x -= vt;
			y -= vt;
		}
		//���� ������(1, -1)
		else if (dx == 1 && dy == -1)
		{
			x += vt;
			y += vt;
		}
		//���� ����� (-1, -1)
		else if (dx == -1 && dy == -1)
		{
			x -= vt;
			y += vt;
		}


		//��������� ����������� ��� ����� �� ������ 
		if (x <= 0)
		{
			dx = 1;

		}
		else if (x >= WIDTH)
		{
			dx = -1;

		}
		if (y <= 0)
		{
			dy = -1;

		}
		else if (y >= HEIGHT)
		{
			dy = 1;

		}

	}
};