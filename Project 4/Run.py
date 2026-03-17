# Zack Lee
# CS 471 - Optimization
# Run.py
# Compile and run the Project 4 C++ program.

import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent
SRC_DIR = PROJECT_ROOT / "src"
BUILD_DIR = PROJECT_ROOT / "build"
EXECUTABLE = BUILD_DIR / "project4.exe"
DATA_DIR = PROJECT_ROOT / "Taillard_TestData"


def compile_program():
    BUILD_DIR.mkdir(exist_ok=True)

    compile_cmd = [
        "g++",
        "-std=c++17",
        "-O2",
        str(SRC_DIR / "main.cpp"),
        str(SRC_DIR / "core.cpp"),
        str(SRC_DIR / "NEH.cpp"),
        str(SRC_DIR / "ACO.cpp"),
        str(SRC_DIR / "RunExperiments.cpp"),
        "-lstdc++fs",
        "-o",
        str(EXECUTABLE),
    ]

    result = subprocess.run(
        compile_cmd,
        capture_output=True,
        text=True,
        cwd=PROJECT_ROOT,
    )

    if result.returncode != 0:
        print("Compilation failed:")
        print(result.stderr)
        sys.exit(1)

    print("Compilation successful.")


def run_program():
    run_cmd = [str(EXECUTABLE), str(DATA_DIR)]

    result = subprocess.run(run_cmd, cwd=PROJECT_ROOT)

    if result.returncode != 0:
        print("Program execution failed.")
        sys.exit(1)

    print("Program executed successfully.")


def main():
    print("Compiling Project 4...")
    compile_program()

    print("Running experiments...")
    run_program()

    print("All done.")
    print("Results are saved in the 'output' directory.")


if __name__ == "__main__":
    main()
