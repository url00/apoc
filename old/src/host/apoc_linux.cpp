#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <cstdio>
#include <cstdlib>

Display *dis;
Screen *screen;
int screen_id;
int screen_width;
int screen_height;
Window win;
GC gc;

void init_x();
void close_x();
void redraw();
void init_x()
{
    /* get the colors black and white (see section for details) */
    unsigned long black, white;

    dis = XOpenDisplay(NULL);
    screen_id = DefaultScreen(dis);
    black = BlackPixel(dis, screen_id),
    white = WhitePixel(dis, screen_id);

    screen = ScreenOfDisplay(dis, 0);
    screen_width = WidthOfScreen(screen);
    screen_height = HeightOfScreen(screen);

    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0,
                              screen_width, screen_height, 0, white, black);
    XSetStandardProperties(dis, win, "Howdy", "Hi", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
    gc = XCreateGC(dis, win, 0, 0);
    XSetBackground(dis, gc, white);
    XSetForeground(dis, gc, black);
    XClearWindow(dis, win);
    XMapRaised(dis, win);
};

void close_x()
{
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(1);
};

void redraw()
{
    XClearWindow(dis, win);
};

int main()
{

    XEvent event;   /* the XEvent declaration !!! */
    KeySym key;     /* a dealie-bob to handle KeyPress Events */
    char text[255]; /* a char buffer for KeyPress Events */

    init_x();

    /* look for events forever... */
    while (1)
    {
        /* get the next event and stuff it into our event variable.
		   Note:  only events we set the mask for are detected!
		*/
        XNextEvent(dis, &event);

        if (event.type == Expose && event.xexpose.count == 0)
        {
            /* the window was exposed redraw it! */
            redraw();
        }
        if (event.type == KeyPress &&
            XLookupString(&event.xkey, text, 255, &key, 0) == 1)
        {
            /* use the XLookupString routine to convert the invent
		   KeyPress data into regular text.  Weird but necessary...
		*/
            if (text[0] == 'q')
            {
                close_x();
            }
            printf("You pressed the %c key!\n", text[0]);
        }
        if (event.type == ButtonPress)
        {
            /* tell where the mouse Button was Pressed */
            int x = event.xbutton.x,
                y = event.xbutton.y;

            strcpy(text, "X is FUN!");
            XSetForeground(dis, gc, rand() % event.xbutton.x % 255);
            XDrawString(dis, win, gc, x, y, text, strlen(text));
        }
    }
}