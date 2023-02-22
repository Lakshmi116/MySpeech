#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

static const long double PI = 3.1415926535;  
static const int INITIAL_SKIP = 150;
static const int NORMALIZATION_RANGE = 5000;
static const int FRAME_SIZE = 320; 
static const int CC_DIM = 12;
static const int MAX_SIGNAL_SIZE = 31500;


class Speech{

public:

    int PreProcess(int* input, std::string input_filename, std::string output_filename); // preprocess the input file inplace and fills input array: return input size
    void CepstralFile(std::string recordings_filename, std::string frames_filename, int N, int M, int p);
    void MakeCodebook(std::string universe_filename, std::string codebook_filename);
    void MakeUniverse(std::string audio_filename, std::string universe_filename);
    void MakeUniverse(std::string audio_filenames[], std::string universe_filename);

private:

    void ApplyHammingWindow(long double* frame, int frame_size);
    void ApplyRaisedSineWindow(long double* frame,int frame_size);
    void DurbinsAlgorithm(long double* lpc, long double* correlations, int p);
    void CalculateCorrelations(long double* correlations, long double* frame, int frame_size, int p);
    void CepstralCoefficients(long double* cepstral, int p, long double* frame, int frame_size);
    void LPCcoefficients(long double* lpc, int p, long double* frame, int frame_size);
    void LPCtoCepstral(long double* cepstral, long double* lpc, int lpc_size);
    void CepstraltoLPC(long double* lpc, long double *cepstral, int cepstral_size);

};