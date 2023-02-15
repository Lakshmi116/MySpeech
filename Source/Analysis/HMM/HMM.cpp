#include "HMM.h"
#include<fstream>
#include<iostream>

HMM::HMM(int N, int M){
    this->filled = false;
    this->alpha_alloc = false;
    this->beta_alloc = false;
    this->gamma_alloc = false;
    this->delta_alloc = false;
    this->psi_alloc = false;
    this->xi_alloc = false;
    this->N = N;
    this->M = M;

    // allocating memory to hmm tables
    this->A = (long double**)malloc(sizeof(long double*)*N);
    this->B = (long double**)malloc(sizeof(long double*)*N);
    this->PI = (long double*)malloc(sizeof(long double)*N);

    for(int i=0;i<N;i++){
        this->A[i] = (long double*)malloc(sizeof(long double)*N);
        this->B[i] = (long double*)malloc(sizeof(long double)*M);
    }
}

void HMM::ReadHMM(std::string filename){
    std::ifstream inp;
    inp.open(filename);

    if(!inp){
        std::cout << "File not found: " << filename << std::endl;
        return;
    }

    // read A, B, PI in that order
    /* !!! No error checks currently */
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->N;j++){
            inp >> this->A[i][j];
        }
    }
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->M;j++){
            inp >> this->B[i][j];
        }
    }
    for(int i=0;i<this->N;i++){
        inp >> this->PI[i];
    }

    filled = true;
    inp.close();
    return;
}

void HMM::WriteHMM(std::string filename){
    std::ofstream out;
    out.open(filename);

    if(!out){
        std::cout<< "Unable to write/open: "<< filename << std::endl;
        return;
    }

    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->N;j++){
            out << this->A[i][j];
        }
    }
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->M;j++){
            out << this->B[i][j];
        }
    }
    for(int i=0;i<this->N;i++){
        out << this->PI[i];
    }
    out.close();
    return;
}
long double** HMM::allocate_memory(int p, int q){
    // allocates and return a ptr to the table created
    long double** ret_ptr = (long double**)malloc(sizeof(long double*)*p);
    for(int i=0;i<p;i++){
        ret_ptr[i] = (long double*) malloc(sizeof(long double)*q);
    }
    return ret_ptr;
}
long double* HMM::allocate_memory(int p){
    long double* ret_ptr = (long double*)malloc(sizeof(long double)*p);
    return ret_ptr;
}
long double HMM::forward_procedure(int* observation_seq, int n){
    // If called for the first time, memory should be allocated
    if(!this->alpha_alloc){
        this->alpha = this->allocate_memory(MAX_TIME, this->N);
        this->alpha_alloc = true;
    }
    // forward_variable
    // alpha_t[i] = P(o1o2o3...ot, qt=i|lambda)
 
    // Initialization (t=0)
    for(int i=0;i<this->N;i++){
        this->alpha[0][i] = this->PI[i]*B[i][observation_seq[0]];
    }
    // Recursion
    for(int t=1;t<n;t++){
        for(int j=0;j<this->N;j++){
            for(int i=0;i<this->N;i++)this->alpha[t][j]+=this->alpha[t-1][i]*this->A[i][j];
            this->alpha[t][j] *= this->B[j][observation_seq[t]]; 
        }
    }
    // Termination
    long double prob = 0.0;
    for(int i=0;i<this->N;i++){
        prob+=this->alpha[n-1][i];
    }
    return prob;
}

void HMM::backward_procedure(int* observation_seq, int T){
    if(!this->beta_alloc){
        this->beta = allocate_memory(MAX_TIME, this->N);
        this->beta_alloc = true;
    }

    // Initialization
    for(int i=0;i<this->N;i++){
        this->beta[T-1][i] = 1;
    }

    // recursion
    for(int t=T-2;t>=0;t--){
        for(int i=0;i<this->N;i++){
            for(int j=0;j<this->N;j++){
                this->beta[t][i]+=this->A[i][j]*this->B[j][observation_seq[t+1]]*this->beta[t+1][j];
            }
        }
    }
    return;
}


