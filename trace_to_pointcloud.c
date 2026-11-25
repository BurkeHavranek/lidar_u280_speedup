#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 16384

int main(void)
{
    FILE *trace = fopen("lidar_trace.txt", "r");
    FILE *cloud = fopen("points.csv", "w");

    if (!trace || !cloud)
    {
        printf("Failed to open file\n");
        return -1;
    }

    fprintf(cloud, "x,y,z,intensity\n");

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), trace))
    {
        if (line[0] != 'C')
            continue;

        char *tok = strtok(line, ",");

        strtok(NULL, ","); /* seq */
        strtok(NULL, ","); /* sec */
        strtok(NULL, ","); /* nsec */

        int point_num = atoi(strtok(NULL, ","));

        float angle_min                  = atof(strtok(NULL, ","));
        float angle_increment            = atof(strtok(NULL, ","));
        float horizontal_start           = atof(strtok(NULL, ","));
        float horizontal_step            = atof(strtok(NULL, ","));

        strtok(NULL, ","); /* scan_period */

        float range_min                  = atof(strtok(NULL, ","));
        float range_max                  = atof(strtok(NULL, ","));

        float a_axis_dist                = atof(strtok(NULL, ","));
        float b_axis_dist                = atof(strtok(NULL, ","));
        float theta_angle_bias           = atof(strtok(NULL, ","));
        float alpha_angle_bias           = atof(strtok(NULL, ","));
        float beta_angle                 = atof(strtok(NULL, ","));
        float xi_angle                   = atof(strtok(NULL, ","));
        float range_bias                 = atof(strtok(NULL, ","));
        float range_scale                = atof(strtok(NULL, ","));

        float sin_beta = sinf(beta_angle);
        float cos_beta = cosf(beta_angle);
        float sin_xi = sinf(xi_angle);
        float cos_xi = cosf(xi_angle);

        float cos_beta_sin_xi = cos_beta * sin_xi;
        float sin_beta_cos_xi = sin_beta * cos_xi;
        float sin_beta_sin_xi = sin_beta * sin_xi;
        float cos_beta_cos_xi = cos_beta * cos_xi;

        float alpha_cur =
            angle_min + alpha_angle_bias;

        float theta_cur =
            horizontal_start + theta_angle_bias;

        for (int i = 0; i < point_num; i++)
        {
            char *rangeTok = strtok(NULL, ",");
            char *intensityTok = strtok(NULL, ",");

            if (!rangeTok || !intensityTok)
                break;

            int range_raw = atoi(rangeTok);
            int intensity = atoi(intensityTok);

            if (range_raw < 1)
            {
                alpha_cur += angle_increment;
                theta_cur += horizontal_step;
                continue;
            }

            float range_float =
                range_scale *
                ((float)range_raw + range_bias);

            if (range_float < range_min ||
                range_float > range_max)
            {
                alpha_cur += angle_increment;
                theta_cur += horizontal_step;
                continue;
            }

            float sin_alpha = sinf(alpha_cur);
            float cos_alpha = cosf(alpha_cur);
            float sin_theta = sinf(theta_cur);
            float cos_theta = cosf(theta_cur);

            float A =
                (-cos_beta_sin_xi +
                 sin_beta_cos_xi * sin_alpha)
                * range_float
                + b_axis_dist;

            float B =
                cos_alpha *
                cos_xi *
                range_float;

            float C =
                (sin_beta_sin_xi +
                 cos_beta_cos_xi * sin_alpha)
                * range_float;

            float x =
                cos_theta * A -
                sin_theta * B;

            float y =
                sin_theta * A +
                cos_theta * B;

            float z =
                C + a_axis_dist;

            fprintf(cloud,
                    "%f,%f,%f,%d\n",
                    x,
                    y,
                    z,
                    intensity);

            alpha_cur += angle_increment;
            theta_cur += horizontal_step;
        }
    }

    fclose(trace);
    fclose(cloud);

    printf("Created points.csv\n");

    return 0;
}