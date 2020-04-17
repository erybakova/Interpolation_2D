#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "window.h"
#include "method1.h"
#include "method2.h"

int main (int argc, char *argv[])
{
    double a, b;
    int n, k;

    if ((argc != 5) || (sscanf(argv[1], "%lf", &a) != 1) || (sscanf (argv[2], "%lf", &b) != 1)
            || (b - a < 1.e-6) || (sscanf (argv[3], "%d", &n) != 1) || (n < 5)
                || (sscanf (argv[4], "%d", &k) != 1) || (k < 0) || (k > 7))
    {
        printf("Usage: ./a.out a b n(>= 5) k\n");
        return 0;
    }

    QApplication app (argc, argv);

    QMainWindow *window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar (window);
    Window *graph_area = new Window (window, a, b, n, k);
    QAction *action;

    action = tool_bar -> addAction ("Change function", graph_area, SLOT (change_func ()));
    action -> setShortcut (QString ("0"));

    action = tool_bar -> addAction ("Change content", graph_area, SLOT (change_content ()));
    action -> setShortcut (QString ("1"));

    action = tool_bar -> addAction ("Zoom in", graph_area, SLOT (scale_mult_2 ()));
    action -> setShortcut (QString ("2"));

    action = tool_bar -> addAction ("Zoom out", graph_area, SLOT (scale_div_2 ()));
    action -> setShortcut (QString ("3"));

    action = tool_bar -> addAction ("Double n", graph_area, SLOT (n_mult_2 ()));
    action -> setShortcut (QString ("4"));

    action = tool_bar -> addAction ("Half n", graph_area, SLOT (n_div_2 ()));
    action -> setShortcut (QString ("5"));

    action = tool_bar -> addAction ("p++", graph_area, SLOT (p_plus_1 ()));
    action -> setShortcut (QString ("6"));

    action = tool_bar -> addAction ("p--", graph_area, SLOT (p_minus_1 ()));
    action -> setShortcut (QString ("7"));

    action = tool_bar -> addAction ("Exit", window, SLOT (close ()));
    action -> setShortcut (QString ("Ctrl+X"));

    tool_bar -> setMaximumHeight (30);

    window -> setMenuBar (tool_bar);
    window -> setCentralWidget (graph_area);
    window -> setWindowTitle ("Graph");
    
    window -> show ();

    app.exec ();

    graph_area -> free_vectors();
    delete graph_area;
    delete tool_bar;
    delete window;

    return 0;
}
