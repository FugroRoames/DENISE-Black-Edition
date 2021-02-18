#!/bin/sh

/usr/bin/time mpirun --allow-run-as-root -n 15 ../bin/denise DENISE_marm_OBC.inp FWI_workflow_marmousi.inp
