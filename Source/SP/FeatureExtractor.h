#pragma once
#include<iostream>
#include<string>
#include<cmath>
#include<fstream>

static const long double PI = 3.1415926535;  



class FeatureExtractor{

public:

    void LPCcoefficients(long double* lpc, int p, long double* frame, int frame_size);
    void CepstralCoefficients(long double* cepstral, int p, long double* frame, int frame_size);
    
    void CepstralFile(std::string input_filename, std::string output_filename, int N, int M);

private:

    void ApplyHammingWindow(long double* frame, int frame_size);
    void ApplyRaisedSineWindow(long double* frame,int frame_size);
    void DurbinsAlgorithm(long double* lpc, long double* correlations, int p);
    void CalculateCorrelations(long double* correlations, long double* frame, int frame_size, int p);
    void LPCtoCepstral(long double* cepstral, long double* lpc, int lpc_size);
    void CepstraltoLPC(long double* lpc, long double *cepstral, int cepstral_size);
};