#include "lpfilter.h"
// a and b coeficient of butterworth filter
// fs = 100 fc = 7.5
//const float b_coefficient[size_filter] = {1.7826, 7.1304, 10.6957, 7.1304, 1.7826};
//const float a_coefficient[size_filter] = {1000, -2773.7, 3019, -1504.8, 287.9304};

// fs = 50; fc = 7.5
const float b_coefficient[size_filter] = {18.5630, 74.2520, 111.3781, 74.2520, 18.5630};
const float a_coefficient[size_filter] = {1000.0, -1570.4, 1275.6, -484.4, 76.2};
float input_buf[size_filter] = {0};
int end_input_buf = 0;
float filted_buf[size_filter] = {0};
int end_filted_buf = 0;

uint8_t last_idex(uint8_t current) {
    if(current == 0) {
        return size_filter - 1;
    } else {
        return current - 1;
    }
}

uint8_t next_idex(uint8_t current) {
    return (current + 1) % size_filter;
}

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
