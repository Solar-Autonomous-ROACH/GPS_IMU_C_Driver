/*
 * imu.h - Header file for IMU (Inertial Measurement Unit) sensor integration
 *
 * This header file provides an interface for interacting with an IMU sensor over
 * the I2C communication protocol. It includes functions and constants for retrieving
 * accelerometer, gyroscope, and magnetometer data from the IMU sensor. The code is
 * designed for a specific IMU sensor and may require configuration adjustments for
 * different hardware or use cases.
 *
 * CREDIT/CODE MODIFIED FROM:
 * https://github.com/GoScoutOrg/Rover/blob/749a7758aef85ed877ad6db56e223f91a708abdf/src/rover/imu.py
 *
 * Dependencies:
 * - i2c/smbus.h, linux/i2c-dev.h, linux/i2c.h: Required for I2C communication.
 * - cstdint, sys/ioctl.h, time.h: Standard C++ libraries.
 *
 * Constants and Definitions:
 * - Various constants for I2C addresses, register addresses, data sizes, and sensitivities.
 *
 * Class:
 * - Imu: A class that encapsulates IMU functionality, including I2C communication,
 *   sensor data retrieval, and telemetry functions.
 *
 * Usage:
 * - Include this header file in your C++ project to interact with an IMU sensor.
 * - Instantiate the Imu class to communicate with the IMU sensor and retrieve data.
 *
 * Note: This code is designed for a specific IMU sensor and may require adaptation for
 *       other IMU sensors or hardware configurations. Refer to the provided credit and
 *       documentation for further details on usage and customization.
 */

#ifndef IMU_H
#define IMU_H

extern "C" {
	#include <i2c/smbus.h>
	#include <linux/i2c-dev.h>
	#include <linux/i2c.h>
}
#include <cstdint>
#include <sys/ioctl.h>
#include <time.h>
#include <chrono>
#include <thread>

/** IMU Constants */
#define TIME_DELAY_MS 1000
#define ACCEL_REG_START 0x00
#define MAGNETO_REG_START 0x00
#define ACCEL_MAG_DATA_SIZE 12
#define SENSORS_GRAVITY_STD 9.81F
#define BYTE_SHIFT_AMOUNT 8
#define BANK_VALUE 0x02
#define PWR_MGMT_2_VALUE 0x000000
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

/** I2C Specifics */
#define IMU_I2C_ADDRESS 0x69
#define IMU_I2C_BUS "/dev/i2c-1"
#define IMU_ID 0xEA

/** General Registers */
#define BANK_SEL 0x7F
#define PWR_MGMT_2 0x07

/** Gyroscope Registers */
#define GYRO_REG_START 0x00
#define GYRO_CONFIG 0x01
#define GYRO_XOUT_H 0x33
#define GYRO_XOUT_L 0x34
#define GYRO_YOUT_H 0x35
#define GYRO_YOUT_L 0x36
#define GYRO_ZOUT_H 0x37
#define GYRO_ZOUT_L 0x38

/** Gyroscope sensitivity at 250dps */
#define GYRO_SENSITIVITY_250DPS (0.0078125F) // Table 35 of datasheet
#define GYRO_DATA_SIZE 6
#define GYRO_CONFIG_VALUE 0x11 << 1

/** Macro for mg per LSB at +/- 2g sensitivity (1 LSB = 0.000244mg) */
#define ACCEL_MG_LSB_2G (0.000244F)
/** Macro for mg per LSB at +/- 4g sensitivity (1 LSB = 0.000488mg) */
#define ACCEL_MG_LSB_4G (0.000488F)
/** Macro for mg per LSB at +/- 8g sensitivity (1 LSB = 0.000976mg) */
#define ACCEL_MG_LSB_8G (0.000976F)

/** Macro for micro tesla (uT) per LSB (1 LSB = 0.1uT) */
#define MAG_UT_LSB (0.1R)

class Imu {
private:
	int i2c_fd;
	int16_t accelerometer[3];
	int16_t magnetometer[3];
	int16_t gyroscope[3];
	void begin(void);

public:
	Imu();
	~Imu();
	void printAccel(void);
	void printMag(void);
	void printGyro(void);
	void Telementary(int delay);
	void readSensorData(void);

    const int16_t* getAccelerometerData() const {
        return accelerometer;
    }

    const int16_t* getMagnetometerData() const {
        return magnetometer;
    }

    const int16_t* getGyroscopeData() const {
        return gyroscope;
    }
};

#endif // IMU_H
