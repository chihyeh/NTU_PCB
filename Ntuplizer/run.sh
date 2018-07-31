#!/bin/bash
make
ls rawdata/Module_1_RUN_050518_0444.raw>runinfo.log
./Ntuplizer
