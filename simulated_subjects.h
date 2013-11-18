#ifndef simulated_subjects_h
#define simulated_subjects_h

#include <glib.h>

typedef enum {NONE, FIXED, MIXED} blocktype;



typedef struct subject {

  GArray * trials;
  int num_of_trials;
  void * params; 

} subject; 


typedef struct subject_popn {

  GArray * subjects;
  int number_of_subjects;

} subject_popn;



void subject_free (subject * subject_to_free);
subject_popn * subject_popn_create (int number_of_subjects);
void subject_popn_free (subject_popn * some_subjects);


#endif 
