#include <iostream>
#include "mmult_baseline.h"

#include <ap_int.h>
#include <ap_fixed.h>



int main()
{
    LidarPointPacket packet;

    packet.point_num = 300;

    packet.angle_min = 0.0;
    packet.angle_increment = 0.010472;

    packet.horizontal_start = 1.75192;
    packet.horizontal_step = 0.000211152;

    packet.scan_period = 0.004618;

    packet.range_min = 0.0;
    packet.range_max = 40.0;

    packet.a_axis_dist = 0.0445;
    packet.b_axis_dist = 0.006586;

    packet.theta_angle_bias = 2.0769;
    packet.alpha_angle_bias = 0.00829031;

    packet.beta_angle = -0.00610865;
    packet.xi_angle = -0.00523599;

    packet.range_bias = -302.812;
    packet.range_scale = 0.000978692;

    for(int i=0;i<300;i++)
    {
        packet.ranges[i] = 2000;
        packet.intensities[i] = 80;
    }

    PointXYZI cloud[300];

    mmult_hw(
        &packet,
        cloud
    );

    for(int i=0;i<10;i++)
    {
        std::cout
            << cloud[i].x << " "
            << cloud[i].y << " "
            << cloud[i].z << std::endl;
    }

    return 0;
}
