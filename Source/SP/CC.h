#pragma once

#include<fstream>

const int P = 12;

class CC{
private:
    long double buffer[P];
    void SetCepstral(long double arr[P]);
    bool filled;
public:
    CC(void);
    CC(long double buffer[P]);

    void GetCepstral(long double buffer[P]);
    void SetCepstral(std::ifstream input_stream);
    void PrintCepstral(void);
    
    void GetMelCepstral(long double buffer[P]);
    void PrintMelCepstral(void);

};