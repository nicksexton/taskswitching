#!/bin/bash

./sim_3_clean.sh
./sim_3_n2rc.py

../3task_basic -t sim_3_trials.conf -m sim_3_model_default.conf
