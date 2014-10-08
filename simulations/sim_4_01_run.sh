#!/bin/bash
# script which runs the asymmetric 3-task switching protocol with symmetric tasks

./sim_4_clean.sh
./sim_4_asymmetric.py

../3task_basic -t sim_4_trials.conf -m sim_4_01_model_symmetric.conf

mv 3task_act.txt sim_4_01_act.txt
mv 3task_data.txt sim_4_01_data.txt
