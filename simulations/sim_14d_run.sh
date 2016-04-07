#!/bin/bash

# ./sim_3_clean.sh
rm sim_14_data.txt
rm sim_14_log.txt
rm sim_14_trials.conf
rm sim_14_lookup.txt

./sim_14_trials.py -n 500 # use 100 for full simulation

# ../3task_basic -t sim_3_trials.conf -m sim_9_model_symmetric.conf > sim_9_log.txt
../3task_basic_koch_conflict -t sim_14_trials.conf -m sim_14d_model_symmetric_low.conf > sim_14_log.txt

# mv 3task_act.txt sim_9_act.txt
rm 3task_act.txt 
mv 3task_data.txt sim_14d_data_low.txt

# ../3task_basic -t sim_3_trials.conf -m sim_9_model_symmetric.conf > sim_9_log.txt
../3task_basic_koch_conflict -t sim_14_trials.conf -m sim_14d_model_symmetric_high.conf > sim_14_log.txt

# mv 3task_act.txt sim_9_act.txt
rm 3task_act.txt 
mv 3task_data.txt sim_14d_data_high.txt
