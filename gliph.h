/* To use it:
 *
 *     #define GLIPH_IMPLEMENTATION
 *     #include "gliph.h"
 *
 * Basic usage:
 *
 *     #define GLIPH_IMPLEMENTATION
 *     #include "gliph.h"
 *
 *     #define HEIGHT (9*2)
 *     #define WIDTH (16*2)
 *
 *     int main()
 *     {
 *         GLIPH_INIT(console, WIDTH, HEIGHT);
 *         ConsoleClear(console, ' ');
 *         PrintRectangle(console, 0, 0, WIDTH, HEIGHT, '#');
 *         PrintRectangle(console, 1, 1, WIDTH-2, HEIGHT-2, ' ');
 *         PrintCircle(console, WIDTH/2, HEIGHT/2, 5, '@');
 *         PrintConsole(console);
 *         return 0;
 *     }
 *
 * The result:
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 * #                                                             #
 * #                                                             #
 * #                                                             #
 * #                               @                             #
 * #                         @ @ @ @ @ @ @                       #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                     @ @ @ @ @ @ @ @ @ @ @                   #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                       @ @ @ @ @ @ @ @ @                     #
 * #                         @ @ @ @ @ @ @                       #
 * #                               @                             #
 * #                                                             #
 * #                                                             #
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 *
 *
 */

#ifndef GLIPH_H_INCLUED
#define GLIPH_H_INCLUED
#include <stdio.h>  // Used for: fprintf(), printf(), putchar()
#include <stdlib.h> // Used for: exit(), malloc(), free()
#include <string.h> // Used for: strerror()
#include <errno.h>  // Used for: errno
#include <stdint.h> // Used for: uint8_t, uint32_t
#include <unistd.h> // Used for: write()

// Regular color
#define BLACK    "\e[0;30m"
#define RED      "\e[0;31m"
#define GREEN    "\e[0;32m"
#define YELLOW   "\e[0;33m"
#define BLUE     "\e[0;34m"
#define PURPLE   "\e[0;35m"
#define CYAN     "\e[0;36m"
#define WHITE    "\e[0;37m"

// Bold
#define BOLD_BLACK    "\e[1;30m"
#define BOLD_RED      "\e[1;31m"
#define BOLD_GREEN    "\e[1;32m"
#define BOLD_YELLOW   "\e[1;33m"
#define BOLD_BLUE     "\e[1;34m"
#define BOLD_PURPLE   "\e[1;35m"
#define BOLD_CYAN     "\e[1;36m"
#define BOLD_WHITE    "\e[1;37m"


// Underline
#define UL_BLACK    "\e[4;30m"
#define UL_RED      "\e[4;31m"
#define UL_GREEN    "\e[4;32m"
#define UL_YELLOW   "\e[4;33m"
#define UL_BLUE     "\e[4;34m"
#define UL_PURPLE   "\e[4;35m"
#define UL_CYAN     "\e[4;36m"
#define UL_WHITE    "\e[4;37m"

// Background
#define BG_BLACK   "\e[40m"
#define BG_RED     "\e[41m"
#define BG_GREEN   "\e[42m"
#define BG_YELLOW  "\e[43m"
#define BG_BLUE    "\e[44m"
#define BG_PURPLE  "\e[45m"
#define BG_CYAN    "\e[46m"
#define BG_WHITE   "\e[47m"

// High Intensity
#define HI_BLACK  "\e[0;90m"
#define HI_RED    "\e[0;91m"
#define HI_GREEN  "\e[0;92m"
#define HI_YELLOW "\e[0;93m"
#define HI_BLUE   "\e[0;94m"
#define HI_PURPLE "\e[0;95m"
#define HI_CYAN   "\e[0;96m"
#define HI_WHITE  "\e[0;97m"

// Bold High Intensity
#define BOLD_HI_BLACK  "\e[1;90m"
#define BOLD_HI_RED    "\e[1;91m"
#define BOLD_HI_GREEN  "\e[1;92m"
#define BOLD_HI_YELLOW "\e[1;93m"
#define BOLD_HI_BLUE   "\e[1;94m"
#define BOLD_HI_PURPLE "\e[1;95m"
#define BOLD_HI_CYAN   "\e[1;96m"
#define BOLD_HI_WHITE  "\e[1;97m"

// High Intensity backgrounds
#define HI_BG_BLACK  "\e[0;100m"
#define HI_BG_RED    "\e[0;101m"
#define HI_BG_GREEN  "\e[0;102m"
#define HI_BG_YELLOW "\e[0;103m"
#define HI_BG_BLUE   "\e[0;104m"
#define HI_BG_PURPLE "\e[0;105m"
#define HI_BG_CYAN   "\e[0;106m"
#define HI_BG_WHITE  "\e[0;107m"

// Other style
#define BOLD           "\e[1m"
#define ITALIC         "\e[3m"
#define BOLD_ITALIC    "\e[3m\e[1"
#define UNDERLINE      "\e[4"
#define STRICKETHROUGH "\e[9m"

// Reset
#define RESET "\e[0m"

// Specific
// R is inclued [0, 5], G is inclued [0, 5], B is inclued [0, 5]
#define PRINT_BG_COLOR(R, G, B, c) printf("\e[48;5;%dm%c"RESET, R*36 + G*6 + B + 16, c)
#define PRINT_CHAR_COLOR(R, G, B, c) printf("\e[38;5;%dm%c"RESET, R*36 + G*6 + B + 16, c)
// Gray is inclued [0, 24]
#define PRINT_BG_GRAYSHADE(G, c) printf("\e[48;5;%dm%c"RESET, G+232, c)
#define PRINT_CHAR_GRAYSHADE(G, c) printf("\e[38;5;%dm%c"RESET, G+232, c)


// #define MAX_GLIPH(x, y) ((x)<(y) ? (y) : (x))
// #define MIN_GLIPH(x, y) ((x)>(y) ? (y) : (x))
// #define CEIL_GLIPH(x) ((x) >= 0.5f ? 1 : 0)
#ifndef STDOUT_FILENO
# define STDOUT_FILENO 1
#endif
#define ABS_GLIPH(x) ((x) < 0 ? (-x) : (x))
#define D2TOD1_GLIPH(x, y, W) (((y)*(W+1)) + (x))
#define MOVETO_GLIPH(y, x) printf("\033[%d;%dH", (y), (x))
#define MOVETO0_0_GLIPH  \
    if (write(STDOUT_FILENO, "\033[0;0H", 7) < 0) return -1
#define ARRAYCONVERTD1TOD2_GLIPH(T, W) ((char (*)[W+1])T)

// This pixels2d simplify the convertion only for this file
#ifdef pixels2d
# warning pixels2d has been undefined
# undef pixels2d
#endif
# define pixels2d ARRAYCONVERTD1TOD2_GLIPH(pixels._1d, pixels.w)

#define TESTALLOC(x)                                                           \
do                                                                             \
{                                                                              \
    if (x == NULL) {                                                           \
        fprintf(stderr, "ERROR: %s, ligne : %d\n", strerror(errno), __LINE__); \
        exit(EXIT_FAILURE);                                                    \
    }                                                                          \
} while(0)

/* DEPRECATED
# define GLIPH_ALLOC2D(type, name, W, H)                      \
    type **name = (type **)malloc(sizeof(type *) * ((H)+1));  \
    TESTMALLOC(name);                                         \
    do {                                                      \
        for (ssize_t i = 0; i < ((H)); i++) {                 \
            name[i] = (type *)malloc(sizeof(type)*((W) + 1)); \
            TESTMALLOC(name[i]);                              \
        }                                                     \
        name[(H)] = NULL;                                     \
    } while (0)
#define GLIPH_FREE2D(name, H)                                                        \
    if (name == NULL) fprintf(stdout, "WARNING: %s is NULL, %d\n", #name, __LINE__); \
    do {                                                                             \
        for (ssize_t i = 0; i < (H); i++) free(name[i]);                             \
        free(name);                                                                  \
    } while (0)
*/

typedef struct
{
    float x, y;
}COORDF;

typedef struct
{
    char *_1d;
    const short w;
    const short h;
} Screen;

#define GLIPH_ALLOC(name, W, H)                                        \
    char *__screen1d = (char *)malloc(sizeof(char) * (((W)+1)*(H)+1)); \
    TESTALLOC(__screen1d);                                             \
    Screen name = {._1d=__screen1d, .w=(W), .h=(H)};                   \
    do {                                                               \
        for (int i = 0; i < (H); i++) {                                \
            ((char (*)[(W)+1])name._1d)[i][(W)] = '\n';                \
        }                                                              \
    } while (0)
#define GLIPH_FREE(name) free(name._1d)

#define GLIPH_INIT(name, W, H)                          \
    char __screen1d[(H)*((W)+1)+1];                     \
    Screen name = {._1d=__screen1d, .w=(W), .h=(H)};    \
    do {                                                \
        for (int i = 0; i < (H); i++) {                 \
            ((char (*)[(W)+1])name._1d)[i][(W)] = '\n'; \
        }                                               \
    } while (0)


void ConsoleClear(Screen pixels, const char clear);

void PrintRectangle(Screen pixels, short x, short y, short width, short height, const char fd);
void PrintCircle(Screen pixels, short x, short y, short radius, const char fd);
void PrintLine(Screen pixels, short ax, short ay, short bx, short by, const char fd);
void PrintTriangle(Screen pixels, short ax, short ay, short bx, short by, short cx, short cy, const char fd);

int PrintConsole(Screen pixels);

void ClearDrawing(uint8_t ***pixels, short width, short height, const uint32_t fd);
void DrawLine(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, const uint32_t fd);
void DrawCircle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd);


# ifdef GLIPH_IMPLEMENTATION

#  if defined(GLIPH_SPACEPADDED)
// To compensate the width/height ratio this print is padded with a ' ' char
int PrintConsole(Screen pixels)
{
    MOVETO_GLIPH(0, 0);
    unsigned short i, j;
    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
	    putchar(pixels2d[i][j]);
	    putchar(' ');
	}
	putchar('\n');
    }
    return 0;
}

#  elif defined(GLIPH_CHARPADDED)
// To compensate the width/height ratio this print double the same char printed
int PrintConsole(Screen pixels)
{
    MOVETO_GLIPH(0, 0);
    unsigned short i, j;
    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
	    putchar(pixels2d[i][j]);
	    putchar(pixels2d[i][j]);
	}
	putchar('\n');
    }
    return 0;
}

#  else
// This is raw printing, no compensation.
int PrintConsole(Screen pixels)
{
    MOVETO_GLIPH(0, 0);
    // MOVETO0_0_GLIPH;
    unsigned short i, j;
    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
	    putchar(pixels2d[i][j]);
	}
	putchar('\n');
    }
    return 0;
    // return write(STDOUT_FILENO, pixels._1d, (pixels.w+1)*pixels.h);
}
#  endif

void ConsoleClear(Screen pixels, const char clear)
{
    short i, j;
    for (i = 0; i < pixels.h; ++i) {
	for (j = 0; j < pixels.w; j++) {
            pixels2d[i][j] = clear;
	}
    }
}

void PrintRectangle(Screen pixels, short x, short y, short width, short height, const char fd)
{
    short i, j;
#ifdef GLIPH_PADDING
    // Problem with the bounderies
    x -= pixels.w/4;
    x *=2;
    width*=2;
#endif

    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
            if (((j >= x) && (j < x+width)) && ((i >= y) && (i < y+height)))
		pixels2d[i][j] = fd;
        }
    }
}

void PrintLine(Screen pixels, short ax, short ay, short bx, short by, const char fd)
{
    COORDF a, b, AB;
    const float midH = (float)pixels.h/2.f;
    const float midW = (float)pixels.w/2.f;
    a.x = (float)ax/(midW);
    a.y = (float)ay/(-midH);
    b.x = (float)bx/(midW);
    b.y = (float)by/(-midH);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;
/* TODO: Modify t step to verify this condition: (step >= 1/sqrt((by-ay)² + (bx-ax)²)) */
       // double dt = 1.f/sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay));
       // // double dt = 1.f/((ABS_GLIPH(bx-ax) + ABS_GLIPH(by-ay))*0.7);
       // for (double t = 0; t < 1; t+=dt) {

    float x, y;
    unsigned short Yaxe, Xaxe;
    for (float t = 0; t < 1; t+=0.01) {
#  ifdef GLIPH_PADDING
        x = (AB.x*t + a.x)*2;
#  else
        x = (AB.x*t + a.x);
#  endif
        y = (AB.y*t + a.y);
        if ((ABS_GLIPH(x*midW)) > midW-1 || (ABS_GLIPH(y*midH)) > midH-1) break;
	Yaxe = (midH)*(1 - y);
	Xaxe = (midW)*(1 + x);
	pixels2d[Yaxe][Xaxe] = fd;
    }
}

void PrintCircle(Screen pixels, short x, short y, short radius, const char fd)
{
    short i, j;
    for (i = 0; i < pixels.h; i++) {
	for (j = 0; j < pixels.w; j++) {
#  ifdef GLIPH_PADDING
	    if ((i-y)*(i-y) + (j-x*2+pixels.w/2)*(j-x*2+pixels.w/2)/4 <= radius*radius)
#  else
	    if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius)
#  endif
		pixels2d[i][j] = fd;
	}
    }
}

// #include <math.h>
// void PrintDisk(char *pixels, short width, short height, int x, int y, int radius, const char fd)
// {
//     /* O(n) avec n = 8*radius */
//     for (double t = 0; t <= 2.f*M_PI; t+=M_PI/(double)(4*radius)) {
// 	int XX = (int)((double)radius*cos(t)) + x;
// 	int YY = (int)((double)radius*sin(t)) + y;

// 	if (XX < 0 || XX > width-1 || YY < 0 || YY > height-1) continue;
// 	pixels[YY][XX] = fd;
//     }
// }


void PrintTriangle(Screen pixels, short ax, short ay, short bx, short by, short cx, short cy, const char fd)
{
    COORDF a, b, c;
    a.x = ax;
    a.y = ay;
    b.x = bx;
    b.y = by;
    c.x = cx;
    c.y = cy;
#  ifdef GLIPH_PADDING
    a.x -= pixels.w/4;
    a.x *= 2;
    b.x -= pixels.w/4;
    b.x *= 2;
    c.x -= pixels.w/4;
    c.x *= 2;
#  endif

    float gA, gB, gC;

    short x, y;
    for (y = 0; y < pixels.h; y++) {
	for (x = 0; x < pixels.w; x++) {
	    gA = ((float)x*(b.y - c.y) + b.x*(c.y - (float)y) + c.x*((float)y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	    gB = (a.x*((float)y - c.y) + (float)x*(c.y - a.y) + c.x*(a.y - (float)y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	    gC = (a.x*(b.y - (float)y) + b.x*((float)y - a.y) + (float)x*(a.y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));

            if(gA >= 0 && gB >= 0 && gC >= 0)
                pixels2d[y][x] = fd;
	}
    }
}

#ifdef TODO
void RotateConsole(Screen pixels, short height, float tetha)
{
   RECT wind;
}
#endif

void ClearDrawing(uint8_t ***pixels, short width, short height, const uint32_t fd)
{
   short i, j;
   for (i = 0; i < height; ++i) {
      for (j = 0; j < width; j++) {
          pixels[i][j][0] = fd;
          pixels[i][j][1] = fd;
          pixels[i][j][2] = fd;
      }
   }
}

void DrawLine(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, const uint32_t fd)
{
    COORDF a, b, AB;
    const int midH = height/2;
    const int midW = width/2;
    a.x = (float)ax/(float)(midW);
    a.y = (float)ay/(float)(-midH);
    b.x = (float)bx/(float)(midW);
    b.y = (float)by/(float)(-midH);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;
    // TODO: Modify t step with 3 if statement if (sqrt(h² + w²) > 1000) t+=0.0001 else reduce

    for (double t = 0; t < 1; t+=0.01) {
	double x = (AB.x*t + a.x);
	double y = (AB.y*t + a.y);
        if (ABS_GLIPH(x*(midW)) > midW-1 || ABS_GLIPH(y*(midH)) > midH-1) break;
        pixels[(int)(midH*(1 - y))][(int)(midW*(1 + x))][0] = fd>>(24); // 8*3
        pixels[(int)(midH*(1 - y))][(int)(midW*(1 + x))][1] = fd>>(16); // 8*2
        pixels[(int)(midH*(1 - y))][(int)(midW*(1 + x))][2] = fd>>(8);// 8*1
    }
}

void DrawCircle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd)
{
    short i, j;
    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
            if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius) {
		pixels[i][j][0] |= fd>>(24); // 8*3
		pixels[i][j][1] |= fd>>(16); // 8*2
		pixels[i][j][2] |= fd>>(8); // 8*1
            }
	}
    }
}

#  ifdef pixels2d
#   undef pixels2d
#  endif

# endif // GLIPH_IMPLEMENTATION
#endif // GLIPH_H_INCLUED

/***********************************
TODO:
- in PrintLine() and DrawLine():
    - Modify t step to verify this condition: (step >= 1/sqrt((by-ay)² + (bx-ax)²))
- Add DrawRectangle()
- Line: 230:
    Change PrintDisk name by PrintCercle and vice vera
 **********************************/
