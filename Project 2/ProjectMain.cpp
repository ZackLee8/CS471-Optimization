// Zack Lee
// ProjectMain.cpp
// 02/03/2026

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <iomanip>

#include "Population.h"
#include "BlindSearch.h"
#include "LocalSearch.h"
#include "RepeatedLocalSearch.h"

// Structure to hold bounds
struct Bounds {
    float min;
    float max;
};

// Skip blank lines or comment lines (# ...) in the txt files for the ranges
static bool skipLine(const std::string& line) {
    for (char c : line) {
        if (c == '#') return true;
        if (!std::isspace(static_cast<unsigned char>(c))) return false;
    }
    return true;
}
int main(int argc, char* argv[])
{
    try {
        
        // argument check for inputs
        if (argc != 5) {
            std::cerr << "Usage: " << argv[0] << " <algorithm 1, 2, 3> <problemType 1-10> <csvFile> <appendFlag 0/1>\n";
            return 1;
        }
        const int algorithm = std::stoi(argv[1]);
        const int problemType = std::stoi(argv[2]);
        const std::string csvFile = argv[3];
        const int appendFlagInt = std::stoi(argv[4]);
        const bool append = (appendFlagInt != 0);

        if (algorithm < 1 || algorithm > 3){
            throw std::runtime_error("Algorithm must be 1, 2, or 3.");
        }
        if (problemType < 1 || problemType > 10) {
            throw std::runtime_error("Problem type must be between 1 and 10.");
        }
        if (appendFlagInt != 0 && appendFlagInt != 1) {
            throw std::runtime_error("appendFlag must be 0 (overwrite) or 1 (append).");
        }


        // choose what file to read based on the algorithm chosen
        std::string inputFile;
        if(algorithm == 1){
            inputFile = "BlindSearch.txt";
        }
        if(algorithm == 2 || algorithm == 3){
            inputFile = "LocalSearch.txt";
        }

        // reading the input file for m, n, and bounds
        std::ifstream in(inputFile);
        if (!in) {
            throw std::runtime_error("Could not open " + inputFile + " (must be in the same folder you run from).");
        }

        int m = -1;
        int n = 30; // default for project
        std::unordered_map<int, Bounds> bounds;
        float alpha = 0.0f;
        int repeats = 1;

        // Read through the file line by line
        std::string line;
        while (std::getline(in, line)) {
            if (skipLine(line)) continue;

            // Use a string stream to parse the line
            std::istringstream iss(line);
            std::string first;
            iss >> first;

            // Check for m, n, alpha, or repeats keywords
            if (first == "m" || first == "M") {
                iss >> m;
                continue;
            }
            if (first == "n" || first == "N") {
                iss >> n;
                continue;
            }
            if (first == "a" || first == "A" || first == "alpha" || first == "ALPHA"){
                iss >> alpha;
                continue;
            }
            if (first == "repeats" || first == "r" || first == "R"){
                iss >> repeats;
                continue;
            }

            int pid = std::stoi(first);
            float lo, hi;
            if (!(iss >> lo >> hi)) {
                throw std::runtime_error("Bad bounds line in " + inputFile + ". Expected: <problemId> <low> <high>");
            }
            bounds[pid] = { lo, hi };
        }

        // check if the alpha for local and repeated local search is a valid alpha
        if (algorithm == 2 || algorithm == 3) {
            if (alpha <= 0.0f) {
                throw std::runtime_error("LocalSearch.txt missing alpha value.");
            }
        }

        // check if repeats is valid for repeated local search
        if (algorithm == 3 && repeats <= 0){
            throw std::runtime_error("Number of repeats must be positive for repeated local search.");
        }


        // edge case checks
        if (m != 10 && m != 20 && m != 30) {
            throw std::runtime_error("m must be 10, 20, or 30 in " + inputFile + " (example: m 20).");
        }
        if (n <= 0) {
            throw std::runtime_error("n must be positive.");
        }
        if (bounds.find(problemType) == bounds.end()) {
            throw std::runtime_error("No bounds found in " + inputFile + " for the selected problem type.");
        }

        // get the bounds for the selected problem type
        const float lower = bounds[problemType].min;
        const float upper = bounds[problemType].max;

        // call the selected algorithm
        algorithmOutput result{};

        switch(algorithm){
            case 1:
                //blindsearch
                result = blindSearch(n, m, lower, upper, problemType, csvFile, append);
                break;
            case 2:
                //localsearch
                result = localSearch(n, m, lower, upper, problemType, alpha, csvFile, append);
                break;
            case 3: 
                //repeated local search
                result = repeatedLocalSearch(n, m, lower, upper, problemType, alpha, repeats, csvFile, append);
                break;
            default:
                throw std::runtime_error("Invalid algorithm.");
        }

        // Get the runtimes of each function and print the n value, the problem type, the csv file name, the append flag, and the runtime in milliseconds
        std::cout << std::fixed << std::setprecision(6)
                  << " alg=" << algorithm
                  << " n=" << n
                  << " problem=" << problemType
                  << " csv=" << csvFile
                  << " append=" << (append ? 1 : 0)
                  << " runtime_ms=" << result.runtimeMs
                  << "\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
}