#pragma once
#include "MySpeech.h"

using namespace std;

// LPC coefficients
long double* calculate_LPC(long double* arr, int st, int end);
long double* calculate_LPC(long double* arr, int frame_no);
void calculate_correlation(long double* arr, int st, int end, long double correlation[P + 1]);



// Cepstral Coefficients
long double* calculate_cepstral(long double* arr, int st, int end);
long double* calculate_cepstral(long double* arr, int frame_no);



// Mel frequency cepstral coeffcients


// Utility functions
void apply_hamming_window(long double* arr, int st, int end, long double* output_arr);
void apply_hamming_window(long double* arr, int frame_no, long double* output_arr);



