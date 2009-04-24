#!/usr/bin/env bash

#Ubuntu linus desktop
#Dual-core intel processor
#2 GB of RAM

CWD=/home/omiyat/git/parashader/src
OBJECT=/home/omiyat/git/parashader/objects/reflective_spheres.obj
NODES=2
MPIOPTS="-np ${NODES}"

function command() {
	time=$(date +%s)
	echo /usr/bin/time mpirun ${MPIOPTS} ${CWD}/render -input ${OBJECT} -background_color 0.2 0.1 0.6 -cam_pos 4 1 4 -output ${time}.ppm "$@" > ${time}.log 2>&1
	/usr/bin/time mpirun ${MPIOPTS} ${CWD}/render -input ${OBJECT} -background_color 0.2 0.1 0.6 -cam_pos 4 1 4 -output ${time}.ppm "$@" >> ${time}.log 2>&1
}

cd ${CWD}

command -num_bounces 1 -num_shadow_samples 10 -size 200 200
command -num_bounces 2 -num_shadow_samples 10 -size 200 200
command -num_bounces 3 -num_shadow_samples 10 -size 200 200
command -num_bounces 4 -num_shadow_samples 10 -size 200 200
command -num_bounces 5 -num_shadow_samples 10 -size 200 200
exit 0
