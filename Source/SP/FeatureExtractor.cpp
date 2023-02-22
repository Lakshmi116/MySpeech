#include "FeatureExtractor.h"



void FeatureExtractor::ApplyHammingWindow(long double* frame, int frame_size){
    // Applying hamming window to the given array
    int N = frame_size;
    for(int n=0;n<N;n++){
        frame[n] *= (0.54 - 0.46*cos(2*PI*n/(N-1)));
    }
    return;
}

void FeatureExtractor::CalculateCorrelations(long double* correlations, long double* frame, int frame_size, int p){
    // correlations has p+1 values [0,p]
    int N = frame_size;
    for(int m=0;m<=p;m++){
        long double r_m = 0.0;
        for(int n=0;n<N-m;n++){
            r_m += frame[n]*frame[n+m];
        }
        correlations[m] = r_m;
    }
}

void FeatureExtractor::DurbinsAlgorithm(long double* lpc, long double* r, int p){
    // lpc has p values
    // correlations has p+1 values
    
    long double E = r[0], k=0.0, alpha[p+1], tmp[p+1];
    int L = 1;

    for(int i=1;i<=p;i++){
        k = r[i];
        for(int j=1;j<L;j++){
            k -= alpha[j]*r[i-j];
        }
        k/=E;
        tmp[i] = k;
        for(int j=1;j<L;j++){
            tmp[j] = alpha[j] - k*alpha[i-j];
        }
        for(int j=1;j<=L;j++){
            alpha[j] = tmp[j];
        }
        E *= (1-k*k);
        L++;
    }
    for(int i=0;i<p;i++){
        lpc[i] = alpha[i+1];
    }
}

void FeatureExtractor::LPCtoCepstral(long double* cepstral, long double* lpc, int p){
    // lpc is of size p
    // cepstral is of size p
    for(int i=0;i<p;i++){
        cepstral[i] = lpc[i];
        for(int k=0;k<i-1;k++){
            cepstral[i] += ((k+1)/(i+1))*cepstral[k]*lpc[i-k];
        }
    }
}


void FeatureExtractor::CepstraltoLPC(long double* lpc, long double *cepstral, int cepstral_size){
    return;
}
void FeatureExtractor::ApplyRaisedSineWindow(long double* frame,int frame_size){

}

void FeatureExtractor::LPCcoefficients(long double* lpc, int p, long double* main_frame, int frame_size){
    long double frame[frame_size];
    for(int i=0;i<frame_size;i++)frame[i] = main_frame[i];
    
    // lpc is of size p
    ApplyHammingWindow(frame, frame_size);
    long double *r = new long double[p+1];
    CalculateCorrelations(r, frame, frame_size, p);
    DurbinsAlgorithm(lpc, r, p);
    return;
}

void FeatureExtractor::CepstralCoefficients(long double* cepstral, int p, long double* frame, int frame_size){
    // cepstral is of size p
    long double* lpc = new long double[p];
    LPCcoefficients(lpc, p, frame, frame_size);
    LPCtoCepstral(cepstral, lpc, p);
    return;
}

void FeatureExtractor::CepstralFile(std::string input_filename, std::string output_filename, int N, int M, int p){
    // input file contains normalized wave
    // output file contains cepstral coefficients of each frame
    // frame size N
    // skip size M (N/3 usually)
    long double* frame = new long double[N];
    long double* cepstral = new long double[p];
    long double tmp;

    std::ifstream in;
    std::ofstream out;
    in.open(input_filename);
    out.open(output_filename);
    bool partial_frame_found = false;
    // reading N-M initial values into frame
    for(int i=M;i<N;i++){
        in >> tmp;
        frame[i] = tmp;
    }

    while(!in.eof()){
        // shift the array to M elements left
        for(int i=0;i<N-M;i++){
            frame[i] = frame[i+M];
        }
        // read M elements
        for(int i=0;i<M;i++){
            if(in.eof()){
                std:: cout << "Last frame reached.. skipping this one\n";
                partial_frame_found = true;
                break;
            }
            in >> frame[N-M+i];
        }
        if(partial_frame_found){
            break;
        }
        CepstralCoefficients(cepstral, p, frame, N);
        for(int i=0;i<p;i++){
            out << cepstral[i] << " ";
        }out<<"\n";
    }
    in.close(); 
    out.close();

}







void FeatureExtractor::MakeCodebook(std::string universe_filename, std::string codebook_filename){
    //viterbi algorithm
    


    //k-means algorithm

    //optimal algorithm

}

void FeatureExtractor::MakeUniverse(std::string audio_filenames[], std::string universe_filename){

}
void FeatureExtractor::MakeUniverse(std::string audio_filename, std::string universe_filename){

}
    