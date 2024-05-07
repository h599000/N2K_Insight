// Libraries from https://github.com/ttlappalainen
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <NMEA2000_esp32.h>
#include <N2kMessages.h>
#include <N2kMessagesEnumToStr.h>

#include <Wire.h> // Library for I2C Communication

// Custom libraries
#include "N2K_decomposed.h"
#include "N2K_composed.h"
#include <N2K_I2C_API.h>

#define ESP32_CAN_TX_PIN GPIO_NUM_25 // Uncomment this and set right CAN TX pin definition, if you use ESP32 and do not have TX on default IO 16
#define ESP32_CAN_RX_PIN GPIO_NUM_26 // Uncomment this and set right CAN RX pin definition, if you use ESP32 and do not have RX on default IO 4
#define I2C_ADDRESS 0x08             // Slave address for this device
#define I2C_SDA_PIN 23
#define I2C_SCL_PIN 22

tNMEA2000_esp32 NMEA2000;

PGN129026_d *n2kCOGSOG;
N2K_composed *composedN2KCOGSOG;


typedef struct
{
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg);
} tNMEA2000Handler;

void COGSOG(const tN2kMsg &N2kMsg);

/**
 * @brief A list of handler methods.
 * 
 */
tNMEA2000Handler NMEA2000Handlers[] = {
    {129026L, &COGSOG},
    {0, 0}};

Stream *OutputStream;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);

void setup()
{
  Serial.begin(115200);
  delay(500);
  OutputStream = &Serial;

  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetForwardStream(OutputStream);
  NMEA2000.SetMode(tNMEA2000::N2km_ListenOnly);

  // Set false below, if you do not want to see messages parsed to HEX within library
  NMEA2000.EnableForward(false);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
  NMEA2000.Open();
  OutputStream->print("Running...");

  Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN); // Sets the pins used for I2C communication
  Wire.onReceive(receiveData);            // Sets method for handling I2C receive events
  Wire.onRequest(requestData);            // Sets method for handling I2C request events
  Wire.begin(I2C_ADDRESS);                // Begins I2C communication and sets this device´s address
}

/**
 * @brief Function for parsing N2kMsg and extracting COG and SOG.
 * 
 * This function parses a NMEA 2000 COG and SOG rapid update message (PGN 129026),
 * extracting the SID (Source ID), Heading Reference, COG (Course Over Ground), and SOG (Speed Over Ground).
 * The parsed values are then used to initialize a new PGN129026_d object.
 * 
 * @param N2kMsg The N2kMsg object representing the NMEA 2000 message to parse. 
 */
void COGSOG(const tN2kMsg &N2kMsg)
{
  unsigned char SID;
  tN2kHeadingReference HeadingReference;
  double COG;
  double SOG;

  if (ParseN2kCOGSOGRapid(N2kMsg, SID, HeadingReference, COG, SOG))
  {
    n2kCOGSOG = new PGN129026_d(N2kMsg.MsgTime, 129026, SID, HeadingReference, SOG, COG);
  }

  /*
  else
  {
    OutputStream->print("Failed to parse PGN: ");
    OutputStream->println(N2kMsg.PGN);
  }
  */
}

//*****************************************************************************
// NMEA 2000 message handler
/**
 * @brief This method finds the handler method corresponding to the PGN in the tN2kMsg parameter and calls that method.
 * 
 * @param N2kMsg The message to handle.
 */
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg)
{
  int iHandler;

  /*
  OutputStream->print("In Main Handler: ");
  OutputStream->println(N2kMsg.PGN);
  */

  // Find handler
  for (iHandler = 0; NMEA2000Handlers[iHandler].PGN != 0 && !(N2kMsg.PGN == NMEA2000Handlers[iHandler].PGN); iHandler++)
    ;

  // If handler is in list, call it.
  if (NMEA2000Handlers[iHandler].PGN != 0)
  {
    NMEA2000Handlers[iHandler].Handler(N2kMsg);
  }
}

//*****************************************************************************
void loop()
{
  NMEA2000.ParseMessages();
}