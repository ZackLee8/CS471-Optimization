// Zackary Lee
// Functions.h
// This file contains declarations of various utility functions.
// Date: 01/14/2026

#ifndef FUNCTIONS_H
#define FUNCTIONS_H 

#include <cmath>
#include <vector>
using namespace std;

float schwefelFunction(const vector<float>& x);
float dejongFunction(const vector<float>& x);   
float rosenbrockFunction(const vector<float>& x);
float rastriginFunction(const vector<float>& x);
float griewankFunction(const vector<float>& x);
float sineEnvelopeSineWaveFunction(const vector<float>& x);
float stretchedVSineWaveFunction(const vector<float>& x);
float ackleyOneFunction(const vector<float>& x);
float ackleyTwoFunction(const vector<float>& x);
float eggHolderFunction(const vector<float>& x);

#endif 