# Zack Lee
# CS 471 - Optimization
# Run_Functions.py
# This script runs the C++ optimization program multiple times,
# collecting results into a CSV file, either overwriting or appending
# based on user input.
import subprocess
import sys
from turtle import mode

# Configuration for the python script
EXECUTABLE = "ProjectMain.exe"   # Windows: "Project1Main.exe"
OUTPUT_CSV = ""
NUM_RUNS = 30

def main():
    algorithm = input("Search Algorithm (1, 2, 3, 4, 5): ").strip()
    strategy_num = ""
    if(algorithm == "4"): 
        strategy_num = input("DE Strategy (1-10): ").strip()
    problem = input("Problem number (1-10): ").strip()
    mode = input("Overwrite or append? Enter 0 = overwrite, 1 = append: ").strip()

    # Edge case checks to make sure the input is valid
    try:
        p = int(problem)
        if p < 1 or p > 10:
            raise ValueError
    except ValueError:
        print("Problem number must be an integer 1-10.")
        sys.exit(1)

    if mode not in ("0", "1"):
        print("Mode must be 0 (overwrite) or 1 (append).")
        sys.exit(1)
        
    if algorithm not in("1", "2", "3", "4", "5"):
        print("Algorithm must be either 1, 2, 3, 4, or 5")
        sys.exit(1)
    
    if algorithm == "4" and strategy_num not in ("1", "2", "3", "4", "5", "6", "7", "8", "9", "10"):
        print("Strategy number must be an integer 1-10 for differential evolution.")
        sys.exit(1)

    for r in range(1, NUM_RUNS + 1):
        
        # If user chose overwrite, only overwrite on run 1.
        append_flag = mode
        if mode == "0" and r > 1:
            append_flag = "1"

        if (algorithm == "1"):
            OUTPUT_CSV = "BlindSearchResults.csv"
        elif (algorithm == "2" or algorithm == "3"):
            OUTPUT_CSV = "LocalSearchResults.csv"
        elif (algorithm == "4"):
            OUTPUT_CSV = "DEResults.csv"
        elif algorithm == "5":
            OUTPUT_CSV = "PSOResults.csv"
        else:
                print("Invalid algorithm.")
                sys.exit(1)

        # Run NUM_RUNS times
    for run_idx in range(1, NUM_RUNS + 1):
        # overwrite only on run 1 if mode==0, else append
        append_flag = "0" if (mode == "0" and run_idx == 1) else "1"

        if algorithm == "4":
            args = [EXECUTABLE, algorithm, strategy_num, str(p), OUTPUT_CSV, append_flag]
        else:
            args = [EXECUTABLE, algorithm, str(p), OUTPUT_CSV, append_flag]

        proc = subprocess.run(args, capture_output=True, text=True)

        if proc.returncode != 0:
            print(f"\nRun {run_idx} FAILED.")
            print("STDERR:\n" + proc.stderr)
            sys.exit(1)

        print(f"Run {run_idx}/{NUM_RUNS}: {proc.stdout.strip()}")

    print(f"\nComputation is done, Wrote results to: {OUTPUT_CSV}")

if __name__ == "__main__":
    main()
