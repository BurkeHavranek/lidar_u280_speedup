#include <hls_math.h>
#include <stdint.h>
#include <ap_int.h>
#include <ap_fixed.h>
#include "mmult_baseline.h"

extern "C"
void mmult_hw(
    LidarPointPacket* packet,
    PointXYZI cloud[MAX_POINTS]
)
{
#pragma HLS INTERFACE m_axi port=packet offset=slave
#pragma HLS INTERFACE m_axi port=cloud offset=slave

#pragma HLS INTERFACE s_axilite port=packet
#pragma HLS INTERFACE s_axilite port=cloud
#pragma HLS INTERFACE s_axilite port=return

    coord_t sin_beta = hls::sin(packet->beta_angle);
    coord_t cos_beta = hls::cos(packet->beta_angle);

    coord_t sin_xi = hls::sin(packet->xi_angle);
    coord_t cos_xi = hls::cos(packet->xi_angle);

    coord_t cos_beta_sin_xi =
        cos_beta * sin_xi;

    coord_t sin_beta_cos_xi =
        sin_beta * cos_xi;

    coord_t sin_beta_sin_xi =
        sin_beta * sin_xi;

    coord_t cos_beta_cos_xi =
        cos_beta * cos_xi;

POINT_LOOP:
    for(int j=0;j<MAX_POINTS;j++)
    {
#pragma HLS PIPELINE II=12

        if(j>=packet->point_num)
            continue;

        uint16_t raw =
            packet->ranges[j];

        if(raw < 1)
            continue;

        coord_t range_fixed =
            packet->range_scale *
            ((coord_t)raw +
             packet->range_bias);

        coord_t alpha =
            packet->angle_min +
            packet->alpha_angle_bias +
            packet->angle_increment*j;

        coord_t theta =
            packet->horizontal_start +
            packet->theta_angle_bias +
            packet->horizontal_step*j;

        coord_t sin_alpha =
            hls::sin(alpha);

        coord_t cos_alpha =
            hls::cos(alpha);

        coord_t sin_theta =
            hls::sin(theta);

        coord_t cos_theta =
            hls::cos(theta);

        coord_t A =
            (-cos_beta_sin_xi +
             sin_beta_cos_xi*sin_alpha)
             * range_fixed +
             packet->b_axis_dist;

        coord_t B =
            cos_alpha *
            cos_xi *
            range_fixed;

        coord_t C =
            (sin_beta_sin_xi +
             cos_beta_cos_xi*sin_alpha)
             * range_fixed;

        cloud[j].x =
            cos_theta*A -
            sin_theta*B;

        cloud[j].y =
            sin_theta*A +
            cos_theta*B;

        cloud[j].z =
            C +
            packet->a_axis_dist;

        cloud[j].intensity =
            packet->intensities[j];
    }
}

// #include "mmult_baseline.h"

// void mmult_hw(int in1[DATA_SIZE * DATA_SIZE], int in2[DATA_SIZE * DATA_SIZE], int out[DATA_SIZE * DATA_SIZE], int dim)
// {
// #pragma HLS INTERFACE m_axi depth=16384 port=in1
// #pragma HLS INTERFACE m_axi depth=16384 port=in2
// #pragma HLS INTERFACE m_axi depth=16384 port=out
// #pragma HLS INTERFACE s_axilite port=dim  bundle=CTRL
// #pragma HLS INTERFACE s_axilite port=return bundle=CTRL

// 	loop1:for (int i = 0; i < dim; i++){
// #pragma HLS loop_tripcount min=128 max=128 avg=128
// 	    loop2:for (int j = 0; j < dim; j++){
// #pragma HLS loop_tripcount min=128 max=128 avg=128
// 	        loop3:for (int k = 0; k < dim; k++){
// #pragma HLS loop_tripcount min=128 max=128 avg=128
// 	            out[i * dim + j] += in1[i * dim + k] * in2[k * dim  + j];
// 	        }
// 	    }
// 	}
// }
