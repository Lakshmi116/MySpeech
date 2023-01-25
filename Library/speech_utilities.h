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


// Functions
void preprocess(char* input_filename, int* input);

