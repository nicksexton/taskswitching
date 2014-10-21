#!/bin/bash

./sim_3_clean.sh
./sim_3_n2rc.py -n 100

../3task_basic -t sim_3_trials.conf -m sim_3_model_asymmetric.conf > sim_3_log.txt

mv 3task_act.txt sim_3_act.txt
mv 3task_data.txt sim_3_data.txt
