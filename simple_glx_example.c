#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

void draw_triangle(Display *dpy, Window win, GLXContext ctx) {
    glXMakeCurrent(dpy, win, ctx);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0); // 红色
        glVertex2f(-0.5, -0.5);
        glColor3f(0.0, 1.0, 0.0); // 绿色
        glVertex2f(0.5, -0.5);
        glColor3f(0.0, 0.0, 1.0); // 蓝色
        glVertex2f(0.0, 0.5);
    glEnd();

    glXSwapBuffers(dpy, win);
}

int main(int argc, char *argv[]) {
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "Cannot connect to X server\n");
        exit(1);
    }

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
        fprintf(stderr, "No appropriate visual found\n");
        exit(1);
    }

    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    Window win = XCreateWindow(dpy, root, 0, 0, 800, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Simple OpenGL Example with GLX");

    GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    XEvent xev;
    int k = 0;
    while (1) {
        XNextEvent(dpy, &xev);
        if (xev.type == Expose && k == 0) {
	    printf("Before draw_triangle\n");
            draw_triangle(dpy, win, glc);
	    printf("After draw_trangle\n");
	    k = 1;
        }
        if (xev.type == KeyPress) {
            break;
        }
    }

    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}
