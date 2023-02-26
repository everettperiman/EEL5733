import subprocess
import statistics
import csv

def export_results(data):
    count = 0
    csv_name = "_threads_test.csv"
    for i in data:
        count = count + 1
        with open(str(count) + csv_name, 'w') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerows(i)

def run_test(test_range_upper, averaging_runs):
    thread_count_test_range = list(range(18, test_range_upper))
    global_results = []
    for i in thread_count_test_range:
        local_job_results = []
        for j in range(averaging_runs):
            result = subprocess.run(['/home/tux/EEL5733/Assignment3/C_Code/./transProg', str(i), '/home/tux/EEL5733/Assignment3/C_Code/input.txt'], stdout=subprocess.PIPE).stdout.decode('utf-8')
            job_results = [int(i.split(" ")[3]) for i in result.split("\n")[:-1]]
            job_results.sort(reverse=True)
            local_job_results.append(job_results)
        global_results.append(local_job_results)
    return global_results

def main():
    x = run_test(20, 200)
    export_results(x)

if __name__ == "__main__":
    main()