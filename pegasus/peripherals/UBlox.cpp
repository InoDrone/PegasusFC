/*
 * UBlox.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: alienx
 */

#include "UBlox.h"
#include "core/include/Trace.h"
#include "core/include/ThreadManager.h"
#include "core/include/MainTimer.h"

namespace pegasus {
	namespace peripherals {

	using namespace pegasus::core;

	UBlox::UBlox(pegasus::hal::UARTDriverBase_t* uart) :
		_mUart(uart) {

		memset(&_mDatas, 0, sizeof(_mDatas));
		_mDatas.state = UBLOX_STEP_SYNC1;
	}

	bool UBlox::init()
	{
		_mUart->init();
		_mUart->open();


		/* INIT UBLOX */
		sendStart();

		sendU1(UBLOX_CFG_CLASS);  // CFG
		sendU1(UBLOX_CFG_PRT);    // PRT

		sendU1(0x14);
		sendU1(0x00);

		sendU1(0x01);   // PortID
		sendU1(0x00);   // Reserved
		sendU2(0x00);   // txRead
		sendU4(0x000008D0);  // MODE
		sendU4(115200); // baudRate
		sendU2(0x0001); // inProtoMask
		sendU2(0x0001); // outProtoMask
		sendU2(0x00); // flags
		sendU2(0x00);

		sendChecksum();
		/* END INIT */

		//_mUart->setBaudRate(9600);
		//_mUart->open();

		mainTimer.delay(220);
		enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_POSLLH, 1);
		enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_SOL, 1);

		setMode();
		getVersion();
		setRate((uint16_t) 100);

		return true;
	}

	uint8_t UBlox::check()
	{
		uint8_t c;
		uint8_t ret = 0;

		while(_mUart->available()) {
			c = _mUart->read();
			ret = charIn(c);
			//pegasus::core::com.write(c, ComManager::DEBUG);
		}

		return ret;
	}

	uint8_t UBlox::charIn(uint8_t c)
	{
		switch (_mDatas.state) {
		case UBLOX_STEP_SYNC1:
			if (c == UBLOX_PROTO_SYNC_CHAR1) {
				_mDatas.state = UBLOX_STEP_SYNC2;
			}
			break;
		case UBLOX_STEP_SYNC2:
			if (c == UBLOX_PROTO_SYNC_CHAR2) {
				_mDatas.state = UBLOX_STEP_CLASS;
			} else {
				_mDatas.state = UBLOX_STEP_SYNC1;
			}
			break;
		case UBLOX_STEP_CLASS:
			_mDatas.uClass = c;
			resetChecksumRX();
			checksumRX(c);
			_mDatas.state = UBLOX_STEP_ID;
			break;
		case UBLOX_STEP_ID:
			_mDatas.id = c;
			checksumRX(c);
			_mDatas.state = UBLOX_STEP_LEN1;
			break;
		case UBLOX_STEP_LEN1:
			_mDatas.length = c;
			checksumRX(c);
			_mDatas.state = UBLOX_STEP_LEN2;
			break;
		case UBLOX_STEP_LEN2:
			_mDatas.length += (c << 8);
			checksumRX(c);
			if (_mDatas.length > (UBLOX_PAYLOAD_SIZE-1)) {
				_mDatas.length = 0;
				_mDatas.state = UBLOX_STEP_SYNC1;
			} else if (_mDatas.length > 0) {
				_mDatas.count = 0;
				_mDatas.state = UBLOX_STEP_PAYLOAD;
			} else {
				_mDatas.state = UBLOX_STEP_CHECK1;
			}
			break;
		case UBLOX_STEP_PAYLOAD:
			*((int8_t *)(&_mDatas.payload) + _mDatas.count) = c;
			checksumRX(c);
			if (++_mDatas.count == _mDatas.length) {
				_mDatas.state = UBLOX_STEP_CHECK1;
			}
			break;
		case UBLOX_STEP_CHECK1:
			if (c == _mDatas.RXCK_A) {
				_mDatas.state = UBLOX_STEP_CHECK2;
			} else {
				_mDatas.state = UBLOX_STEP_SYNC1;
			}
			break;
		case UBLOX_STEP_CHECK2:
			_mDatas.state = UBLOX_STEP_SYNC1;
			if (c == _mDatas.RXCK_B) {
				return receive();
			} else {
				return 5;
			}
			break;
		}

		return 0;
	}

	uint8_t UBlox::receive()
	{
		threadManager.lock();

		if (_mDatas.uClass == UBLOX_NAV_CLASS && _mDatas.id == UBLOX_NAV_POSLLH) {
			pegasus::fc::gpsDatas.lon = (double)_mDatas.payload.posllh.lon * (double)1e-7;
			pegasus::fc::gpsDatas.lat = (double)_mDatas.payload.posllh.lat * (double)1e-7;
			pegasus::fc::gpsDatas.height = _mDatas.payload.posllh.hMSL * 0.001f; // mm to meter
			pegasus::fc::gpsDatas.hAcc = _mDatas.payload.posllh.hAcc * 0.001f; // mm to meter
			pegasus::fc::gpsDatas.vAcc = _mDatas.payload.posllh.vAcc * 0.001f; // mm to meter

			pegasus::fc::gpsDatas.iTOW = _mDatas.payload.posllh.iTOW;
		}

		if (_mDatas.uClass == UBLOX_NAV_CLASS && _mDatas.id == UBLOX_NAV_SOL)
		{
			pegasus::fc::gpsDatas.sats = _mDatas.payload.sol.numSV;
			switch(_mDatas.payload.sol.gpsFix) {
				case 0x02:
					pegasus::fc::gpsDatas.fixType = GPS_FIX2D;
					break;
				case 0x03:
					pegasus::fc::gpsDatas.fixType = GPS_FIX3D;
					break;
				default:
					pegasus::fc::gpsDatas.fixType = GPS_NOFIX;
					break;

			}
		}

		threadManager.unlock();

		if (_mDatas.uClass == UBLOX_MON_CLASS && _mDatas.id == UBLOX_MON_VER) {
			_mDatas.hwver = atoi(_mDatas.payload.ver.hwVersion) / 10000;
			if (_mDatas.hwver > 6) {
				setRate((uint16_t) 100); // Rate to 10Hz -> 100ms
				setSBAS(0);
				trace.debug("[UBLOX] Set to 10Hz / 100ms, disabled SBAS",_mDatas.payload.ver.hwVersion, _mDatas.hwver);
			} else {
				setRate((uint16_t) 200);
				trace.debug("[UBLOX] Set to 5Hz / 200ms",_mDatas.payload.ver.hwVersion, _mDatas.hwver);
			}

			trace.debug("[UBLOX] version %s, V%d",_mDatas.payload.ver.hwVersion, _mDatas.hwver);
		}

		return 0;
	}

	void UBlox::resetChecksumRX()
	{
		_mDatas.RXCK_A = 0;
		_mDatas.RXCK_B = 0;
	}

	void UBlox::checksumRX(uint8_t c)
	{
		_mDatas.RXCK_A += c;
		_mDatas.RXCK_B += _mDatas.RXCK_A;
	}

	void UBlox::resetChecksumTX()
	{
		_mDatas.TXCK_A = 0;
		_mDatas.TXCK_B = 0;
	}

	void UBlox::checksumTX(uint8_t c)
	{
		_mDatas.TXCK_A += c;
		_mDatas.TXCK_B += _mDatas.TXCK_A;
	}

	/**************************************************************
	 * UBLOX SEND SECTIONS
	 **************************************************************/

	void UBlox::sendStart()
	{
		_mUart->write(UBLOX_PROTO_SYNC_CHAR1);
		_mUart->write(UBLOX_PROTO_SYNC_CHAR2);

		resetChecksumTX();
	}

	void UBlox::sendChecksum()
	{
		_mUart->write(_mDatas.TXCK_A);
		_mUart->write(_mDatas.TXCK_B);
	}

	void UBlox::enableMessage(uint8_t uClass, uint8_t id, uint8_t rate)
	{
		sendStart();

		sendU1(UBLOX_CFG_CLASS);
		sendU1(UBLOX_CFG_MSG);

		// Send length 3
		sendU1(0x03);
		sendU1(0x00);

		sendU1(uClass);
		sendU1(id);
		sendU1(rate);

		sendChecksum();
		mainTimer.delay(20);
	}

	void UBlox::getVersion()
	{
		sendStart();

		sendU1(UBLOX_MON_CLASS);
		sendU1(UBLOX_MON_VER);

		// Send length 0
		sendU1(0x00);
		sendU1(0x00);

		sendChecksum();
		mainTimer.delay(20);
	}

	void UBlox::setRate(uint16_t ms)
	{
		sendStart();

		sendU1(UBLOX_CFG_CLASS);
		sendU1(UBLOX_CFG_RATE);

		sendU1(0x06);
		sendU1(0x00);

		sendU2(ms);		// rate ms
		sendU2(0x01);	// Navigation Rate, in number of measurement cycles.
						// This parameter cannot be changed, and must be set to 1.
		sendU2(0x01);	// Alignment to reference time: 0 = UTC time, 1 = GPS time

		sendChecksum();
		mainTimer.delay(20);
	}

	void UBlox::setSBAS(bool enabled) {
		uint8_t modeFlag = enabled ? 0x01 : 0;

		sendStart();

		sendU1(UBLOX_CFG_CLASS);      // CFG
		sendU1(UBLOX_CFG_SBAS);       // SBAS

		sendU1(0x08);                 // length lsb
		sendU1(0x00);                 // length msb

		sendU1(modeFlag);		     // mode
		sendU1(0b011);				 // usage
		sendU1(3);					 // Tracking channel
		sendU1(0);					 //
		sendU4(0x00000000);			// SBAS System

		sendChecksum();
		mainTimer.delay(20);
	}

	void UBlox::setMode()
	{
		uint8_t i;

		sendStart();

		sendU1(UBLOX_CFG_CLASS);
		sendU1(UBLOX_CFG_NAV5);

		sendU1(0x24); // 36 Bytes
		sendU1(0x00);

		// Mask FixMode, dyn
		sendU1(0b0000101); 	// Mask LSB
		sendU1(0x00);		// Mask MSB (reserved)

		sendU1(0x06); // dynModel ( 6 = AirBone with <1g ACC, 7 = AirBorne with <2g, 8 = AirBorne with <4g)
		sendU1(0x02); // FixMode (1 = 2D, 2 = 3D, 3= Auto 2D/3D)

		for (i=0;i<32;i++) {
			sendU1(0x00);
		}

		sendChecksum();
		mainTimer.delay(20);
	}

	/* UNSIGNED SECTION */
	void UBlox::sendU1(uint8_t c)
	{
		_mUart->write(c);
		checksumTX(c);
	}

	void UBlox::sendU2(uint16_t c)
	{
		sendU1(c);
		sendU1(c >> 8);
	}

	void UBlox::sendU4(uint32_t c)
	{
		sendU1(c);
		sendU1(c >> 8);
		sendU1(c >> 16);
		sendU1(c >> 24);
	}

	/* SIGNED SECTION */
	void UBlox::sendI1(int8_t c)
	{
		_mUart->write((uint8_t) c);
		checksumTX(c);
	}

	void UBlox::sendI2(int16_t c)
	{
		sendI1(c);
		sendI1(c >> 8);
	}

	void UBlox::sendI4(int32_t c)
	{
		sendI1(c);
		sendI1(c >> 8);
		sendI1(c >> 16);
		sendI1(c >> 24);
	}

	} /* namespace peripherals */
} /* namespace pegasus */
