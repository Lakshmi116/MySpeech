#pragma once
#include "MySpeech.h"

using namespace std;


// Utilities Module
long double calculate_energy(long double arr[], int arr_len, long double output_arr[], int frame_len);
long double calculate_zcr(long double arr[], int arr_len, long double output_arr[], int frame_len);


// Preprocess Module
int preprocess_signal(long double* arr, int arr_len, long double* output_arr, long double *energy, long double *zcr);


