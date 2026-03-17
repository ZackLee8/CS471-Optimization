// Zack Lee
// 03/09/2026
// main.cpp

#include "RunExperiments.h"

// Include statements
#include <exception>
#include <iostream>
#include <string>

// Main entry point for the application. Initializes the RunExperiments class
// and executes the runAll method to perform the experiments.
int main(int argc, char* argv[]) {
    try {
        const std::string dataDirectory =
            (argc > 1) ? argv[1] : "Taillard_TestData";

        RunExperiments runner(dataDirectory);
        runner.runAll();

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
