/* speech_utilities.cpp */


/*

 TO DO 
1. restrict the input size usign an if statement


*/


#include "speech_utilities.h"

using namespace std;

// IO utilities

int preprocess(string filename, int* input){  
    // opening the given file
    ifstream file;
    ofstream out_file;
    file.open(filename);

    // terminating the process if the file doesn't exist
    if(!file){
        cout<< "File: " << filename << " does not exist in the current folder" << endl;
        return;
    }

    // reading the input
    // skipping the first few samples
    // calculating the avg for DC shifting
    // Noting the maximum amplitude for normalization
    int input_size = 0, val = 0, cnt = INITIAL_SKIP, abs_mx = -1;
    ld avg = 0.0;
    while(file >> val){
        if(cnt>0){
            cnt--;
            continue;
        }
        input[input_size++] = val;
        avg+=(ld)val;
        
        int abs_val = val>=0 ? val : -val;
        if(abs_val > abs_mx){
            abs_mx = abs_val;
        } 
    } avg/=(ld)input_size;
    file.close();
    

    // DC shift & normalization
    // Output to the same file
    out_file.open(filename);
    for(int i=0;i<input_size;i++){
        ld new_val_d = ((ld)input[i] - avg)*NORMALIZATION_RANGE/(ld)abs_mx;
        int new_val = (int) new_val_d;

        out_file << new_val << "\n";
    }
    return input_size;
}

int loadInput(string filename, int* input){
    ifstream file;
    file.open(filename);

    if(!file){
        cout<< "File: "<<filename << " does not exist in the current folder"<<endl;
        return;
    }

    int input_size = 0, val = 0;
    while(file >> val){
        input[input_size++] = val;
    }
    file.close();
    return input_size;
}

// Signal processing utilities ex: hamming window etc



// LPC utilities ex: calculate lpc features of a given frame


 
// Cepstra Utilities ex: calculate cepstral features of a given frame