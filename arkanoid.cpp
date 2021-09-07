#include <iostream>
#include "Header.h"
#include <conio.h>
#include <stdio.h>

int main()
{
	UINT duration = 10 * MILLIS_IN_SECOND; //длительность игры в мс
	UINT delay = 300; //задержка обновления кадра в мс
	SYSTEMTIME tempTime; //системное время
	GetSystemTime(&tempTime);
	UINT time = eraTimeToStartMonth(tempTime); //время от начала текущего месяца
	UINT startTime = time; //время начала обновления кадра

	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	//устанавливаем размер окна
	COORD coord = makeCoord(WIDTH, HEIGHT);
	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = HEIGHT - 1;
	Rect.Right = WIDTH - 1;
	SetConsoleScreenBufferSize(hCon, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(hCon, TRUE, &Rect);            // Set Window Size 

	int gameOver = 0;
	int youWin = 0;
	
	Stick stick;
	Ball ball(stick.y);
	std::vector<Brick> bricks;

	Brick tempBrick;
	tempBrick.x = 1;
	tempBrick.y = 0;
	bricks.push_back(tempBrick);

	int prevX = tempBrick.x;
	int prevY = tempBrick.y;
	int count = 1;

	//36 кирпичей , 6*6
	for (int i = 0; i < 35; i++)
	{
		if (count == 6)
		{
			count = 0;
			tempBrick.y = prevY + 2; 
			tempBrick.x = 1;
		}
		else {
			tempBrick.x = prevX + 13;
			tempBrick.y = prevY;
		}
	
		bricks.push_back(tempBrick);

		prevX = tempBrick.x;
		prevY = tempBrick.y;

		
		count++;
	}

	
				
	int keyNumber = 0; //код нажатой клавиши
	
	while ((time - startTime < duration) && !gameOver && !youWin)
	{
		GetSystemTime(&tempTime); //обновление времени
		time = eraTimeToStartMonth(tempTime); 
		

		if (time - startTime > delay) // обновление кадра
		{

			startTime = time;
			
			system("cls");

			stick.draw(hCon);

			ball.position();

			ball.crashStick(stick);
			
			int tempIndex = 100;
			int tempCrashBrick = 0;

			for (int i = 0; i < bricks.size(); i++)
			{			
				tempCrashBrick = ball.crashBrick(bricks[i]);
				bricks[i].draw(hCon);

				if (tempCrashBrick)
				{
					tempIndex = i;
				}
			}
			
			ball.draw(hCon);
			
			if (tempIndex != 100)
			{
				bricks.erase(bricks.begin() + tempIndex);
			}

			if (ball.y >= HEIGHT-1)
				gameOver = 1;

			if (bricks.size() == 0)
				youWin = 1;
		
		}

		

		bool somethingPressed = _kbhit();
		if (somethingPressed) {
			//получаем код нажатой клавиши
			keyNumber = _getch();
		}
		else
		{
			keyNumber = 0;
		}

		if (keyNumber == 75)
		{
			stick.position(0);
		}
		else if (keyNumber == 77)
		{
			stick.position(1);
		}

		
		
	}
		
	if (gameOver)
	{
		std::cout << std::endl;
		std::cout << "GAME OVER!" << std::endl;
	}
	else if (youWin)
	{
		std::cout << std::endl;
		std::cout << "YOU WIN!" << std::endl;
	}
	else
	{
		std::cout << std::endl;
		std::cout << "TIME IS OUT!" << std::endl;
	}
	
	return 0;
}


