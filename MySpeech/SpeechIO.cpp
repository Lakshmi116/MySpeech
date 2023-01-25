#include "SpeechIO.h"


// Input Module
long double* read_array_txt(string filename, int* inp_size) {
    // Inputs :Empty array and file pointer
    // Output :arbitrary 
    // FILE* ptr = fopen(filename, "r");
    ifstream input(string(filename), ios::in);

    long double reader;
    vector<long double> tmp;
    int arr_size = 0;
    while (input >> reader) {
        tmp.push_back(reader);
        arr_size++;
    }

    input.close();
    long double* arr = (long double*)malloc(sizeof(long double) * arr_size);
    for (int i = 0; i < arr_size; i++) {
        arr[i] = tmp[i];
    }
    *inp_size = arr_size;

    return arr;
}

int read_input_txt(string filename, long double *wave, long double* energy, long double* zcr) {


    // 1. read from input into a tmp array
    int* inp_size_ptr = (int*)malloc(sizeof(int));
    long double* inp = read_array_txt(filename, inp_size_ptr);
    if (inp_size_ptr == NULL) {
        printf("ERROR: Unable to allocate memory from heap\n");
        return -1;
    }
    int inp_size = (*inp_size_ptr);
    // 2. preprocess the wave
    // 3. put the final wave into wave

    // Never re-process a pre-preocessed wave
    inp_size = preprocess_signal(inp, inp_size, wave, energy, zcr);
    return inp_size;
}


// Output Module
void print_array(long double* arr, int arr_len) {
    // Input: arr
    // Output: display is on the screen space seperated
    for (int i = 0; i < arr_len; i++) {
        printf("%lf ", arr[i]);
    }printf("\n");
}

int write_array_txt(long double* arr, int arr_len, string filename) {
    // Input: file to written
    // Output: Write the given array to file
    ofstream output(filename, ios::out);
    for (int i = 0; i < arr_len; i++) {
        output << arr[i] << "\n";
    }
    output.close();
    return 0;
}
