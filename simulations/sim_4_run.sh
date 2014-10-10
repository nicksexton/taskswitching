#!/bin/bash

./sim_4_clean.sh
./sim_4_asymmetric.py

../3task_basic -t sim_4_trials.conf -m sim_4_model_asymmetric.conf

mv 3task_act.txt sim_4_act.txt
mv 3task_data.txt sim_4_data.txt
