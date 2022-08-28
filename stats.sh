#!/bin/bash

#LIBS="LibOPF-Parallel/bin/opf_train_list \
#LibOPF-Parallel/bin/opf_train_listvect LibOPF/bin/opf_train_parallel LibOPF/bin/opf_train_parvect"

LIBS="LibOPF-Parallel/bin/opf_learn_list \
LibOPF-Parallel/bin/opf_learn_listvect LibOPF/bin/opf_learn_parallel \
LibOPF/bin/opf_learn_parvect"

#APPS="susy"
APPS="susy miniboone letter statlog" 

SAIDADIR="resultados"
SAIDA="saida"

REPODIR="repo"
  
TEMPFILE="tmp"

OUTPUTFILE="iterations"




  echo "Iteration results" > $OUTPUTFILE


  for app in $APPS; do

    echo "$app" >> $OUTPUTFILE
    echo "Cores   NoPQ    NoPQ-Vect  WithPQ  WithPQ-Vect" >> "$OUTPUTFILE"

    #for cores in 1 2 4 8 12 16 20 24 28 32 40 48 56 64; do
    #for cores in {1..8}; do
    for cores in 1 3 6 12 18 24; do
    
		  BUFFER=""
      for lib in $LIBS; do

        IFS="/"
        tokens=( $lib );
        unset IFS

        libname=${tokens[2]}

        appname=${app}-${libname}

        filename="${SAIDADIR}/${appname}-${cores}"
	
#        echo "" > $TEMPFILE

        echo $filename
			  awk 'BEGIN{ counter = 0; nf = 0; }
           {
             if (FNR == 1) {
               nf++;
             }
             if ($1 == "running" && $2 == "iteration") { counter++; }
           }
           END { 
             if (nf != 0)
					     printf " %.2f", counter/nf
             else
               printf " 0"
					 }
			    ' $filename-* > $TEMPFILE
			  
        AOUT=`cat $TEMPFILE`
        
        BUFFER="$BUFFER $AOUT"
     
        #echo "$BUFFER"
      done; # libs
    
      echo "$cores $BUFFER" >> "$OUTPUTFILE"

    done;  # cores
	 
    echo "-------------------" >> $OUTPUTFILE
    echo "  " >> $OUTPUTFILE
	

  done;




