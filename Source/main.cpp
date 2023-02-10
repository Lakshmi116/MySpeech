#include<iostream>
#include<fstream>
#include<string>


// #include "IO/IO.h"
#include "SP/PreProcess.h"
#include "SP/FeatureExtractor.h"


static const std::string DATA_ROOT = "./Data/";
static const std::string REC_ROOT = DATA_ROOT + "Recordings/"; 
static const std::string FRM_ROOT = DATA_ROOT + "Frames/";

const int N = 320;
const int M = 108;
const int P = 12;

void preprocess_all(void){

    PreProcess *pp = new PreProcess();
    int input_buffer[35000];
    for(int i=1;i<=5;i++){
        for(int grp = 0;grp<2;grp++){
            for(int nos=1;nos<=20;nos++){
                std::string input_filename, output_filename;
                input_filename = REC_ROOT + std::to_string(i) + std::string(1,'a'+grp) + "/" + std::to_string(i) + std::string(1,'a'+grp) + "20_" + std::to_string(nos);
                output_filename = input_filename + "n";


                pp->preprocess(input_buffer, input_filename+".txt", output_filename+".txt");
                std::cout<< "PreProcessing done.. "+ input_filename <<std::endl;
            }
            
        }
    }
}

void cepstral_all(void){

    FeatureExtractor *sp = new FeatureExtractor();
    int input_buffer[35000];
    for(int i=1;i<=5;i++){
        for(int grp = 0;grp<2;grp++){
            for(int nos=1;nos<=20;nos++){
                std::string input_filename, output_filename;
                input_filename =  std::to_string(i) + std::string(1,'a'+grp) + "/" + std::to_string(i) + std::string(1,'a'+grp) + "20_" + std::to_string(nos) + "n";
                // output_filename = input_filename + "n";


                sp->CepstralFile(REC_ROOT + input_filename+".txt", FRM_ROOT + input_filename+".txt", N, M, P);
                std::cout<< "Cepstral extraction done.. "+ input_filename <<std::endl;
            }
            
        }
    }
}





int main(void){

     cepstral_all();

    

    // IO *tmp = new IO();
    // tmp->IoTest();

    return 0;
}

