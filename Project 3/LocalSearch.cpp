// Zack Lee
// LocalSearch.cpp
// Local Search class to run the local search

#include "LocalSearch.h"
#include "Population.h"
#include "BlindSearch.h"


// Perform local search algorithm
algorithmOutput localSearch(int n, int m, float lower, float upper, int problemType, float alpha, const std::string& csvFile, bool append) {
    
    // Output struct
    algorithmOutput out{};

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Initialize population
    Population pop(n, m);
    pop.generate(lower, upper);

    // Evaluate initial population
    for (int i =0; i < pop.size(); ++i) {
        std::vector<float> x_i = pop.getRow(i);
        std::vector<float> x_new = improveOnce(pop, x_i, problemType, alpha, lower, upper);
        pop.setRow(i, x_new);
    }

    // Final evaluation
    pop.evaluate(problemType);
    auto best = pop.best();

    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> runtime = end - start;

    // Write best to CSV
    pop.writeBestCSV(csvFile, append, best, static_cast<float>(runtime.count()));

    // Fill output struct
    out.bestFitness = best.fitness;
    out.runtimeMs = static_cast<float>(runtime.count());
    return out;

}

// Perform one local step to generate new candidate solution
std::vector<float> localStep(const Population& pop, const std::vector<float>& x_t, int problemType, float alpha, float lowerBound, float upperBound){

    // Dimension of solution
    const int N = static_cast<int>(x_t.size());

    // f(X_i^t)
    const float f_xt = pop.evaluateVector(x_t, problemType);

    // New candidate solution X^N
    std::vector<float> xN(N);
    std::vector<float> xT = x_t;   

    // For each dimension
    for (int j = 0; j < N; ++j) {

        // Step: "add alpha to indexed value" => xT[j] = x_t[j] + alpha
        xT[j] = x_t[j] + alpha;

        // Step: "calculate new objective for this solution" => f(X^T)
        const float f_xT = pop.evaluateVector(xT, problemType);

        // Step: "calculate new value for X^N" => X^N[j] = X_t[j] - alpha * (f(X^T) - f(X_i^t))
        float newVal = x_t[j] - alpha * (f_xT - f_xt);

        // reset xT[j] back to original x_t[j] for next iteration
        xT[j] = x_t[j];

        // Ensure newVal is within bounds
        if (newVal < lowerBound) newVal = lowerBound;
        if (newVal > upperBound) newVal = upperBound;

        // Assign to new candidate solution
        xN[j] = newVal;
    }

    // Return new candidate solution
    return xN;
}

// Improve once: generate new candidate and replace if better
std::vector<float> improveOnce(const Population& pop, const std::vector<float>& x_t, int problemType, float alpha, float lowerBound, float upperBound)
{
    // Current fitness
    float f_xt = pop.evaluateVector(x_t, problemType);
    std::vector<float> xN = localStep(pop, x_t, problemType, alpha, lowerBound, upperBound);
    float f_xN = pop.evaluateVector(xN, problemType);

    // If improved, replace
    if (f_xN < f_xt) return xN;
    return x_t;
}
