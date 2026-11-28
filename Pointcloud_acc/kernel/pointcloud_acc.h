// Copyright (C) 2024 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <ap_int.h>
#include <ap_fixed.h>

#ifndef MMULT_BASELINE_H
#define MMULT_BASELINE_H

#define MAX_POINTS 300

typedef ap_fixed<24,8> coord_t;
typedef ap_uint<16> range_t;
typedef ap_uint<8> intensity_t;


struct LidarPointPacket
{
    coord_t angle_min;
    coord_t angle_increment;

    coord_t horizontal_start;
    coord_t horizontal_step;

    coord_t scan_period;
    coord_t range_min;
    coord_t range_max;

    coord_t range_scale;
    coord_t range_bias;

    coord_t a_axis_dist;
    coord_t b_axis_dist;

    coord_t theta_angle_bias;
    coord_t alpha_angle_bias;

    coord_t beta_angle;
    coord_t xi_angle;

    ap_uint<9> point_num;

    range_t ranges[300];
    intensity_t intensities[300];
};

struct PointXYZI
{
    coord_t x;
    coord_t y;
    coord_t z;
    intensity_t intensity;
};

extern "C" void mmult_hw(LidarPointPacket* packet,PointXYZI cloud[MAX_POINTS]);

#endif
