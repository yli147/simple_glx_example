#include <X11/Xlib.h>
#include <stdio.h>
#include <unistd.h> // for close()

int main() {
    Display *display;
    Window window;
    int screen;

    // 连接到 X 服务器
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }
    printf("Connected to X server\n");

    // 获取默认屏幕
    screen = DefaultScreen(display);

    // 创建一个简单的窗口
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 100, 100, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));
    XMapWindow(display, window);

    // 强制关闭连接以模拟 EPIPE 错误
    close(ConnectionNumber(display));
    printf("Closed connection to X server\n");

    // 尝试与 X 服务器进行通信
    XStoreName(display, window, "Test Window");
    XFlush(display);

    // 由于连接已经关闭，以上操作应该会导致错误
    printf("Attempted to communicate with X server after closing connection\n");

    // 关闭显示连接
    XCloseDisplay(display);

    return 0;
}

