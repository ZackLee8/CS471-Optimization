// Zack Lee
// Functions.cpp
// This file contains implementations of various utility functions.
// Date: 02/18/2026

#include "Functions.h"

// constant PI value
static constexpr float PI = 3.14159265358979323846f;

// Schwefels function
float schwefelFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += (-x[i]) * sin(sqrtf(fabs(x[i])));
    }
    return 418.9829f * n - sum;
}

// 1st De Jong's Function
float dejongFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += x[i] * x[i];
    }
    return sum;
}

// Rosenbrock's Function
float rosenbrockFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n - 1; ++i) {
        float term1 = (x[i] * x[i] - x[i+1]);
        float term2 = (1 - x[i]);
        sum += 100.0f * term1 * term1 + term2 * term2;
    }
    return sum;
}

// Rastrigin's Function
float rastriginFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += (x[i] * x[i]) - 10.0f * cosf(2.0f * PI * x[i]);
    }
    return 10.0f * n * sum;
}

// Griewank's Function
float griewankFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    float prod = 1.0f;
    for (int i = 0; i < n; ++i) {
        sum += (x[i] * x[i]) / 4000.0f;
        prod *= cosf(x[i] / sqrtf(i + 1));
    }
    return 1 + sum - prod;
}

// Sine Envelope Sine Wave Function
float sineEnvelopeSineWaveFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n - 1; ++i) {
        float s = x[i] * x[i] +x[i + 1] * x[i + 1];
        float num = sinf(s - 0.5f);
        num *= num;
        float den = 1.0f + 0.001f * s;
        den *= den;
        sum += 0.5f + num / den;
    }
    return -sum;
}

// Stretched V Sine Wave Function
float stretchedVSineWaveFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n - 1; ++i) {
        float s = x[i] * x[i] + x[i + 1] * x[i + 1];
        float s_quarter = powf(s, 0.25f);               
        float s_tenth   = powf(s, 0.1f);       
        float inner = s_quarter * sinf(50.0f * s_tenth);
        sum += inner * inner + 1.0f;
    }
    return sum;
}

// Ackley's One Function
float ackleyOneFunction(const vector<float>& x) {
    int n = x.size();
    if (n < 2) return 0.0f;
    float e = 1.0f / exp(0.2f);
    float sum = 0.0f;
    for (int i = 0; i < n - 1; ++i) {
        float term1 = x[i];
        float term2 = x[i + 1];
        float rad = sqrt(term1 * term1 + term2 * term2);
        float term3 = e * rad + 3.0f * (cosf(2.0f * term1) + sinf(2.0f * term2));
        sum += term3;
    }
    return sum;
}

// Ackley's Two Function
float ackleyTwoFunction(const vector<float>& x) {
    int n = x.size();
    if (n < 2) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < n - 1; ++i) {
        float term1 = x[i];
        float term2 = x[i + 1];
        float rad = sqrtf((term1 * term1 + term2 * term2) / 2.0f);
        float part1 = 20.0f / expf(0.2f * rad);
        float part2 = expf(0.5f * (cosf(2.0f * PI * term1) + cosf(2.0f * PI * term2)));
        float term3 = 20.0f + expf(1.0f) - part1 - part2;
        sum += term3;
    }

    return sum;
}

// Egg Holder Function
float eggHolderFunction(const vector<float>& x) {
    int n = x.size();
    float sum = 0.0f;
    for (int i = 0; i < n - 1; ++i) {
        float term1 = -(x[i + 1] + 47.0f) * sinf(sqrtf(fabs(x[i + 1] + x[i] / 2.0f + 47.0f)));
        float term2 = -x[i] * sinf(sqrtf(fabs(x[i] - (x[i + 1] + 47.0f))));
        sum += term1 + term2;
    }
    return sum;
}
