// Zack Lee
// ProjectMain.cpp
// 02/18/2026

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
#include "DE.h"
#include "PSO.h"

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
        int algorithm = -1;
        int strategy = -1;           // only used for DE (algorithm == 4)
        int problemType = -1;
        std::string csvFile;
        int appendFlagInt = -1;

        // --- Argument parsing ---
        if (argc < 5) {
            std::cerr << "Usage:\n"
                      << "  " << argv[0] << " <alg 1,2,3,5> <problem 1-10> <csvFile> <append 0/1>\n"
                      << "  " << argv[0] << " 4 <strategy 1-10> <problem 1-10> <csvFile> <append 0/1>\n";
            return 1;
        }

        algorithm = std::stoi(argv[1]);

        if (algorithm == 4) {
            // DE needs strategy => argc must be 6
            if (argc != 6) {
                std::cerr << "Usage: " << argv[0]
                          << " 4 <strategy 1-10> <problemType 1-10> <csvFile> <appendFlag 0/1>\n";
                return 1;
            }
            strategy     = std::stoi(argv[2]);
            problemType  = std::stoi(argv[3]);
            csvFile      = argv[4];
            appendFlagInt = std::stoi(argv[5]);

            if (strategy < 1 || strategy > 10) {
                throw std::runtime_error("DE strategy must be between 1 and 10.");
            }
        } else {
            // non-DE => argc must be 5
            if (argc != 5) {
                std::cerr << "Usage: " << argv[0]
                          << " <algorithm 1,2,3,5> <problemType 1-10> <csvFile> <appendFlag 0/1>\n";
                return 1;
            }
            problemType  = std::stoi(argv[2]);
            csvFile      = argv[3];
            appendFlagInt = std::stoi(argv[4]);
        }

        const bool append = (appendFlagInt != 0);

        /* ===== TEMP TEST BLOCK =====
        if (algorithm == 99) {   // use 99 as a debug mode
            int D = 10; // dimension you want to test

            std::vector<float> testVec = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f , 7.0f, 8.0f, 9.0f, 10.0f};

            Population evaluator(1, D);

            float fitness = evaluator.evaluateVector(testVec, problemType);

            std::cout << "Custom vector fitness = " << fitness << "\n";

            return 0; // exit early
        }
        // ===== END TEST BLOCK ===== */

        

        // --- Basic validations ---
        if (algorithm < 1 || algorithm > 5) {
            throw std::runtime_error("Algorithm must be 1, 2, 3, 4, or 5.");
        }
        if (problemType < 1 || problemType > 10) {
            throw std::runtime_error("Problem type must be between 1 and 10.");
        }
        if (appendFlagInt != 0 && appendFlagInt != 1) {
            throw std::runtime_error("appendFlag must be 0 (overwrite) or 1 (append).");
        }

        // --- choose what file to read based on the algorithm chosen ---
        std::string inputFile;
        if (algorithm == 1) {
            inputFile = "BlindSearch.txt";
        } else if (algorithm == 2 || algorithm == 3) {
            inputFile = "LocalSearch.txt";
        } else if (algorithm == 4) {
            inputFile = "DE.txt";
        } else if (algorithm == 5) {
            inputFile = "PSO.txt";
        }

        // reading the input file for m, n, and bounds
        std::ifstream in(inputFile);
        if (!in) {
            throw std::runtime_error("Could not open " + inputFile + " (must be in the same folder you run from).");
        }

        // constants / parsed settings
        int m = -1;
        int n = 30; // default for project
        std::unordered_map<int, Bounds> bounds;
        float alpha = 0.0f;
        int repeats = 1;

        // DE-specific settings (only used if algorithm == 4)
        int NP = -1;
        float CR = -1.0f;
        float F = -1.0f;
        float lamda = -1.0f;

        // DE and PSO generations param (same name for both since they both use it)
        int generations = -1;

        // PSO-specific settings (only used if algorithm == 5)
        int particles = -1; 
        float c1 = -1.0f;
        float c2 = -1.0f;


        // Read through the file line by line
        std::string line;
        while (std::getline(in, line)) {
            if (skipLine(line)) continue;

            std::istringstream iss(line);
            std::string first;
            iss >> first;

            // check for known settings lines (m, n, alpha, repeats, DE params)
            if (first == "m" || first == "M") { iss >> m; continue; }
            if (first == "n" || first == "N") { iss >> n; continue; }
            if (first == "a" || first == "A" || first == "alpha" || first == "ALPHA") { iss >> alpha; continue; }
            if (first == "repeats" || first == "r" || first == "R") { iss >> repeats; continue; }

            // DE params
            if (first == "NP" || first == "np") { iss >> NP; continue; }
            if (first == "CR" || first == "cr") { iss >> CR; continue; }
            if (first == "F" || first == "f") { iss >> F; continue; }
            if (first == "lambda" || first == "lamda" || first == "LAMDA") { iss >> lamda; continue; }

            // DE and PSO generations param (same name for both since they both use it)
            if (first == "generations" || first == "g" || first == "G") { iss >> generations; continue; }

            // PSO params
            if (first == "particles" || first == "p" || first == "P") { iss >> particles; continue; }
            if (first == "c1") { iss >> c1; continue; } 
            if (first == "c2") { iss >> c2; continue; }


            // Otherwise: bounds line
            int pid = std::stoi(first);
            float lo, hi;
            if (!(iss >> lo >> hi)) {
                throw std::runtime_error("Bad bounds line in " + inputFile + ". Expected: <problemId> <low> <high>");
            }
            bounds[pid] = { lo, hi };
        }

        // checks for local / repeated local
        if (algorithm == 2 || algorithm == 3) {
            if (alpha <= 0.0f) {
                throw std::runtime_error("LocalSearch.txt missing alpha value.");
            }
        }
        if (algorithm == 3 && repeats <= 0) {
            throw std::runtime_error("Number of repeats must be positive for repeated local search.");
        }

        // edge case checks for shared settings
        if (m != 10 && m != 20 && m != 30) {
            throw std::runtime_error("m must be 10, 20, or 30 in " + inputFile + " (example: m 20).");
        }
        if (n <= 0) {
            throw std::runtime_error("n must be positive.");
        }
        if (bounds.find(problemType) == bounds.end()) {
            throw std::runtime_error("No bounds found in " + inputFile + " for the selected problem type.");
        }


        // DE parameter checks (only if algorithm == 4)
        if (algorithm == 4) {
            if (NP <= 0) {
                throw std::runtime_error("NP must be positive for differential evolution.");
            }
            if (CR < 0.0f || CR > 1.0f) {
                throw std::runtime_error("CR must be between 0 and 1 for differential evolution.");
            }
            if (F <= 0.0f || F > 2.0f) {
                throw std::runtime_error("F must be > 0 and <= 2 for differential evolution.");
            }
            if (lamda <= 0.0f) {
                throw std::runtime_error("lambda must be positive for differential evolution.");
            }
            if (generations <= 0) {
                throw std::runtime_error("generations must be positive for differential evolution.");
            }
        }

        // PSO parameter checks (only if algorithm == 5)
        if (algorithm == 5) {
            if (particles <= 0) {
                throw std::runtime_error("particles must be positive for PSO.");
            }
            if (c1 < 0.0f || c1 > 2.0f) {
                throw std::runtime_error("c1 must be between 0 and 2 for PSO.");
            }
            if (c2 < 0.0f || c2 > 2.0f) {
                throw std::runtime_error("c2 must be between 0 and 2 for PSO.");
            }
            if (generations <= 0) {
                throw std::runtime_error("generations must be positive for PSO.");
            }
        }

        // get bounds for the selected problem
        const float lower = bounds[problemType].min;
        const float upper = bounds[problemType].max;

        // choose csv file name based on algorithm and dimension m (e.g. PSOResults20.csv)
        if (algorithm == 4) { // DE
            csvFile = "DEResults" + std::to_string(m) + ".csv";
        } else if (algorithm == 5) { // PSO
            csvFile = "PSOResults" + std::to_string(m) + ".csv";
        }


        // call the selected algorithm
        algorithmOutput result{};

        switch (algorithm) {
            case 1:
                result = blindSearch(n, m, lower, upper, problemType, csvFile, append);
                break;
            case 2:
                result = localSearch(n, m, lower, upper, problemType, alpha, csvFile, append);
                break;
            case 3:
                result = repeatedLocalSearch(n, m, lower, upper, problemType, alpha, repeats, csvFile, append);
                break;
            case 4:
                result = differentialEvolution(NP, m, lower, upper, problemType, strategy, F, CR, lamda, generations, csvFile, append);
                break;
            case 5:
                result = particleSwarmOptimization(particles, m, lower, upper, problemType, generations, c1, c2, csvFile, append);
                break;
            default:
                throw std::runtime_error("Invalid algorithm.");
        }

        // Print summary
        std::cout << std::fixed << std::setprecision(6)
                  << " alg=" << algorithm
                  << " n=" << n
                  << " problem=" << problemType;

        if (algorithm == 4) {
            std::cout << " strategy=" << strategy;
        }

        std::cout << " csv=" << csvFile
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
