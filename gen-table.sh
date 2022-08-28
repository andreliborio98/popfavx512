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
	echo '-h'
	echo 'show this message!'
}


function genTable {

	[ -z "${OPF_FLAVORS}" ] && OPF_FLAVORS=${OPF_FLAVORS_DEFAULT}
	[ -z "${DATASETS}" ]    && DATASETS=${DATASETS_DEFAULT}
	[ -z "${NCORES}" ]      && NCORES=${NCORES_DEFAULT}

	[ ! -d $TABLEDIR ] && mkdir -p $TABLEDIR
	[ ! -d $GRAPHDIR ] && mkdir -p $GRAPHDIR

	sed 's|[ |\$]|\n|g' <<<${NCORES} > ncores.column

	for dataset in ${DATASETS}; do
		tablefile="${TABLEDIR}/${dataset}.table"
		columnfiles=""
		for flavor in ${OPF_FLAVORS}; do
			columnfile="${TABLEDIR}/${flavor}.column"
			columnfiles="$columnfiles $columnfile"
	    for cores in ${NCORES}; do
				logfile="${LOGDIR}/${dataset}-${flavor}-${cores}.log"
				grep -e '^Learning[ ]\+time:[ ]\+' $logfile | \
					awk '{print $3}' | ./mean_ci.R  >> $columnfile
	    done # FOR EACH NUMBER OF CORES
		done # FOR EACH OPF FLAVOR
    echo "Cores List CI List-AVX CI List-SIMD CI" > $tablefile
		paste -d' ' ncores.column $columnfiles >> $tablefile
		rm -rf $columnfiles
    
		# plot graph
	  TITLE=${dataset}                   \
		INPUT=${tablefile}                 \
		OUTPUT=${GRAPHDIR}/${dataset}      \
		COLS=$(head -1 $tablefile | wc -w) \
    gnuplot bar.gnu
	done # FOR EACH DATA SET
	rm -rf ncores.column
}

while getopts "f:d:c:h" opt;
do
  case $opt in
		f) OPF_FLAVORS=$OPTARG ;;
		d) DATASETS=$OPTARG ;;
		c) NCORES=$OPTARG ;;
		h) usage && exit 0 ;;
		\?) echo $0" : error - invalid option -- $OPTARG" && exit 1 ;;
	esac
done

genTable
