#!/bin/bash

#./sim_4_clean.sh
#./sim_6_clean.sh

#./sim_4_asymmetric.py -n 2500
#mv sim_4_trials.conf sim_6_trials.conf
#mv sim_4_lookup.txt sim_6_lookup.txt

# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log.txt
../3task_basic_koch_conflict -t ../3task_import_n2_basic.conf -m sim_6_model_asymmetric.conf > sim_6_log.txt

#mv 3task_act.txt sim_6_act.txt
#mv 3task_data.txt sim_6_data.txt