# Zack Lee
# CS 471 - Optimization
# Run_Functions.py
# This script runs the C++ optimization program multiple times,
# collecting results into a CSV file, either overwriting or appending
# based on user input.
import subprocess
import sys

# Configuration for the python script
EXECUTABLE = "./Project1Main"   # Windows: "Project1Main.exe"
OUTPUT_CSV = "results.csv"
NUM_RUNS = 30

def main():
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

    for r in range(1, NUM_RUNS + 1):
        
        # If user chose overwrite, only overwrite on run 1.
        append_flag = mode
        if mode == "0" and r > 1:
            append_flag = "1"

        # Call C++: <problemType> <csvFile> <appendFlag>
        proc = subprocess.run(
            [EXECUTABLE, str(p), OUTPUT_CSV, append_flag],
            capture_output=True,
            text=True
        )

        if proc.returncode != 0:
            print(f"\nRun {r} FAILED.")
            print("STDERR:\n" + proc.stderr)
            sys.exit(1)

        print(f"Run {r}/{NUM_RUNS}: {proc.stdout.strip()}")

    print(f"\nComputation is done, Wrote results to: {OUTPUT_CSV}")

if __name__ == "__main__":
    main()
