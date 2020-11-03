/* assert */
#include <assert.h>

/* fabs */
#include <math.h>

/* printf, fopen, fclose, fscanf, scanf */
#include <stdio.h>

/* EXIT_SUCCESS, malloc, calloc, free, qsort */
#include <stdlib.h>

struct distance_metric {
  size_t viewer_id;
  double distance;
};

int
main(int argc, char * argv[])
{
  size_t n, m;

  /* Validate command line arguments. */
  assert(2 == argc);

  /* ... */
  char const * const fn = argv[1];

  /* Validate input. */
  assert(fn);

  /* Open file. */
  FILE * const fp = fopen(fn, "r");
  assert(fp);

  /* Read file. */
  fscanf(fp, "%zu %zu", &n, &m);

  /* Allocate memory. */
  double * const rating = malloc(n * m * sizeof(*rating));

  /* Check for success. */
  assert(rating);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      fscanf(fp, "%lf", &rating[i * m + j]);
    }
  }

  /* Close file. */
  int const ret = fclose(fp);
  assert(!ret);

  /* Allocate more memory. */
  double * const urating = malloc(m * sizeof(*urating));

  /* Check for success. */
  assert(urating);

  /* Get user input. */
  for (size_t j = 0; j < m - 1; j++) {
    printf("Enter your rating for movie %zu: ", j + 1);
    scanf("%lf", &urating[j]);
  }

  /* Allocate more memory. */
  double prob[10] = { 0.0 };

  /* Compute probabilities */
  for (int k = 0; k < 10; k++) {
    for (size_t i = 0; i < n; i++) {
      prob[k] += (rating[i * m + 4] == (k + 1) / 2.0);
    }
  }

  /* Finalize computation of probabilities. */
  for (int k = 0; k < 10; k++) {
    prob[k] /= n;
  }

  //for (int k = 0; k < 10; k++) {
    //printf("prob[%d] = %lf\n", k, prob[k]);
  //}

  /* Make Prediction */
  /* Allocate more memory. */
  double cprob[40] = { 0.0 };
  double count[40] = { 0.0 };

  /* populate count array with values calculated above*/
  for (size_t i = 0; i < 40; i+=10) {
    for (size_t j = 0; j < 10; j++) {
      count[i + j] = prob[j] * n;
    }
  }

  /* compute conditional probabilities */
  for (size_t w = 0; w < 4; w++) {
    for (int k = 0; k < 10; k++) {
      for (size_t i = 0; i < m - 1; i++) {
        for (size_t j = 0; j < n; j++) {
          cprob[(w*10) + k] += ((rating[j * m + i] == urating[i]) && (rating[j * m + 4] == (k + 1) / 2.0));
        }
      }
    }
  }

  for (size_t i = 0; i < 40; i++) {
    cprob[i] /= count[i];
  }
  /* compute finalPredictions for each rating */
  double finalPred[10] = { 0.0 };
  for (size_t i = 0; i < 10; i++) {
    finalPred[i] = prob[i] * cprob[i] * cprob[i + 10] * cprob[i + 20] * cprob[i + 30];
  } 
  /* Find index of maximum (most-likely) value in finalPredictions */
  double max = finalPred[0];
  double prediction = 0.0;
  for (size_t i = 1; i < 10; i++) {
    if (finalPred[i] > max) {
        max = finalPred[i];
	prediction = i;
      }
  }
  /* Output prediction. */
  printf("The predicted rating for movie five is %.1lf.\n", (prediction + 1) / 2.0);

  /* Deallocate memory. */
  free(rating);
  free(urating);

  return EXIT_SUCCESS;
}
