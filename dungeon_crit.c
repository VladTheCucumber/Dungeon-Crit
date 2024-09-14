#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <locale.h>

#define width 140
#define height 16
#define framerate 0

#pragma region GAME

int jmpdef = 5;

short pp[2] = { 2, 2 };

FILE *fptr;

void _log(const char info[100])
{
    fprintf(fptr, "Some text");
}

// Close the file
fclose(fptr);

typedef struct camera
{
    int l, r;
    int t, b;
    int m[2];
    int w, h;
} Camera;


int camspeed = 1;

void cgoto(short x, short y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
}

char map[height][width] = 
{
    "############################################################################",
    "#                                                                          #",
    "#                    $                                                     #",
    "#               #####$                                                     #",
    "#                    $                   *                                 #",
    "#                    $      ##      ##########                             #",
    "###########          $                                                     #",
    "#          $         $                                         *           #",
    "#          $############                              ####     ###         #",
    "#          $                                                              *#",
    "#          $                                                               #",
    "#          $                                                               #",
    "#          $                                                               #",
    "#          $                                                             *+#",
    "#############################                                         ######"
};

struct camera cam;

void initializeCamera(void)
{
    cam.l = 0;
    cam.r = 20;
    cam.t = 0;
    cam.b = height;
    cam.h = height;
    cam.w = 50;
}

void updatemiddle(Camera* cam)
{
    (*cam).m[0] = pp[0];
    (*cam).m[1] = pp[1];
}

void updatecamera(Camera* cam)
{
    (*cam).l = (*cam).m[0] - (*cam).w / 2;
    (*cam).r = (*cam).m[0] + (*cam).w / 2;
}

float pspeed = 1;

char screen[height][width];

char isdisplay(Camera cam, short p[2])
{
    return p[0] >= cam.l && p[0] <= cam.r &&
           p[1] >= cam.t && p[1] <= cam.b;
}

void showmap(void)
{
    for (int y = 0; y < height; y++)
    {
        cgoto(0, y);

        for (int x = 0; x < width; x++)
        {
            short p[2] = { x, y };
            if (isdisplay(cam, p)) printf("%c", screen[y][x]);
        }
    }
}

int jmp = 10;
short ijump = 0;

char jumpitr(void)
{
    if (jmp <= 0)
    {
        ijump = 0;
        return 0;
    }

    pp[1] -= 1;

    jmp--;

    if (map[pp[1]][pp[0]] == '#')
    {
        pp[1] += 1;
        ijump = 0;
        jmp = -10;
    }

    return 1;    
}

void jump(void)
{
    if (pp[1] - 1 > height)
        return;

    if (map[pp[1] + 1][pp[0]] != '#')
    {
        _log("cant jump because you are flying now!1");
        return;
    }

    jmp = jmpdef;
    ijump = 1;
}

void clearscreen(void)
{
    for (int y = 0; y < height; y++)
    {
        cgoto(0, y);

        for (int x = 0; x < width; x++)
        {
            screen[y][x] = map[y][x];
        }
    }
}

void playerphysics(void)
{
    if (map[pp[1] + 1][pp[0]] != '#')
    {
        pp[1] += 1;
    }
}

void putplayer(void)
{
    screen[pp[1]][pp[0]] = '@';
}

void moveleft(void)
{
    pp[0] -= pspeed;

    if (screen[pp[1]][pp[0]] == '#')
    {
        pp[0] += pspeed;
    }
}

void mcamleft(Camera* cam)
{
    (*cam).l -= camspeed;
    (*cam).r -= camspeed;

    if ((*cam).l < 0 || (*cam).r > width) 
    {
        (*cam).l += camspeed;
        (*cam).r += camspeed;
    }
}

void mcamright(Camera* cam)
{
    (*cam).l += camspeed;
    (*cam).r += camspeed;
    
    if ((*cam).l < 0 || (*cam).r > width) 
    {
        (*cam).l -= camspeed;
        (*cam).r -= camspeed;
    }
}

void moveright(void)
{
    pp[0] += pspeed;

    if (screen[pp[1]][pp[0]] == '#')
    {
        pp[0] -= pspeed;
    }
}

char movestairs(void)
{
    if (!(map[pp[1]][pp[0]] == '$')) return 1;

    if (GetKeyState('W') & 0x8000)
    {
        pp[1] -= pspeed;
    }

    if (GetKeyState('S') & 0x8000)
    {
        pp[1] += pspeed;

        if (map[pp[1]][pp[0]] == '#')
        {
            pp[1] -= pspeed;
        }
    }

    return 0;
}

int level = 1;

int CreateLevel(int level)
{
    if (level == 1)
    {
        char ar[height][width] = 
        {
            "############################################################################",
            "#                                                                          #",
            "#                    $                                                     #",
            "#               #####$                                                     #",
            "#                    $                   *                                 #",
            "#                    $      ##      ##########                             #",
            "###########          $                                                     #",
            "#          $         $                                         *           #",
            "#          $############                              ####     ###         #",
            "#          $                                                              *#",
            "#          $                                                               #",
            "#          $                                                               #",
            "#          $                                                               #",
            "#          $                                                             *+#",
            "#############################                                         ######"
        };

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                map[y][x] = ar[y][x];
            }
        }

        pp[0] = 2;
        pp[1] = 2;
    }

    else if (level == 2)
    {
        char ar[height][width] = 
        {
            "############################################################################",
            "#                                                                          #",
            "#                         $##                                              #",
            "#                         $                        #                       #",
            "#                                       #####                              #",
            "#                                       #####   ##    ###                  #",
            "###########         $########      #    #####                              #",
            "#                    ########           #####              ###             #",
            "#                    ########           #####                              #",
            "#                    ########           #####                              #",
            "#                    ########           #####                              #",
            "#                    ########           #####                            ++#",
            "#                    ########           #####                            ++#",
            "#                    ########           #####                            ++#",
            "##                   ########           #####                            ##"
        };

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                map[y][x] = ar[y][x];
            }
        }

        pp[0] = 2;
        pp[1] = 2;
    }

    initializeCamera();

    system("cls||clear");

    return 0;
}

#pragma endregion

int main(void)
{
    #pragma region INITIALIZING EVERYTHING
    char *locale;

    locale = setlocale(LC_ALL, "");
    fptr = fopen("filename.txt", "w");
    initializeCamera();

    CONSOLE_CURSOR_INFO info;
    info.bVisible = 0;
    info.dwSize = 10;
    SetConsoleCursorInfo(GetStdHandle( STD_OUTPUT_HANDLE ), &info);

    system("color 0F");

    char gravity = 0;

    #pragma endregion

    #pragma region Start Of Game

    while (1)
    {
        clearscreen();

        if (GetKeyState(VK_SPACE) & 0x8000 || GetKeyState('W') & 0x8000)
        {
            jump();
        }

        char isjump = jumpitr();

        gravity = movestairs();

        if (gravity && !isjump) playerphysics();

        if (GetKeyState('A') & 0x8000)
        {
            moveleft();
        }

        if (GetKeyState('D') & 0x8000)
        {
            moveright();
        }

        if (pp[1] > height || map[pp[1]][pp[0]] == '*')
        {
            CreateLevel(level);
        }

        if (map[pp[1]][pp[0]] == '+')
        {
            level += 1;
            CreateLevel(level);
        }

        updatemiddle(&cam);
        putplayer();
        updatecamera(&cam);
        showmap();

        Sleep(framerate);
    }

    #pragma endregion

    getchar();
    return 0;
}