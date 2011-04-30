#include "SDL.h"
#include "drawlib.h"

void die(const char *str, ...) {
	va_list argsp;
	va_start(argsp, str);
	vfprintf(stderr, str, argsp);
	va_end(argsp);
	exit(1);
}


/* assumes 32bit, 4 byte pixel */
void put_pixel(sketch *s, int x, int y, Uint32 pixel) {
  Uint32 *p = (Uint32 *)s->surface->pixels + y * ( s->surface->pitch / 4 ) + x;
  *p = pixel;
}

Uint32 *get_pixel(sketch *s, int x, int y) {
  return (Uint32 *)s->surface->pixels + y * ( s->surface->pitch / 4 ) + x;
}

/* maps a function over a surface, pixel by pixel */

void map_surface(sketch *s, void (*func)(SDL_Surface *s, int x, int y, Uint32 *pixel)) {
  Uint32 *p = (Uint32 *)s->surface->pixels;

  SDL_LockSurface(s->surface);
  for( int y = 0; y < s->height; y++ ) {
    for( int x = 0; x < s->width; x++ ) {
      func(s->surface, x, y, p);
      p++;
    }
  }
  SDL_UnlockSurface(s->surface);
}

linear_scale *createls(double max, double offset) {
  linear_scale *s;
  
  if( !(s = malloc(sizeof(linear_scale))) )
    die("fatal: could not malloc() %u bytes for linear_scale struct", sizeof(linear_scale));

  s->max = max;
  s->offset = offset;
  
  return s;
}

double applyls(linear_scale *scale, double value) {
  return (value / scale->max) + scale->offset;
}

void destroyls(linear_scale *scale) {
  free(scale);
}

sketch *createsk() {
  sketch *s = malloc(sizeof(sketch));

  s->width = 800;
  s->height = 600;
  
  s->running = 0;

  s->setup = NULL;
  s->draw = NULL;
  s->teardown = NULL;
  
  s->mouse_button_down = NULL;
  s->mouse_button_up = NULL;
  s->mouse_motion = NULL;
  s->key_down = NULL;
  s->key_up = NULL;

  s->surface = NULL;
  s->xscale = NULL;
  s->yscale = NULL;
  
  s->data = NULL;

  return s;
}

int initsk(sketch *s) {
  if ( ( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_AUDIO ) == -1 ) ) { 
    fprintf(stderr, "Could not initialize SDL: %s.\n", SDL_GetError() );
    return 0;
  }

  s->surface = SDL_SetVideoMode(s->width, s->height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
  s->xscale = createls(s->width,0);
  s->yscale = createls(s->height,0);

  if ( s->surface == NULL ) {
    fprintf(stderr, "Unable to get SDL surface.: %s\n", SDL_GetError());
    return 0;
  } else {
    printf("Created SDL Surface %dx%d with %d bits-per-pixel\n",
	   s->width, s->height,
	   s->surface->format->BitsPerPixel);
  }
  
  if ( s->setup != NULL ) {
    s->setup(s);
  }
  return 1;
}


int sk_default_keydown(sketch *s,SDL_keysym k) {
  if ( k.sym == SDLK_q ) {
    s->running = 0;
  }
  return 1;
}

int sk_default_keyup(sketch *s,SDL_keysym k) {
  return 1; 
}

void runsk(sketch *s) {
  s->running = 1;

  while( s->running == 1 ) {
    
    if ( s->draw != NULL ) {
      s->draw(s);
      SDL_Flip(s->surface);
    }

    SDL_Event event;
    while( s->running == 1 && SDL_PollEvent( &event ) ) {
      int handled = 0;
      
      switch( event.type ){
	
      case SDL_KEYDOWN:
	if ( s->key_down != NULL ) {
	  handled = s->key_down(s,event.key.keysym);
	}
	if ( handled == 0 ) {
	  handled = sk_default_keydown(s,event.key.keysym);
	}
	break;
	
      case SDL_KEYUP:
	if ( s->key_up != NULL ) {
	  handled = s->key_up(s,event.key.keysym);
	}
	if ( handled == 0 ) {
	  handled = sk_default_keyup(s,event.key.keysym);
	}
	break;

      case SDL_MOUSEMOTION:
	if ( s->mouse_motion != NULL ) {
	  handled = s->mouse_motion(s, event.motion.x,
				   event.motion.y,
				   event.motion.xrel,
				   event.motion.yrel
				   );
	}
	break;

      case SDL_MOUSEBUTTONDOWN:
	if ( s->mouse_button_down != NULL ) {
	  handled = s->mouse_button_down(s, event.button.button,
				       event.button.x,
				       event.button.y );
	}
	break;

      case SDL_MOUSEBUTTONUP:
	if ( s->mouse_button_up != NULL ) {
	  handled = s->mouse_button_up(s, event.button.button,
				     event.button.x,
				     event.button.y );
	}
	break;


      case SDL_VIDEOEXPOSE:
	handled = 1;
	if ( s->draw != NULL ) {
	  s->draw(s);
	  SDL_Flip(s->surface);
	}
	break;
	
      case SDL_QUIT:
	handled = 1;
	s->running = 0;
	break;
	
      default:
	break;
      }
    }
  }
}

void destroysk(sketch *s) {
  if ( s->xscale != NULL ) {
    destroyls(s->xscale);
    s->xscale = NULL;
  }

  if ( s->yscale != NULL ) {
    destroyls(s->yscale);
    s->yscale = NULL;
  }

  SDL_Quit();
}
