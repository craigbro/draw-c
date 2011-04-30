/* assumes 32bit, 4 byte pixel */

void die(const char *str, ...);

typedef struct{
  double max;
  double offset;
} linear_scale;

linear_scale *createls(double max, double offset);
double applyls(linear_scale *scale, double value);
void destroyls(linear_scale *scale);


typedef struct{
  int pixel_width;
  int pixel_height;
  double width;
  double height;
} grid_transform;


typedef struct sketch {
  int width;
  int height;
  
  void (*setup)(struct sketch *s);
  void (*draw)(struct sketch *s);
  void (*teardown)(struct sketch *s);

  int (*mouse_button_down)(struct sketch *s, Uint8 button, Uint16 x, Uint16 y);
  int (*mouse_button_up)(struct sketch *s, Uint8 button, Uint16 x, Uint16 y);
  int (*mouse_motion)(struct sketch *s, Uint16 x, Uint16 y, Uint16 xrel, Uint16 yrel);
  int (*key_down)(struct sketch *s, SDL_keysym ks);
  int (*key_up)(struct sketch *s, SDL_keysym ks);

  /* private */
  SDL_Surface *surface;
  linear_scale *xscale;
  linear_scale *yscale;
  int running;

  void *data;
  
} sketch;


sketch *createsk();
int initsk(sketch *s);
void runsk(sketch *s);
void destroysk(sketch *s);


typedef struct
{
  Uint32 *c;
  Uint32 *nw;
  Uint32 *n;
  Uint32 *ne;
  Uint32 *e;
  Uint32 *se;
  Uint32 *s;
  Uint32 *sw;
  Uint32 *w;
} px_moore_hood;


void put_pixel(sketch *s, int x, int y, Uint32 pixel);
Uint32 *get_pixel(sketch *s, int x, int y);

void map_surface(sketch *s, void (*func)(SDL_Surface *s, int x, int y, Uint32 *pixel));
