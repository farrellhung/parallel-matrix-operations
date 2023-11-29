# Benchmarking

1. If there is an issue with permission run `chmod +x benchmark.sh`
2. Build `pmo.c` before running
3. Run `./benchmark.sh output_file no_of_runs`  
   - e.g. ./benchmark.sh benchmark_blocked.txt 10

`lscpu` -> Intel® Xeon® Processor E7-8860 v3
Total 16 cores, 32 threads
<https://www.intel.com/content/www/us/en/products/sku/84680/intel-xeon-processor-e78860-v3-40m-cache-2-20-ghz/specifications.html>

NUM_THREAD

| Tests | 4      | 4&8    | 8      | 16     | 32     |
| :---: | ------ | ------ | ------ | ------ | ------ |
|   1   | 0.0164 | 0.0177 | 0.0195 | 0.0187 | 0.0226 |
|   2   | 0.0175 | 0.0176 | 0.0180 | 0.0192 | 0.0224 |
|   3   | 0.0180 | 0.0167 | 0.0191 | 0.0204 | 0.0235 |
|   4   | 0.0921 | 0.0923 | 0.0964 | 0.0962 | 0.1044 |
|   5   | 0.1517 | 0.1604 | 0.1460 | 0.1377 | 0.1711 |
|   6   | 0.1257 | 0.1345 | 0.1310 | 0.1291 | 0.1271 |
|   7   | 0.1288 | 0.1290 | 0.1253 | 0.1180 | 0.1249 |
|   8   | 1.2403 | 1.2114 | 1.2180 | 1.2705 | 1.4647 |
|   9   | 1.5019 | 1.4786 | 1.4838 | 1.6023 | 2.0202 |
|  10   | 1.2312 | 1.2099 | 1.2085 | 1.2445 | 1.4307 |

| TEST | Test Info                | Time Out       | a                                                            |
| ---- | ------------------------ | -------------- | ------------------------------------------------------------ |
| 1    | smaller than 100 * 100   | 1 sec timeout  | small matrix addition                                        |
| 2    | smaller than 100 * 100   | 1 sec timeout  | small matrix multiplication                                  |
| 3    | smaller than 100 * 100   | 1 sec timeout  | small matrix subtraction                                     |
| 4    | smaller than 500 * 500   | 1 sec timeout  | matrix additions/subtractions                                |
| 5    | smaller than 500 * 500   | 1 sec timeout  | consecutive matrix multiplications                           |
| 6    | smaller than 500 * 500   | 1 sec timeout  | combination of matrix additions and matrix multiplications   |
| 7    | smaller than 500 * 500   | 1 sec timeout  | combination of matrix additions and matrix multiplications 2 |
| 8    | smaller than 1000 * 1000 | 10 sec timeout | complex matrix operations                                    |
| 9    | smaller than 1000 * 1000 | 10 sec timeout | complex matrix operations 2                                  |
| 10   | smaller than 1000 * 1000 | 10 sec timeout | complex matrix operations 3                                  |
