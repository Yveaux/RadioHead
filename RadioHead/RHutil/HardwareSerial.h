// HardwareSerial.h
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2015 Mike McCauley
// $Id: HardwareSerial.h,v 1.2 2015/08/12 02:22:23 mikem Exp mikem $
#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <stdio.h>

// Provides access to a serial port on Unix.
// Equivalent to HardwareSerial in Arduino
// We simulate just enough to provide the services RadioHead needs on Unix
// and Windows
class HardwareSerial
{
public:
    HardwareSerial(const char* deviceName);
    void begin(int baud);
    void end();
    void flush();
    int peek(void);
    int available();
    int read();
    size_t write(uint8_t ch);

    // These are not usually in HardwareSerial but we 
    // need them in a Unix environment

    /// Wait until a character is available from the port.
    void waitAvailable();

    /// Wait until a a character is available from the port.
    /// or the timeout expires
    /// \param[in] timeout The maximum time to wait in milliseconds
    /// \return true if a message is available as reported by available()
    bool waitAvailableTimeout(uint16_t timeout);

protected:
    bool openDevice();
    bool closeDevice();
    bool setBaud(int baud);

private:
    const char* _deviceName;
    int         _device; // file desriptor
    int         _baud;
};

#endif
