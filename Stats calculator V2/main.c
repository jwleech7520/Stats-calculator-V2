#include "misc.h"
#include "mathfunctions.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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