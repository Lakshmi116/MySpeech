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
    this->reestimation_lock = false;
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

    // setting A, B and PI to default values
    this->set_to_default();
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

long double HMM::viterbi_algorithm(int* observation_seq, int T, int* buffer){
    // fills the buffer with optimal state sequence
    if(!this->delta_alloc){
        this->delta = allocate_memory(MAX_TIME, this->N);
        this->delta_alloc = true;
    }
    if(!this->psi_alloc){
        this->psi = allocate_memory(MAX_TIME, this->N);
        this->psi_alloc = true;
    }

    // Initialization
    for(int i=0;i<this->N;i++){
        this->delta[0][i] = this->PI[i]*this->B[i][observation_seq[0]];
        psi[0][i] = 0;
    }

    //recursion
    for(int t=1;t<T;t++){
        for(int j=0;j<this->N;j++){
            this->delta[t][j] = this->delta[t-1][0]*this->A[0][j];
            this->psi[t][j] = 0;
            for(int i=1;i<this->N;i++){
                if(this->delta[t][j] < this->delta[t-1][i]*this->A[i][j] ){
                    this->delta[t][j] = this->delta[t-1][i]*this->A[i][j];
                    this->psi[t][j] = i;
                }
            }
            this->delta[t][j]*=this->B[j][observation_seq[t]];
        }
    }

    // Termination
    long double P_star = 0.0;
    int q_star = 0;
    for(int i=0;i<this->N;i++){
        if(P_star < this->delta[T-1][i]){
            P_star = this->delta[T-1][i];
            q_star = i;
        }
    }
    // filling the buffer with optimal state sequence
    buffer[T-1] = q_star;
    for(int t=T-2;t>=0;t--){
        buffer[t] = this->psi[t+1][buffer[t+1]];
    }

    // return the probability of occurance (small value)
    return P_star;
}

void HMM::correct_estimation(void){
    long double diff = 1.0;
    // correcting PI
    for(int i=0;i<this->N;i++)diff-=this->PI[i];
    PI[0]+=diff;

    //correcting A and B
    for(int i=0;i<this->N;i++){
        diff = 1.0;
        for(int j=0;j<this->N;j++)diff-=this->A[i][j];
        A[i][0]+=diff;
        diff = 1.0;
        for(int j=0;j<this->M;j++)diff-=this->B[i][j];
        B[i][0]+=diff;
    }
    return;
}

void HMM::set_to_default(void){
    long double x, y;
    
    // setting PI
    x = 1/this->N;
    for(int i=0;i<this->N;i++)this->PI[i] = x;
    
    // setting A
    for(int i=0;i<this->N-1;i++){
        for(int j=0;j<this->N;j++){
            if(i==j)this->A[i][j] = 0.8;
            else if(i==j-1)this->A[i][j] = 0.2;
            else this->A[i][j] = 0.0;
        }
    }
    for(int i=0;i<this->N-1;i++)this->A[this->N-1][i] = 0.0;
    this->A[this->N-1][this->N-1] = 1;

    // setting B
    y = 1/this->M;
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->M;j++){
            this->B[i][j] = y;
        }
    }
    return;
}

void HMM::reestimation_algorithm(int* observation_seq, int T){
    if(this->reestimation_lock){
        return;
    }
    // requires alpha and beta to be calculated
    long double P_O_lambda = this->forward_procedure(observation_seq, T);
    this->backward_procedure(observation_seq, T);

    if(!this->gamma_alloc){
        this->gamma = this->allocate_memory(MAX_TIME, this->N);
    }
    if(!this->xi_alloc){
        this->xi = this->allocate_memory(this->N, this->N);
    }

    // calculate gamma and sigma gamma
    long double sigma_gamma[this->N] = {0.0};
    for(int t=0;t<T;t++){
        for(int i=0;i<this->N;i++){
            this->gamma[t][i] = this->alpha[t][i]*this->beta[t][i];
            this->gamma[t][i]/=P_O_lambda;
            if(t<T-1){
                sigma_gamma[i]+=this->gamma[t][i];
            }
        }
    }

    //calculate SIGMA(xi, time)
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->N;j++){
            this->xi[i][j] = 0.0;
        }
    }
    for(int t=0;t<T-1;t++){
        for(int i=0;i<this->N;i++){
            for(int j=0;j<this->N;j++){
                this->xi[i][j] += this->alpha[t][i]
                                 *this->A[i][j]
                                 *this->B[j][observation_seq[t+1]]
                                 *this->beta[t+1][j];
            }
        }
    }
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->N;j++){
            this->xi[i][j] /= P_O_lambda;
        }
    }
    
    // reestimating HMM parameters
    // reestimating PI
    for(int i=0;i<this->N;i++){
        this->PI[i] = this->gamma[0][i];
    }
    // reestimating A
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->N;j++){
            this->A[i][j] = this->xi[i][j]/sigma_gamma[i];
        }
    }
    // reestimating B
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->M;j++){
            this->B[i][j] = 0.0;
        }
    }
    for(int t=0;t<T;t++){
        for(int i=0;i<this->N;i++){
            this->B[i][observation_seq[t]]+=this->gamma[t][i];
        }
    }
    for(int i=0;i<this->N;i++){
        for(int j=0;j<this->M;j++){
            this->B[i][j] /= (sigma_gamma[i] + this->gamma[T-1][i]);
        }
    }

    // ensure the restimation is consistant
    this->correct_estimation();
    return;
}


