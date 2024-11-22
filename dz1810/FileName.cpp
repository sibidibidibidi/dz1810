#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

const int MAX_X = 100;
const int MAX_Y = 25;

void SetCursorPosition(int x, int y);
void SetCursorVisibility(bool visible);

static int currentScore;

int DisplayMenu()
{
    int choice;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "|                                                                    |" << std::endl;
    std::cout << "|                        0. Start Game                               |" << std::endl;
    std::cout << "|                        1. Exit                                     |" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "Enter choice: "; std::cin >> choice;
    return choice;
}

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

struct Coord
{
    int x;
    int y;
};

class Board
{
public:
    char grid[MAX_Y][MAX_X + 1];
    void CreateBorders(char ch);
    void Display();
    void GeneratePrize();
};

class SnakeGame
{
private:
    int length;
    Coord snakeBody[1000];
    Direction currentDirection;

public:
    SnakeGame(int _length, Direction _direction) :
        length(_length), currentDirection(_direction) {};

    void Initialize();
    void Move(char grid[MAX_Y][MAX_X + 1]);
    void ExtendBody();
    bool IsGameOver();
};

int main()
{
    int choice;
    srand(time(NULL));
    choice = DisplayMenu();

    SetCursorVisibility(false);

    Board gameBoard;
    SnakeGame snake(1, Up);

    gameBoard.CreateBorders(219);
    snake.Initialize();

    for (int i = 0; i < 20; i++)
        gameBoard.GeneratePrize();

    while (choice == 0)
    {
        if (snake.IsGameOver()) break;

        snake.Move(gameBoard.grid);
        gameBoard.Display();

        Sleep(85);
        std::cout << std::endl << "Score: " << currentScore << std::endl;
    }

    getchar();
}

void Board::CreateBorders(char ch)
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (y == 0 || y == MAX_Y - 1 ||
                x == 0 || x == MAX_X - 1)
            {
                grid[y][x] = ch;
            }
            else
            {
                grid[y][x] = ' ';
            }
        }
        grid[y][MAX_X] = '\0';
    }
}

void Board::Display()
{
    SetCursorPosition(0, 0);

    for (int y = 0; y < MAX_Y; y++)
    {
        std::cout << grid[y] << std::endl;
    }
}

void Board::GeneratePrize()
{
    int x = rand() % (MAX_X - 2) + 1;
    int y = rand() % (MAX_Y - 2) + 1;

    bool isValidPosition = false;
    while (true)
    {
        if (x % 2 == 0) isValidPosition = true;

        if (grid[y][x] == 'O' || grid[y][x] == '*') isValidPosition = false;

        if (isValidPosition) break;

        x = rand() % (MAX_X - 2) + 1;
    }

    grid[y][x] = '$';
}

void SnakeGame::Initialize()
{
    for (int i = 0; i < length; i++)
    {
        snakeBody[i].x = 0;
        snakeBody[i].y = 0;
    }

    length = 1;

    snakeBody[0] = { MAX_X / 2, MAX_Y / 2 };

    SetCursorPosition(snakeBody[0].x, snakeBody[0].y);
    putchar('O');
}

void SnakeGame::Move(char grid[MAX_Y][MAX_X + 1])
{
    Sleep(100);
    if (GetAsyncKeyState('W'))
    {
        currentDirection = Up;
    }
    else if (GetAsyncKeyState('S'))
    {
        currentDirection = Down;
    }
    else if (GetAsyncKeyState('A'))
    {
        currentDirection = Left;
    }
    else if (GetAsyncKeyState('D'))
    {
        currentDirection = Right;
    }

    grid[snakeBody[length - 1].y][snakeBody[length - 1].x] = ' ';

    for (int i = length; i > 0; i--)
    {
        snakeBody[i] = snakeBody[i - 1];
    }

    switch (currentDirection)
    {
    case Up:
        snakeBody[0].y = snakeBody[0].y - 1;
        break;

    case Down:
        snakeBody[0].y = snakeBody[0].y + 1;
        break;

    case Left:
        snakeBody[0].x = snakeBody[0].x - 2;
        break;

    case Right:
        snakeBody[0].x = snakeBody[0].x + 2;
        break;
    }

    if (grid[snakeBody[0].y][snakeBody[0].x] == '$') ExtendBody();

    grid[snakeBody[0].y][snakeBody[0].x] = 'O';

    if (length > 1)
    {
        grid[snakeBody[1].y][snakeBody[1].x] = '*';
    }
}

void SnakeGame::ExtendBody()
{
    snakeBody[length].x = snakeBody[length - 1].x;
    snakeBody[length].y = snakeBody[length - 1].y;
    currentScore++;
    length++;
}

bool SnakeGame::IsGameOver()
{
    bool gameOver = false;

    if (snakeBody[0].x <= 0 || snakeBody[0].x >= MAX_X - 1 ||
        snakeBody[0].y <= 0 || snakeBody[0].y >= MAX_Y - 1)
    {
        std::ofstream out("Score.txt", std::ios_base::app);
        out << "Score: " << currentScore << std::endl;
        gameOver = true;
    }

    for (int i = 2; i < length; i++)
    {
        if (snakeBody[0].x == snakeBody[i].x &&
            snakeBody[0].y == snakeBody[i].y)
        {
            std::ofstream out("Score.txt", std::ios_base::app);
            out << "Score: " << currentScore << std::endl;
            gameOver = true;
        }
    }
    return gameOver;
}

void SetCursorPosition(int x, int y)
{
    COORD pos = { x, y };
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void SetCursorVisibility(bool visible)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(handle, &cursorInfo);
}
