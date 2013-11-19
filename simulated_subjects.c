

#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "simulated_subjects.h"

// #define NUMBER_OF_SUBJECTS 100






void subject_free (subject * subject_to_free) {

  free (subject_to_free->params);
  g_array_free (subject_to_free->fixed_trials, TRUE); // 2nd arg frees the data as well
  free (subject_to_free);

  return;
}


subject_popn * subject_popn_create (int number_of_subjects) {

  subject_popn * some_subjects;
  some_subjects = malloc (sizeof(subject_popn));
  some_subjects->subjects = g_array_sized_new (FALSE, FALSE, sizeof(subject), number_of_subjects);
  some_subjects->number_of_subjects = number_of_subjects;

  return some_subjects;

}


void subject_popn_free (subject_popn * some_subjects) {

  g_array_free (some_subjects->subjects, TRUE);
  free (some_subjects);

  return;
}
