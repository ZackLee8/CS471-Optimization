// Zack Lee
// PSO.cpp
// 02/18/2026

// PSO implementation for the optimization project. This file defines the 
// particleSwarmOptimization function which performs the PSO algorithm and writes results to a CSV file.

// includes
#include "PSO.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

// Helper function to clamp a float value between a lower and upper bound
static inline float clampf(float x, float lo, float hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// Generate a random float in [0, 1) using the provided random generator
static float rand01(std::mt19937& gen) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

// Writes a row of PSO results to the specified CSV file, including headers if not appending.
static void writePSORow(
    const std::string& filename,
    bool append,
    int problemType,
    int D,
    int particles,
    float c1,
    float c2,
    int generations,
    float bestFitness,
    double runtimeMs
) {
    // Open the file for appending or overwrite if not appending
    std::ofstream out;
    out.open(filename, append ? (std::ios::out | std::ios::app) : std::ios::out);
    if (!out) throw std::runtime_error("Failed to open CSV file: " + filename);

    if (!append) {
        out << "problem,dim,particles,c1,c2,generations,best_fitness,runtime_ms\n";
    }

    // Write the results row
    out << problemType << ","
        << D << ","
        << particles << ","
        << c1 << ","
        << c2 << ","
        << generations << ","
        << bestFitness << ","
        << runtimeMs << "\n";
}

// Main PSO function
algorithmOutput particleSwarmOptimization(
    int particles,
    int m,
    float lower,
    float upper,
    int problemType,
    int generations,
    float c1,
    float c2,
    const std::string& csvFile,
    bool append
) {
    // Validate input parameters
    if (particles <= 0) {
        throw std::runtime_error("particles must be positive for PSO.");
    }
    if (m <= 0) {
        throw std::runtime_error("Dimension m must be positive for PSO.");
    }
    if (c2 < 0.0f || c2 > 2.0f) {
        throw std::runtime_error("c2 must be between 0 and 2 for PSO.");
    }
    if (c1 < 0.0f || c1 > 2.0f) {
        throw std::runtime_error("c1 must be between 0 and 2 for PSO.");
    }
    if (generations <= 0) {
        throw std::runtime_error("generations must be positive for PSO.");
    }
    if (!(lower < upper)) {
        throw std::runtime_error("Invalid bounds: lower must be < upper.");
    }

    // PSO hyperparameters
    const float range = upper - lower;
    const float vmax = 0.02f * range; 
    std::uniform_real_distribution<float> velDist(-vmax, vmax);
    const float wallDamp = 0.05f;  

    // Seed the random generator with a high-resolution clock for better randomness across runs
    unsigned seed = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    // Create a random generator
    std::mt19937 gen(seed);

    // Distribution for initializing particle positions uniformly in [lower, upper]
    std::uniform_real_distribution<float> posDist(lower, upper);

    // Create a Population object to evaluate fitness of particles
    Population evaluator(1, m);

    // Initialize particle positions, velocities, and personal/global bests
    std::vector<std::vector<float>> x(particles, std::vector<float>(m, 0.0f));
    std::vector<std::vector<float>> v(particles, std::vector<float>(m, 0.0f));
    std::vector<std::vector<float>> pBestPos(particles, std::vector<float>(m, 0.0f));
    std::vector<float> pBestFit(particles, std::numeric_limits<float>::infinity());

    // Global best position and fitness
    float gBestFit = std::numeric_limits<float>::infinity();
    std::vector<float> gBestPos(m, 0.0f);

    // Initialize particle positions and velocities, then set pBest/gBest.
    for (int i = 0; i < particles; ++i) {
        for (int d = 0; d < m; ++d) {
            x[i][d] = posDist(gen);
            v[i][d] = velDist(gen);
        }

        // Evaluate initial fitness and set personal/global bests
        const float fit = evaluator.evaluateVector(x[i], problemType);
        pBestPos[i] = x[i];
        pBestFit[i] = fit;

        // Update global best if this particle is the best so far
        if (fit < gBestFit) {
            gBestFit = fit;
            gBestPos = x[i];
        }
    }

    // Main PSO loop
    auto t0 = std::chrono::high_resolution_clock::now();

    // For each generation, update velocities and positions of all particles, then evaluate fitness and update personal/global bests.
    for (int t = 0; t < generations; ++t) {
        for (int i = 0; i < particles; ++i) {
            for (int d = 0; d < m; ++d) {
                const float r1 = rand01(gen);
                const float r2 = rand01(gen);

                // Update velocity based on inertia, cognitive, and social components
                v[i][d] = v[i][d] + c1 * r1 * (pBestPos[i][d] - x[i][d]) + c2 * r2 * (gBestPos[d] - x[i][d]);

                // Keep velocity bounded to avoid runaway motion.
                v[i][d] = clampf(v[i][d], -vmax, vmax);

                // Update position based on velocity
                x[i][d] = x[i][d] + v[i][d];

                // Keep particles in bounds and damp velocity at walls.
                if (x[i][d] > upper) {
                    x[i][d] = upper;
                    v[i][d] *= wallDamp;
                } else if (x[i][d] < lower) {
                    x[i][d] = lower;
                    v[i][d] *= wallDamp;
                }
            }

            // Evaluate fitness of the new position
            const float fit = evaluator.evaluateVector(x[i], problemType);

            // Update personal best if this position is better
            if (fit < pBestFit[i]) {
                pBestFit[i] = fit;
                pBestPos[i] = x[i];
            }

            // Update global best if this particle is the best so far
            if (pBestFit[i] < gBestFit) {
                gBestFit = pBestFit[i];
                gBestPos = pBestPos[i];
            }
        }
    }

    // After all generations, record the runtime and write results to CSV
    auto t1 = std::chrono::high_resolution_clock::now();
    double runtimeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Write the results to the CSV file
    writePSORow(csvFile, append, problemType, m, particles, c1, c2, generations, gBestFit, runtimeMs);

    // Return the best fitness and runtime in the output struct
    algorithmOutput out{};
    out.bestFitness = gBestFit;
    out.runtimeMs = static_cast<float>(runtimeMs);
    return out;
}