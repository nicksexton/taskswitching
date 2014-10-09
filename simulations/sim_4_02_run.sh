#!/bin/bash
# script which runs the asymmetric 3-task switching protocol with symmetric tasks

./sim_4_clean.sh
./sim_4_asymmetric.py

mv sim_4_lookup.txt sim_4_02_lookup.txt
mv sim_4_trials.conf sim_4_02_trials.conf

../3task_basic -t sim_4_02_trials.conf -m sim_4_02_model_asymmetric.conf
#../3task_basic -t sim_4_02_trials.conf -m sim_4_model_asymmetric.conf # default model params to test results

mv 3task_act.txt sim_4_02_act.txt
mv 3task_data.txt sim_4_02_data.txt
