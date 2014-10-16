#!/bin/bash

./sim_3_clean.sh
./sim_5_clean.sh
./sim_3_n2rc.py -n 50

mv sim_3_trials.conf sim_5_trials.conf
mv sim_3_lookup.txt sim_5_lookup.txt

../3task_basic_koch_conflict -t sim_5_trials.conf -m sim_5_model_default.conf

mv 3task_act.txt sim_5_act.txt
mv 3task_data.txt sim_5_data.txt
