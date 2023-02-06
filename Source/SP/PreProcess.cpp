#include "PreProcess.h"

namespace PreProcess{

    int preprocess(int* input, std::string input_filename, std::string output_filename){
        // opening the given file
        std::ifstream file;
        std::ofstream out_file;
        file.open(input_filename);

        // terminating the process if the file doesn't exist
        if(!file){
            std::cout<< 
                    "File: " << input_filename << " does not exist in the current folder" 
            << std::endl;
            return;
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

    int LoadInputInteger(int* input, std::string filename){
        std::ifstream file;
        file.open(filename);

        if(!file){
            std::cout<< "File: "<<filename << " does not exist in the current folder"<<std::endl;
            return;
        }

        int input_size = 0, val = 0;
        while(file >> val){
            input[input_size++] = val;
        }
        file.close();
        return input_size;
    }

    int LoadInputReal(long double* input, std::string filename){
        std::ifstream file;
        file.open(filename);

        if(!file){
            std::cout<<
                     "File: "<<filename << " does not exist in the current folder"
            <<std::endl;
            return;
        }

        int input_size = 0;
        long double val = 0.0;
        while(file >> val){
            input[input_size++] = val;
        }
        file.close();
        return input_size;
    }
}
