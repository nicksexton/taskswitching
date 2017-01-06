#!/bin/bash

./sim_3_clean.sh
rm sim_9_data.txt
rm sim_9_log.txt

./sim_3_n2rc_DIAGNOSTIC.py -n 100 # use 100 for full simulation

# ../3task_basic -t sim_3_trials.conf -m sim_9_model_symmetric.conf > sim_9_log.txt
../3task_basic_koch_conflict -t sim_3_trials.conf -m sim_9_model_symmetric.conf > sim_9_log.txt

# mv 3task_act.txt sim_9_act.txt
rm 3task_act.txt 
mv 3task_data.txt sim_9_data_graded.txt
