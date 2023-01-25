#pragma once
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<vector>
#include<fstream>

#define ARR_LIMIT 32000
#define FRAME_LIMIT 100 // round(arr_limit / frame_size);
#define FRAME_SIZE 320 // frame_size
#define F 16000 // sample rate
#define P 12 // Cepstral coeffcients length
#define PI 3.1415926L // value of PI