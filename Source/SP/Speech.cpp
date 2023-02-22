#include "Speech.h"

int Speech::PreProcess(int* input, std::string input_filename, std::string output_filename){
    // opening the given file
    std::ifstream file;
    std::ofstream out_file;
    file.open(input_filename);

    // terminating the process if the file doesn't exist
    if(!file){
        std::cout<< 
                "File: " << input_filename << " does not exist in the current folder" 
        << std::endl;
        return -1;
    }

    // reading the input
    // skipping the first few samples
    // calculating the avg for DC shifting
    // Noting the maximum amplitude for normalization
    int input_size = 0, val = 0, cnt = INITIAL_SKIP, abs_mx = -1;
    long double avg = 0.0;
    while(file >> val){
        if(cnt>0){
            cnt--;
            continue;
        }
        input[input_size++] = val;
        avg+=(long double)val;
        
        int abs_val = val>=0 ? val : -val;
        if(abs_val > abs_mx){
            abs_mx = abs_val;
        } 
    } avg/=(long double)input_size;
    file.close();
    

    // DC shift & normalization
    // Output to the same file
    out_file.open(output_filename);
    for(int i=0;i<input_size;i++){
        long double new_val_d = ((long double)input[i] - avg)*NORMALIZATION_RANGE/(long double)abs_mx;
        // int new_val = (int) new_val_d;

        out_file << new_val_d << "\n";
    }
    return input_size;
} // preprocess the input file, fills input array and stores in output_filename: return input size

void Speech::ApplyHammingWindow(long double* frame, int frame_size){
    // Applying hamming window to the given array
    int N = frame_size;
    for(int n=0;n<N;n++){
        frame[n] *= (0.54 - 0.46*cos(2*PI*n/(N-1)));
    }
    return;
}

void Speech::CalculateCorrelations(long double* correlations, long double* frame, int frame_size, int p){
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

void Speech::DurbinsAlgorithm(long double* lpc, long double* r, int p){
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

void Speech::LPCtoCepstral(long double* cepstral, long double* lpc, int p){
    // lpc is of size p
    // cepstral is of size p
    for(int i=0;i<p;i++){
        cepstral[i] = lpc[i];
        for(int k=0;k<i-1;k++){
            cepstral[i] += ((k+1)/(i+1))*cepstral[k]*lpc[i-k];
        }
    }
}


void Speech::CepstraltoLPC(long double* lpc, long double *cepstral, int cepstral_size){
    return;
}
void Speech::ApplyRaisedSineWindow(long double* frame,int frame_size){

}

void Speech::LPCcoefficients(long double* lpc, int p, long double* main_frame, int frame_size){
    long double frame[frame_size];
    for(int i=0;i<frame_size;i++)frame[i] = main_frame[i];
    
    // lpc is of size p
    ApplyHammingWindow(frame, frame_size);
    long double *r = new long double[p+1];
    CalculateCorrelations(r, frame, frame_size, p);
    DurbinsAlgorithm(lpc, r, p);
    return;
}

void Speech::CepstralCoefficients(long double* cepstral, int p, long double* frame, int frame_size){
    // cepstral is of size p
    long double* lpc = new long double[p];
    LPCcoefficients(lpc, p, frame, frame_size);
    LPCtoCepstral(cepstral, lpc, p);
    return;
}

void Speech::CepstralFile(std::string input_filename, std::string output_filename, int N, int M, int p){
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


void Speech::MakeCodebook(std::string universe_filename, std::string codebook_filename){
    //viterbi algorithm
    


    //k-means algorithm

    //optimal algorithm

}

void Speech::MakeUniverse(std::string audio_filenames[], std::string universe_filename){

}
void Speech::MakeUniverse(std::string audio_filename, std::string universe_filename){

}


