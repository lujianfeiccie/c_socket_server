/*
droid VNC server  - a vnc server for android
Copyright (C) 2011 Jose Pereira <onaips@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef KEYMANIP_H
#define KEYMANIP_H

//#include "common.h"  //remove by duliqun
//#include "gui.h"
#define BUS_VIRTUAL 0x06

extern int inputfd;
void  initInput();
int  keysym2scancode(int down, int c, int cl, int *sh, int *alt);
void transformTouchCoordinates(int *x, int *y,int,int);
void ptrEvent(int buttonMask, int x, int y, int width,int height);
void keyEvent(int down, int key, int cl);
void cleanupInput();

#endif
