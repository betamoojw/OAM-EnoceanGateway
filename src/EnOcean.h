/*
   EnOcean.h

*/

#ifndef ENOCEAN_H_
#define ENOCEAN_H_

#include "Arduino.h"
//#include "DebugUtil.h"
#include "EnoceanGateway.h"
#include <knx.h>

// important to change this to the number of devices defined.
#define MAX_NUMBER_OF_DEVICES     50

#define SIMULATE_NOTHING 0
#define SIMULATE_PUSH    1
#define SIMULATE_RELEASE 2
#define SIMULATE_PAUSE_BEFORE_RELEASE 100


#define ENOCEAN_OK                 0
#define ENOCEAN_OUT_OF_RANGE      21
#define ENOCEAN_NOT_VALID_CHKSUM   7
#define ENOCEAN_NO_RX_TEL          6
#define ENOCEAN_NEW_RX_BYTE        3


/*
  	Packet type
*/

#define u8RADIO_ERP1 0x01
#define u8RESPONSE   0x02

/*
  	EEP type
*/

// Receive
#define u8RORG_1BS 0xD5
#define u8RORG_RPS 0xF6
#define u8RORG_VLD 0xD2
#define u8RORG_4BS 0xA5
#define u8RORG_Rocker 0xFA

// Send
#define u8RORG_COMMON_COMMAND 0x05
// COMANDS

#define u8CO_RD_IDBASE 0x08

#define RPS_BUTTON_CHANNEL_AI 0
#define RPS_BUTTON_CHANNEL_AO 1
#define RPS_BUTTON_CHANNEL_BI 2
#define RPS_BUTTON_CHANNEL_BO 3

#define RPS_BUTTON_2NDACT_NO    0
#define RPS_BUTTON_2NDACT_VALID 1

#define VLD_CMD_ID_01 0x01
#define VLD_CMD_ID_02 0x02
#define VLD_CMD_ID_03 0x03
#define VLD_CMD_ID_04 0x04
#define VLD_CMD_ID_05 0x05
#define VLD_CMD_ID_06 0x06

#define RPS_Func_10 0xA

#define BASEID_BYTES 4
#define DATBUF_SZ 100

#define SER_SYNCH_CODE 0x55
#define SER_HEADER_NR_BYTES 4

//Rocker States
#define AI_pressed 0x10
#define AO_pressed 0x30
#define BI_pressed 0x50
#define BO_pressed 0x70
#define CI_pressed 0x15
#define CO_pressed 0x37

#define AI_release 0x00
#define AO_release 0x20
#define BI_release 0x40
#define BO_release 0x60
#define CI_release 0x05
#define CO_release 0x07


/*
struct VLD_D2_04_00_TELEGRAM_CMD_SENRESP_TYPE
{
  uint8_t DB_Bit7 
  ...   
  uint8_t DB_Bit0 
};
*/

struct ONEBS_TEL_DATA_TYPE
{
  uint8_t State : 1; // (DB_BIT 0)   open 1: closed
  uint8_t NA : 7;    // (DB_BIT 7-1) not used
};

struct RPS_TEL_DATA_TYPE
{
  uint8_t SA : 1; // (DB_BIT 0)   No 2nd action; 1: Valid second action
  uint8_t R2 : 3; // (DB_BIT 3-2) Rocker second action.
  uint8_t EB : 1; // (DB_BIT 4)   Released; 1: Pressed
  uint8_t R1 : 3; // (DB_BIT 7-5) Rocker first action. See
};

struct RPS_F6_10_00_DATA_TYPE
{  
  uint8_t NA : 4;    // (DB_BIT 3-0) Not use XXXX
  uint8_t STATE : 4; // (DB_BIT 7-4) Status
};

struct RPS_F6_10_01_DATA_TYPE
{
  uint8_t NA2 : 1;      // (DB_BIT 0)   Not used
  uint8_t HANDLE : 1;   // (DB_BIT 1)   Handle
  uint8_t NA1 : 2;      // (DB_BIT 3-2) Not used
  uint8_t STATE : 2;    // (DB_BIT 5-4) Status
  uint8_t NA : 2;       // (DB_BIT 7-6) only 0x11
};


struct FOURBS_A5_06_01_DATA_TYPE  // good?
{
  uint8_t NA2 : 4; // (DB_BIT 3-0) NA
  uint8_t LRN : 1; // (DB_BIT 4)   LRN Bit
  uint8_t NA : 2;  // (DB_BIT 6-5) NA
  uint8_t RS : 1;  // (DB_BIT 7)   Range Select
};

struct FOURBS_A5_06_03_DATA_TYPE  // Good
{
  uint8_t Na : 6;  // (DB_BIT 5-0) NA
  uint8_t LUX : 2; // (DB_BIT 7-6) LUX
};

struct FOURBS_A5_07_01_DATA_TYPE
{
  uint8_t AVAILABLE_BAT : 1 ; // (DB_BIT 0)   Available Bat voltage Status
  uint8_t NA : 2;             // (DB_BIT 2-1) NA
  uint8_t LRN : 1;            // (DB_BIT 3)   LRN Bit
  uint8_t NA2 : 4;            // (DB_BIT 7-4) NA
};

struct FOURBS_A5_07_02_DATA_TYPE
{
  uint8_t Na : 7;  // (DB_BIT 6-0) Not used
  uint8_t PIR : 1; // (DB_BIT 7)   PIR
};

struct FOURBS_A5_07_03_DATA_TYPE  
{
  uint8_t Na : 6;  // (DB_BIT 5-0) NA
  uint8_t LUX : 2; // (DB_BIT 7-6) LUX
};

struct FOURBS_A5_07_03_DATA_TYPE2
{
  uint8_t Na : 7;  // (DB_BIT 6-0) Not used
  uint8_t PIR : 1; // (DB_BIT 7)   PIR
};

/*
struct 
{
  uint8_t DB_Bit0 | Arduino Bit7  
  ...   
  uint8_t DB_Bit7 | Arduino Bit0   
};
*/

struct VLD_TEL_MEASUREMENT_TYPE
{
  uint8_t type : 3; // NOT USED
  uint8_t dummy : 5; //
};

struct VLD_D2_01_TELEGRAM_CMD_03_ACTS_TYPE
{
  uint8_t outputCH : 5; // (DB_BIT 4-0) Channel
  uint8_t notused : 3;  // (DB_BIT 7-5) NOT USED
};

struct VLD_D2_01_TELEGRAM_CMD_04_ACTRESP_TYPE
{
  uint8_t outputValue : 7; // (DB_BIT 6-0) Output Value
  uint8_t LC : 1;          // (DB_BIT 7)   Load control
};

struct VLD_D2_01_TELEGRAM2_CMD_04_ACTRESP_TYPE
{  
  uint8_t IOChannel : 5;   // (DB_BIT 4-0) I/O Channel
  uint8_t EL : 2;          // (DB_BIT 6-5) Error Level
  uint8_t OC : 1;          // (DB_BIT 7)   Overcurrent
};

struct VLD_D2_04_00_TELEGRAM_CMD_SENRESP_TYPE  
{
  uint8_t nouse : 4;    // (DB_BIT 0-3) not use     
  uint8_t batt : 3;     // (DB_BIT 4-6) Battery autonomy      
  uint8_t daynight : 1; // (DB_BIT 7)   Day / Night          
};

struct VLD_D2_05_TELEGRAM_CMD_04_ACTRESP_TYPE
{
  uint16_t CMD : 4;       // (DB_BIT 3-0) CMD = 0x4
  uint16_t IOChannel : 4; // (DB_BIT 7-4) Channel
  uint16_t LM : 3;        // (DB_BIT 2-0) locking MOde
  uint16_t NA : 5;        // (DB_BIT 7-3) NOT USED
};





struct VLD_D2_01_TELEGRAM_CMD_03_TYPE
{
  VLD_D2_01_TELEGRAM_CMD_03_ACTS_TYPE u8VldTelActSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
  uint8_t u8SubTelegramm;
  uint8_t u8DestinationId_p[4];
};

struct VLD_D2_01_TELEGRAM_CMD_04_TYPE
{
  VLD_D2_01_TELEGRAM_CMD_04_ACTRESP_TYPE u8VldTelActResp;
  VLD_D2_01_TELEGRAM2_CMD_04_ACTRESP_TYPE u8VldTelActResp2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_04_00_TELEGRAM
{
  uint8_t voc;
  uint8_t hum;
  uint8_t temp;
  VLD_D2_04_00_TELEGRAM_CMD_SENRESP_TYPE u8VldTelSenSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_05_00_TELEGRAM_CMD_04_TYPE
{
  uint8_t pos;
  uint8_t angle;
  VLD_D2_05_TELEGRAM_CMD_04_ACTRESP_TYPE u8VldTelSenSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};


struct RPS_TELEGRAM_TYPE
{
  RPS_TEL_DATA_TYPE u8RpsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct RPS_F6_10_00_TYPE
{
  RPS_F6_10_00_DATA_TYPE u8RpsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct RPS_F6_10_01_TYPE
{
  RPS_F6_10_01_DATA_TYPE u8RpsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_02_TYPE
{
  uint8_t NA;
  uint8_t NA1;
  uint8_t Temp;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_02_2030TYPE
{
  uint8_t NA;
  uint8_t TempMSB;
  uint8_t TempLSB;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_04_TYPE
{
  uint8_t NA;
  uint8_t Hum;
  uint8_t Temp;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_04_03_TYPE
{
  uint8_t Hum;
  uint8_t TempMSB;
  uint8_t TempLSB;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_06_01_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Illumination1;
  uint8_t u8Illumination2;
  FOURBS_A5_06_01_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_06_03_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8LuxMSB;
  FOURBS_A5_06_03_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};



struct FOURBS_A5_07_01_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t Free;
  uint8_t PIR;
  FOURBS_A5_07_01_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_07_02_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Illumination1;
  uint8_t u8Illumination2;
  FOURBS_A5_07_02_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_07_03_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8LuxMSB;
  FOURBS_A5_07_03_DATA_TYPE u84BsTelData2;
  FOURBS_A5_07_03_DATA_TYPE2 u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};



struct ONEBS_TELEGRAM_TYPE
{
  ONEBS_TEL_DATA_TYPE u81bsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

//! uart_getPacket state machine states.
enum STATES_GET_PACKET
{
  //! Waiting for the synchronisation byte 0x55
  GET_SYNC_STATE = 0,
  //! Copying the 4 after sync byte: raw data length (2 bytes), optional data length (1), type (1).
  GET_HEADER_STATE,
  //! Checking the header CRC8 checksum. Resynchronisation test is also done here
  CHECK_CRC8H_STATE,
  //! Copying the data and optional data bytes to the paquet buffer
  GET_DATA_STATE,
  //! Checking the info CRC8 checksum.
  CHECK_CRC8D_STATE,
};

//! Packet structure (ESP3)
struct PACKET_SERIAL_TYPE
{
  // Amount of raw data bytes to be received. The most significant byte is sent/received first
  uint16_t u16DataLength;
  // Amount of optional data bytes to be received
  uint8_t u8OptionLength;
  // Packetype code
  uint8_t u8Type;
  // Data buffer: raw data + optional bytes
  uint8_t *u8DataBuffer;
};

class IEnOceanDevice
{
  protected:
    uint8_t firstComObj = 0;
    uint8_t firstParameter = 0;
    uint8_t index = 0;
  public :
    uint8_t numberOfComObjects;
    uint8_t numberOfParameters;

    virtual ~IEnOceanDevice()
    {
    }
    virtual void init(uint8_t startAtComObj, uint8_t startAtParameter, uint8_t channel) = 0;
    virtual void task() = 0;
    virtual bool handleEnOceanPacket(PACKET_SERIAL_TYPE* f_Pkt_st) = 0;
    virtual void handleKnxEvents(byte koIndex, byte koNr, GroupObject &iKo) = 0;
};

class EnOcean
{
    EnOcean();
    virtual ~EnOcean();

  public:
    static EnOcean Eno;

    

    void initSerial(Stream &serial);
    void init();
    void task();

    bool sendPacket(PACKET_SERIAL_TYPE *pPacket);
    void obtainSenderId(uint8_t* senderId);
    void handleKnxEvents(byte koIndex, byte koNr,GroupObject &iKo);

    void configureDevice(IEnOceanDevice &device, uint8_t channel);
    uint16_t getNumberDevices();
    uint8_t* getBaseId();
  private:
    Stream* _serial;   
    bool isInited;

    uint8_t u8CRC;
    uint8_t u8RxByte;
    uint8_t u8RetVal;

    uint8_t lastParam;
    uint8_t lastComObj;
    uint8_t lastDevice;
    uint8_t lastSenderIdOffset = 0;

    uint8_t u8datBuf[DATBUF_SZ];

    IEnOceanDevice* deviceRegistry[MAX_NUMBER_OF_DEVICES] { NULL };

    STATES_GET_PACKET u8State;
    PACKET_SERIAL_TYPE m_Pkt_st;

    static uint8_t u8CRC8Table[256];

    uint8_t lui8_BaseID_p[BASEID_BYTES];

    void readBaseId(uint8_t* fui8_BaseID_p);
    void getEnOceanMSG(uint8_t u8RetVal, PACKET_SERIAL_TYPE* f_Pkt_st);

    uint8_t uart_getPacket(PACKET_SERIAL_TYPE *pPacket, uint16_t u16BufferLength);
    uint8_t uart_sendPacket(PACKET_SERIAL_TYPE *pPacket);
};
extern EnOcean& enOcean;

#endif /* ENOCEAN_H_ */