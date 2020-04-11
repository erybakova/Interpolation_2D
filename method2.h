#ifndef METHOD2_H
#define METHOD2_H

void set_points(double *x, double a, int n, double h);
int compute_i (double *x, double xx, int n);
int bin_search(double *x, double xx, int left, int right, int n);
double d (int i, double  *y, double h);
double apply_method (double *x, double *y, int n, double xx, int i, double h, double d1, double d2);

#endif
