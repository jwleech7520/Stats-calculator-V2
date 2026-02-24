#ifndef MISC_H
#define MISC_H

struct statistics {
  double datapoints[256];
  int n;
  double mean;
  double median;
  double q1;
  double range;
  double q3;
  double iqr;
  double stdev;
};

double* createSortedCopy(const double x[], int n);
struct statistics getStatistics(double x[], int n);

#endif