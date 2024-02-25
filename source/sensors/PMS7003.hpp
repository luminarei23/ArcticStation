/*********************************************************************
 * An C++ library for interfacing with the Plantower PMS7003 laser
 * particle counter. This should also work with the PMS5003 sensor,
 * as they share the same protocol.
 *********************************************************************/

#ifndef _PMS7003_HPP_
#define _PMS7003_HPP_

// Standard libraries
#include <stdint.h>

// Custom libraries
#include "aqi_sensor_serial.hpp"

// Pico SDK libraries
#include "hardware/uart.h"

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#define PMS7003_DATA_SIZE 32 // 0x42 + 0x4d  + 28 bytes data + 2 bytes checksum = 32 bytes

/**
 * \brief Struct to hold the data received from the PMS7003 sensor.
 */
typedef struct pms7003_struct
{
  uint8_t start_1;       /**< Start char 1 0x42 (fixed) */
  uint8_t start_2;       /**< Start char 2 0x4d (fixed) */
  uint16_t frame_length; /**< Frame length = 2x13 + 2 (data + parity) */

  uint16_t pm_1_0;  /**< PM1.0 concentration (CF = 1, standard particles) Unit ug/m^3 */
  uint16_t pm_2_5;  /**< PM2.5 concentration (CF = 1, standard particulates) Unit ug/m^3 */
  uint16_t pm_10_0; /**< PM10 concentration (CF = 1, standard particulate matter) Unit ug/m^3 */

  uint16_t pm_1_0_atmos;  /**< PM1.0 concentration (in the atmosphere) Unit ug/m^3 */
  uint16_t pm_2_5_atmos;  /**< PM2.5 concentration (in the atmosphere) Unit ug/m^3 */
  uint16_t pm_10_0_atmos; /**< PM10 concentration (in the atmosphere) Unit ug/m^3 */

  uint16_t raw_gt_0_3;  /**< Particles in 0.1 liter of air > 0.3um */
  uint16_t raw_gt_0_5;  /**< Particles in 0.1 liter of air > 0.5um */
  uint16_t raw_gt_1_0;  /**< Particles in 0.1 liter of air > 1.0um */
  uint16_t raw_gt_2_5;  /**< Particles in 0.1 liter of air > 2.5um */
  uint16_t raw_gt_5_0;  /**< Particles in 0.1 liter of air > 5.0um */
  uint16_t raw_gt_10_0; /**< Particles in 0.1 liter of air > 10um */

  uint8_t version_number; /**< Version number */
  uint8_t error_code;     /**< Error code */

  uint16_t checksum; /**< Sum of each byte from start_1 ... error_code */
} PMS7003_STRUCT;

/**
 * \brief Union to handle the data buffer for the PMS7003 sensor.
 */
typedef union pms7003_data
{
  unsigned char bytes[PMS7003_DATA_SIZE]; /**< Bytes array */
  uint16_t words[PMS7003_DATA_SIZE / 2];  /**< Words array */
  PMS7003_STRUCT values;                  /**< Values struct */
} PMS7003_DATABUF;

/**
 * \brief Class for interfacing with the Plantower PMS7003 sensor.
 */
class Plantower_PMS7003
{
public:
  bool debug; /**< Debug flag */

  /**
   * \brief Default constructor.
   */
  Plantower_PMS7003();

  /**
   * \brief Initialize the UART.
   */
  void init();

  /**
   * \brief Initialize the sensor with a given serial stream.
   *
   * \param serial Pointer to the serial stream.
   */
  void init(AirSensorStream *serial);

  /**
   * \brief Update the frame data.
   */
  void updateFrame();

  /**
   * \brief Check if new data is available.
   *
   * \return True if new data is available, false otherwise.
   */
  bool hasNewData();

  /**
   * \brief Get the PM1.0 concentration.
   *
   * \return PM1.0 concentration value.
   */
  uint16_t getPM_1_0();

  /**
   * \brief Get the PM2.5 concentration.
   *
   * \return PM2.5 concentration value.
   */
  uint16_t getPM_2_5();

  /**
   * \brief Get the PM10 concentration.
   *
   * \return PM10 concentration value.
   */
  uint16_t getPM_10_0();

  /**
   * \brief Get the PM1.0 concentration in the atmosphere.
   *
   * \return PM1.0 concentration in the atmosphere value.
   */
  uint16_t getPM_1_0_atmos();

  /**
   * \brief Get the PM2.5 concentration in the atmosphere.
   *
   * \return PM2.5 concentration in the atmosphere value.
   */
  uint16_t getPM_2_5_atmos();

  /**
   * \brief Get the PM10 concentration in the atmosphere.
   *
   * \return PM10 concentration in the atmosphere value.
   */
  uint16_t getPM_10_0_atmos();

  /**
   * \brief Get the raw particle count greater than 0.3um.
   *
   * \return Raw particle count greater than 0.3um value.
   */
  uint16_t getRawGreaterThan_0_3();

  /**
   * \brief Get the raw particle count greater than 0.5um.
   *
   * \return Raw particle count greater than 0.5um value.
   */
  uint16_t getRawGreaterThan_0_5();

  /**
   * \brief Get the raw particle count greater than 1.0um.
   *
   * \return Raw particle count greater than 1.0um value.
   */
  uint16_t getRawGreaterThan_1_0();

  /**
   * \brief Get the raw particle count greater than 2.5um.
   *
   * \return Raw particle count greater than 2.5um value.
   */
  uint16_t getRawGreaterThan_2_5();

  /**
   * \brief Get the raw particle count greater than 5.0um.
   *
   * \return Raw particle count greater than 5.0um value.
   */
  uint16_t getRawGreaterThan_5_0();

  /**
   * \brief Get the raw particle count greater than 10um.
   *
   * \return Raw particle count greater than 10um value.
   */
  uint16_t getRawGreaterThan_10_0();

  /**
   * \brief Get the hardware version number.
   *
   * \return Hardware version number.
   */
  uint8_t getHWVersion();

  /**
   * \brief Get the error code.
   *
   * \return Error code.
   */
  uint8_t getErrorCode();

private:
  PMS7003_DATABUF sensorData; /**< Sensor data buffer */
  bool dataReady;             /**< Data ready flag */
  bool initialized;           /**< Initialized flag */
  AirSensorStream *serial;    /**< Pointer to the serial stream */
  unsigned char lastByte,     /**< Last byte received */
      nextByte;               /**< Next byte to be received */
  int bufferIndex;            /**< Buffer index */

  /**
   * \brief Dump the received bytes.
   */
  void dumpBytes();

  /**
   * \brief Convert sensor data.
   */
  void convertSensorData();

  /**
   * \brief Check if the checksum is valid.
   *
   * \return True if the checksum is valid, false otherwise.
   */
  bool isValidChecksum();

  /**
   * \brief Extract a uint16_t value from the data buffer.
   *
   * \param buff Pointer to the buffer.
   * \param loc Location to extract the value from.
   * \return Extracted uint16_t value.
   */
  uint16_t uint16FromBufferData(unsigned char *buff, int loc);
};

#endif /* _PMS7003_HPP_ */
