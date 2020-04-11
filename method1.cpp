#include "method1.h"

#include <math.h>
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
    double mult, phi = 1, sum = 0, y_i = 1;

    for (i = 0; i < n; i++)
    {
        mult = 1;
        for (j = 0; j < n; j++)
        {
            if (j == i) continue;
            mult *= (x[i] - x[j]);
        }
        if (fabs (mult) < 1e-6) printf ("Dividing by zero!\n");
        y_i = y[i] / mult;

        phi *= (xx - x[i]);
        sum += y_i / (xx - x[i]);
    }

    return phi * sum;
}