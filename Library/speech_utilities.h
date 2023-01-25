/* speech_utilities.h */
#pragma once

#include <bits/stdc++.h>


// Definitions
#define ll long long
#define ld long double

// Constants
static const int INITIAL_SKIP = 150;
static const int NORMALIZATION_RANGE = 5000;
static const int FRAME_SIZE = 320; 
static const int CC_DIM = 12;
static const int MAX_SIGNAL_SIZE = 31500;


// Functions
int preProcess(string input_filename, int* input); // preprocess the input file inplace and fills input array: return input size
int loadInput(string input_filename, int* input); // loads the preprocessed signal into input array: returns input size

