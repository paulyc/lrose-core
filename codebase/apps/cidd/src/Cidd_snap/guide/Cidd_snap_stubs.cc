//
// Cidd_snap_stubs.cc - Notify and event callback function stubs.
// This file was generated by `gxv++' from `Cidd_snap.G'.
//

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/xv_xrect.h>
#include "Cidd_snap_ui.h"


//
// Global object definitions.
//
Cidd_snap_window1_objects	Cidd_snap_window1;

#ifdef MAIN

//
// Instance XV_KEY_DATA key.  An instance is a set of related
// user interface objects.  A pointer to an object's instance
// is stored under this key in every object.  This must be a
// global variable.
//
Attr_attribute	INSTANCE;

main(int argc, char **argv)
{
	//
	// Initialize XView.
	//
	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
	INSTANCE = xv_unique_key();
	
	//
	// Initialize user interface components.
	// Do NOT edit the object initializations by hand.
	//
	Cidd_snap_window1.objects_initialize(NULL);
	
	
	//
	// Turn control over to XView.
	//
	xv_main_loop(Cidd_snap_window1.window1);
	exit(0);
}

#endif


//
// Event callback function for `canvas1'.
//
Notify_value
can_event_func(Xv_window win, Event *event, Notify_arg arg, Notify_event_type type)
{
	Cidd_snap_window1_objects *ip = (Cidd_snap_window1_objects *) xv_get(xv_get(win, CANVAS_PAINT_CANVAS_WINDOW), XV_KEY_DATA, INSTANCE);
	
	fprintf(stderr, "Cidd_snap: can_event_func: event %d\n", event_id(event));
	
	// gxv_start_connections DO NOT EDIT THIS SECTION

	// gxv_end_connections

	return notify_next_event_func(win, (Notify_event) event, arg, type);
}

//
// Repaint callback function for `canvas1'.
//
void
can_repaint_proc(Canvas canvas, Xv_window paint_window, Display *display, Window xid, Xv_xrectlist *rects)
{
	fputs("Cidd_snap: can_repaint_proc\n", stderr);
	
	// gxv_start_connections DO NOT EDIT THIS SECTION

	// gxv_end_connections

}

//
// Resize callback function for `canvas1'.
//
void
resize_can_proc(Canvas canvas, int width, int height)
{
	fputs("Cidd_snap: resize_can_proc\n", stderr);
	
	// gxv_start_connections DO NOT EDIT THIS SECTION

	// gxv_end_connections

}

//
// Notify callback function for `button1'.
//
void
exit_func(Panel_item item, Event *event)
{
	Cidd_snap_window1_objects *ip = (Cidd_snap_window1_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
	
	fputs("Cidd_snap: exit_func\n", stderr);
	
	// gxv_start_connections DO NOT EDIT THIS SECTION

	// gxv_end_connections

}

//
// Event callback function for `window1'.
//
Notify_value
win_event_func(Xv_window win, Event *event, Notify_arg arg, Notify_event_type type)
{
	Cidd_snap_window1_objects *ip = (Cidd_snap_window1_objects *) xv_get(win, XV_KEY_DATA, INSTANCE);
	
	fprintf(stderr, "Cidd_snap: win_event_func: event %d\n", event_id(event));
	
	// gxv_start_connections DO NOT EDIT THIS SECTION

	// gxv_end_connections

	return notify_next_event_func(win, (Notify_event) event, arg, type);
}
