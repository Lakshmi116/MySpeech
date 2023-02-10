#include "CC.h"
#include <iostream>
#include <cmath>

CC::CC(long double buffer[P]){
    for(int i=0;i<P;i++){
        this->buffer[i] = buffer[i];
    }
    this->filled = true;
}

CC::CC(void){
    this->filled = false;
}

void CC::GetCepstral(long double buffer[12]){
    for(int i=0;i<P;i++){
        buffer[i] = this->buffer[i];
    }
    return;
}

void CC::PrintCepstral(void){
    for(int i=0;i<P;i++){
        std::cout<< this->buffer[i] << " ";
    }
    std::cout << std::endl;
}

void CC::SetCepstral(long double arr[P]){
    for(int i=0;i<P;i++){
        this->buffer[i]  = arr[i];
    }
}

void CC::SetCepstral(std::ifstream input_stream){
    long double arr[P];
    for(int i=0;i<P;i++){
        if(!input_stream.eof()){
            input_stream >> arr[i];
        }else{
            std::cout<< "Insufficient cepstral coeffcients given. " << std::endl;
            return;
        }
    }
    this->SetCepstral(arr);
}

void CC::GetMelCepstral(long double buffer[P]){
    if(!this->filled){
        std::cout<< "Object not filled with cepstral coefficients" << std::endl;
    }
    return;
}

void CC::PrintMelCepstral(void){
    return;
}