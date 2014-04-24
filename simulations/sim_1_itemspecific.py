#!/usr/bin/python

# simulation 1 - item-specific priming effects
# replicating effects in experiment 5 of Allport & Wylie 2000
# straight replication of G&S simulation, see Gilbert & Shallice 2002



def write_trial (block_id, trial_id, wordin, colourin, task, param1, param2):
    f = open ("sim_1_test.txt", "a") # opens file for appending
    f.write (str(block_id))
    f.write ("\t")

    f.write (str(trial_id))
    f.write ("\t")

    f.write (str(wordin))
    f.write ("\t")

    f.write (str(colourin))
    f.write ("\t")

    f.write (str(task))
    f.write ("\t")

    f.write (param1)
    f.write ("\t")

    f.write (param2)
    f.write ("\t\n")

    f.close()


write_trial (0, 1, 1, 2, 0, "HebP=1", "")
