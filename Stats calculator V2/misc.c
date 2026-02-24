#include "mathfunctions.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "misc.h"

double* createSortedCopy(const double x[], int n) {
    double* ret = malloc(n * sizeof(double));
    if (!ret) return NULL;

    for (int i = 0; i < n; i++) {
        ret[i] = x[i];
    }

    for (int i = 0; i < n - 1; i++) {
      for (int j = 0; j < n - i - 1; j++) {
          if (ret[j] > ret[j + 1]) {
              double temp = ret[j];
              ret[j] = ret[j + 1];
              ret[j + 1] = temp;
          }
      }
    }

    return ret;
}

struct statistics getStatistics(double x[], int n){
  struct statistics y;
  y.n = n;
  memcpy(y.datapoints, createSortedCopy(x, n), sizeof(double) * n);
  y.mean = mean(x, n);
  y.median = median(x, n);
  y.q1 = q1(x, n);
  y.q3 = q3(x, n);
  y.iqr = y.q3 - y.q1;
  y.range = y.datapoints[n - 1] - y.datapoints[0];
  y.stdev = standardDeviation(x, n);
  return y;
}