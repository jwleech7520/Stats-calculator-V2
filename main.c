#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

double mean(double z[], int n);
double standardDeviation(double x[], int n);
double* createSortedCopy(const double x[], int n);
struct statistics getStatistics(double x[], int n);
double median(double x[], int n);
double q1(double x[], int n);
double q3(double x[], int n);
double iqr(double x[], int n);
double range(double x[], int n);

int main(){

  //main loop

  while (1) {
    printf("> ");
    char input[1024];

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;  // EOF (Ctrl+D)
    }

    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      break;
    }

    // Skip empty lines
    if (input[0] == '\0') {
      continue;
    }

    // Count how many numbers we expect (rough estimate)
    int expected_count = 1;
    for (size_t i = 0; input[i] != '\0'; i++) {
      if (input[i] == ',' || input[i] == ' ') {
        expected_count++;
      }
    }

    // Allocate array — start with estimated size
    double *numbers = malloc(expected_count * sizeof(double));
    if (!numbers) {
      printf("Memory allocation failed\n");
      continue;
    }

    // Make a copy because strtok modifies the string
    char *copy = strdup(input);
    if (!copy) {
      free(numbers);
      printf("Memory allocation failed\n");
      continue;
    }

    int count = 0;
    char *token = strtok(copy, ", ");   // split on comma OR space

    while (token != NULL && count < expected_count) {
      char *endptr;
      double value = strtod(token, &endptr);

      // Check if we parsed a valid number and consumed the whole token
      if (endptr != token && (*endptr == '\0' || isspace(*endptr))) {
        numbers[count++] = value;
      }
      // else: skip invalid token (word, empty part, etc.)

      token = strtok(NULL, ", ");
    }

    free(copy);

    double *shrunk = realloc(numbers, count * sizeof(double));
    if (shrunk) numbers = shrunk;

    for (int i = 0; i < count; i++) {
      printf("%.3f", numbers[i]);
      if (i < count - 1) printf(", ");
    } 
    printf("\n");

    if (count == 0) {
      printf("No valid numbers parsed.\n");
      free(numbers);
      continue;
    }

    printf("Parsed %d values. Now enter commands (mean, median, stdev, q1, q3, iqr, range, stats, done)\n", count);

    // ──────────────────────────────
    // Inner command loop
    // ──────────────────────────────
    while (1) {
      printf("> ");
      char cmd[1024];
      if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
          break;  // Ctrl+D → exit inner loop
      }
      cmd[strcspn(cmd, "\n")] = '\0';

      if (strcmp(cmd, "done") == 0) {
          break;
      }

      if (strncmp(cmd, "mean", 4) == 0) {
          printf("Mean   = %.2f\n", mean(numbers, count));
      }
      else if (strncmp(cmd, "stdev", 5) == 0) {
          printf("Stdev  = %.2f\n", standardDeviation(numbers, count));
      }
      else if (strncmp(cmd, "med", 3) == 0 || strncmp(cmd, "median", 6) == 0) {
          printf("Median = %.2f\n", median(numbers, count));
      }
      else if (strncmp(cmd, "q1", 2) == 0) {
          printf("Q1     = %.2f\n", q1(numbers, count));
      }
      else if (strncmp(cmd, "q3", 2) == 0) {
          printf("Q3     = %.2f\n", q3(numbers, count));
      }
      else if (strncmp(cmd, "iqr", 3) == 0) {
          printf("IQR    = %.2f\n", iqr(numbers, count));
      }
      else if (strncmp(cmd, "range", 5) == 0) {
          printf("Range  = %.2f\n", range(numbers, count));
      }
      else if (strncmp(cmd, "stats", 5) == 0) {
          struct statistics stats = getStatistics(numbers, count);
          printf("**********************************\n");
          printf("Datapoints: ");
          for (int i = 0; i < stats.n - 1; i++) {
              printf("%.2f, ", stats.datapoints[i]);
          }
          if (stats.n > 0) {
              printf("%.2f\n", stats.datapoints[stats.n-1]);
          }
          printf("Number of points: %d\n", stats.n);
          printf("Range:            %.2f\n", stats.range);
          printf("Mean:             %.2f\n", stats.mean);
          printf("Median:           %.2f\n", stats.median);
          printf("Lower quartile:   %.2f\n", stats.q1);
          printf("Upper quartile:   %.2f\n", stats.q3);
          printf("IQR:              %.2f\n", stats.iqr);
          printf("Std deviation:    %.2f\n", stats.stdev);
          printf("**********************************\n");
      }else {
        printf("Unknown command. Try: mean, stdev, median, q1, q3, iqr, range, stats, done\n");
     }
    }

    free(numbers);   // very important!
  }


  return 0;
}

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