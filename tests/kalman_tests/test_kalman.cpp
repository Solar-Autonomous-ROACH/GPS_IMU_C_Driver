#include "../../gps_module/gps.h"
#include "../../imu_module/imu.h"
#include "../../extended_kalman_filter/ekfNavINS.h"
#include <stdio.h>
#include <csignal>
#include <iostream>

#define CURRENT_YEAR 2024

// Define a flag to indicate if the program should exit gracefully.
volatile bool exit_flag = false;

// Signal handler function for Ctrl+C (SIGINT)
void signal_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Ctrl+C received. Cleaning up..." << std::endl;

        // Set the exit flag to true to trigger graceful exit.
        exit_flag = true;
    }
}

int main(void) {
    // Register the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, signal_handler);

    Gps gps_module;
    Imu imu_module;
    ekfNavINS ekf;
    float ax, ay, az, gx, gy, gz, hx, hy, hz, pitch, roll, yaw;

    while(!exit_flag) {
        PVTData gps_data = gps_module.GetPvt(true, 1);
        if (gps_data.year == CURRENT_YEAR && gps_data.numberOfSatellites > 0) {
            const int16_t *accel_data = imu_module.getAccelerometerData();
            if (accel_data[0] == ACCEL_MAX_THRESHOLD && accel_data[1] == ACCEL_MAX_THRESHOLD && accel_data[2] == ACCEL_MAX_THRESHOLD) {
                printf("Accelerometer data is invalid.\n");
                continue;
            } else {
                printf("Acceleration (m/s^2): (X: %d, Y: %d, Z: %d)\n", accel_data[0], accel_data[1], accel_data[2]);
            }

            ax = accel_data[0];
            ay = accel_data[1];
            ay = ay * -1;
            az = accel_data[2];

            const int16_t *gyro_data = imu_module.getGyroscopeData();
            if (gyro_data[0] == GYRO_MAX_THRESHOLD && gyro_data[1] == GYRO_MAX_THRESHOLD && gyro_data[2] == GYRO_MAX_THRESHOLD) {
                printf("Gyroscope data is invalid.\n");
                continue;
            } else {
                printf("Gyroscope (radians/s): (X: %d, Y: %d, Z: %d)\n", gyro_data[0], gyro_data[1], gyro_data[2]);
            }

            gx = gyro_data[0];
            gy = gyro_data[1];
            gz = gyro_data[2];

            const int16_t *mag_data = imu_module.getMagnetometerData();
            if (mag_data[0] == MAG_MAX_THRESHOLD && mag_data[1] == MAG_MAX_THRESHOLD && mag_data[2] == MAG_MAX_THRESHOLD) {
                printf("Magnetometer data is invalid.\n");
                continue;
            } else {
                printf("Magnetometer (uTesla): (X: %d, Y: %d, Z: %d)\n", mag_data[0], mag_data[1], mag_data[2]);
            }

            hx = mag_data[0];
            hy = mag_data[1];
            hz = mag_data[2];

            std::tie(pitch,roll,yaw) = ekf.getPitchRollYaw(ax, ay, az, hx, hy, hz);
            ekf.ekf_update(time(NULL) /*,gps.getTimeOfWeek()*/, gps_data.velocityNorth*1e-3, gps_data.velocityEast*1e-3,
                           gps_data.velocityDown*1e-3, gps_data.latitude*1e-7*DEG_TO_RAD,
                           gps_data.longitude*1e-7*DEG_TO_RAD, (gps_data.height*1e-3),
                           gx*DEG_TO_RAD, gy*DEG_TO_RAD, gz*DEG_TO_RAD,
                           ax, ay, az, hx, hy, hz);

            /* Plot the Long, Lat, and Height */
//            printf("Longitude: %f\n", data.longitude);
//            printf("Latitude: %f\n", data.latitude);
//            printf("Height: %d\n", data.height);
//            printf("Height above MSL: %d\n", data.heightMSL);
//            printf("Horizontal Accuracy: %u\n", data.horizontalAccuracy);
//            printf("Vertical Accuracy: %u\n", data.verticalAccuracy);
            /* Plot the Velocities and Headings */
//            printf("North Velocity: %d\n", data.velocityNorth);
//            printf("East Velocity: %d\n", data.velocityEast);
//            printf("Down Velocity: %d\n", data.velocityDown);
//            printf("Ground Speed: %d\n", data.groundSpeed);
//            printf("Vehicle Heading: %d\n", data.vehicalHeading);
//            printf("Motion Heading: %d\n", data.motionHeading);
//            printf("Speed Accuracy: %u\n", data.speedAccuracy);
//            printf("Motion Heading Accuracy: %d\n", data.motionHeadingAccuracy);
            /* Don't Plot These */
            //printf("Magnetic Declination: %d\n", data.magneticDeclination);
            //printf("Magnetic Declination Accuracy: %u\n", data.magnetDeclinationAccuracy);
            printf("\n---------------------\n");


        } else {
            printf("No GPS data\n");
        }
        sleep(1);
    }

    return 0;
}
