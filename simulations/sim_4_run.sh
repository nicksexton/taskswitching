#!/bin/bash

./sim_3_clean.sh
./sim_5_clean.sh
./sim_3_n2rc.py -n 500

mv sim_3_trials.conf sim_5_trials.conf
mv sim_3_lookup.txt sim_5_lookup.txt

../3task_basic -t sim_4_trials.conf -m sim_4_model_asymmetric.conf 

mv 3task_act.txt sim_5_act.txt
mv 3task_data.txt sim_5_data.txt
