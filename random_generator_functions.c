#include <gsl/gsl_rng.h>
#include <time.h>

gsl_rng * random_generator_create ()
{
  gsl_rng * r;
  const gsl_rng_type *R;
  R = gsl_rng_default;
  r = gsl_rng_alloc (R);
  gsl_rng_env_setup ();
  gsl_rng_set (r, time(0));  // set random seed, default is 0, prob ok for this application
  return r;
}


void random_generator_free (gsl_rng *r)
{
  gsl_rng_free (r);
}

