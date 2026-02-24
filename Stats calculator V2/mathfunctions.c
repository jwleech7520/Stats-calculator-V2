#include "misc.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

double mean(double z[], int n){
  double total = 0;
  for( int i = 0; i < n; i++){
    total += z[i];
  }
  return (double) total/n;
}

double standardDeviation(double x[], int n){
  double y = 0;
  double m = mean(x, n);
  for(int i = 0; i < n; i++){
    y += pow((x[i]-m), 2);
  }
  return sqrt(y/(n - 1));
}

double median(double x[], int n) {
    double* sorted = createSortedCopy(x, n);
    if (!sorted) return 0.0; // or handle error

    double result;
    if (n % 2 == 1) {
        result = sorted[n/2];
    } else {
        result = (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
    }

    free(sorted);
    return result;
}

double q1(double x[], int n){
  double* y = createSortedCopy(x, n);
  if (!y) return 0.0;

  if (n < 4) {
      printf("An array must have at least 4 values to find the quartiles\n");
      free(y);
      return 0.0;
  }

  double pos = (n + 1.0) / 4.0;
  int lower = (int)pos - 1;           // 0-based index
  double frac = pos - (lower + 1.0);

  double ret;
  if (frac < 1e-9) {                  // almost exact integer position
      ret = y[lower];
  } else {
      ret = y[lower] + frac * (y[lower + 1] - y[lower]);
  }

  free(y);
  return ret;
}

double q3(double x[], int n){
  double* y = createSortedCopy(x, n);
  if (!y) return 0.0;

  if (n < 4) {
      printf("An array must have at least 4 values to find the quartiles\n");
      free(y);
      return 0.0;
  }

  double pos = 3.0 * (n + 1.0) / 4.0;
  int lower = (int)pos - 1;           // 0-based index
  double frac = pos - (lower + 1.0);

  double ret;
  if (frac < 1e-9) {
      ret = y[lower];
  } else {
      ret = y[lower] + frac * (y[lower + 1] - y[lower]);
  }

  free(y);
  return ret;
}

double iqr(double x[], int n){
  double Q1 = q1(x, n);
  double Q3 = q3(x, n);
  return Q3 - Q1;
}

double range(double x[], int n){
  double* y = createSortedCopy(x, n);
  if (!y) return 0.0;

  return y[n-1] - y[0];
}