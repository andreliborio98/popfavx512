#!/bin/bash

source $PWD/run.cfg
[ $? != 0 ] && echo -e "\nerror: running scripts from wrong directory!" && exit -1

function usage {

	echo $0' <flags bellow>'
	
	echo
	echo '-f <OPF flavor>'
	echo "${OPF_FLAVORS_ALL// / | }"
	echo 'default = ALL'
	echo
	echo '-d <dataset>'
	echo "${DATASETS_ALL// / | }"
	echo 'default = ALL'
	echo
	echo '-c <number of cores>'
	echo '1 | 2 | 4 | etc... | "1 2 4 8" | "4 8 16" | etc...'
	echo "default = ${NCORES_DEFAULT}"
	echo
	echo '-r <number of runs>'
	echo '1 | 5 | 8 | etc... | 100 | 200 | etc...'
	echo "default = ${NRUNS_DEFAULT}"
	echo
	echo '-i <iterations number flag>'
	echo 'set ---> run till score stabilizes and writes iterations file at <DATASETS_DIR>'
	echo 'unset -> use number of iterations in <DATASETS_DIR>/iterations file'
	echo "default = unset"
	echo
	echo '-h'
	echo 'show this message!'
}


function execute {

	[ -z "${OPF_FLAVORS}" ] && OPF_FLAVORS=${OPF_FLAVORS_DEFAULT}
	[ -z "${DATASETS}" ]    && DATASETS=${DATASETS_DEFAULT}
	[ -z "${NCORES}" ]      && NCORES=${NCORES_DEFAULT}
	[ -z "${NRUNS}" ]       && NRUNS=${NRUNS_DEFAULT}

	[ ! -d $LOGDIR ] && mkdir -p $LOGDIR

	for flavor in ${OPF_FLAVORS}; do
	  
		case $flavor in	
			list*)
				appbin=$PWD/LibOPF-List/bin/opf_learn-${flavor}.x ;;
			par*)
				appbin=$PWD/LibOPF-Parallel/bin/opf_learn-${flavor}.x ;;
		esac
	
	  for dataset in ${DATASETS}; do

			[ -z "${ITERATIONS_FLAG}" ] && local iter="$(cat ${DATASETS_DIR}/${dataset}/iterations)" || iter="0"
	
	    for cores in ${NCORES}; do
	       
				logfile="${LOGDIR}/${dataset}-${flavor}-${cores}.log"
	
	      for t in $(seq 1 ${NRUNS}); do
	
	        echo "execution $t: $(basename $appbin) with ${dataset}, $cores cores and $iter learn iterations (logfile = $(basename $logfile))"
	        GOMP_CPU_AFFINITY="$AFFINITY" OMP_NUM_THREADS=${cores} NITERATIONS=$iter \
						${appbin} ${DATASETS_DIR}/${dataset}/{training,evaluating}.dat >> $logfile
					[ -e iterations ] && cat iterations >> ${DATASETS_DIR}/${dataset}/iterations && rm -f iterations

	      done # FOR EACH RUN
	    done # FOR EACH NUMBER OF CORES
	  done # FOR EACH DATA SET
	done # FOR EACH OPF FLAVOR
}

while getopts "f:d:c:r:ia:h" opt;
do
  case $opt in
		f) OPF_FLAVORS=$OPTARG ;;
		d) DATASETS=$OPTARG ;;
		c) NCORES=$OPTARG ;;
		r) NRUNS=$OPTARG ;;
		i) ITERATIONS_FLAG='dummy-value';;
		a) AFFINITY=$OPTARG ;; 
		h) usage && exit 0 ;;
		\?) echo $0" : error - invalid option -- $OPTARG" && exit 1 ;;
	esac
done

[[ -z "$AFFINITY" ]] && echo "error: thread affinity was not specified!" && exit 1

execute
