/*
        charlcd.c

        Copyright Luki <humbell@ethz.ch>
        Copyright 2011 Michel Pollet <buserror@gmail.com>

        This file is part of simavr.

        simavr is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        simavr is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with simavr.  If not, see <http://www.gnu.org/licenses/>.


        Modified by EETagent
 */

#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avr_ioport.h"
#include "sim_avr.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_hex.h"
#include "sim_vcd_file.h"

#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <pthread.h>

#include "hd44780_glut.h"

// float pixsize = 16;
int window;

avr_t *avr = NULL;
avr_vcd_t vcd_file;
hd44780_t hd44780;

int color = 0;
uint32_t colors[][4] = {
    {0x00aa00ff, 0x00cc00ff, 0x000000ff, 0x00000055}, // fluo green
    {0xaa0000ff, 0xcc0000ff, 0x000000ff, 0x00000055}, // red
};

static void *avr_run_thread(void *ignore) {
  while (1) {
    avr_run(avr);
  }
  return NULL;
}

void keyCB(unsigned char key, int x, int y) /* called on key press */
{
  switch (key) {
  case 'q':
    exit(0);
    break;
  }
}

void displayCB(void) /* function called whenever redisplay needed */
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); // Select modelview matrix
  glPushMatrix();
  glLoadIdentity(); // Start with an identity matrix
  glScalef(3, 3, 1);

  hd44780_gl_draw(&hd44780, colors[color][0], /* background */
                  colors[color][1],           /* character background */
                  colors[color][2],           /* text */
                  colors[color][3] /* shadow */);
  glPopMatrix();
  glutSwapBuffers();
}

// gl timer. if the lcd is dirty, refresh display
void timerCB(int i) {
  // static int oldstate = -1;
  //  restart timer
  glutTimerFunc(1000 / 64, timerCB, 0);
  glutPostRedisplay();
}

int initGL(int w, int h) {
  // Set up projection matrix
  glMatrixMode(GL_PROJECTION); // Select projection matrix
  glLoadIdentity();            // Start with an identity matrix
  glOrtho(0, w, 0, h, 0, 10);
  glScalef(1, -1, 1);
  glTranslatef(0, -1 * h, 0);

  glutDisplayFunc(displayCB); /* set window's display callback */
  glutKeyboardFunc(keyCB);    /* set window's key callback */
  glutTimerFunc(1000 / 24, timerCB, 0);

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  hd44780_gl_init();

  return 1;
}

int main(int argc, char *argv[]) {
  bool debug = false;
  const char *program = NULL;

  if (argc < 2) {
    printf(
        "No hex / elf provided. Please provide a filename as an argument.\n");
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    if ((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--debug") == 0)) {
      debug = true;
    } else if ((strcmp(argv[i], "-h") == 0) ||
               (strcmp(argv[i], "--help") == 0)) {
      printf("Usage: %s [-d] [-h] [filename]\n", argv[0]);
      return 0;
    } else {
      program = argv[i];
    }
  }

  FILE *file = fopen(program, "r");

  if (!file) {
    printf("File does not exist.\n");
    return 1;
  } else {
    fclose(file);
  }

  elf_firmware_t f = {{0}};

  // atmega328p setup
  strcpy(f.mmcu, "atmega328p");
  f.frequency = 8000000;

  sim_setup_firmware(program, AVR_SEGMENT_OFFSET_FLASH, &f, program);

  printf("firmware %s f=%d mmcu=%s\n", program, (int)f.frequency, f.mmcu);

  avr = avr_make_mcu_by_name(f.mmcu);
  if (!avr) {
    fprintf(stderr, "%s: AVR '%s' not known\n", argv[0], f.mmcu);
    exit(1);
  }

  avr_init(avr);
  avr_load_firmware(avr, &f);

  hd44780_init(avr, &hd44780, 16, 2);

  // ; Shield pinout:
  // ; UNO   AVR	    LCD
  // ; D4	PD4	    D4
  // ; D5	PD5	    D5
  // ; D6	PD6	    D6
  // ; D7	PD7	    D7
  // ; D8	PB0	    RS
  // ; D9	PB1	    E
  // ; D10	PB2	    Backlight
  // ; A0	ADC0	    Buttons

  /* Connect Data Lines to Port D, 4-7 */
  /* These are bidirectional too */
  for (int i = 0; i < 4; ++i) {
    avr_irq_t *iavr = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 4 + i);
    avr_irq_t *ilcd = hd44780.irq + IRQ_HD44780_D4 + i;
    // AVR -> LCD
    avr_connect_irq(iavr, ilcd);
    // LCD -> AVR
    avr_connect_irq(ilcd, iavr);
  }
  /* Port B, 0 */
  avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 0),
                  hd44780.irq + IRQ_HD44780_RS);
  /* Port B, 1 */
  avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 1),
                  hd44780.irq + IRQ_HD44780_E);
  /* Port B, 2 */
  // avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 2),
  //                 hd44780.irq + IRQ_HD44780_E);

  printf("HD44780 LCD Keypad Shield atmega328p emulátor\n");

  /*
   * OpenGL init, can be ignored
   */
  glutInit(&argc, argv); /* initialize GLUT system */

  int w = 5 + hd44780.w * 6;
  int h = 5 + hd44780.h * 8;
  int pixsize = 3;

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(w * pixsize, (h * pixsize));
  window = glutCreateWindow("Press 'q' to quit");

  initGL(w * pixsize, h * pixsize);

  avr->gdb_port = 1234;
  if (debug) {
    avr->state = cpu_Stopped;
    avr_gdb_init(avr);
    printf("avr-gdb -q -n -ex 'target remote 127.0.0.1:%d'\n", avr->gdb_port);
  }

  pthread_t run;
  pthread_create(&run, NULL, avr_run_thread, NULL);

  glutMainLoop();
}
