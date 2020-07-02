#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define LeftWall 10
#define RightWall 110
#define UpWall 2
#define DownWall 25

using namespace std;
void GotoXY(int column, int row);
void Nocursortype();
void SetColor(int ScreenColor, int TextColor);
struct Point
{
    int x, y;
    int ox, oy;
};
class Snake
{
public:
    struct Point A[100];
    struct Point Food;
    int Length;
    Snake() 
    {
        Length = 5;
        A[0].x = 48;
        A[0].y = 10;
        A[1].x = 49;
        A[1].y = 10;
        A[2].x = 50;
        A[2].y = 10;
        A[3].x = 51;
        A[3].y = 10;
        A[4].x = 52;
        A[4].y = 10;
        Food.x = Food.y = -1;
    }
    void DrawSnake() 
    {
        SetColor(0, 10);
        GotoXY(A[Length - 1].ox, A[Length - 1].oy);
        cout << " ";
        GotoXY(A[0].x, A[0].y);
        SetColor(10, 10);
        cout << "O";
        SetColor(0, 10);
        for (int i = 1 ; i < Length; i++) 
        {
            GotoXY(A[i].x, A[i].y);
            cout << "o";
        }
    }
    void TouchBody()
    {
        SetColor(0, 10);
        for (int i = 3; i < Length; i++)
        {
            if (A[0].x == A[i].x && A[0].y == A[i].y)
            {
                for (int j = i; j < Length; j++)
                {
                    GotoXY(A[j].x, A[j].y);
                    cout << " ";
                }
                Length -= (Length - 1 - i);
            }
        }
    }
    bool GameOver()
    {
        if (A[0].x == RightWall || A[0].x == LeftWall || A[0].y == UpWall || A[0].y == DownWall)
            return 1;
        return 0;
    }
    void ScoreSystem(int &s, int &s1)
    {
        for (int i = 3; i < Length; i++)
        {
            if (A[0].x == A[i].x && A[0].y == A[i].y)
            {
                s = (1 + i - 4) * 10 + s1;
            }
        }
        if (A[0].x == Food.x && A[0].y == Food.y)
        {
            s = (Length - 4) * 10 + (++s1);
        }
    }
    void DrawWall()
    {
        char c = 176;
        SetColor(0, 14);
        for (int i = LeftWall; i <= RightWall; i++)
        {
            GotoXY(i, UpWall);
            cout << c;
            GotoXY(i, DownWall);
            cout << c;
        }
        for (int i = UpWall; i <= DownWall; i++)
        {
            GotoXY(LeftWall, i);
            cout << c;
            GotoXY(RightWall, i);
            cout << c;
        }
    }
    void DrawFood()
    {
        GotoXY(Food.x, Food.y);
        cout << " ";
        srand((unsigned int)time(NULL));
        Food.x = rand() % (RightWall - LeftWall - 2) + (LeftWall + 1);
        Food.y = rand() % (DownWall - UpWall - 2) + (UpWall + 1);
        GotoXY(Food.x, Food.y);
        SetColor(0, 4);
        cout << "*";
    }
    void EatFood()
    {
        if (A[0].x == Food.x && A[0].y == Food.y)
        {
            DrawFood();
            Length++;
        }
    }
    void Move(int Direction)
    {
        for (int i = 0; i < Length; i++)
        {
            if (i == 0)
            {
                A[0].ox = A[0].x;
                A[0].oy = A[0].y;
                if (Direction == 0) A[0].x = A[0].x + 1;
                if (Direction == 1) A[0].y = A[0].y + 1;
                if (Direction == 2) A[0].x = A[0].x - 1;
                if (Direction == 3) A[0].y = A[0].y - 1;
            }
            else
            {
                A[i].ox = A[i].x;
                A[i].oy = A[i].y;
                A[i].x = A[i - 1].ox;
                A[i].y = A[i - 1].oy;
            }
        }
    }
    void SpeedUp(int &n)
    {
        if (A[0].x == Food.x && A[0].y == Food.y && n > 50)
            n -= 10;
    }
    void DrawScoreBoard(int s)
    {
        GotoXY(80, 27);
        SetColor(1, 14);
        cout << " Score: " << s << " ";
    }
};

int main()
{
    Snake S;
    int Direction = 2;
    int Direction0 = 2;
    char t;
    int n = 320;
    int s = 0, s1 = 0;
    S.DrawWall();
    S.DrawFood();
    Nocursortype();
    while (1)
    {
        S.DrawSnake();
        Direction0 = Direction;
        if (_kbhit()) 
        {
            t = _getch();
            if (t == 75 && Direction0 != 0)
                Direction = 2;
            if (t == 72 && Direction0 != 1) 
                Direction = 3;
            if (t == 77 && Direction0 != 2)
                Direction = 0;
            if (t == 80 && Direction0 != 3)
                Direction = 1;
        }
        S.DrawScoreBoard(s);
        S.EatFood();
        S.TouchBody();
        S.Move(Direction);
        Sleep(n);
        S.SpeedUp(n);
        S.ScoreSystem(s, s1);
        if (S.GameOver() == 1)
            break;
    }
    SetColor(0, 12);
    GotoXY(10, 28);
    cout << "  GAME OVER. \n\n\n";
    system("pause");
}
void GotoXY(int column, int row)
{
    COORD coord;
    coord.X = column;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Nocursortype()
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
void SetColor(int ScreenColor, int TextColor)
{
    HANDLE hConsoleColor;
    hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleColor, ScreenColor << 4 | TextColor);
}