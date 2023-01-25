#pragma once
#include "MySpeech.h"
#include "SpeechPP.h"

using namespace std;

// Input Module
long double* read_array_txt(string filename, int* inp_size);
int read_input_txt(string filename, long double *wave, long double* energy, long double *zcr);


// Output Module
void print_array(long double* arr, int arr_len);
int write_array_txt(long double* arr, int arr_len, string filename);

