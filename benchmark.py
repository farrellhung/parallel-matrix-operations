import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--benchmark_result',
                    help='name of benchmark result file', type=str, required=True)
parser.add_argument('--test_runs',
                    help='number of test runs', type=int, required=True)
args = parser.parse_args()

benchmark_result = args.benchmark_result
test_runs = args.test_runs

# can do something with this for stats
results = [[0 for x in range(test_runs)] for y in range(10)]

with open(benchmark_result, 'r') as f:
    # Until the end of the file
    for i in range(args.test_runs):
        # Read the next line
        line = f.readline()
        print(line)
        for j in range(10): # TEST 1 to 10
            test_result = float(f.readline().rstrip('\n'))
            results[j][i] = test_result
            print(f"{test_result:.3f}")

print(results)
for i in results:
    print(sum(i)/len(i))