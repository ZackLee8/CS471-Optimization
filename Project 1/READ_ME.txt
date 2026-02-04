# CS471 Optimization Project 1

----------------------- Project Overview -----------------------
This project selected ten standard benchmark functions with different properties. These functions included: Schwefel, 1st De Jong's, 
Rosenbrock's, Rastrigin, Griewangk, Sine Envelope Sine Wave, Stretch V Sine Wave, Ackley's One, Ackley's Two, and Egg Holder. Using these 
ten functions, this project performs blind search with a Mersenne Twister pseudo-random number generator to generate solution vectors.

All ten of these functions are scalable, and this project generates at least 30 pseudo-random solution vectors to test these ten 
standard benchmark functions. After the solution vectors are solved and the fitness of the functions are given, statistical analysis using 
Python will be used to find out the average, standard deviation, range, and total time in milliseconds. 

This project contains source code for the functions, population of the vectors, and fitness values stored in a CSV file. A Jupyter Notebook file 
to compute the statistical analysis. A LaTeX report on the statistical analysis and my findings from completing this project. 

----------------------- Requirements -----------------------
C++ compiler supporting C++ 17 standard or newer
Python 3.8 or newer
Python Packages:
    -NumPy
    -pandas
    -matplotlib
    -Jupyter
Operating system: Linux or Windows

----------------------- Compilation Steps -----------------------
1. Using the terminal, navigate to where the source code/files are located

2. Once in the correct directory, compile the source code using this line:
    - Linux: g++ Project1Main.cpp Population.cpp Functions.cpp -std=c++17 -O2 -o Project1Main
    - Windows: g++ Project1Main.cpp Population.cpp Functions.cpp -std=c++17 -O2 -o Project1Main.exe

3. Run the compiled Project1Main file:
    - Linux: ./Project1Main <Function Number> results.csv 0
    - Windows: ./Project1Main.exe <Function Number> results.csv 0
    - This will run a single trial of the program to print out one iteration of the function chosen and append the fitness and time to
    the "results.csv" file.

4. Run the Python script with the desired function:
    - Linux: python3 Run_Functions.py 
    - Windows: python Run_Functions.py 
    - This will prompt you with which function you want to run, and whether you want to append the data to the "results.csv" file or
    to overwrite the data in the "results.csv" file
    - Example: 
    CS471 Optimization\Project 1> python Run_Functions.py
    Problem number (1-10): 8
    Overwrite or append? Enter 0=overwrite, 1=append: 1
    Run 1/30: n=30 problem=8 csv=results.csv append=1 runtime_ms=0.000000
                    (Output Continues for 30 total cases)

    - if the desired result is to get all 10 functions and their fitness values, re-run step 4 starting at 
    function 1, with an overwrite value of 0 to clear the "results.csv" file, then proceed with re-running step 
    4 with function number 2-10 and an overwrite value of 1 to append the data to the "results.csv" file.

----------------------- Editing the Python Script/InputFile.txt -----------------------
1. To edit the number of iterations and/or total number of runs. Open the Run_Functions.py and on line 9 there will be a constant for the number
of runs.
2. To edit the number of rows of the matrix, open the InputFile.txt and on line one there will be variable m for the number of rows, it 
will initially be set to 30 but may be changed to 10, 20, or 30 depending on the users desired matrix dimensions.

----------------------- Statistical Analysis-----------------------
The statistical analysis of the "results.csv" file is performed in the Jupyter Notebook file "Project1Analysis.ipynb"

1. Ensure "results.csv" has been fully filled out with all (1-10) functions and have been appended to it.
2. From the project directory, launch the "Project1Analysis.ipynb" notebook:
    - Linux: jupyter notebook Project1Analysis.ipynb
    - Windows: jupyter notebook Project1Analysis.ipynb
3. Run all cells in the notebook to compute and display all the statistical results.