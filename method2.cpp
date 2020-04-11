#include "method2.h"

#include <math.h>
#include <stdio.h>

void set_points (double *x, double a, int n, double h)
{
    int i;

    for (i = 0; i < n; i++)
        x[i] = a + i * h;
}

int compute_i (double *x, double xx, int n)
{
    int i;

    if (xx <= x[0] && xx >= x[0]) return 0;
    if (xx <= x[n - 1] && xx >= x[n - 1]) return n - 2;

    for (i = 0; i < n; i++)
        if (xx < x[i]) return i - 1;

    printf("Smth wrong..\n");
    return 0;
}

int bin_search(double *x, double xx, int left, int right, int n)
{
    int mid;

    if (xx <= x[0] && xx >= x[0]) return 0;
    if (xx <= x[n - 1] && xx >= x[n - 1]) return n - 2;

    while (left < right)
    {
        mid = (left + right) / 2;
        if (xx < x[mid])
            right = mid;
        else
            left = mid + 1;
    }

    return left - 1;
}

double d (int i, double  *y, double h)
{
    return (y[i + 1] - y[i - 1]) / (2 * h);
}

double apply_method (double *x, double *y, int n,
                     double xx, int i, double h, double d1, double d2)
{
    double c1, c2, c3, c4;
    double d_i, d_ii;
    double delta, br, razd_razn;

    delta = x[i + 1] - x[i];
    br = xx - x[i];

    razd_razn = (y[i + 1] - y[i]) / delta;

    if (i > 0 && i < n - 2)
    {
        d_i = d (i, y, h);
        d_ii = d (i + 1, y, h);
    }
    else if (i == 0)
    {
        d_ii = d (i + 1, y, h);
        d_i = 1.5 * razd_razn - 0.25 * d1 * delta - 0.5 * d_ii;
    }
    else // i = n - 2
    {
        d_i = d (i, y, h);
        d_ii = 1.5 * razd_razn + 0.25 * d2 * delta - 0.5 * d_i;
    }

    c1 = y[i];
    c2 = d_i;
    c3 = (3 * razd_razn - 2 * d_i - d_ii) / delta;
    c4 = (d_i + d_ii - 2 * razd_razn) / (delta * delta);

    return c1 + c2 * br + c3 * br * br + c4 * br * br * br;
}
