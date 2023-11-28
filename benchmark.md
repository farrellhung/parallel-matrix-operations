# Benchmarking

1. If there is an issue with permission run `chmod +x benchmark.sh`
2. Build `pmo.c` before running
3. Run `./benchmark.sh output_file no_of_runs`  
   - e.g. ./benchmark.sh benchmark_blocked.txt 10

`lscpu` -> Intel® Xeon® Processor E7-8860 v3
Total 16 cores, 32 threads
<https://www.intel.com/content/www/us/en/products/sku/84680/intel-xeon-processor-e78860-v3-40m-cache-2-20-ghz/specifications.html>


NUM_THREAD

| Tests | 4      | 8      | 16     | 32     | Test Info                               |
| :---: | ------ | ------ | ------ | ------ | --------------------------------------- |
|   1   | 0.0164 | 0.0195 | 0.0187 | 0.0226 | smaller than 100 * 100, 1 sec timeout   |
|   2   | 0.0175 | 0.0180 | 0.0192 | 0.0224 | smaller than 100 * 100, 1 sec timeout   |
|   3   | 0.0180 | 0.0191 | 0.0204 | 0.0235 | smaller than 100 * 100, 1 sec timeout   |
|   4   | 0.0921 | 0.0964 | 0.0962 | 0.1044 | smaller than 500 * 500, 1 sec timeout   |
|   5   | 0.1517 | 0.1460 | 0.1377 | 0.1711 | smaller than 500 * 500, 1 sec timeout   |
|   6   | 0.1257 | 0.1310 | 0.1291 | 0.1271 | smaller than 500 * 500, 1 sec timeout   |
|   7   | 0.1288 | 0.1253 | 0.1180 | 0.1249 | smaller than 500 * 500, 1 sec timeout   |
|   8   | 1.2403 | 1.2180 | 1.2705 | 1.4647 | smaller than 1000 * 1000, 10 sec timeou |
|   9   | 1.5019 | 1.4838 | 1.6023 | 2.0202 | smaller than 1000 * 1000, 10 sec timeou |
|  10   | 1.2312 | 1.2085 | 1.2445 | 1.4307 | smaller than 1000 * 1000, 10 sec timeou |
