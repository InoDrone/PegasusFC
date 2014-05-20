/*
 * UBlox.h
 *
 *  Created on: 17 avr. 2014
 *      Author: alienx
 */

#ifndef UBLOX_H_
#define UBLOX_H_

#include "core/include/Pegasus.h"
#include "fc/include/GPSBase.h"
#include "fc/include/GPS.h"

#define UBLOX_PROTO_SYNC_CHAR1 	0xB5
#define UBLOX_PROTO_SYNC_CHAR2 	0x62

#define UBLOX_PAYLOAD_SIZE	512

#define UBLOX_NAV_CLASS	0x01
#define UBLOX_RXM_CLASS	0x02
#define UBLOX_INF_CLASS 0x04
#define UBLOX_ACK_CLASS 0x05
#define UBLOX_CFG_CLASS 0x06
#define UBLOX_MON_CLASS 0x0A
#define UBLOX_AID_CLASS 0x0B
#define UBLOX_TIM_CLASS 0x0D
#define UBLOX_LOG_CLASS 0x21

/* ACK CLASS ID */
#define UBLOX_ACK_ACK	0x01
#define UBLOX_ACK_NAK	0x00

/* CFG CLASS ID */
#define UBLOX_CFG_ANT	0x13
#define UBLOX_CFG_CFG	0x09
#define UBLOX_CFG_DAT	0x06
#define UBLOX_CFG_GNSS	0x3E
#define UBLOX_CFG_INF	0x02
#define UBLOX_CFG_ITFM	0x39
#define UBLOX_CFG_LOGFILTER	0x47
#define UBLOX_CFG_MSG	0x01
#define UBLOX_CFG_RATE	0x08
#define UBLOX_CFG_NAV5 	0x24
#define UBLOX_CFG_PRT	0x00
#define UBLOX_CFG_SBAS  0x16

/* MON CLASS ID */
#define UBLOX_MON_VER	0x04

/* NAV CLASS ID*/
#define UBLOX_NAV_POSLLH 0x02
#define UBLOX_NAV_SOL	 0x06

namespace pegasus
{
	namespace peripherals
	{

		class UBlox : public pegasus::fc::GPSBase {
			public:
				UBlox(pegasus::hal::UARTDriverBase_t* uart);

				bool init();
				uint8_t check();

			private:
				pegasus::hal::UARTDriverBase_t* _mUart;

				enum PROTO {
					UBLOX_STEP_SYNC1 = 0,
					UBLOX_STEP_SYNC2,
					UBLOX_STEP_CLASS,
					UBLOX_STEP_ID,
					UBLOX_STEP_LEN1,
					UBLOX_STEP_LEN2,
					UBLOX_STEP_PAYLOAD,
					UBLOX_STEP_CHECK1,
					UBLOX_STEP_CHECK2
				};

				struct UBloxStructVER_t {
					char swVersion[30];
					char hwVersion[10];
					char extension[30][7];
				} __attribute__((packed)) ;

				struct UBloxStructPOSLLH_t {
					uint32_t iTOW;
					int32_t lon;
					int32_t lat;
					int32_t height;
					int32_t hMSL;
					uint32_t hAcc;
					uint32_t vAcc;
				} __attribute__((packed)) ;

				struct UBloxStructSOL_t {
					  uint32_t iTOW;
					  int32_t  fTOW;
					  int16_t  week;
					  uint8_t  gpsFix;
					  uint8_t  flags;
					  int32_t  ecefX;
					  int32_t  ecefY;
					  int32_t  ecefZ;
					  int32_t  pAcc;
					  int32_t  ecefVX;
					  int32_t  ecefVY;
					  int32_t  ecefVZ;
					  int32_t  sAcc;
					  uint16_t pDOP;
					  uint8_t  res1;
					  uint8_t  numSV;
					  uint32_t res2;
				} __attribute__((packed)) ;

				struct {
					uint8_t state;
					uint8_t uClass;
					uint8_t id;
					uint16_t length;
					uint16_t count;

					uint8_t RXCK_A;
					uint8_t RXCK_B;

					uint8_t TXCK_A;
					uint8_t TXCK_B;

					uint8_t hwver;

					union {
						UBloxStructSOL_t sol;
						UBloxStructVER_t ver;
						UBloxStructPOSLLH_t posllh;
						int8_t datas[UBLOX_PAYLOAD_SIZE];
					} payload;


				} _mDatas;

				uint8_t charIn(uint8_t);
				uint8_t receive();
				void resetChecksumRX();
				void checksumRX(uint8_t);
				void resetChecksumTX();
				void checksumTX(uint8_t);

				void sendStart();
				void sendChecksum();

				void enableMessage(uint8_t uClass, uint8_t id, uint8_t rate);
				void getVersion();
				void setRate(uint16_t ms);
				void setSBAS(bool enabled);
				void setMode();

				void sendU1(uint8_t);
				void sendU2(uint16_t);
				void sendU4(uint32_t);

				void sendI1(int8_t);
				void sendI2(int16_t);
				void sendI4(int32_t);
		};

	} /* namespace peripherals */
} /* namespace pegasus */

#endif /* UBLOX_H_ */
