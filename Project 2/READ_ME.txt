# CS471 Optimization Project 2
# Zackary Lee 
# Central Washington University

----------------------- Project Overview -----------------------
This project selected ten standard benchmark functions with different properties. These functions included: Schwefel, 1st De Jong's, 
Rosenbrock's, Rastrigin, Griewangk, Sine Envelope Sine Wave, Stretch V Sine Wave, Ackley's One, Ackley's Two, and Egg Holder. Using these 
ten functions, this project performs blind search with a Mersenne Twister pseudo-random number generator to generate solution vectors.

All ten of these functions are scalable, and this project we run three different algorithms to help optimize these vectors and their fitnesses 
to get them lower to their functions global optima. The three algorithms that will be include in this project is Blind Search, Local Search, and 
Repeated Local Search. Using these three algorithms now, we can test them against each other to find which one provides the best fitness values 
with statistical analysis performed on the Blind Algorithm and the Repeated Local Search only. 

This project contains source code for the algorithms, the functions, population of the vectors, and fitness values stored in a CSV file. A Jupyter Notebook 
file to compute the statistical analysis. A LaTeX report on the statistical analysis and my findings from completing this project. 

----------------------- Requirements -----------------------
C++ compiler supporting C++ 17 standard or newer
Python 3.8 or newer
Python Packages:
    -NumPy
    -pandas
    -matplotlib
    -Jupyter
Operating system: Linux or Windows

----------------------- File Layout -----------------------
Project2/
│
├── src/
│   ├── ProjectMain.cpp
│   ├── Population.cpp
│   ├── BlindSearch.cpp
│   ├── LocalSearch.cpp
│   ├── RepeatedLocalSearch.cpp
│   └── Functions.cpp
│
├── include/
│   ├── Population.h
│   ├── BlindSearch.h
│   ├── LocalSearch.h
│   ├── RepeatedLocalSearch.h
│   └── Functions.h
│
├── scripts/
│   └── Run_Functions.py
│
├── docs/
│   ├── CS471_Project2.pdf
│   ├── CS471_Proj2.pdf
│   
│       
│
├── Reslts_CSV/
│   └── BlindSearchResults.csv
│   └── LocalSearchResults.csv
│ 
│ 
│ 
├── notes/
│   ├── BlindSearch.txt
│   └── LocalSearch.txt
│
└── README.txt

----------------------- Compilation Steps -----------------------
1. Using the terminal, navigate to where the source code/files are located

2. Once in the correct directory, compile the source code using this line:
    - Linux: g++ -std=c++17 ProjectMain.cpp Population.cpp Functions.cpp BlindSearch.cpp LocalSearch.cpp RepeatedLocalSearch.cpp -o ProjectMain
    - Windows: g++ -std=c++17 ProjectMain.cpp Population.cpp Functions.cpp BlindSearch.cpp LocalSearch.cpp RepeatedLocalSearch.cpp -o ProjectMain.exe

3. Run the compiled ProjectMain file:
    - Linux: ./ProjectMain <Function Number> results.csv 0
    - Windows: ./ProjectMain.exe <Function Number> results.csv 0
    - This will run a single trial of the program to print out one iteration of the function chosen and append the fitness and time to
    the "results.csv" file.

4. Run the Python script with the desired function:
    - Linux: python3 Run_Functions.py 
    - Windows: python Run_Functions.py 
    - This will prompt you with which which algorithm to run, function you want to run, and whether you want to append the data to the CSV file 
        file or to overwrite the data in the CSV file
    - If you run the Blind Search algorithm, the data will get put into the "BlindSearhResults.csv"
    - If you run the Local and/or Repeated Local Search algorithm, the data will get put into "LocalSearchResults.csv"
    - Example: 
    CS471 Optimization\Project 2> python Run_Functions.py                                                                                              
    Search Algorithm (1, 2, or 3): 1
    Problem number (1-10): 8
    Overwrite or append? Enter 0 = overwrite, 1 = append: 1
    Run 1/30: alg=1 n=30 problem=8 csv=BlindSearchResults.csv append=1 runtime_ms=0.000000
                    (Output Continues for 30 total cases)

    - If you desire Blind Search type "1" for the search algorithm
    - If you desire Local Search type "2" for the search algorithm
    - If you desire Repeated Local Search type "3" for the search algorithm
    - if the desired result is to get all 10 functions and their fitness values, re-run step 4 starting at 
    function 1, with an overwrite value of 0 to clear the CSV file, then proceed with re-running step 
    4 with function number 2-10 and an overwrite value of 1 to append the data to the CSV file. 
    - If full analysis is desired, repeat step 4 with both algorithms 1 and 3, for functions 1-10. 

----------------------- Editing the Python Script/Input Files -----------------------
1. To edit the number of iterations and/or total number of runs. Open the Run_Functions.py and on line 13 there will be a constant for the number
of runs.
2. To edit the number of rows of the matrix, open the BlindSearch.txt and/or LocalSearch.txt and on line one there will be variable m for the number of rows, it 
will initially be set to 30 but may be changed to 10, 20, or 30 depending on the users desired matrix dimensions.
3. For Local Search and Repeated Local Search, on line 3, the variable a represents the alpha variable in the Local and Repeated Local Search algrothims,
this variale dictates the step size or the change in a vectors values after iterating through Local and Repeated Local Search. This value will be set to the 
optimal step value found, but if you desire to test different step sizes, replace the number next to the "a" variable.
3. To edit the number of iterations for a repeated local search, open the LocalSearch.txt and on line four, there will be a a "repeats" variable where the default
value is set to 30.


----------------------- Statistical Analysis-----------------------
The statistical analysis of the "BlindSearchResult.csv" and "LocalSearchResults.csv" files is performed in the Jupyter Notebook file "Project2Analysis.ipynb"

1. Ensure both CSV files have been fully filled out with all (1-10) functions and have been appended to it.
2. From the project directory, launch the "Project2Analysis.ipynb" notebook:
    - Linux: jupyter notebook Project2Analysis.ipynb
    - Windows: jupyter notebook Project2Analysis.ipynb
3. Run all cells in the notebook to compute and display all the statistical results.