import sys
import subprocess
import threading
import os

# ANSI escape codes for colors
RED = '\033[0;31m'
GREEN = '\033[0;32m'
NC = '\033[0m'  # No Color

def print_colored(message, color):
    """ Print a message in the specified color """
    print(f"{color}{message}{NC}")

def handle_host(host):
    global num_computers, total_processors, tested_local_host

    if host == current_host and tested_local_host:
        return

    try:
        threads = subprocess.getoutput(f'ssh -o ConnectTimeout=2 -o BatchMode=yes {username}@{host} nproc --all')

        if threads.isdigit():
            with threading.Lock():
                print_colored(f"{host}: Success - {threads} threads", GREEN)
                with open('hosts', 'a') as file:
                    file.write(f"{host} slots={threads}\n")
                num_computers += 1
                total_processors += int(threads)
                if host == current_host:
                    tested_local_host = True
        else:
            print_colored(f"{host}: Failed to connect or retrieve thread count", RED)

    except Exception as e:
        print_colored(f"{host}: Exception occurred", RED)

# Check if username is provided as an argument
if len(sys.argv) < 2:
    print("Usage: {} <username>".format(sys.argv[0]))
    sys.exit(1)

username = sys.argv[1]
current_host = subprocess.getoutput('hostname')

# Define the list of hosts
hosts = [
    current_host,  # Placeholder for the actual current host name
    "MI103-01", "MI103-02", "MI103-03", "MI103-04", "MI103-05",
    "MI103-06", "MI103-07", "MI103-08", "MI103-09", "MI103-10",
    "MI103-11", "MI103-12", "MI103-13", "MI103-14", "MI103-15",
    "MI104-01", "MI104-02", "MI104-03", "MI104-04", "MI104-05",
    "MI104-06", "MI104-07", "MI104-08", "MI104-09", "MI104-10",
    "MI104-11", "MI104-12", "MI104-13", "MI104-14", "MI104-15",
    "MI104-16", "MI104-17", "MI104-18", "MI104-19", "MI104-20",
    "MI105-14", "MI105-13", "MI105-12", "MI105-11", "MI105-10",
    "MI105-09", "MI105-08", "MI105-07", "MI105-06", "MI105-05",
    "MI105-04", "MI105-03", "MI105-02", "MI105-01",
    "MI121-01", "MI121-02", "MI121-03", "MI121-04", "MI121-05",
    "MI121-06", "MI121-07", "MI121-08"
]

if os.path.exists('hosts'):
    os.remove('hosts')

num_computers = 0
total_processors = 0
tested_local_host = False

# Process the first host synchronously
handle_host(hosts.pop(0))

# Create threads for remaining hosts
threads = [threading.Thread(target=handle_host, args=(host,)) for host in hosts]

# Start all threads
for thread in threads:
    thread.start()

# Wait for all threads to complete
for thread in threads:
    thread.join()

# Final output
average_cpus_per_computer = total_processors / num_computers if num_computers > 0 else 0
print(f"Number of computers available: {num_computers}")
print(f"Total processors available: {total_processors}")
if num_computers > 0:
    print(f"Average CPUs per computer: {average_cpus_per_computer}")
else:
    print_colored("No computers tested successfully.", RED)
