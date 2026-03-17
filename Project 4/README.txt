# CS471 Optimization Project 4
# Zackary Lee 
# Central Washington University

----------------------- Project Overview -----------------------
This project solves two production scheduling problems: Flow Shop Scheduling (FSS) and Flow Shop Scheduling with Blocking (FSSB). 
For both problems, this project implements the Nawaz-Enscore-Ham (NEH) heuristic and the Ant Colony Optimization (ACO) metaheuristic 
to generate job schedules for the Taillard benchmark instances.

The input data consists of 120 Taillard problem instances. Each instance contains the number of machines, the number of jobs, and the 
processing times of every job on every machine. Using these instances, this project runs both NEH and ACO on FSS and FSSB, then records 
the generated job sequence, the makespan, and the execution time for each run. The results are stored in CSV files for later analysis 
and reporting. 

This project contains the source code for the shared scheduling utilities, the NEH heuristic, the ACO metaheuristic, the experiment runner, 
and the Python script used to compile and execute the project. The project also includes the Taillard input data files, the CSV results files, 
and the LaTeX report / Doxygen PDF documents generated for submission. 

----------------------- Requirements -----------------------
C++ compiler supporting C++ 17 standard or newer
Python 3.8 or newer
Operating system: Linux or Windows

----------------------- File Layout -----------------------
Project4/
|-- Run.py
|-- READ_ME.txt
|-- CS471_Project4.pdf
|-- CS471_Proj4.pdf
|-- src/
|   |-- main.cpp
|   |-- core.cpp
|   |-- core.h
|   |-- NEH.cpp
|   |-- NEH.h
|   |-- ACO.cpp
|   |-- ACO.h
|   |-- RunExperiments.cpp
|   |-- RunExperiments.h
|-- Taillard_TestData/
|   |-- 1.txt
|   |-- 2.txt
|   |-- ...
|   |-- 120.txt
|-- output/
|   |-- neh_fss.csv
|   |-- neh_fssb.csv
|   |-- aco_fss.csv
|   |-- aco_fssb.csv

----------------------- Compilation Steps -----------------------
1. Using the terminal, navigate to where the project files are located

2. Compile the source code manually if desired:
    - Linux: g++ -std=c++17 -O2 src/main.cpp src/core.cpp src/NEH.cpp src/ACO.cpp src/RunExperiments.cpp -lstdc++fs -o project4
    - Windows: g++ -std=c++17 -O2 src/main.cpp src/core.cpp src/NEH.cpp src/ACO.cpp src/RunExperiments.cpp -lstdc++fs -o project4.exe

3. Run the Python script to compile and execute the full project:
    - Linux: python3 Run.py
    - Windows: python Run.py

    - The Python script compiles the C++ project and runs all experiments on the 120 Taillard instances
    - The program automatically solves all four combinations:
        - NEH + FSS
        - NEH + FSSB
        - ACO + FSS
        - ACO + FSSB
    - The generated CSV files are saved in the "output" directory

4. If running the compiled executable manually:
    - Linux: ./project4 Taillard_TestData
    - Windows: .\project4.exe Taillard_TestData

----------------------- Output Files -----------------------
The program generates four CSV files in the "output" directory:

    - neh_fss.csv
    - neh_fssb.csv
    - aco_fss.csv
    - aco_fssb.csv

Each CSV file contains:
    - Instance name
    - Problem type
    - Solution method
    - Makespan
    - Execution time in seconds
    - Job sequence

----------------------- Editing the Source Files -----------------------
1. To edit the shared file loading and makespan calculations, open:
    - src/core.h
    - src/core.cpp

2. To edit the NEH heuristic, open:
    - src/NEH.h
    - src/NEH.cpp

3. To edit the ACO metaheuristic and its parameters, open:
    - src/ACO.h
    - src/ACO.cpp

    - The ACO constructor includes parameters for:
        - Number of ants
        - Maximum iterations
        - Alpha
        - Beta
        - Evaporation rate
        - Pheromone deposit
        - Initial pheromone
        - Random seed

4. To edit the full batch experiment workflow, open:
    - src/RunExperiments.h
    - src/RunExperiments.cpp

5. To edit the program entry point, open:
    - src/main.cpp

6. To edit the Python build / run script, open:
    - Run.py

----------------------- Report / Documentation -----------------------
1. The LaTeX report for the project is saved as:
    - CS471_Project4.pdf

2. The Doxygen documentation PDF for the source files is saved as:
    - CS471_Proj4.pdf

3. The report includes:
    - An introduction to FSS and FSSB
    - An introduction to NEH and ACO
    - Makespan and execution time results for the Taillard instances
    - Analysis comparing NEH and ACO on FSS and FSSB

----------------------- Errors/issues Encountered -----------------------
- Older MinGW / g++ versions may not fully support the standard <filesystem> header, so linking with -lstdc++fs may be required.
- ACO generally takes longer to run than NEH because it constructs and evaluates many candidate schedules over multiple iterations.
- The baseline ACO implementation used in this project does not outperform NEH on most Taillard instances, which is expected since NEH is a heuristic specifically designed for flow shop scheduling.
