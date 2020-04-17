#include "method1.h"

#include <cmath> 
#include <stdio.h>

void set_Chebyshev_zeros (double *x, double a, double b, int n)
{
    int m;
    int u = (a + b) / 2.;
    int v = (b - a) / 2.;

    for (m = 0; m < n; m++)
    {
        x[m] = u + v * cos (M_PI * (2 * m + 1) / (2. * n));
    }
}

double apply_method (double *x, double *y, int n, double xx)
{
    int i, j;
    double mult, phi = 1., sum = 0., y_i = 1., delta;

    for (i = 0; i < n; i++)
    {
        mult = 1.;
        for (j = 0; j < n; j++)
        {
            if (j == i) continue;
            mult *= (x[i] - x[j]);
        }

        if (fabs (mult) < 1e-6)
            mult = 1e-6;

        y_i = y[i] / mult;

        delta = xx - x[i];

        if (fabs (delta) < 1e-6)
            delta = 1e-6;

        phi *= delta;
        sum += y_i / delta;
    }

    return phi * sum;
}
