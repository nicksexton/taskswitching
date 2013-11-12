#include <stdio.h>

double act_gs(double net_input, double old_activation,
              double step_size, double act_max, double act_min) {

  /* TODO - make activation function generic, so that a variety of
     functions can be implmented*/

    double eta;
    if (net_input > 0) {
        eta = net_input * (act_max - old_activation);
    }
    else {
        if (net_input < 0) {
            eta = net_input * (old_activation - act_min);
        }
        else
            eta = 0;
    }


    /*    printf ("activation function returning\n"); */
    
    return  old_activation + (step_size * eta);

}
