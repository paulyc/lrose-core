// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
//
// Cidd_snap_stubs.cc - Notify and event callback function stubs.
// This file was generated by `gxv++' from `Cidd_snap.G'.
//

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/xv_xrect.h>
#include "Cidd_snap_ui.h"
using namespace std;

//
// Global object definitions.
//
Cidd_snap_window1_objects	Cidd_snap_window1;

int im_height;
int im_width;
Drawable can_xid;
Drawable src_xid;

Display *dpy;
GC def_gc;

Attr_attribute INSTANCE;

void resize_can_proc(Canvas canvas, int width, int height);
//
// Instance XV_KEY_DATA key.  An instance is a set of related
// user interface objects.  A pointer to an object's instance
// is stored under this key in every object.  This must be a
// global variable.
//

int 
main(int argc, char **argv)
{
	//
	// Initialize XView.
	//
	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
	INSTANCE = xv_unique_key();

        if (argc < 4) {
          cerr << "Minimal usage: Cidd_snap xid width height" << endl;
          cerr << "  Also, Cidd_snap -help" << endl;
          exit(1);
        }

	src_xid = (Drawable) atoi(argv[1]);
	im_width = atoi(argv[2]);
	im_height = atoi(argv[3]);
	//
	// Initialize user interface components.
	// Do NOT edit the object initializations by hand.
	//
	Cidd_snap_window1.objects_initialize(0);

        dpy = (Display *) xv_get(Cidd_snap_window1.window1, XV_DISPLAY);
	def_gc = DefaultGC(dpy, DefaultScreen(dpy));
        can_xid =  xv_get(canvas_paint_window(Cidd_snap_window1.canvas1), XV_XID);
	
        xv_set(Cidd_snap_window1.window1,
	       WIN_WIDTH, (im_width/2) + 20,
	       WIN_HEIGHT, (im_height/2) + 40,
	       NULL);

        xv_set(Cidd_snap_window1.canvas1,
	       CANVAS_AUTO_SHRINK, FALSE,
	       CANVAS_AUTO_EXPAND,FALSE,
	       CANVAS_WIDTH,  im_width,
	       CANVAS_HEIGHT, im_height,
	       NULL);

	 char msg[1024];
	 time_t now = time(0);
	 strftime(msg,1024,"Recorded %H:%M:%S %Z",localtime(&(now)));
	 xv_set(Cidd_snap_window1.msg1,PANEL_LABEL_STRING,msg,NULL);

	//
	// Turn control over to XView.
	//
	xv_main_loop(Cidd_snap_window1.window1);
	exit(0);
}

//
// Event callback function for `canvas1'.
//
Notify_value
can_event_func(Xv_window win, Event *event, Notify_arg arg, Notify_event_type type)
{
	fprintf(stderr,"EVENT: %d\n",event_id(event));
	switch(event_id(event)) {
		case 27:  // ESC
		case 81:  // Q
		case 113: // q
		exit(0);
	}
	return notify_next_event_func(win, (Notify_event) event, arg, type);
}

//
// Resize callback function for `canvas1'.
//
void
resize_can_proc(Canvas canvas, int width, int height)
{
       int h,w;

       // Clamp the window to the max image size
       h = xv_get(Cidd_snap_window1.window1,WIN_HEIGHT);
       w = xv_get(Cidd_snap_window1.window1,WIN_WIDTH);

       if((h > im_height + 40) || (w > im_width + 20)) {
	   xv_set(Cidd_snap_window1.window1,
		  WIN_HEIGHT, im_height + 40,
		  WIN_WIDTH,  im_width + 20,
		  NULL);
       }
}

//
// Repaint callback function for `canvas1'.
//
void
can_repaint_proc(Canvas canvas, Xv_window paint_window, Display *display, Window xid, Xv_xrectlist *rects)
{
	 
    XCopyArea(dpy,src_xid,can_xid,def_gc,0,0,im_width,im_height,0,0);
}

//
// Notify callback function for `button1'.
//
void
exit_func(Panel_item item, Event *event)
{
  exit(0);
}

//
// Event callback function for `window1'.
//
Notify_value
win_event_func(Xv_window win, Event *event, Notify_arg arg, Notify_event_type type)
{
	// fprintf(stderr,"Event: %d\n",event_id(event));
	switch(event_action(event)) {
		case ACTION_CLOSE:
		exit(0);
	}
        return notify_next_event_func(win, (Notify_event) event, arg, type);
}
