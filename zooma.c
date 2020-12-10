#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
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
int tailX[41];
int tailY[41]; //coordonatele mingilor
char tailC[41]; //caracterul mingilor
int nTail = 41; //numarul de mingi
int currentTail = 0;
enum Direction {S = 0, L, R, U, D, E}; //directiile posibile de deplasare acursoarelor
enum Direction dir = S;
clock_t init, fin; //timpul pentru cronometrarea aparitiei mingilor

void cls() //in schimbul functiei de curatare ca aia provoaca epilepsie.
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}
void LevelAssignBall() //Stabileste Ruta mingilor pentru nivelul anumit.
{
    int i = 10, j = 3;

    for (int k = 0; k < nTail; k++)
    {
        if (i > 2 && j == 3)
        {
            tailX[k] = j;
            tailY[k] = i;
            i--;
        }
        else if(j < 27)
        {
            tailX[k] = j;
            tailY[k] = i;
            j++;
        }
        else
        {
            tailX[k] = j;
            tailY[k] = i;
            i++;
        }
        tailC[k] = '*';
    }

}


char AddBall() //Functie care adauga mingi la sfarsitul cozii
{
    int b = rand() % 4;
    switch(b)
    {
        case 0 : return 'B'; break;
        case 1 : return 'R'; break;
        case 2 : return 'G'; break;
        case 3: return 'W'; break;
    }
}
void BallMove(int x) //Functie responsabila de miscarea cozii.
{
    for (int i = 1; i < x; i++)
        tailC[i - 1] = tailC[i];
    if (currentTail >= nTail) tailC[x - 1] = '*';
    else tailC[x - 1] = AddBall();
    init = clock();
}
void Setup() //Seteaza parametrii la inceputul jocului.
{

    gameOver = 0;

    cx = 15;
    cy = 7;
    can = 'A';

    hy = 2;
    wx = 2;

    LevelAssignBall();

}

void Draw() //Deseneaza board-ul
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
void Input() //Verifica input-ul
{
    char in = 'A'; //A este o valoare aleatorie utilizata pentru initializarea curata a variabilei in.
    if(_kbhit()) in = _getch();
    switch(in)
    {
        case 'w' : dir = U; break;
        case 'a' : dir = L; break;
        case 'd' : dir = R; break;
        case 's' : dir = D; break;
        case 13 : dir = E; break;
        default  : dir = S; break;
    }
}
void Logic() //Functie ce raspunde de logica jocului.
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
    if (dir == E);

    fin = clock();
    if(fin - init >= 1000) BallMove(nTail);


    if (wx <= 0) wx = WIDTH - 1; else if (wx >= WIDTH) wx = 1;
    if (hy <= 0) hy = HEIGHT - 1; else if (hy >= HEIGHT - 1) hy = 1;
}
int main()
{
    Setup();
    init = clock();
    while(!gameOver)
    {
        srand(time(NULL));
        Draw();
        Input();
        Logic();
        Sleep(100);
    }
}
