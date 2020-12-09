#include <stdio.h>
#include <windows.h>
int gameOver; //variabila care determina daca jocul inca mai mergw
char board[15][30]= {
    " #############################",
    " #                           #",
    " # ************************* #",
    " # *                       * #",
    " # *                       * #",
    " # *                       * #",
    " # *                       * #",
    " # *                       * #",
    " # *                       * #",
    " # *                       * #",
    " # *                       * #",
    " # @                       @ #",
    " #                           #",
    " #############################",
    "                              ",
    }; //Boardul de joc. Am vrut 20 pe 20, dar arata strasnic. Spatiile pustii de sus si stanga sunt entru cursoare.
const int WIDTH = 30;
const int HEIGHT = 15;
const int hx = 0;   //coordonata x a cursorului vertical
const int wy = 14; // coordonata y a cursorului orizontal
int cx, cy, hy, wx, score; // c - canon, h - cursor vertical, w - cursor orizontal.
char can; //caracterul din canon
int tailX[100] = {};
int tailY[100] = {}; //coordonatele mingilor
char tailC[100] = {}; //caracterul mingilor
int nTail; //numarul de mingi
enum Direction {S = 0, L, R, U, D}; //directiile posibile de deplasare
enum Direction dir = S;

void cls() //in schimbul functiei de curatare ca aia provoaca epilepsie.
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}
void Timer()
{

}
void Setup()
{

    gameOver = 0;

    cx = 15;
    cy = 7;
    can = 'A';

    hy = 2;
    wx = 2;
   // nTail = 41;
    //tailX = {};
    //tailY = {};
    //tailC = {'*'};

}

void Draw()
{
    cls();
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
            if (i == cy && j == cx) printf("%c ", can);
            else if (i == hy && j == hx) printf ("> ");
            else if (i == wy && j == wx) printf("^ ");
            else
            {
                int balls = 0;
                for (int k = 0; k < nTail; k++)
                  if(tailX[k] == j && tailY[k]==i)
                    {
                        printf("%c ", tailC[k]);
                        balls = 1;
                    }
                if(!balls) printf("%c ", board[i][j]);
            }
        printf("\n");
    }
}

void Input()
{
    char in = 'A';
    if(_kbhit()) in = _getch();
    switch(in)
    {
        case 'w' : dir = U; break;
        case 'a' : dir = L; break;
        case 'd' : dir = R; break;
        case 's' : dir = D; break;
        default  : dir = S; break;
    }
}
void BallMove()
{

}

void Logic()
{

    switch(dir)
    {
    case L:
        wx--;
        break;
    case R:
        wx++;
        break;
    case U:
        hy--;
        break;
    case D:
        hy++;
        break;
    case S:
        hy = hy;
        wx = wx;
    default: break;
    }
    if (dir == S);
    else
    {
    BallMove();
    Timer();
    }
    if (wx <= 0) wx = WIDTH - 1; else if (wx >= WIDTH) wx = 1;
    if (hy <= 0) hy = HEIGHT - 1; else if (hy >= HEIGHT - 1) hy = 1;
}
int main()
{
    Setup();
    while(!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }
}
