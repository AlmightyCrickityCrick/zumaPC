#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void welcome();//guide of the game
void draw();//show the game board
void LevelAssignBall();//show the path and characters on it
char cannon();//generate a random bill for the cannon
char AddBall();//add a new ball in a row
void Input();//assign the values for the moves of the cursors
void Logic();//describe conditions of the game
void cls();//clear the screen
void MoveFront(int x, int throwFlag);//describe the move of the row
void CheckCollision();//check if the bill hits on the right place, hits on the row or misses
void timer(int sec);//control time of the game not to be exceeded

char board[16][31];//board

const int WIDTH = 30;//board's width
const int HEIGHT = 16;//board's height
const int hx = 0;   //coordonata x a cursorului vertical
const int wy = 14; // coordonata y a cursorului orizontal
const int cx = 15, cy = 10;//cannon coordinates

int hy, wx, score = 0, v = 0, s = 0, x, y; // c - canon, h - cursor vertical, w - cursor orizontal.
char can, nextcan; //caracterul curent si precedent din cannon
int speed = 1000;//set the speed of the game
int tailX[41];
int tailY[41]; //coordonatele mingilor
char tailC[41]; //caracterul mingilor
int nTail = 41; //numarul de mingi
int billsnr = 40;//the number of generated bills
int currentTail = 0;
int gameOver = 0;//check the conditions while game is still active
int timelim = 90;//time limit (in seconds)
clock_t init, fin, initial = clock(); //timpul pentru cronometrarea aparitiei mingilor

enum Direction {S = 0, L, R, U, D, E, Stop, Change}; //directiile posibile de deplasare acursoarelor
enum Direction dir = S;

int main(){
    int i, j, k;
    char c;
    srand((unsigned) time(0));
    //basic of the board
    for(i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++) board[i][j] = ' ';
    board[cy - 1][cx] = board[cy][cx - 1] = board[cy + 1][cx] = board[cy][cx + 1] =
    board[cy - 1][cx - 1] = board[cy + 1][cx + 1] = board[cy - 1][cx + 1] = board[cy + 1][cx - 1] = '*';
    //assigning values for the current and next bills in cannon
    can = cannon();
    nextcan = cannon();
    LevelAssignBall();
    welcome();
    scanf("%c", &c);
    if(c == '\n'){
        system("cls");
        while(!gameOver){
            cls();
            draw();
            Input();
            Logic();
            timer(timelim);
            printf("\n   Time remaining: %2d", timelim);
            printf("\n   Your score: %d", score);
            Sleep(speed);
        }}
    if(gameOver == 2) printf("\n   You lose");//check the conditions of losing
    else if(gameOver == 3) printf("\n   You win");//check the condition of winning

    return 0;
}

void draw(){
    int i, j, k;
    //basic of the board
    for(i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++){
                if(i == HEIGHT - 1) board[i][j] = ' ';
                else if(j == 0) board[i][j] = ' ';
        }
    for(k = 0; k < nTail; k++) board[tailX[k]][tailY[k]] = tailC[k];//display the path and bills on it
    board[0][0] = nextcan;//show the value of the next bill
    //set the start and end points, the cannon
    board[tailX[0] + 1][tailY[3]] = '@';
    board[tailX[0] + 1][tailY[38]] = '@';
    board[cy][cx] = can;
    //set the cursors
    board[hy][0] = '>';
    board[15][wx] = '^';
    //set the margins
    for(i = 0; i < HEIGHT - 2; i++) board[i][1] = '#';
    for(i = 0; i < HEIGHT - 2; i++) board[i][WIDTH - 1] = '#';
    for(j = 1; j < WIDTH; j++) board[0][j] = '#';
    for(j = 1; j < WIDTH; j++) board[HEIGHT - 2][j] = '#';
    //display the board
    for(i = 0; i < HEIGHT; i++){
        printf("\n");
        for(j = 0; j < WIDTH; j++) printf(" %c", board[i][j]);
    }

}

void LevelAssignBall() //Stabileste Ruta mingilor pentru nivelul anumit.
{
    int i = 10, j = 3, k = 0;
        for(i = 10; i > 1; i--){
            tailX[k] = i;
            tailY[k] = 3;
            tailC[k] = '*';
            k++;
        }
        for(j = 4; j < 28; j++){
            tailX[k] = 2;
            tailY[k] = j;
            tailC[k] = '*';
            k++;
        }
        for(i = 3; i < 11; i++){
            tailX[k] = i;
            tailY[k] = 27;
            tailC[k] = '*';
            k++;
        }
}

char cannon(){
        int b = rand() % 4;
        switch(b)
        {
            case 0 : return 'B'; break;
            case 1 : return 'R'; break;
            case 2 : return 'G'; break;
            default: return 'W'; break;
        }
}

char AddBall() //Functie care adauga mingi la sfarsitul cozii
{
    int b = rand() % 4;
    //check the row not to exceed the given value
    if(billsnr >= 0){
        billsnr--;
        switch(b)
        {
            case 0 : return 'B'; break;
            case 1 : return 'R'; break;
            case 2 : return 'G'; break;
            default: return 'W'; break;
        }
    }
    else return '*';
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
        case 'x' : dir = Stop; break;//end the game(without winning or loosing)
        case 'c' : dir = Change; break;//change the value of the current bill in cannon with the next one
        case 13 : dir = E; break;
        default  : dir = S; break;
    }
}
void Logic() //Functie ce raspunde de logica jocului.
{
    //check the condition of winning
    int n = 0;
    if(s > 1){
        while((n < nTail) && (tailC[n] == '*')) n++;
        if(n == nTail) gameOver = 3;
    }
    s++;
    //assign actions for the player's input
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
    case Stop:
        gameOver = 1;
        break;
    //change the value of the current bill in the cannon with the value of the next one
    case Change:
        {
            int aux = nextcan;
            nextcan = can;
            can = aux;
        }
    case S:
        hy = hy;
        wx = wx;
    default: break;
    }
    if (wx <= 2) wx = WIDTH - 3;
    else if (wx >= WIDTH - 2) wx = 3;
    if (hy <= 1) hy = HEIGHT - 6;
    else if (hy >= HEIGHT - 5) hy = 2;

    if (dir == E) CheckCollision();
    fin = clock();
    if(dir != E && fin - init >= speed) MoveFront(nTail, 0);
    //check condition of loosing(reaching the end point)
    if(tailC[0] != '*') v++;
    else v = 0;
    if(v > 1) gameOver = 2;
}

void cls() //in schimbul functiei de curatare ca aia provoaca epilepsie.
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void MoveFront(int x, int throwFlag) //Functie responsabila de miscarea cozii.
{
    for (int i = 1; i < x; i++)
        tailC[i - 1] = tailC[i];
    if (currentTail >  nTail || throwFlag == 1) tailC[x - 1] = '*';
    else tailC[x - 1] = AddBall();
    init = clock();
}

void CheckCollision()
{
    int i = 0, j = 0, n = 0, aux;
    int follow = 0,k = 0;
        //check if the bill reach the right or left column
        if((wx == 3) || (wx == 27)){
            while(tailY[k] != wx) k++;
            while(tailX[k] != hy) k++;
        }
        //check if the cursor position is parallel with the cannon
        else if(wx == cx){
            while(tailX[k] != 3) k++;
            while(tailY[k] != wx) k++;
        }
        //check if the bill reach the top
        else if(hy == 2){
            while(tailX[k] != hy) k++;
            while(tailY[k] != wx) k++;
        }
        //diagonal throw
        else{
            i = hy;
            j = wx;
            //right part
            if(wx > 15){
                while((i != 2) && (j != 27)){
                    i--;
                    j++;
               }
                if(i == 2){
                    while(tailX[k] != hy) k++;
                    while(tailY[k] != j) k++;
                }
                else{
                    while(tailY[k] != wx) k++;
                    while(tailX[k] != i) k++;
                }
            }
            //left part
            else{
                while((i != 2) && (j != 3)){
                    i--;
                    j--;
                }
                if(i == 2){
                    while(tailX[k] != hy) k++;
                    while(tailY[k] != j) k++;
                }
                else{
                    while(tailY[k] != wx) k++;
                    while(tailX[k] != i) k--;
                }
            }
        }
        //check if bill has missed or not
        if((tailC[k + 1] != '*') || (tailC[k - 1] != '*')){
                if((tailC[k] != '*') && (tailC[k - 1] != '*')){
                    MoveFront(k + 1, 1);
                    tailC[k] = can;
                }
                else if((tailC[k - 1] != '*') && (tailC[k] == '*')){
                    MoveFront(k, 1);
                    k--;
                    tailC[k] = can;
                }
                else if((tailC[k + 1] != '*') && (tailC[k] == '*')) tailC[k] = can;
                else if((tailC[k] != '*') && (tailC[k - 1] != '*')) MoveFront(k, 1);
        }
        //check if there are at least three components in a row and if yes, canceled them
        //same element from bot parts
        if ((tailC[k - 1] == can) && (tailC[k + 1] == can))
        {
            int f = k - 1, b = k + 1;
            while (tailC[f] == can)
            {
                follow++;
                f--;
            }
            while(tailC[b] == can)
            {
                follow++;
                b++;
            }
            if (follow >= 2){
                for (i = f + 1; i < b; i++)
            {
                score += 100;
                tailC[i] = '*';
                }
                do{
                    if(tailC[f] == '*') n++;
                    aux = tailC[f + follow + 1];
                    tailC[f + follow + 1] = tailC[f];
                    tailC[f] = aux;
                    f--;
                }
                while((n < follow) && (f >= 0));
            }
        }
        //same elements only from the left
        else if(tailC[k - 1] == can){
            int f = k - 1;
            while (tailC[f] == can)
            {
                follow++;
                f--;
            }
            if (follow >= 2){
                for (int i = f + 1; i <= k; i++)
            {
                score += 100;
                tailC[i] = '*';
                }
                do{
                    if(tailC[f] == '*') n++;
                    aux = tailC[f + follow + 1];
                    tailC[f + follow + 1] = tailC[f];
                    tailC[f] = aux;
                    f--;
                }
                while((n <= follow) && (f >= 0));
            }
        }
        //same elements only from the right
        else if(tailC[k + 1] == can){
            int b = k + 1;
            while(tailC[b] == can)
            {
                follow++;
                b++;
            }
            if (follow >= 2){
                for (int i = k; i < b; i++)
                {
                score += 100;
                tailC[i] = '*';
                }
                do{
                    if(tailC[k] == '*') n++;
                    aux = tailC[k + follow + 1];
                    tailC[k + follow + 1] = tailC[k];
                    tailC[k] = aux;
                    k--;
                }
                while((n < follow) && (k >= 0));
            }
        }
    can = nextcan;//assign to the current value of bill in cannon, the next value
    nextcan = cannon();//give the next value of the bill in the cannon
    follow = 0;
}

void timer(int sec){
    if(timelim > 0) timelim = timelim - 1;
    //check the condition of loosing (time limit exceeded)
    if(timelim == 0) gameOver = 2;

}

void welcome(){
    //version 1
    printf("\n   Welcome to the game!\n\n   Your goal is to destroy all bills in 90 seconds\n");
    printf("   Press \"W\" to move up\n   Press \"S\" to move down\n   Press \"A\" to move left\n   Press \"D\" to move right\n   Press \"C\" to change the ball\n   Press \"X\" quit the game");
    printf("\n   Press \"Enter\" to shoot");
    printf("\n\n   Press \"Enter\" to continue");
}

