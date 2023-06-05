#include "lpfilter.h"

// butterworth
// fs = 50; fc = 7.5
//const float b_coefficient[size_filter] = {18.5630, 74.2520, 111.3781, 74.2520, 18.5630};
//const float a_coefficient[size_filter] = {1000.0, -1570.4, 1275.6, -484.4, 76.2};

// shebyshev 
// fs = 50, Rp = 1, fc = 4
const float b_coefficient[size_filter] = {0.78833, 3.2, 4.7, 3.2, 0.78833};
const float a_coefficient[size_filter] = {1000, -3292.8, 4294.2, -2607.4, 620.1};
	
float input_buf[size_filter] = {0};
int end_input_buf = 0;
float filted_buf[size_filter] = {1};
int end_filted_buf = 0;

#define last_idex(current) ((current-1+size_filter)%size_filter)
#define next_idex(current) ((current+1)%size_filter)

float low_pass_filter(float input) {
    uint8_t input_buf_idex = end_input_buf;
    input_buf[end_input_buf] = input; 
    end_input_buf = next_idex(end_input_buf);
    float MA_input = 0;
    for(uint8_t i = 0; i < size_filter; i++) {           
        MA_input += input_buf[input_buf_idex] * b_coefficient[i];
        input_buf_idex = last_idex(input_buf_idex);
    }

    float MA_output = 0;
    uint8_t filted_buf_idex = last_idex(end_filted_buf);
    for(uint8_t i = 1; i < size_filter; i++) {
        MA_output += filted_buf[filted_buf_idex] * a_coefficient[i];
        filted_buf_idex = last_idex(filted_buf_idex);
    }
    float temp = (MA_input - MA_output) / a_coefficient[0];
    filted_buf[end_filted_buf] = temp; 
    end_filted_buf = next_idex(end_filted_buf);
    return temp;
}

uint8_t is_peak(void) {
		uint8_t a[5];
		a[0] = last_idex(end_filted_buf);
		for(int i = 1; i < 5; i++) {
				a[i] = last_idex(a[i-1]);
		}
		if ((filted_buf[a[0]] < filted_buf[a[1]]) && (filted_buf[a[1]] < filted_buf[a[2]]) 
				&& (filted_buf[a[2]] > filted_buf[a[3]]) && (filted_buf[a[3]] > filted_buf[a[4]]) && filted_buf[a[2]] > 1.2) {
						return 1;
				}
		return 0;
}
