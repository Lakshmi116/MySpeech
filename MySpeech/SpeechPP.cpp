#include "SpeechPP.h"


// Utilities Module
long double calculate_energy(long double arr[], int arr_len, long double output_arr[], int frame_len) {
    // Input: Normalized wave signal
    // Output: Fill the energy array, return the avg energy of the frame
    long double avg_frame_energy = 0.0;
    if (arr_len < FRAME_SIZE * frame_len) {
        printf("ERROR: Out of Range - frame_len is more than permitted - while calculating framewise ENERGY\n");
        return -1.0;
    }
    memset(output_arr, 0, sizeof(long double) * FRAME_LIMIT);
    for (int i = 0; i < frame_len; i++) {
        long double frame_energy = 0.0;
        int frame_shift = i * FRAME_SIZE;
        for (int j = 0; j < FRAME_SIZE; j++) {
            frame_energy += arr[frame_shift + j] * arr[frame_shift + j];
        }
        frame_energy /= FRAME_SIZE;
        output_arr[i] = frame_energy;
        avg_frame_energy += frame_energy;
    }
    avg_frame_energy /= frame_len;
    return avg_frame_energy;
}

long double calculate_zcr(long double arr[], int arr_len, long double output_arr[], int frame_len) {
    // Input: Normalized wave signal
    // Output: Fill the zcr array, return avg_zcr
    long double avg_frame_zcr = 0.0;
    if (arr_len < frame_len * FRAME_SIZE) {
        printf("ERROR: Out of Range - frame_len is more permitted - while calculating framewise ZCR");
        return -1.0;
    }
    memset(output_arr, 0, sizeof(long double) * FRAME_LIMIT);
    for (int i = 0; i < frame_len; i++) {
        long double frame_zcr = 0.0;
        int cnt_crosses = 0;
        for (int j = i * FRAME_SIZE + 1; j < (i + 1) * FRAME_SIZE; j++) {
            if (arr[j - 1] < 0 && arr[j] >= 0 || arr[j - 1] > 0 && arr[j] <= 0) {
                cnt_crosses++;
            }
        }
        frame_zcr = ((double)cnt_crosses / (double)FRAME_SIZE) * 100;
        output_arr[i] = frame_zcr;
        avg_frame_zcr += frame_zcr;
    }
    avg_frame_zcr /= frame_len;
    return avg_frame_zcr;
}


// Preprocess Module
int preprocess_signal(long double* arr, int arr_len, long double* output_arr, long double* energy, long double* zcr) {

    // Input: raw_signal, it's size
    // Output: preprocess the [wave], calculate Energy and ZCR and return it's size

    long double avg = 0.0, mx = -1;

    //  a. DC shift
    for (int i = 0; i < arr_len; i++) {
        avg += arr[i];
    }avg /= arr_len;
    for (int i = 0; i < arr_len; i++) {
        arr[i] -= avg;
        mx = std::max(mx, abs(arr[i]));
    }
    //  b. Normalize the wave [-5000, 5000]
    for (int i = 0; i < arr_len; i++) {
        arr[i] = (arr[i] / mx) * 5000;
    }

    if (output_arr == NULL) {
        // If the wave is just to be normalized in place
        return arr_len;
    }

    //  c. read the significant part of the wave
    // (Assuming that we use only one word per recording) skip silence from both the ends
    int frame_len = arr_len / FRAME_SIZE;
    long double avg_energy = calculate_energy(arr, arr_len, energy, frame_len);

    int st_frame_ind = 0, end_frame_ind = frame_len - 1;
    while (st_frame_ind < frame_len && energy[st_frame_ind] < 0.1 * avg_energy) {
        // while the frame energy is less than 10% avg frame energy skip
        st_frame_ind++;
    }
    while (end_frame_ind >= 0 && energy[end_frame_ind] < 0.1 * avg_energy) {
        end_frame_ind--;
    }
    if (st_frame_ind >= end_frame_ind) {
        // Quit with return status -1 is signal given is invalid
        printf("ERROR: Empty signal given to process\n");
        return -1;
    }
    // Copy the significant part of the given wave into wave
    frame_len = (1 + end_frame_ind - st_frame_ind);
    int output_arr_len = frame_len * FRAME_SIZE;
    memset(output_arr, 0, sizeof(long double) * frame_len * FRAME_SIZE);
    for (int frame_no = st_frame_ind; frame_no <= end_frame_ind; frame_no++) {
        int frame_shift = (frame_no - st_frame_ind) * FRAME_SIZE;
        for (int i = 0; i < FRAME_SIZE; i++) {
            output_arr[frame_shift + i] = arr[frame_no * FRAME_SIZE + i];
        }
    }
    avg_energy = calculate_energy(output_arr, output_arr_len, energy, frame_len);
    long double avg_zcr = calculate_zcr(output_arr, output_arr_len, zcr, frame_len);
    return output_arr_len;
}