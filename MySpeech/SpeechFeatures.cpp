#include "SpeechFeatures.h"

long double* calculate_cepstral(long double* wave, int st, int end) {
	return NULL;
}

long double* calculate_cepstral(long double* wave, int frame_no) {
	return NULL;
}

long double* calculate_LPC(long double* wave, int st, int end) {
	// Input: wave frame
	// Output: LPC coefficients as an array
	if (end - st != FRAME_SIZE) {
		printf("ERROR: Invalid frame size (%d - %d) = %d", end, st, end - st);
		return NULL;
	}
	
	long double r[P + 1]; // correlation vector
	// long double* alpha = (long double*)malloc(sizeof(long double)*(P+1));
	long double* arr = (long double*)malloc(sizeof(long double) * FRAME_SIZE);

	long double* alpha = new long double[P + 1];

	apply_hamming_window(wave, st, end, arr);
	calculate_correlation(arr, 0, FRAME_SIZE, r);

	// DURBIN's algorithm
	long double E = r[0];
	for (int i = 1; i <= P; i++) {
		// At every iteration size of alpha increases by one
		long double new_alpha[P + 1];
		long double ki = r[i];
		for (int j = 1; j < i; j++) {
			ki -= alpha[j - 1] * r[i - j];
		}
		ki /= E;

		for (int j = 1; j < i; j++) {
			new_alpha[j - 1] = alpha[j - 1] - ki * alpha[i - j - 1];
		}
		alpha[i] = ki;
		for (int j = 0; j < i-1; j++) {
			alpha[j] = new_alpha[j];
		}
		E = (1 - ki * ki) * E;
	}
	return alpha;
}

long double* calculate_LPC(long double* wave, int frame_no) {
	return NULL;
}

void calculate_correlation(long double* arr, int st, int end, long double correlation[P + 1]) {
	for (int i = 0; i <= P; i++) {
		correlation[i] = 0;
		for (int m = st; m + i < end; m++) {
			correlation[i] += arr[m] * arr[m + i];
		}
	}
}


vector<double> calc_alpha(vector<double>& r) {
	// Durbins Algorithm (pg:115)
	
	vector<double> alpha;
	double E = r[0];
	for (int i = 1; i <= P; i++) {
		vector<double> new_alpha;
		// Calculating ki
		double ki = r[i];
		for (int j = 1; j < i; j++) {
			ki -= alpha[j - 1] * r[i - j];
		}
		ki /= E;

		// a[j] = a[j] - ki*(a[i-j]);
		for (int j = 1; j < i; j++) {
			new_alpha.push_back(alpha[j - 1] - ki * alpha[i - j - 1]);
		}
		// a[i] = ki
		new_alpha.push_back(ki);

		alpha = new_alpha;

		// Updating eneygy for the next iteration
		E = (1 - ki * ki) * E;
	}

	return alpha;
}





// Utility functions

void apply_hamming_window(long double* arr, int st, int end, long double* output_arr) {
	int arr_size = end - st;
	if (arr_size != FRAME_SIZE) {
		printf("ERROR: given frame's size is not equal to FRAME_SIZE\n");
		return;
	}
	for (int i = 0; i < FRAME_SIZE; i++) {
		output_arr[i] = (0.54 - 0.46 * cos(2.0 * PI * (double)i / ((double)FRAME_SIZE - 1.0)));
		output_arr[i] *= arr[st + i];
	}
	return;
}

void apply_hamming_window(long double* arr, int frame_no, long double* output_arr) {
	apply_hamming_window(arr, frame_no * FRAME_SIZE, (frame_no + 1) * FRAME_SIZE, output_arr);
}

// -------------------



vector<double> calc_cepstral(vector<double>& alpha) {
	// LPC conversion to cepstral coefficients (pg:115)
	// c[i] = a[i] + sum((k/i)*c[k]*a[i-k]) : 1<= k < i
	if (alpha.size() != P) {
		printf("alpha size(%d) mismatch with P(%d)\n", (int)alpha.size(), (int)P);
		printf("Stoping execution in function calc_cepstral()\n");
		exit(1);
	}
	vector<double> c;
	for (int i = 1; i <= P; i++) {
		double new_c = alpha[i - 1];
		for (int j = 1; j < i; j++) {
			new_c += j * c[j - 1] * alpha[i - j - 1] / i;
		}
		c.push_back(new_c);
	}
	return c;
}
