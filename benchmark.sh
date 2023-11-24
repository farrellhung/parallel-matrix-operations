#! /bin/bash -f

output_file=benchmark_$1.txt
no_of_runs=$2

if [[ -n $1 && -n $2 ]]; then
    if [[ -s $output_file ]]; then
        > $output_file
    fi
    
    for i in $(seq 1 $no_of_runs)
    do
        echo "Run $i" >> ${output_file}
        make run | grep -oE '[0-9]+\.[0-9]+' >> ${output_file}
    done
    python3 benchmark.py --benchmark_result ${output_file} --test_runs ${no_of_runs}

fi
