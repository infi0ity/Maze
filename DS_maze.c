#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>


#define L 15//迷宫大小
#define ROAD 0
#define WALL 1
#define GOAL 2

int canvas[L][L];
int positionX, positionY;
int end;
int win;
int startTime;
int score;

typedef struct node 
{
    int x, y;
    struct node * next;
}Node;

typedef struct Stack
{
    Node * head;
} Stack;

void start(int c);
void show(int c);
void updateWithoutInput();
int updateWithInput(int c);
void gotoxy(int x, int y);
void HideCursor();
int game(int c);
int menu();
int cmenu();
int mapMenu();
int mapMenu1();
void CreateMaze(int x, int y);
void selectMaze();
void saveMaze(char *ch);
void loadMaze(char *ch);
void editMaze();
void findRoute();
void push(Stack *que, int x, int y);
void pop(Stack *que);
void DFS(int a[L][L], int x, int y, Stack *que);

int main()
{
    int choice, res;
    score = 0;
    choice = menu();
    while (choice != 4)
    {
        if (choice == 1)
        {
            res = game(choice);
            if (res == 1)
                printf("YOU WIN!!!");
            else
                printf("YOU LOSE!!!");
            Sleep(800);
        }
        else if(choice == 2)
            editMaze();
        else if(choice == 3)
            findRoute();
        else 
        {
            printf("enter the true choice");
            Sleep(300);
        }
        choice = menu();
    }
    system("pause");
    return 0;
}

int game(int c)
{
    int choice;
    choice = mapMenu();
    start(choice);
    while (end)
    {
        show(1);
        updateWithoutInput();
        updateWithInput(1);
    }
    return win;
}

int menu()
{
    system("cls");
    int c;
    printf("#######################################\n");
    printf("\t\t  WELCOME!\n");
    printf("\t\t1.start game\n");    
    printf("\t\t2.edit maze\n");
    printf("\t\t3.find route\n");
    printf("\t\t4.exit game\n");
    printf("#######################################\n");
    scanf("%d", &c);
    return c;
}
int cmenu()
{
    system("cls");
    int c;
    
    printf("1.continue\n");
    printf("2.exit\n");
    scanf("%d", &c);
    return c;
}
int mapMenu()
{
    system("cls");
    int c;
    printf("#######################################\n");
    printf("\t\t  MAZE\n");
    printf("\t\t1.random maze\n");
    printf("\t\t2.saved maze\n");
    printf("#######################################\n");
    scanf("%d", &c);
    return c;
}

int mapMenu1()
{
    system("cls");
    int c;
    printf("#######################################\n");
    printf("\t\t  MAZE\n");
    printf("\t\t1.Blank maze\n");
    printf("\t\t2.saved maze\n");
    printf("#######################################\n");
    scanf("%d", &c);
    return c;
}

void start(int c)
{
    win = 0;
    end = 1;
    

    int i, j;                               
    //mouse position
    positionX = 1;
    positionY = 2;                   
    
    for (int i = 0; i < L; i++)
        for (int j = 0; j < L; j++)
            canvas[i][j] = WALL;
    for (int i = 0; i < L; i++)
    {
        canvas[i][0] = ROAD;
        canvas[0][i] = ROAD;
        canvas[i][L - 1] = ROAD;
        canvas[L - 1][i] = ROAD;
    }
    Sleep(100);
    Sleep(300);
    if (c == 1)
    {
        CreateMaze(2, 2);
        //画迷宫的入口和出口
        canvas[2][1] = ROAD;

        //由于算法随机性，出口有一定概率不在（L-3,L-2）处，此时需要寻找出口
        for (int i = L - 3; i >= 0; i--)
        {
            if (canvas[i][L - 3] == ROAD)
            {
                canvas[i][L - 2] = GOAL;
                break;
            }
        }
    }
    if (c == 2)
        selectMaze();
    //saveMaze();
    HideCursor();
    
    system("cls");
    startTime = clock(); //time
}
void show(int c)
{
    gotoxy(0, 0);
    if (c == 1)
    {
        int t = clock();
        printf("Time: %2d\tScore: %d\n", 30 - (t - startTime) / CLOCKS_PER_SEC, score);
    }
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < L; j++)
        {
            if(i == 0 && j > 0 && j < L - 1)
                printf("%d", (j)%10);
            else if(i > 0 && j == 0 && i < L - 1)
                printf("%d", (i)%10);
            else if (canvas[i][j] == WALL)
                printf("#");            //walls
            else if (i == positionY && j == positionX)
                printf("*");            //mouse
            else if (canvas[i][j] == GOAL)
                printf("@");            //goal
            else
                printf(" ");            //route
        }
        printf("\n");
    }
    printf("w,a,s,d to move,q to quit.\n");
    if(c == 1)
        printf("e to save maze\n");
    else
        printf("j->wall, k->road, l->goal\n");
    Sleep(100);
}
void updateWithoutInput()
{
    int t = clock();
    if (canvas[positionY][positionX] == GOAL)
    {
        win = 1;
        score++;
        end = 0;
    }
    if ((30 - (t - startTime) / CLOCKS_PER_SEC) == 0)
    {
        win = 0;
        end = 0;
    }
}
int updateWithInput(int c)
{
    char input;
    char filename[10] = "maze0.txt";
    filename[10] = '\0';
    if (kbhit())
    {
        input = getch();
        if(c == 1)
        {
            if (input == 's' && canvas[positionY + 1][positionX] != WALL)
                positionY +=1;
            if (input == 'w' && canvas[positionY - 1][positionX] != WALL)
                positionY -= 1;
            if (input == 'd' && canvas[positionY][positionX + 1] != WALL)
                positionX += 1;
            if (input == 'a' && canvas[positionY][positionX - 1] != WALL)
                positionX -= 1;
        }
        if (c != 1)
        {
            if (input == 's')
                positionY += 1;
            if (input == 'w')
                positionY -= 1;
            if (input == 'd')
                positionX += 1;
            if (input == 'a')
                positionX -= 1;
            if (input == 'j')//wall
                canvas[positionY][positionX] = WALL;
            if (input == 'k')//road
                canvas[positionY][positionX] = ROAD;
            if (input == 'l')//goal
                canvas[positionY][positionX] = GOAL;
        }
        if (input == 'e')
            saveMaze(filename);
        if (input == 'q')
            end = 0;
    }
}
void CreateMaze(int x, int y)
{
    int i, j;
    int direction[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 4; i++)
    {
        int r = rand() % 4;
        int temp = direction[0][0];
        direction[0][0] = direction[r][0];
        direction[r][0] = temp;

        temp = direction[0][1];
        direction[0][1] = direction[r][1];
        direction[r][1] = temp;
    }

    //向四个方向开挖
    for (int i = 0; i < 4; i++)
    {
        int dx = x;
        int dy = y;

        int range = 1;
        while (range > 0)
        {
            dx += direction[i][0];
            dy += direction[i][1];

            //排除掉回头路
            if (canvas[dx][dy] == ROAD)
            {
                break;
            }

            //判断是否挖穿路径
            int count = 0;
            for (int j = dx - 1; j < dx + 2; j++)
            {
                for (int k = dy - 1; k < dy + 2; k++)
                {
                    //abs(j - dx) + abs(k - dy) == 1 确保只判断九宫格的四个特定位置
                    if (abs(j - dx) + abs(k - dy) == 1 && canvas[j][k] == ROAD)
                    {
                        count++;
                    }
                }
            }

            if (count > 1) //挖之前要确保上下左右四个位置中只有一个位置是路
            {
                break;
            }

            //确保不会挖穿时，前进
            --range;
            canvas[dx][dy] = ROAD;
        }

        //没有挖穿危险，以此为节点递归
        if (range <= 0)
        {
            CreateMaze(dx, dy);
        }
    }
}
void selectMaze()
{
    char temp[15];
    printf("enter file name:");
    scanf("%s", temp);
    loadMaze(temp);  
}

void saveMaze(char *ch)
{
    FILE *fp;
    int temp[L*L];
    int t = 0;
    for (int i = 0; i < L; i++)
        for (int j = 0; j < L; j++)
            temp[t++] = canvas[i][j];
    fp = fopen(ch, "w");
    for (int i = 0; i < L*L; i++)
        fputc(temp[i], fp);
    fclose(fp);
}

void loadMaze(char * ch)
{
    FILE *fp;
    fp = fopen(ch, "r");
    for (int i = 0; i < L; i++)
        for (int j = 0; j < L; j++)
            canvas[i][j] = fgetc(fp);
    fclose(fp);
}

void editMaze()
{
    char filename[15], filename1[15];
    end = 1;
    printf("enter the name of maze you want to edit:\n");
    printf("you saved in the game, named \"maze0.txt\"\n");
    scanf("%s", filename);
    loadMaze(filename);
    positionX = 1;
    positionY = 2;
    HideCursor();
    system("cls");
    while(end)
    {
        show(2);
        updateWithInput(2);
    }
    printf("enter the name of maze you want to save:\n");
    scanf("%s", filename1);
    saveMaze(filename1);
}

void findRoute()
{
    int i;
    char filename[15];
    printf("enter the name of maze:");
    scanf("%s", filename);
    loadMaze(filename);
    Stack *top, *temp;
    top = (Stack *)malloc(sizeof(Stack));
    top->head = (Node *)malloc(sizeof(Node));
    top->head->next = NULL;
    
    temp = (Stack *)malloc(sizeof(Stack));
    temp->head = (Node *)malloc(sizeof(Node));
    temp->head->next = NULL;

    DFS(canvas, 1, 2, top);
    system("cls");
    printf("(1, 2)\n");
    
    //int PATH[L*L] = {0};
    int count = 0;

    while (top->head->next != NULL)
    {
        push(temp, top->head->x, top->head->y);
        top->head = top->head->next;
    }
    while (temp->head->next != NULL)
    {
        printf("(%d, %d)\n", temp->head->x, temp->head->y);
        temp->head = temp->head->next;
    }

    int c;
    printf("enter 1 to continue\n");
    scanf("%d", &c);
    while(c!=1)
    {
        printf("enter 1 to continue\n");
        scanf("%d", &c);
    }
}

void push(Stack *que, int x, int y)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->x = x;
    p->y = y;
    p->next = que->head;
    que->head = p;
}

void pop(Stack *que)
{
    //如果不合适将刚入栈的节点删除
    que->head = que->head->next;
}

void DFS(int a[L][L], int x, int y, Stack *que)
{
    if (x < L-1 && x >= 1 && y < L-1 && y >= 1)
    {
        if (a[y][x + 1] == 2)
            return;
        if (a[y][x + 1] == 1 && a[y + 1][x] == 1 && a[y][x - 1] == 1)
        {
            a[y][x] = 1;
            a[y - 1][x] = 0;
            pop(que);
            y--;
            DFS(a, x, y, que);
            return;
        }
        if (a[y][x + 1] == 1 && a[y + 1][x] == 1 && a[y - 1][x] == 1)
        {
            a[y][x] = 1;
            a[y][x - 1] = 0;
            pop(que);
            x--;
            DFS(a, x, y, que);
            return;
        }
        if (a[y][x + 1] == 1 && a[y - 1][x] == 1 && a[y][x - 1] == 1)
        {
            a[y][x] = 1;
            a[y + 1][x] = 0;
            pop(que);
            y++;
            DFS(a, x, y, que);
            return;
        }
        if (a[y - 1][x] == 1 && a[y + 1][x] == 1 && a[y][x - 1] == 1)
        {
            a[y][x] = 1;
            a[y][x + 1] = 0;
            pop(que);
            x++;
            DFS(a, x, y, que);
            return;
        }
        if (a[y][x + 1] == 0 && (x + 1) < L-1)
        {
            a[y][x + 1] = 3;
            x++;
            push(que, x, y);
            DFS(a, x, y, que);
            return;
        }
        if (a[y + 1][x] == 0 && (y + 1) < L-1)
        {
            a[y + 1][x] = 3;
            y++;
            push(que, x, y);
            DFS(a, x, y, que);
            return;
        }
        if (a[y - 1][x] == 0 && y - 1 >= 2)
        {
            a[y - 1][x] = 3;
            y--;
            push(que, x, y);
            DFS(a, x, y, que);
            return;
        }
        if (a[y][x - 1] == 0 && x - 1 >= 2)
        {
            a[y][x - 1] = 3;
            x--;
            push(que, x, y);
            DFS(a, x, y, que);
            return;
        }
    }
}

void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}