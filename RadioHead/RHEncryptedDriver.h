// RHEncryptedDriver.h 
// Generic encrypted layer that could use any driver
// But will encrypt all data.
//
// Author: Philippe.Rochat'at'gmail.com
 

#ifndef RHEncryptedDriver_h
#define RHEncryptedDriver_h

#include <RHGenericDriver.h>
#include <BlockCipher.h>

class RHEncryptedDriver : public RHGenericDriver {
public:
	/// Constructor. 
    /// \param[in] driver The RadioHead driver to use to transport messages.
	/// \param[in] bloccipher The blockcipher (from arduinolibs) that crypth/decrypt data.
	RHEncryptedDriver(RHGenericDriver& driver, BlockCipher& blockcipher);
	
	/// Tests whether a new message is available
    /// from the Driver. 
    /// On most drivers, this will also put the Driver into RHModeRx mode until
    /// a message is actually received by the transport, when it wil be returned to RHModeIdle.
    /// This can be called multiple times in a timeout loop
    /// \return true if a new, complete, error-free uncollected message is available to be retreived by recv()
    bool available();

    /// Turns the receiver on if it not already on.
    /// If there is a valid message available, copy it to buf and return true
    /// else return false.
    /// If a message is copied, *len is set to the length (Caution, 0 length messages are permitted).
    /// You should be sure to call this function frequently enough to not miss any messages
    /// It is recommended that you call it in your main loop.
    /// \param[in] buf Location to copy the received message
    /// \param[in,out] len Pointer to available space in buf. Set to the actual number of octets copied.
    /// \return true if a valid message was copied to buf
    bool recv(uint8_t* buf, uint8_t* len);

    /// Waits until any previous transmit packet is finished being transmitted with waitPacketSent().
    /// Then optionally waits for Channel Activity Detection (CAD) 
    /// to show the channnel is clear (if the radio supports CAD) by calling waitCAD().
    /// Then loads a message into the transmitter and starts the transmitter. Note that a message length
    /// of 0 is permitted. 
    /// \param[in] data Array of data to be sent
    /// \param[in] len Number of bytes of data to send
    /// specify the maximum time in ms to wait. If 0 (the default) do not wait for CAD before transmitting.
    /// \return true if the message length was valid and it was correctly queued for transmit. Return false
    /// if CAD was requested and the CAD timeout timed out before clear channel was detected.
    bool send(const uint8_t* data, uint8_t len);

    /// Returns the maximum message length 
    /// available in this Driver.
    /// \return The maximum legal message length
    uint8_t maxMessageLength();


private:
	/// The Driver we are to use
    RHGenericDriver&        _driver;
	/// The CipherBlock we are to use for encrypting/decrypting
	BlockCipher&		_blockcipher;
	/// Struct for with buffers for ciphering
	typedef struct {
		size_t blockSize = 0;
		uint8_t *inputBlock = NULL;
		//uint8_t *outputBlock = NULL;		
	} CipherBlocks;
	CipherBlocks cipheringBlocks;
	/// Buffer to store encrypted/decrypted message
	uint8_t *buffer;
};

#endif