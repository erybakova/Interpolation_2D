#include <QPainter>
#include <stdio.h>

#include "window.h"
#include "method1.h"
#include "method2.h"

#include <math.h>

double Window:: polynomial_approximation (double xx)
{
    return apply_method (x, y, n, xx);
}

double Window:: piecewise_polynomial_approximation (double xx)
{
    int i;
    double d1, d2;
    double h = (b - a) / (n - 1);

    d1 = (this ->* d2_f) (a);
    d2 = (this ->* d2_f) (b);

    i = compute_i (x, xx, n);
    return apply_method (x, y, n, xx, i, h, d1, d2);
}

void Window:: set_curr_f (bool t)
{
    if (t == 0)
    {
        set_Chebyshev_zeros (x, a, b, n);
        set_values ();
        curr_f = &Window:: polynomial_approximation;
    }
    else
    {
        double h = (b - a) / (n - 1);
        set_points (x, a, n, h);
        set_values ();
        curr_f = &Window:: piecewise_polynomial_approximation;
    }
}

double Window:: error (double x)
{
    return fabs ((this ->* f) (x) - (this ->* curr_f) (x));
}

Window:: Window (QWidget *parent, double a_, double b_, int n_, int k_) : QWidget (parent)
{
    a = a_;
    b = b_;
    n = n_;
    k = k_;
    s = 0;
    p = 0;
    max_abs = 0;

    x = new double[n];
    y = new double[n];

    change_func();
}

QSize Window:: minimumSizeHint () const
{
    return QSize (100, 100);
}

QSize Window:: sizeHint () const
{
    return QSize (1000, 1000);
}

// change content of current widget
void Window::change_content ()
{
    s = 0;
    p = 0;
    content = (content + 1) % 4;

    switch (content)
    {
        case method1:
          method_name = "Method 1";
          set_curr_f (0);
          break;
        case method2:
          method_name = "Method 2";
          set_curr_f (1);
          break;
        case method1_and_method2:
          method_name = "Methods 1 and 2";
          set_curr_f (0);
          break;
        case error_graph:
          method_name = "Error graphs";
          set_curr_f (0);
          break;
    }

    update ();
}

// change current function for drawing
void Window::change_func ()
{
    s = 0;
    p = 0;
    k = k % 7;

    switch (k)
    {
        case 0:
          f_name = "f(x) = 1";
          f    = &Window:: f_0;
          d2_f = &Window:: d2_f_0;
          break;
        case 1:
          f_name = "f(x) = x";
          f    = &Window:: f_1;
          d2_f = &Window:: d2_f_1;
          break;
        case 2:
          f_name = "f(x) = x^2";
          f    = &Window:: f_2;
          d2_f = &Window:: d2_f_2;
          break;
        case 3:
          f_name = "f(x) = x^3";
          f    = &Window:: f_3;
          d2_f = &Window:: d2_f_3;
          break;
        case 4:
          f_name = "f(x) = x^4";
          f    = &Window:: f_4;
          d2_f = &Window:: d2_f_4;
          break;
        case 5:
          f_name = "f(x) = exp(x)";
          f    = &Window:: f_5;
          d2_f = &Window:: d2_f_5;
          break;
        case 6:
          f_name = "f(x) = 1 / (25 x^2 + 1)";
          f    = &Window:: f_6;
          d2_f = &Window:: d2_f_6;
          break;
    }

    content = 0;
    method_name = "Method 1";
    set_curr_f (0);

    k++;
    update ();
}

void Window::paintEvent (QPaintEvent * /* event */)
{
    double max_y, min_y, max_y1, min_y1, max_yy, min_yy;
    double f_max = 0, f_min = 0, abs_value;
    double delta_x = (b - a) / width();
    double resid = 0, resid1 = 0;

    QPainter painter (this);
    QPen pen_black (Qt::black, 0, Qt::SolidLine);
    QPen pen_red (Qt::red, 0, Qt::SolidLine);
    QPen pen_blue (Qt::darkBlue, 0, Qt::SolidLine);
    QPen pen_blue1 (Qt::darkBlue, 0, Qt::DotLine);
    QPen pen_green (Qt::darkGreen, 0, Qt::SolidLine);
    QFont font;
    font.setPixelSize (10);
    painter.setFont (font);

    // calc min and max of functions
    switch (content)
    {
      case method1:
        set_curr_f (0);
        max_and_min (f, delta_x, &min_yy, &max_yy, &abs_value);
        max_and_min (curr_f, delta_x, &min_y, &max_y, &abs_value);
        f_min = min_y < min_yy ? min_y : min_yy;
        f_max = max_y > max_yy ? max_y : max_yy;
        break;
      case method2:
        set_curr_f (1);
        max_and_min (curr_f, delta_x, &min_y, &max_y, &abs_value);
        max_and_min (f, delta_x, &min_yy, &max_yy, &abs_value);
        f_min = min_y < min_yy ? min_y : min_yy;
        f_max = max_y > max_yy ? max_y : max_yy;
        break;
      case method1_and_method2:
        max_and_min (f, delta_x, &min_yy, &max_yy, &abs_value);
        set_curr_f (0);
        max_and_min (curr_f, delta_x, &min_y, &max_y, &abs_value);
        f_min = min_y < min_yy ? min_y : min_yy;
        f_max = max_y > max_yy ? max_y : max_yy;
        set_curr_f (1);
        max_and_min (curr_f, delta_x, &min_y1, &max_y1, &abs_value);
        f_min = f_min < min_y1 ? f_min : min_y1;
        f_max = f_max > max_y1 ? f_max : max_y1;
        break;
      case error_graph:
        set_curr_f (0);
        max_and_min (&Window:: error, delta_x, &min_y, &max_y, &abs_value);
        set_curr_f (1);
        max_and_min (&Window:: error, delta_x, &min_y1, &max_y1, &abs_value);
        f_min = min_y < min_y1 ? min_y : min_y1;
        f_max = max_y > max_y1 ? max_y : max_y1;
        break;
    }

    // scaling
    scaling_and_axes (painter, f_max, f_min, pen_blue1);

    // draw graphs
    switch (content)
    {
        case method1:
          draw (painter, f, delta_x, pen_blue);
          set_curr_f (0);
          resid = residual (curr_f, delta_x);
          draw (painter, curr_f, delta_x, pen_red);
          break;
        case method2:
          draw (painter, f, delta_x, pen_blue);
          set_curr_f (1);
          resid = residual (curr_f, delta_x);
          draw (painter, curr_f, delta_x, pen_green);
          break;
        case method1_and_method2:
          draw (painter, f, delta_x, pen_blue);
          set_curr_f(0);
          draw (painter, curr_f, delta_x, pen_red);
          resid = residual (curr_f, delta_x);
          set_curr_f(1);
          draw (painter, curr_f, delta_x, pen_green);
          resid1 = residual (curr_f, delta_x);
          break;
        case error_graph:
          set_curr_f (0);
          draw (painter, &Window:: error, delta_x, pen_red);
          set_curr_f (1);
          draw (painter, &Window:: error, delta_x, pen_green);
          break;
    }
    painter.restore ();

    // print necessary information
    print_info (painter, pen_black, f_min, f_max, abs_value, resid, resid1);
}

void Window:: scaling_and_axes (QPainter& painter, double f_max, double f_min, QPen pen)
{
    painter.save ();

    double delta = fabs (f_max - f_min);
    if (delta < 1e-20) delta = 1e-15;

    /// make Coordinate Transformations
    painter.translate (0.5 * width (), 0.5 * height ());
    painter.scale (width () / (b - a), -height () / delta);
    painter.translate (-0.5 * (a + b), -0.5 * (f_min + f_max));

    /// draw axis
    painter.setPen (pen);
    painter.drawLine (QPointF (a, 0), QPointF (b, 0));
    painter.drawLine (QPointF (0, f_max * 1.5), QPointF (0, f_min - 1));
}

void Window:: max_and_min (double (Window:: *ff) (double), double delta_x,
                       double *min, double *max, double *abs_value)
{
    double x1, y1;

    x1 = a;
    y1 = (this ->* ff) (x1);
    *max = *min = y1;
    *abs_value = fabs (y1);

    for (x1 = a + delta_x; x1 - b < 1.e-6; x1 += delta_x)
    {
      y1 = (this ->* ff) (x1);
      if (y1 < *min)
          *min = y1;
      if (y1 > *max)
          *max = y1;
      if (fabs (y1) > *abs_value)
          *abs_value = fabs (y1);
    }

    max_abs = *abs_value;
}

void Window:: draw (QPainter& painter, double (Window:: *ff) (double), double delta_x, QPen pen)
{
    double x1, y1, x2, y2;

    painter.setPen (pen);

    x1 = a;
    y1 = (this ->* ff) (x1);

    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = (this ->* ff) (x2);
      painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));
      x1 = x2, y1 = y2;
    }

    x2 = b;
    y2 = (this ->* ff) (x2);
    painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));
}

double Window:: residual (double (Window:: *ff) (double), double delta_x)
{
    double x1, x2;
    double resid, r;

    x1 = a;
    resid = fabs((this ->* f) (x1) - (this ->* ff) (x1));

    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      r = fabs((this ->* f) (x2) - (this ->* ff) (x2));
      if (r > resid)
          resid = r;
      x1 = x2;
    }

    x2 = b;
    r = fabs((this ->* f) (x2) - (this ->* ff) (x2));
    if (r > resid)
        resid = r;

    return resid;
}

void Window:: print_info (QPainter& painter, QPen pen, double f_min,
                          double f_max, double abs_value, double resid, double resid1)
{
    double max, l = 10, r = 5 * width () / 6, b = 5 * height () / 6;
    char buf[1234];

    QPen pen_black (Qt::black, 0, Qt::SolidLine);
    QPen pen_blue (Qt::darkBlue, 0, Qt::SolidLine);
    QPen pen_red (Qt::red, 0, Qt::SolidLine);
    QPen pen_green (Qt::darkGreen, 0, Qt::SolidLine);
    QFont font;
    font.setPixelSize (20);
    QFont font1;
    font1.setPixelSize (14);
    QFont font2;
    font2.setPixelSize (10);

    painter.setPen (pen);

    switch (content)
    {
      case method1:
        sprintf (buf, "max |y| = %.3g", abs_value);
        painter.drawText (l, 90, buf);
        printf("%s\n", method_name);
        printf("k = %d  max |y| = %.3g\n", k - 1, abs_value);
        max = fabs (f_min) > fabs (f_max) ? fabs (f_min) : fabs (f_max);
        sprintf (buf, "max {|F_min|,|F_max|} = %.3g", max);
        painter.drawText (l, 110, buf);
        printf("       max {|F_min|,|F_max|} = %.3g\n\n", max);
        sprintf (buf, "residual = %.3e", resid);
        painter.drawText (l, 130, buf);
        painter.setFont (font1);
        painter.drawText (l, 60, f_name);

        painter.setFont (font2);
        painter.fillRect (r - 10, b - 10, 90, 45, Qt::white);
        painter.fillRect (r, b, 8, 8, Qt::darkBlue);
        painter.setPen (pen_blue);
        painter.drawText (r + 18, b + 8, "function");
        painter.fillRect (r, b + 15, 8, 8, Qt::red);
        painter.setPen (pen_red);
        painter.drawText (r + 18, b + 23, "method 1");
        break;
      case method2:
        sprintf (buf, "max |y| = %.3g", abs_value);
        painter.drawText (l, 90, buf);
        printf("%s\n", method_name);
        printf("k = %d  max |y| = %.3g\n", k - 1, abs_value);
        max = fabs (f_min) > fabs (f_max) ? fabs (f_min) : fabs (f_max);
        sprintf (buf, "max {|F_min|,|F_max|} = %.3g", max);
        painter.drawText (l, 110, buf);
        printf("       max {|F_min|,|F_max|} = %.3g\n\n", max);
        sprintf (buf, "residual = %.3e", resid);
        painter.drawText (l, 130, buf);
        painter.setFont (font1);
        painter.drawText (l, 60, f_name);

        painter.setFont (font2);
        painter.fillRect (r - 10, b - 10, 90, 45, Qt::white);
        painter.fillRect (r, b, 8, 8, Qt::darkBlue);
        painter.setPen (pen_blue);
        painter.drawText (r + 18, b + 8, "function");

        painter.fillRect (r, b + 15, 8, 8, Qt::darkGreen);
        painter.setPen (pen_green);
        painter.drawText (r + 18, b + 23, "method 2");
        break;
      case method1_and_method2:
        sprintf (buf, "max |y| = %.3g", abs_value);
        painter.drawText (l, 90, buf);
        printf("%s\n", method_name);
        printf("k = %d  max |y| = %.3g\n", k - 1, abs_value);
        max = fabs (f_min) > fabs (f_max) ? fabs (f_min) : fabs (f_max);
        sprintf (buf, "max {|F_min|,|F_max|} = %.3g", max);
        painter.drawText (l, 110, buf);
        printf("       max {|F_min|,|F_max|} = %.3g\n\n", max);
        sprintf (buf, "residual_1 = %.3e", resid);
        painter.drawText (l, 130, buf);
        sprintf (buf, "residual_2 = %.3e", resid1);
        painter.drawText (l, 150, buf);
        painter.setFont (font1);
        painter.drawText (l, 60, f_name);

        painter.setFont (font2);
        painter.fillRect (r - 10, b - 10, 90, 60, Qt::white);
        painter.fillRect (r, b, 8, 8, Qt::darkBlue);
        painter.setPen (pen_blue);
        painter.drawText (r + 18, b + 8, "function");

        painter.fillRect (r, b + 15, 8, 8, Qt::red);
        painter.setPen (pen_red);
        painter.drawText (r + 18, b + 23, "method 1");

        painter.fillRect (r, b + 30, 8, 8, Qt::darkGreen);
        painter.setPen (pen_green);
        painter.drawText (r + 18, b + 38, "method 2");
        break;
      case error_graph:
        max = fabs (f_min) > fabs (f_max) ? fabs (f_min) : fabs (f_max);
        sprintf (buf, "max {|F_min|,|F_max|} = %.3g", max);
        painter.drawText (l, 90, buf);
        printf("%s\n", method_name);
        printf("       max {|F_min|,|F_max|} = %.3g\n\n", max);

        painter.setFont (font1);
        painter.drawText (l, 60, f_name);

        painter.setFont (font2);
        painter.fillRect (r - 10, b + 5, 130, 45, Qt::white);
        painter.fillRect (r, b + 15, 8, 8, Qt::red);
        painter.setPen (pen_red);
        painter.drawText (r + 18, b + 23, "Error of method 1");

        painter.fillRect (r, b + 30, 8, 8, Qt::darkGreen);
        painter.setPen (pen_green);
        painter.drawText (r + 18, b + 38, "Error of method 2");
        break;
    }

    painter.setPen (pen_black);
    painter.setFont (font);
    painter.drawText (l, 30, method_name);
    painter.setFont (font1);
    sprintf (buf, "k = %d", k);
    painter.drawText (r, 35, buf);
    sprintf (buf, "n = %d", n);
    painter.drawText (r, 57, buf);
    sprintf (buf, "s = %d", s);
    painter.drawText (r, 79, buf);
    sprintf (buf, "p = %d", p);
    painter.drawText (r, 101, buf);
}

void Window:: scale_div_2 ()
{
    double len = b - a;
    a -= len / 2;
    b += len / 2;

    s--;

    update ();
}

void Window:: scale_mult_2 ()
{
    double len = b - a;
    a += len / 4;
    b -= len / 4;

    s++;

    update ();
}

void Window:: n_div_2 ()
{
    n /= 2;

    update ();
}

void Window:: n_mult_2 ()
{
    n *= 2;

    update ();
}

void Window:: p_plus_1 ()
{
    p++;

    update ();
}

void Window:: p_minus_1 ()
{
    p--;

    update ();
}
