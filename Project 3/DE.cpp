// Zack Lee
// DE.cpp
// 02/18/2026

#include "DE.h"
#include "Population.h"
#include <random>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <limits>

// ------------------------ helpers ------------------------

// clamp x to [lo, hi]
static inline float clampf(float x, float lo, float hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// Random number utilities
static int randInt(std::mt19937& gen, int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(gen);
}

// uniform real in [0,1)
static float rand01(std::mt19937& gen) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

// pick k distinct indices in [0, NP-1], all != avoid
static std::vector<int> pickDistinct(std::mt19937& gen, int NP, int avoid, int k) {
    if (NP <= k) throw std::runtime_error("NP too small for distinct index selection.");
    std::vector<int> out;
    out.reserve(k);

    while ((int)out.size() < k) {
        int r = randInt(gen, 0, NP - 1);
        if (r == avoid) continue;
        bool used = false;
        for (int v : out) {
            if (v == r) { used = true; break; }
        }
        if (!used) out.push_back(r);
    }
    return out;
}

// Binomial crossover: for each j, with prob CR take from v, else from xi. Ensure at least one from v.
static std::vector<float> crossoverBin(
    std::mt19937& gen,
    const std::vector<float>& xi,
    const std::vector<float>& v,
    float CR
) {
    const int D = (int)xi.size();
    std::vector<float> u(D);

    int jrand = randInt(gen, 0, D - 1);
    for (int j = 0; j < D; ++j) {
        if (rand01(gen) < CR || j == jrand) u[j] = v[j];
        else u[j] = xi[j];
    }
    return u;
}

// Exponential crossover: start at n, copy a contiguous block from v into u,
// at least 1 element, keep copying while rand < CR (wrap around).
static std::vector<float> crossoverExp(
    std::mt19937& gen,
    const std::vector<float>& xi,
    const std::vector<float>& v,
    float CR
) {
    const int D = (int)xi.size();
    std::vector<float> u = xi;

    int n = randInt(gen, 0, D - 1);
    int L = 1; // must copy at least 1
    u[n] = v[n];

    int j = (n + 1) % D;
    while (L < D && rand01(gen) < CR) {
        u[j] = v[j];
        j = (j + 1) % D;
        ++L;
    }
    return u;
}

// Write one row to the DE results CSV file
static void writeDERow(
    const std::string& filename,
    bool append,
    int strategy,
    int problemType,
    int D,
    float bestFitness,
    double runtimeMs
) {
    std::ofstream out;
    out.open(filename, append ? (std::ios::out | std::ios::app) : std::ios::out);
    if (!out) throw std::runtime_error("Failed to open CSV file: " + filename);

    if (!append) {
        out << "strategy,problem,dim,best_fitness,runtime_ms\n";
    }
    out << strategy << ","
        << problemType << ","
        << D << ","
        << bestFitness << ","
        << runtimeMs << "\n";
}

// ------------------------ strategy mutant builder ------------------------

// Build the mutant vector v based on the given strategy and indices.
std::vector<float> makeMutant(
    int strategy,
    const std::vector<std::vector<float>>& pop,
    const std::vector<float>& xi,
    const std::vector<float>& best,
    int r1, int r2, int r3, int r4, int r5,
    float F, float lambda
) {
    const int D = (int)xi.size();
    std::vector<float> v(D, 0.0f);

    auto addScaledDiff1 = [&](const std::vector<float>& a, const std::vector<float>& b) {
        // + F*(a - b)
        for (int j = 0; j < D; ++j) v[j] += F * (a[j] - b[j]);
    };


    // + F*(a + b - c - d)
    auto addScaledDiff2 = [&](const std::vector<float>& a, const std::vector<float>& b,
                              const std::vector<float>& c, const std::vector<float>& d) {
        // + F*(a + b - c - d)
        for (int j = 0; j < D; ++j) v[j] += F * (a[j] + b[j] - c[j] - d[j]);
    };

    // Build mutant based on strategy
    switch (strategy) {
        // 1: DE/best/1/exp
        case 1:
        // 6: DE/best/1/bin  (same mutant)
        case 6:
            v = best;
            addScaledDiff1(pop[r2], pop[r3]);
            break;

        // 2: DE/rand/1/exp
        case 2:
        // 7: DE/rand/1/bin (same mutant)
        case 7:
            v = pop[r1];
            addScaledDiff1(pop[r2], pop[r3]);
            break;

        // 3: DE/rand-to-best/1/exp
        case 3:
        // 8: DE/rand-to-best/1/bin (same mutant)
        case 8:
            v = xi;
            for (int j = 0; j < D; ++j) {
                v[j] += lambda * (best[j] - xi[j]);
            }
            // + F*(xr1 - xr2)
            addScaledDiff1(pop[r1], pop[r2]);
            break;

        // 4: DE/best/2/exp
        case 4:
        // 9: DE/best/2/bin (same mutant)
        case 9:
            v = best;
            addScaledDiff2(pop[r1], pop[r2], pop[r3], pop[r4]);
            break;

        // 5: DE/rand/2/exp
        case 5:
        // 10: DE/rand/2/bin (same mutant)
        case 10:
            v = pop[r5];
            addScaledDiff2(pop[r1], pop[r2], pop[r3], pop[r4]);
            break;

        default:
            throw std::runtime_error("Invalid DE strategy (must be 1..10).");
    }

    return v;
}

// ------------------------ main DE run ------------------------

// Run the DE algorithm with the given parameters and return the best fitness and runtime.
algorithmOutput differentialEvolution(
    int NP,
    int D,
    float lower,
    float upper,
    int problemType,
    int strategy,
    float F,
    float CR,
    float lambda,
    int generations,
    const std::string& csvFile,
    bool append
) {
    // Parameter checks
    if (NP < 4) throw std::runtime_error("DE requires NP >= 4.");
    if (D <= 0) throw std::runtime_error("Dimension D must be positive.");
    if (CR < 0.0f || CR > 1.0f) throw std::runtime_error("CR must be in [0,1].");
    if (F <= 0.0f || F > 2.0f) throw std::runtime_error("F must be in (0,2].");
    if (lambda <= 0.0f) throw std::runtime_error("lambda must be positive.");
    if (generations <= 0) throw std::runtime_error("generations must be positive.");
    if (strategy < 1 || strategy > 10) throw std::runtime_error("strategy must be 1..10.");
    if (!(lower < upper)) throw std::runtime_error("Invalid bounds: lower must be < upper.");

    // time-based seed (matches your preference)
    unsigned seed = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(lower, upper);

    // We'll use Population only for evaluation (your function switch is inside it)
    Population evaluator(1, D);

    // Initialize population uniformly in bounds
    std::vector<std::vector<float>> pop(NP, std::vector<float>(D));
    std::vector<float> fit(NP, 0.0f);

    for (int i = 0; i < NP; ++i) {
        for (int j = 0; j < D; ++j) pop[i][j] = dist(gen);
        fit[i] = evaluator.evaluateVector(pop[i], problemType);
    }

    // Start timer
    auto t0 = std::chrono::high_resolution_clock::now();

    // Main DE loop
    for (int g = 0; g < generations; ++g) {
        // find best in current generation
        int bestIdx = 0;
        float bestFit = fit[0];
        for (int i = 1; i < NP; ++i) {
            if (fit[i] < bestFit) {
                bestFit = fit[i];
                bestIdx = i;
            }
        }
        const std::vector<float> best = pop[bestIdx];

        // for each target
        for (int i = 0; i < NP; ++i) {
            // Pick indices depending on strategy
            int r1=-1,r2=-1,r3=-1,r4=-1,r5=-1;

            if (strategy == 1 || strategy == 2 || strategy == 6 || strategy == 7) {
                // need r1,r2,r3
                auto idx = pickDistinct(gen, NP, i, 3);
                r1 = idx[0]; r2 = idx[1]; r3 = idx[2];
            } else if (strategy == 3 || strategy == 8) {
                // need r1,r2 (still pick 2 distinct)
                auto idx = pickDistinct(gen, NP, i, 2);
                r1 = idx[0]; r2 = idx[1];
            } else if (strategy == 4 || strategy == 9) {
                // need r1,r2,r3,r4
                auto idx = pickDistinct(gen, NP, i, 4);
                r1 = idx[0]; r2 = idx[1]; r3 = idx[2]; r4 = idx[3];
            } else if (strategy == 5 || strategy == 10) {
                // need r1,r2,r3,r4,r5
                auto idx = pickDistinct(gen, NP, i, 5);
                r1 = idx[0]; r2 = idx[1]; r3 = idx[2]; r4 = idx[3]; r5 = idx[4];
            }

            // Mutation: build mutant v
            std::vector<float> v = makeMutant(strategy, pop, pop[i], best, r1,r2,r3,r4,r5, F, lambda);

            // Crossover: exp for 1-5, bin for 6-10
            std::vector<float> u;
            if (strategy >= 1 && strategy <= 5) u = crossoverExp(gen, pop[i], v, CR);
            else u = crossoverBin(gen, pop[i], v, CR);

            // Clamp back into bounds
            for (int j = 0; j < D; ++j) u[j] = clampf(u[j], lower, upper);

            // Selection
            float fu = evaluator.evaluateVector(u, problemType);
            if (fu <= fit[i]) {
                pop[i] = std::move(u);
                fit[i] = fu;
            }
        }
    }

    // Final best
    float finalBestFit = fit[0];
    for (int i = 1; i < NP; ++i) finalBestFit = std::min(finalBestFit, fit[i]);

    // End timer
    auto t1 = std::chrono::high_resolution_clock::now();
    double runtimeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Write one row per run
    writeDERow(csvFile, append, strategy, problemType, D, finalBestFit, runtimeMs);

    algorithmOutput out{};
    out.runtimeMs = runtimeMs;   // this matches what your main prints
    return out;
}
