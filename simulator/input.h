#ifndef __INPUT_H
#define __INPUT_H

/* X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* Normal C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

// simulation headers
#include "render.h"

// device headers
#include "../device/jelly.h"

#define PROXIMITY_WINDOW_WIDTH 50
#define PROXIMITY_WINDOW_HEIGHT 100

struct Mouse {
  int x_pos, y_pos;
  bool pressed;
};

extern struct Mouse *mouse_pos;

void i_init_input(void);
void i_process_input(void);

#endif
