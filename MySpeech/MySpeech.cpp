// MySpeech.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MySpeech.h"
#include "SpeechIO.h"
#include "SpeechPP.h"
#include "SpeechFeatures.h"
#include "SpeechVQ.h"
#include "SpeechHMM.h"


using namespace std;

// Global variables

long double wave[ARR_LIMIT];
long double energy[FRAME_LIMIT];
long double zcr[FRAME_LIMIT];


int main()
{
    printf("Hello World!\n");
    // read - preprocess - output the energy and zcr values from the recording four.txt
    // open cool edit to verify the output wave

    /*
    
    char inp_filename[40] = "four.txt";
    char out_filename[40] = "pp_four.txt";
    int wave_len = read_input_txt(inp_filename, wave, energy, zcr);
    if (wave_len == -1) {
        printf("Exiting from main");
        return 0;
    }
    write_array_txt(wave, wave_len, out_filename);

    int frame_len = wave_len / FRAME_SIZE;

    */

    cout << "a";
    /* 
    
    printf("Energy: \n");
    print_array(energy, frame_len);
    printf("ZCR: \n");
    print_array(zcr, frame_len); 
     
     */
    return 0;
}


// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
