#include <stdio.h>
#include "graphique.h"


char **mem_alloc(int H, int W)
{
   char **ptr = (char **)malloc(sizeof(char *) * H);
   ERRALLOC(ptr);
   for (int i = 0; i < H; i++) {
      ptr[i] = (char *)malloc(sizeof(char)*W + 1);
      ERRALLOC(ptr[i]);
   }
   return ptr;
}

void mem_free(char **ptr, int H)
{
   for (int i = 0; i < H; i++) {
      free(ptr[i]);
   }
   free(ptr);
}

void print_cons(char **pixels, unsigned int width, unsigned int height)
{
   unsigned int i = 0;
   unsigned int j = 0;
    for (i = 0; i < height; i++) {
	moveto(i, 0);
	printf("\x1B[2K");
	for (j = 0; j < width; j++) {
	    putchar(pixels[i][j]);
	    putchar(' ');
	}
    }
    moveto(0, 0);
}

void cons_clear(char **pixels, short width, short height, const char clear)
{
   short i, j;
   for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
         pixels[i][j] = clear;
      }
   }
}

// Fonction revérifié pas de point faible !
void cons_rect(char **pixels, short width, short height, int x, int y, int largeur, int hauteur, const char fd)
{
   RECT rect, wind;
   
   wind.A.x = -width/2;
   wind.A.y = -height/2;
   wind.B.x = width/2;
   wind.B.y = height/2;

   rect.A.x = x;
   rect.A.y = y;
   rect.B.x = x+largeur;
   rect.B.y = y+hauteur;

   short i, j;
   for (i = wind.A.y; i < wind.B.y; i++) {
       for (j = wind.A.x; j < wind.B.x; j++) {
	   if (((j >= rect.A.x) && (j < rect.B.x)) && ((i >= rect.A.y) && (i < rect.B.y)))
	       pixels[i+height/2][j+width/2] = fd;
       }
   }
}

void cons_ligne(char **pixels, short width, short height, int ax, int ay, int bx, int by, const char fd)
{
    COORDF a, b, AB;
    a.x = (float)ax/(float)(width/2);
    a.y = (float)ay/(float)(-height/2);
    b.x = (float)bx/(float)(width/2);
    b.y = (float)by/(float)(-height/2);
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;


    for (double t = 0; t < 1; t+=0.1) {
	float x = (AB.x*t + a.x);
	float y = (AB.y*t + a.y);
	if (ABS(x*(width/2)) > width/2 || ABS(y*(height/2)) > height/2) break;
	pixels[(int)(-y*(height/2)) + height/2][(int)(x*width/2) + width/2] = fd;
    }
}


void cons_cercle(char **pixels, short width, short height, int x, int y, int radius, const char fd)
{
    short i, j;
    for (i = (-width/2); i < (height/2); i++) {
	for (j = (-width/2); j < (width/2); j++) {
	    if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius) {
		pixels[i-(-height/2)][j-(-width/2)] = fd;
	    }
	}
    }
}

#ifdef TODO
void cons_rotate(char **pixels, short width, short height, float tetha)
{
   RECT wind;
}
#endif
// Fonction revérifié pas de point faible !
void cons_triangle(char **pixels, short width, short height, int ax, int ay, int bx, int by, int cx, int cy, const char fd)
{
   RECT wind;
   wind.A.x = -width/2;
   wind.A.y = -height/2;
   wind.B.x = width/2;
   wind.B.y = height/2;

   COORDF a, b, c;
   a.x = ax;
   a.y = ay;
   b.x = bx;
   b.y = by;
   c.x = cx;
   c.y = cy;
   
   float gA, gB, gC;

   short x, y;
   for (y = wind.A.y; y < wind.B.y; y++) {
       for (x = wind.A.x; x < wind.B.x; x++) {
	   gA = ((float)x*(b.y - c.y) + b.x*(c.y - (float)y) + c.x*((float)y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	   gB = (a.x*((float)y - c.y) + (float)x*(c.y - a.y) + c.x*(a.y - (float)y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
	   gC = (a.x*(b.y - (float)y) + b.x*((float)y - a.y) + (float)x*(a.y - b.y))/(a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));

	   if(gA >= 0 && gB >= 0 && gC >= 0)
	       pixels[y+height/2][x+width/2] = fd;	   
       }
   }
}


void draw_clear(uint8_t ***pixels, short width, short height)
{
   short i, j;
   for (i = 0; i < height; ++i) {
      for (j = 0; j < width; j++) {
	  pixels[i][j][0] = 0x00;
	  pixels[i][j][1] = 0x00;
	  pixels[i][j][2] = 0x00;
      }
   }
}

void draw_ligne(uint8_t ***pixels, short width, short height, int ax, int ay, int bx, int by, int ep, const uint32_t fd)
{
    RECT wind;
    wind.A.x = -width/2;
    wind.A.y = -height/2;
    wind.B.x = width/2;
    wind.B.y = height/2;

    COORDF a, b, AB;
    a.x = ax;
    a.y = ay;
    b.x = bx;
    b.y = by;
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;

    short x, y;
    for (y = wind.A.y; y < wind.B.y; y++) {
	if (y < min(ay, by) || y > max(ay, by)) continue;
	for (x = wind.A.x; x < wind.B.x; x++) {
	    if (x < min(ax, bx) || x > max(ax, bx)) continue;
	    if ((AB.x*(y - a.y) - AB.y*(x - a.x) > -ep) && (AB.x*(y - a.y) - AB.y*(x - a.x) < ep)) {
		pixels[y-wind.A.y][x-wind.A.x][0] = fd>>(8*3);
		pixels[y-wind.A.y][x-wind.A.x][1] = fd>>(8*2);
		pixels[y-wind.A.y][x-wind.A.x][2] = fd>>(8*1);
	    }
	}
    }
}

void draw_cercle(uint8_t ***pixels, short width, short height, int x, int y, int radius, const uint32_t fd)
{
   RECT wind;

   wind.A.x = -width/2;
   wind.A.y = -height/2;
   wind.B.x = width/2;
   wind.B.y = height/2;

   short i, j;
   for (i = wind.A.y; i < wind.B.y; i++) {
      for (j = wind.A.x; j < wind.B.x; j++) {
	  if ((i-y)*(i-y) + (j-x)*(j-x) <= radius*radius) {
	      pixels[i-wind.A.y][j-wind.A.x][0] |= fd>>(8*3);
	      pixels[i-wind.A.y][j-wind.A.x][1] |= fd>>(8*2);
	      pixels[i-wind.A.y][j-wind.A.x][2] |= fd>>(8*1);
	  }
      }
   }
}
