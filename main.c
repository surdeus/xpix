/* Made by "k1574". Look "license". */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>

int xerror(Display *dpy, XErrorEvent *err);
void usage(void);
int getrootptr(int *x, int *y);
Window selwin(int *x, int *y);

static char *argv0;
static Display *dpy;
static Window root;
/* The cursor that will be shown on grabbing. */
static Cursor cur;
/* Win which in we choose the pixel. */
static Window win;
/* Buffers. */
static XColor clr;
static XImage *img;


int
xerror(Display *dpy, XErrorEvent *err)
{
	return 0 ;
}

void
usage(void)
{
	fprintf(stderr, "Usage: %s\n", argv0);
	exit(1);
}

int
getrootptr(int *x, int *y)
{
        int di;
        unsigned int dui;
        Window dw;

        return XQueryPointer(dpy, root, &dw, &dw, x, y, &di, &di, &dui) ;
}

Window
selwin(int *x, int *y)
{
	/* Get the window by click and return it's ID and coordinates of pointer relative on it.
	* If no success then returns 0. */ 
	int status;
	XEvent ev;
	Window w;
	if(XGrabPointer(dpy,
			root,
			False,
			ButtonPressMask,
			GrabModeSync,
			GrabModeAsync,
			root,
			cur,
			CurrentTime)==GrabSuccess){
		XAllowEvents(dpy, SyncPointer, CurrentTime);
		XWindowEvent(dpy, root, ButtonPressMask, &ev);
		*x = ev.xbutton.x ; *y = ev.xbutton.y ;
		return ev.xbutton.subwindow ;
	}
	return 0 ;
}

void
main(int argc, char **argv) {
	int x, y;
	XWindowAttributes wa;

	argv0 = argv[0] ;
	if( 1<argc )
		usage();
	

	if(!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "%s: Cannot open display.\n", argv0);
		exit(1);
	}

	XSetErrorHandler(xerror);
	root = RootWindow(dpy, DefaultScreen(dpy)) ;
	cur = XCreateFontCursor(dpy, XC_tcross) ;
	win = selwin(&x, &y) ;

	img = XGetImage(dpy, win, x, y, 1, 1, AllPlanes, ZPixmap) ;
	if(!img){
		win = root ;
		img = XGetImage(dpy, win, x, y, 1, 1, AllPlanes, ZPixmap) ;
	}
	clr.pixel = XGetPixel(img, 0, 0) ;
	XDestroyImage(img);

	if(XGetWindowAttributes(dpy, win, &wa))
	XQueryColor(dpy, wa.colormap, &clr);

	printf("%02x%02x%02x\n",
		clr.red>>8,
		clr.green>>8,
		clr.blue>>8);

	XCloseDisplay(dpy);
	exit(0);
}
