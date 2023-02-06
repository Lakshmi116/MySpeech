#pragma once
#include<string>
#include<fstream>
#include<iostream>

static const int INITIAL_SKIP = 150;
static const int NORMALIZATION_RANGE = 5000;
static const int FRAME_SIZE = 320; 
static const int CC_DIM = 12;
static const int MAX_SIGNAL_SIZE = 31500;

class PreProcess{
public:

    int preprocess(int* input, std::string input_filename, std::string output_filename); // preprocess the input file inplace and fills input array: return input size
    int LoadInputInteger(int* input, std::string input_filename); // loads the preprocessed signal into input array: returns input size
    int LoadInputReal(long double* input, std::string input_filename); // loads the signal as a double vector
    
};
