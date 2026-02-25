# CS471 Optimization Project 3
# Zackary Lee 
# Central Washington University

----------------------- Project Overview -----------------------
This project selected ten standard benchmark functions with different properties. These functions included: Schwefel, 1st De Jong's, 
Rosenbrock's, Rastrigin, Griewank, Sine Envelope Sine Wave, Stretched V Sine Wave, Ackley's One, Ackley's Two, and Egg Holder. Using these 
ten functions, this project performs blind search with a Mersenne Twister pseudo-random number generator to generate solution vectors.

All ten of these functions are scalable, and this project runs five different algorithms to help optimize these vectors and their fitnesses 
to get them lower to their functions global optima. The two new algorithms that are being introduced in this project are Differential Evolution (DE) and 
Particle Swarm Optimization (PSO). Using these two algorithms now, we can test them against each other to find which one provides the best fitness values 
with statistical analysis performed on the DE and the PSO. 

This project contains source code for the algorithms, the functions, population of the vectors, and fitness values stored in a CSV file. A Jupyter Notebook 
file to compute the statistical analysis. A LaTeX report on the statistical analysis and my findings from completing this project. 

----------------------- Requirements -----------------------
C++ compiler supporting C++ 17 standard or newer
Python 3.8 or newer
Python Packages:
    -NumPy
    -pandas
    -matplotlib
    -SciPy
    -Jupyter
Operating system: Linux or Windows

----------------------- File Layout -----------------------
Project3/
|-- BlindSearch.cpp
|-- BlindSearch.h
|-- BlindSearch.txt
|-- LocalSearch.cpp
|-- LocalSearch.h
|-- LocalSearch.txt
|-- RepeatedLocalSearch.cpp
|-- RepeatedLocalSearch.h
|-- Population.cpp
|-- Population.h
|-- Functions.cpp
|-- Functions.h
|-- DE.cpp
|-- DE.h
|-- DE.txt
|-- PSO.cpp
|-- PSO.h
|-- PSO.txt
|-- ProjectMain.cpp
|-- Run_Functions.py
|-- Project3Analysis.ipynb
|-- Project_3.pdf
|-- READ_ME.txt
|-- Results_CSV/
|   |-- DEResults10.csv
|   |-- DEResults20.csv
|   |-- DEResults30.csv
|   |-- PSOResults10.csv
|   |-- PSOResults20.csv
|   |-- PSOResults30.csv
|-- Documentation/
|   |-- CS471_Proj3.pdf
|   |-- CS471_Project3.pdf
----------------------- Compilation Steps -----------------------
1. Using the terminal, navigate to where the source code/files are located

2. Once in the correct directory, compile the source code using this line:
    - Linux: g++ -std=c++17 -O2 -Wall -Wextra ProjectMain.cpp Population.cpp Functions.cpp BlindSearch.cpp LocalSearch.cpp RepeatedLocalSearch.cpp DE.cpp PSO.cpp -o ProjectMain
    - Windows: g++ -std=c++17 -O2 -Wall -Wextra ProjectMain.cpp Population.cpp Functions.cpp BlindSearch.cpp LocalSearch.cpp RepeatedLocalSearch.cpp DE.cpp PSO.cpp -o ProjectMain.exe

3. Run the Python script with the desired function:
    - Linux: python3 Run_Functions.py 
    - Windows: python Run_Functions.py 
    - This will prompt you with which algorithm to run, which strategy you want to run if you are doing DE, function you want to run, and whether 
        you want to append the data to the CSV file or to overwrite the data in the CSV file

    - If you run the Blind Search algorithm, the data will get put into the "BlindSearchResults.csv"
    - If you run the Local and/or Repeated Local Search algorithm, the data will get put into "LocalSearchResults.csv"
    - If you run the Differential Evolution Algorithm, depending on the given dimensions (10, 20, or 30), the data will get put into "DEResults10.csv" or the other 20, or 30 dimension.
    - If you run the Particle Swarm Optimization Algorithm, depending on the given dimensions (10, 20, or 30), the data will get put into "PSOResults10.csv" or the other 20, or 30 dimension.

    - Example 1 non-DE: 
    CS471 Optimization\Project 3> python Run_Functions.py                                                                                              
    Search Algorithm (1, 2, 3, 4, 5): 1
    Problem number (1-10): 8
    Overwrite or append? Enter 0 = overwrite, 1 = append: 1
    Run 1/30: alg=1 n=30 problem=8 csv=BlindSearchResults.csv append=1 runtime_ms=0.000000
                    (Output Continues for 30 total cases)
    - Example 2 DE: 
    CS471 Optimization\Project 3> python Run_Functions.py                                                                                              
    Search Algorithm (1, 2, 3, 4, 5): 4
    DE Strategy (1-10): 4
    Problem number (1-10): 4
    Overwrite or append? Enter 0 = overwrite, 1 = append: 1
    Run 1/30: alg=4 n=30 problem=4 strategy=4 csv=DEResults30.csv append=1 runtime_ms=490.074188
                    (Output Continues for 30 total cases)

    - If you desire Blind Search type "1" for the search algorithm
    - If you desire Local Search type "2" for the search algorithm
    - If you desire Repeated Local Search type "3" for the search algorithm
    - If you desire Differential Evolution type "4" for the search algorithm
        - For the 10 different DE strategies they are numbered 1-10
    - If you desire Particle Swarm Optimization type "5" for the search algorithm
    - if the desired result is to get all 10 functions and their fitness values, re-run step 3 starting at 
    function 1, with an overwrite value of 0 to clear the CSV file, then proceed with re-running step 
    4 with function number 2-10 and an overwrite value of 1 to append the data to the CSV file. 
    - If full DE vs PSO analysis is desired, repeat step 3 for algorithms 4 and 5, for functions 1-10. 

----------------------- Editing the Python Script/Input Files -----------------------
1. To edit the number of repeated runs for each experiment call, open `Run_Functions.py` and change `NUM_RUNS`.

2. To edit DE settings, open `DE.txt`:
    - `m`: problem dimension (10, 20, or 30). This controls which DE output file is generated (`DEResults10.csv`, `DEResults20.csv`, or `DEResults30.csv`).
    - `n`: shared evaluation/population setting read from file; DE search size is mainly controlled by `np`.
    - `np`: DE population size (number of candidate vectors per generation).
    - `cr`: crossover rate in [0,1], controls how much mutant information is copied into the trial vector.
    - `f`: differential weight (mutation scale), controls mutation step size.
    - `lamda`: scaling factor used by the rand-to-best strategies.
    - `generations`: total DE iterations (larger value increases search effort and runtime).
    - Problem bounds lines (`1 lo hi` through `10 lo hi`): search range for each benchmark function.

3. To edit PSO settings, open `PSO.txt`:
    - `m`: problem dimension (10, 20, or 30). This controls which PSO output file is generated (`PSOResults10.csv`, `PSOResults20.csv`, or `PSOResults30.csv`).
    - `n`: shared evaluation/population setting read from file; PSO swarm size is mainly controlled by `particles`.
    - `particles`: number of swarm particles.
    - `c1`: cognitive coefficient (how strongly a particle moves toward its personal best).
    - `c2`: social coefficient (how strongly a particle moves toward the global best).
    - `generations`: total PSO iterations (larger value increases search effort and runtime).
    - Problem bounds lines (`1 lo hi` through `10 lo hi`): search range for each benchmark function.

4. To edit Blind Search settings, open `BlindSearch.txt` (`m`, `n`, and per-problem bounds).

5. To edit Local Search / Repeated Local Search settings, open `LocalSearch.txt`:
    - `m`, `n`, and per-problem bounds.
    - `a` (alpha): local search step-size parameter.
    - `repeats`: number of repeated-local-search restarts.

----------------------- Statistical Analysis-----------------------
The statistical analysis of the DE and PSO result files is performed in the Jupyter Notebook file "Project3Analysis.ipynb"

1. Ensure all DE and PSO CSV files have been generated for dimensions 10, 20, and 30.
2. From the project directory, launch the "Project3Analysis.ipynb" notebook:
    - Linux: jupyter notebook Project3Analysis.ipynb
    - Windows: jupyter notebook Project3Analysis.ipynb
3. Run all cells in the notebook to compute:
    - DE summary tables (Avg, Std, Time) for 10/20/30 dimensions
    - PSO summary tables (Avg, Std, Time) for 10/20/30 dimensions
    - DE vs PSO significance tables (t-value, p-value, p < 0.05)

----------------------- Errors/issues Encountered -----------------------
- For PSO, if particle velocity is not damped correctly, particles can leave the search bounds. With a damping factor of 0.05, I have not observed this issue.
- For both DE and PSO, if generations is close to or less than the population size, results are usually weaker because there are not enough iterations for 
vectors/particles to move toward the global optimum.
