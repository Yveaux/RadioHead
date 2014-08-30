// RHSPIDriver.h
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2014 Mike McCauley
// $Id: RHSPIDriver.h,v 1.6 2014/04/14 08:37:11 mikem Exp $

#ifndef RHSPIDriver_h
#define RHSPIDriver_h

#include <RHGenericDriver.h>
#include <RHHardwareSPI.h>

// This is the bit in the SPI address that marks it as a write
#define RH_SPI_WRITE_MASK 0x80

class RHGenericSPI;

/////////////////////////////////////////////////////////////////////
/// \class RHSPIDriver RHSPIDriver.h <RHSPIDriver.h>
/// \brief Base class for a RadioHead driver that use the SPI bus
/// to communicate with its transport hardware.
///
/// This class can be subclassed by Drivers that require to use the SPI bus.
/// It can be configured to use either the RHHardwareSPI class (if there is one available on the platform)
/// of the bitbanged RHSoftwareSPI class. The dfault behaviour is to use a pre-instantiated built-in RHHardwareSPI
/// interface.
/// 
/// The read and write routines use commonly used SPI conventions, but these can be overrided in sublcasses if necessary.
///
/// Application developers are not expected to instantiate this class directly: 
/// it is for the use of Driver developers
class RHSPIDriver : public RHGenericDriver
{
public:
    /// Constructor
    /// \param[in] slaveSelectPin The controler pin to use to select the desired SPI device. This pin will be driven LOW
    /// during SPI communications with the SPI device that uis iused by this Driver.
    /// \param[in] spi Reference to the SPI interface to use. The default is to use a default built-in Hardware interface.
    RHSPIDriver(uint8_t slaveSelectPin = SS, RHGenericSPI& spi = hardware_spi);

    /// Initialise the Driver transport hardware and software.
    /// Make sure the Driver is properly configured before calling init().
    /// \return true if initialisation succeeded.
    bool init();

    /// Reads a single register from the SPI device
    /// \param[in] reg Register number
    /// \return The value of the register
    uint8_t        spiRead(uint8_t reg);

    /// Writes a single byte to the SPI device
    /// \param[in] reg Register number
    /// \param[in] val The value to write
    void           spiWrite(uint8_t reg, uint8_t val);

    /// Reads a number of consecutive registers from the SPI device using burst read mode
    /// \param[in] reg Register number of the first register
    /// \param[in] dest Array to write the register values to. Must be at least len bytes
    /// \param[in] len Number of bytes to read
    void           spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len);

    /// Write a number of consecutive registers using burst write mode
    /// \param[in] reg Register number of the first register
    /// \param[in] src Array of new register values to write. Must be at least len bytes
    /// \param[in] len Number of bytes to write
    void           spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len);

protected:
    /// Reference to the RHGenericSPI instance to use to trasnfer data with teh SPI device
    RHGenericSPI&       _spi;

    /// The pin number of the Slave Selct pin that is used to select the desired device.
    uint8_t             _slaveSelectPin;
};

#endif
