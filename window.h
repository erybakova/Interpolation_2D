#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QtWidgets>

enum content
{
    method1,
    method2,
    method1_and_method2,
    error_graph,
};

class Window : public QWidget
{
  Q_OBJECT

private:
  double a;
  double b;
  int n; // количество точек приближения
  int k; // номер функции
  int s; // текущий масштаб
  int p; //текущее возмущение
  int content; // текущее содержимое виджета
  double max_abs; // максимум модуля
  const char *f_name;
  const char *method_name;
  double (Window:: *f) (double);
  double (Window:: *d2_f) (double);
  double (Window:: *curr_f) (double);
  double *x; // точки
  double *y; // значения

public:
  Window (QWidget *parent, double a, double b, int n, int k);
  QSize minimumSizeHint () const;
  QSize sizeHint () const;
  double polynomial_approximation (double xx);
  double piecewise_polynomial_approximation (double xx);
  void set_curr_f (bool t);
  void create_vectors(int n)
  {
      x = new double[n];
      y = new double[n];
  }
  void free_vectors()
  {
      delete [] x;
      delete [] y;
  }
  void set_values ()
  {
      int i;

      for (i = 0; i < n; i++)
      {
          y[i] = (this ->* f) (x[i]);
      }
      y[n / 2] += p * 0.1 * max_abs;
  }
  double f_ (double x);
  double error (double x);
  double f_0 (double)
  {
      return 1;
  }
  double d2_f_0 (double)
  {
      return 0;
  }
  double f_1 (double x)
  {
      return x;
  }
  double d2_f_1 (double)
  {
      return 0;
  }
  double f_2 (double x)
  {
      return x * x;
  }
  double d2_f_2 (double)
  {
      return 2;
  }
  double f_3 (double x)
  {
      return x * x * x;
  }
  double d2_f_3 (double x)
  {
      return 6 * x;
  }
  double f_4 (double x)
  {
      return x * x * x * x;
  }
  double d2_f_4 (double x)
  {
      return 12 * x * x;
  }
  double f_5 (double x)
  {
      return exp (x);
  }
  double d2_f_5 (double x)
  {
      return exp (x);
  }
  double f_6 (double x)
  {
      return 1. / (25 * x * x + 1);
  }
  double d2_f_6 (double x)
  {
      return (5000. * x * x) / ((1 + 25 * x * x) * (1 + 25 * x * x) * (1 + 25 * x * x))
              - 50. / ((1 + 25 * x * x) * (1 + 25 * x * x));
  }

public slots:
  void change_func ();
  void change_content ();
  void scale_div_2 ();
  void scale_mult_2 ();
  void n_div_2 ();
  void n_mult_2 ();
  void p_plus_1 ();
  void p_minus_1 ();

protected:
  void paintEvent (QPaintEvent *event);
  void scaling_and_axes (QPainter& painter, double f_max, double f_min, QPen pen);
  void max_and_min (double (Window:: *ff) (double), double delta_x,
                         double *min, double *max, double *abs_value);
  void draw (QPainter& painter, double (Window:: *ff) (double), double delta_x, QPen pen);
  double residual (double (Window:: *ff) (double), double delta_x);
  void print_info (QPainter& painter, QPen pen, double f_min, double f_max,
                   double abs_value, double resid, double resid1);
};

#endif
