This is the source code of the OPF, modified to add parallelism and AVX-512 support on LibOPF-List.

Here is a brief description of the contents:

LibOPF - main path using the original algorithm (with Priority Queue)

LibOPF-List - main path using an alternative algorithm (a parallel list - 
                  no Priority Queue)
                  
repo - contains the datasets. Note that Susy and Miniboone are NOT present due 
        to large size

resultados - log of the executions will be saved in this directory

run.sh - used to run the experiments. You might need to change some of the 
         parameters, such as number of cores and number of times to repeat a run
         
gen-table.sh - generates a table for each application (data set) and plot a 
         graph using gnuplot. The application 'R' is also required
         
stats.sh - calculates the iteration counter (average)

susyminiboone.sh - downloads Susy and MiniBooNE datasets

dataAnalyser/dataAnalyser.sh - interpretates results and creates a log

Before running 'run.sh' you need to manually compile the opf binaries. Just 
enter the corresponding directory and issue a 'make' command. For 'LibOPF' the 
following binaries will be compiled:
  - opf_learn_parallel  -> with PQ and OpenMP
  - opf_learn_paravx   -> with PQ, OpenMP and manual vectorization using AVX instructions
  - opf_learn_parsimd -> with PQ, OpenMP and auto vectorization using OpenMP SIMD clause
  
For 'LibOPF-List':
  - opf_learn_list     -> without PQ and with OpenMP
  - opf_learn_listavx  -> without PQ, OpenMP and manual vectorization using AVX-512 + AVX2 instructions
  - opf_learn_listsimd -> without PQ, OpenMP and auto vectorization using OpenMP SIMD clause


Also notice that binaries will fail to compile if the target processor
does not have AVX support. 

