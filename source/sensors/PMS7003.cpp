/*********************************************************************
An C++ library for interfacing with the Plantower PMS7003 laser
particle counter. This should also work the the PMS5003 sensor,
as they share the same protocol.
**********************************************************************/
#include "pms7003.hpp"

Plantower_PMS7003::Plantower_PMS7003()
{
  dataReady = false;
  initialized = false;
  debug = false;
}

void Plantower_PMS7003::init()
{
  serial->init();
}

void Plantower_PMS7003::init(AirSensorStream *s)
{
  dataReady = false;
  serial = s;
  nextByte = 0;
  lastByte = 0;
  bufferIndex = 0;
  initialized = true;
}

void Plantower_PMS7003::updateFrame()
{
  if (!initialized)
  {
    return;
  }
  dataReady = false;
  if (serial->isBusy())
  {
    nextByte = serial->read();

    if (nextByte == 0x4d && lastByte == 0x42)
    {
      sensorData.bytes[0] = 0x42;
      bufferIndex = 1;
    }
    sensorData.bytes[bufferIndex] = nextByte;
    bufferIndex++;
    lastByte = nextByte;
  }

  if (bufferIndex == PMS7003_DATA_SIZE)
  {
    if (sensorData.bytes[0] == 0x42 && sensorData.bytes[1] == 0x4d)
    {
      if (debug)
      {
        dumpBytes();
      }
      convertSensorData();
      if (debug)
      {
        dumpBytes();
      }

      if (isValidChecksum())
      {
        dataReady = true;
      }
    }
    bufferIndex = 0;
  }
}

bool Plantower_PMS7003::hasNewData()
{
  return dataReady;
}

uint16_t Plantower_PMS7003::getPM_1_0()
{
  return sensorData.values.pm_1_0;
}
uint16_t Plantower_PMS7003::getPM_2_5()
{
  return sensorData.values.pm_2_5;
}
uint16_t Plantower_PMS7003::getPM_10_0()
{
  return sensorData.values.pm_10_0;
}
uint16_t Plantower_PMS7003::getPM_1_0_atmos()
{
  return sensorData.values.pm_1_0_atmos;
}
uint16_t Plantower_PMS7003::getPM_2_5_atmos()
{
  return sensorData.values.pm_2_5_atmos;
}
uint16_t Plantower_PMS7003::getPM_10_0_atmos()
{
  return sensorData.values.pm_10_0_atmos;
}

uint16_t Plantower_PMS7003::getRawGreaterThan_0_3()
{
  return sensorData.values.raw_gt_0_3;
}
uint16_t Plantower_PMS7003::getRawGreaterThan_0_5()
{
  return sensorData.values.raw_gt_0_5;
}
uint16_t Plantower_PMS7003::getRawGreaterThan_1_0()
{
  return sensorData.values.raw_gt_1_0;
}
uint16_t Plantower_PMS7003::getRawGreaterThan_2_5()
{
  return sensorData.values.raw_gt_2_5;
}
uint16_t Plantower_PMS7003::getRawGreaterThan_5_0()
{
  return sensorData.values.raw_gt_5_0;
}
uint16_t Plantower_PMS7003::getRawGreaterThan_10_0()
{
  return sensorData.values.raw_gt_10_0;
}

uint8_t Plantower_PMS7003::getHWVersion()
{
  return sensorData.values.version_number;
}
uint8_t Plantower_PMS7003::getErrorCode()
{
  return sensorData.values.error_code;
}

void Plantower_PMS7003::dumpBytes()
{
  for (int i = 0; i < PMS7003_DATA_SIZE; i++)
  {
    char buffer[2];
    buffer[0] = sensorData.bytes[i];
    buffer[1] = '\0'; // Null-terminate the string
    // TODO: add logic to forward this data somewhere idk for now
  }
}

// fix sensor data endianness
void Plantower_PMS7003::convertSensorData()
{
  int loc = 0;

  // first word (bytes 0 and 1) are 8 bit
  loc += 1;

  // next 13 words are 16-bit ints
  for (int i = 0; i < 13; i++)
  {
    sensorData.words[loc] = uint16FromBufferData(sensorData.bytes, loc * 2);
    loc++;
  }

  // next word is 2 8-bit bytes
  loc += 1;

  // final checksum word is 16-bit
  sensorData.words[loc] = uint16FromBufferData(sensorData.bytes, loc * 2);
}

bool Plantower_PMS7003::isValidChecksum()
{
  uint16_t sum = 0;

  for (int i = 0; i < (PMS7003_DATA_SIZE - 2); i++)
  {
    sum += sensorData.bytes[i];
  }
  if (sum == sensorData.values.checksum)
  {
    return 1;
  }
  return 0;
}

uint16_t Plantower_PMS7003::uint16FromBufferData(unsigned char *buff, int loc)
{
  if (loc > PMS7003_DATA_SIZE - 2 || loc < 2)
  {
    return -1;
  }
  return ((buff[loc] << 8) + buff[loc + 1]);
}
