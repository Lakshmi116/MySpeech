#pragma once

#include<string>

const int MAX_TIME = 100;

class HMM{
public:
    int N; // number of states in the HMM
    int M; // number of distinct observation sequence elements
    long double** A; // transition probability
    long double** B; // consitional probablity of distinct obs vals at each state
    long double* PI; // Initial distribution of states

    HMM(int N, int M); // HMM constructor

    // HMM api
    void ReadHMM(std::string filename); // read a .hmm file
    void WriteHMM(std::string filename); // write into a .hmm file

private:
    // hidden variables of the HMM
    long double** alpha;
    long double** beta;
    long double** gamma;
    long double** delta;
    long double** psi;
    long double** xi;
    bool alpha_alloc, beta_alloc, gamma_alloc, delta_alloc, psi_alloc, xi_alloc;
    bool filled; // helper variable to identify if the HMM is filled properly
    bool reestimation_lock;
    // utility functions
    long double** allocate_memory(int p, int q); // allocates 2D matrix
    long double* allocate_memory(int p); // allocates 1D array
    void correct_estimation(void);
    void set_to_default(void); // configure your default states here

    // logic functions
    long double forward_procedure(int* observation_seq, int T); // fills alpha table and returns P(O|lambda)
    void backward_procedure(int* observation_seq, int T); // fills beta table
    long double viterbi_algorithm(int* observation_seq, int T, int* buffer); // fills the buffer with optimal state sequence and returs P_star
    void reestimation_algorithm(int* observation_seq, int T);

};