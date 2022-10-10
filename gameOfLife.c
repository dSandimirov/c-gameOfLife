#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // для рандома
#include <unistd.h>  // для интеракива
#include <ncurses.h>  // для интерактива
#define height 25
#define width 80

void game();
void draw(int map[height][width]);
int rules(int map[height][width], int i, int j, int mapcopy[height][width]);
// забирает генерацию из папки resourses
void creategenerate(int map[height][width], int mapcopy[height][width]);
void get(int * flag);  // возвращает было ли что-либо введено

int main() {
    game();
    return 0;
}

void game() {
    int map[height][width];
    int mapcopy[height][width];
    creategenerate(map, mapcopy);
    initscr();  // инициализация нкерса
    cbreak();   // чтобы не переводить на новую строку
    noecho();  // чтобы не отображался ввод
    nodelay(stdscr, TRUE);  // нельзя написать больше одного символа
    scrollok(stdscr, TRUE);  // нельзя перемещаться на колесико мыши
    draw(map);
    int speed = 250000;
    int flag;
    sleep(1);
    int p = 0;
    long t = clock();  // текущее время в программе
    // вычисление с момента запуска программы до текущего
    while ((double)(clock() - t) / CLOCKS_PER_SEC <= 0.15) {
        get(&flag);
        if (flag == 1) {
        char c = getch();
        if (c == 'e')
            break;
        if (c == 's')
            speed += 10000;
        if (c == 'w' && speed != 10000)
            speed -= 10000;
        else if (c == 'w' && speed <= 10000 && speed != 1000)
            speed -= 1000;
        c = ' ';
        }
        if (p % 2 == 0) {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    rules(mapcopy, i, j, map);
            draw(map);
        }
        if (p % 2 == 1) {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    rules(map, i, j, mapcopy);
            draw(mapcopy);
        }
        usleep(speed);
        p++;
    }
}

void creategenerate(int map[height][width], int mapcopy[height][width]) {
    int helpertoread[height][width];
    srand(time(NULL));
    int filerand = rand() % 5;
    FILE *f;
    switch (filerand) {
        case 0:
            f = fopen("resourses/1.txt", "r"); break;
        case 1:
            f = fopen("resourses/2.txt", "r"); break;
        case 2:
            f = fopen("resourses/3.txt", "r"); break;
        case 3:
            f = fopen("resourses/4.txt", "r"); break;
        case 4:
            f = fopen("resourses/5.txt", "r"); break;
    }
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            fscanf(f, "%i", &helpertoread[i][j]);
            map[i][j] = helpertoread[i][j];
            mapcopy[i][j] = helpertoread[i][j];
        }
}

void draw(int map[height][width]) {
    clear();
    for (int i = 0; i < height ; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] == 1)
                printw("*");
            else
                printw(" ");
        }
        printw("\n");
    }
}

int rules(int map[height][width], int i, int j, int mapcopy[height][width]) {
    int counter = 0;
    for (int k = i - 1; k < i + 2; k++)
        for (int n = j - 1; n < j + 2; n++)
            counter = counter + map[k][n];
    counter = counter - map[i][j];
    if (counter == 3 && map[i][j] == 0) {
        mapcopy[i][j] = 1;
    } else if (((counter == 2 || counter == 3) && map[i][j] == 1)) {
        mapcopy[i][j] = 1;
    } else {
        mapcopy[i][j] = 0;
    }
    return mapcopy[i][j];
}

void get(int * flag) {
    char c;
    if ((c = getch()) != ERR) {  // ошибка если ничего не вводишь (ERR)
        ungetch(c);  // чтобы буфер не был занят
        * flag = 1;
        return;
    }
    * flag = 0;
    return;
}

