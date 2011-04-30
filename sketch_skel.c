/* Public Domain
 * Craig Brozefsky <craig@red-bean.com>
 */

#include "SDL.h"
#include "drawlib.h"
#include <stdio.h>

sketch *sk = NULL;

void myexit(void) {
  if (sk != NULL) { destroysk(sk); }
}

void paint_it(SDL_Surface *s, int x, int y, Uint32 *p) {
  *p  = SDL_MapRGB(s->format, 0x0,
			    applyls(sk->xscale, (double)x) * (double)0xff,
			    applyls(sk->yscale, (double)y) * (double)0xff);
}

void setup(sketch *s) {
  map_surface(s, &paint_it);
}

void increment_it(SDL_Surface *s, int x, int y, Uint32 *p) {
  Uint8 r, g, b, a;
  SDL_GetRGBA(*p, s->format, &r, &g, &b, &a);
  r++; g++; b++;
  if ( r > 0xff ) { r = 0; }
  if ( g > 0xff ) { g = 0; }
  if ( b > 0xff ) { b = 0; }

  *p = SDL_MapRGB(s->format, r, g, b);
}

void draw(sketch *s) {
  map_surface(s, &increment_it);
}


int main(int argc, char *argv[]) {
  sk = createsk();
  sk->setup = &setup;
  sk->draw = &draw;
  atexit(myexit);
  if (!initsk(sk))
    exit(-1);
  runsk(sk);
  exit(0);
}



 
