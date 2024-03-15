#include <N2kMsg.h>
#include <NMEA2000.h>
#include "BluetoothSerial.h"
#define ESP32_CAN_TX_PIN GPIO_NUM_25 // Uncomment this and set right CAN TX pin definition, if you use ESP32 and do not have TX on default IO 16
#define ESP32_CAN_RX_PIN GPIO_NUM_26 // Uncomment this and set right CAN RX pin definition, if you use ESP32 and do not have RX on default IO 4
#include <NMEA2000_esp32.h>          // https://github.com/ttlappalainen/NMEA2000_esp32
//
tNMEA2000_esp32 NMEA2000;
#include <Arduino.h>
// #define N2k_SPI_CS_PIN 53    // Pin for SPI select for mcp_can
// #define N2k_CAN_INT_PIN 21   // Interrupt pin for mcp_can
// #define USE_MCP_CAN_CLOCK_SET 8  // Uncomment this, if your mcp_can shield has 8MHz chrystal

// #define NMEA2000_ARDUINO_DUE_CAN_BUS tNMEA2000_due::CANDevice1    // Uncomment this, if you want to use CAN bus 1 instead of 0 for Arduino DUE
// #include <NMEA2000_CAN.h>
#include <N2kMessages.h>
#include <N2kMessagesEnumToStr.h>
int LED_R = 2;
int LED_B = 4;
int LED_G = 15;

// #define N2k_CAN_INT_PIN 21
#include <Arduino.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
// Demo: NMEA2000 library.
// This demo reads messages from NMEA 2000 bus and
// sends them translated to clear text to Serial.

// Note! If you use this on Arduino Mega, I prefer to also connect interrupt line
// for MCP2515 and define N2k_CAN_INT_PIN to related line. E.g. MessageSender
// sends so many messages that Mega can not handle them. If you only use
// received messages internally without slow operations, then youmay survive
// without interrupt.

#include <Arduino.h>
// #define N2k_SPI_CS_PIN 53    // Pin for SPI select for mcp_can
// #define N2k_CAN_INT_PIN 21   // Interrupt pin for mcp_can
// #define USE_MCP_CAN_CLOCK_SET 8  // Uncomment this, if your mcp_can shield has 8MHz chrystal
// #define ESP32_CAN_TX_PIN GPIO_NUM_16 // Uncomment this and set right CAN TX pin definition, if you use ESP32 and do not have TX on default IO 16
// #define ESP32_CAN_RX_PIN GPIO_NUM_17 // Uncomment this and set right CAN RX pin definition, if you use ESP32 and do not have RX on default IO 4
// #define NMEA2000_ARDUINO_DUE_CAN_BUS tNMEA2000_due::CANDevice1    // Uncomment this, if you want to use CAN bus 1 instead of 0 for Arduino DUE
// #include <NMEA2000_CAN.h>
#include <N2kMessages.h>
#include <N2kMessagesEnumToStr.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

AsyncWebServer server(80);

const char *ssid = "Sensario_guests"; // Your WiFi SSID
const char *password = "hello-boat";  // Your WiFi Password

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
// CustomWebSerial WebSerial;

typedef struct
{
    unsigned long PGN;
    void (*Handler)(const tN2kMsg &N2kMsg);
} tNMEA2000Handler;

void SystemTime(const tN2kMsg &N2kMsg);
void Rudder(const tN2kMsg &N2kMsg);
void EngineRapid(const tN2kMsg &N2kMsg);
void EngineDynamicParameters(const tN2kMsg &N2kMsg);
void TransmissionParameters(const tN2kMsg &N2kMsg);
void TripFuelConsumption(const tN2kMsg &N2kMsg);
void Speed(const tN2kMsg &N2kMsg);
void WaterDepth(const tN2kMsg &N2kMsg);
void BinaryStatus(const tN2kMsg &N2kMsg);
void FluidLevel(const tN2kMsg &N2kMsg);
void OutsideEnvironmental(const tN2kMsg &N2kMsg);
void Temperature(const tN2kMsg &N2kMsg);
void TemperatureExt(const tN2kMsg &N2kMsg);
void DCStatus(const tN2kMsg &N2kMsg);
void BatteryConfigurationStatus(const tN2kMsg &N2kMsg);
void COGSOG(const tN2kMsg &N2kMsg);
void GNSS(const tN2kMsg &N2kMsg);
void LocalOffset(const tN2kMsg &N2kMsg);
void Attitude(const tN2kMsg &N2kMsg);
void Heading(const tN2kMsg &N2kMsg);
void Humidity(const tN2kMsg &N2kMsg);
void Pressure(const tN2kMsg &N2kMsg);
void UserDatumSettings(const tN2kMsg &N2kMsg);
void GNSSSatsInView(const tN2kMsg &N2kMsg);




tNMEA2000Handler NMEA2000Handlers[] = {
    {126992L, &SystemTime},
    {127245L, &Rudder},
    {127250L, &Heading},
    {127257L, &Attitude},
    {127488L, &EngineRapid},
    {127489L, &EngineDynamicParameters},
    {127493L, &TransmissionParameters},
    {127497L, &TripFuelConsumption},
    {127501L, &BinaryStatus},
    {127505L, &FluidLevel},
    {127506L, &DCStatus},
    {127513L, &BatteryConfigurationStatus},
    {128259L, &Speed},
    {128267L, &WaterDepth},
    {129026L, &COGSOG},
    {129029L, &GNSS},
    {129033L, &LocalOffset},
    {129045L, &UserDatumSettings},
    {129540L, &GNSSSatsInView},
    {130310L, &OutsideEnvironmental},
    {130312L, &Temperature},
    {130313L, &Humidity},
    {130314L, &Pressure},
    {130316L, &TemperatureExt},
    {0, 0}};

Stream *OutputStream;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "RED"){
    digitalWrite(LED_R, 0);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
    Serial.println("RED");
  }
  if (d=="GREEN"){
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, 0);
    digitalWrite(LED_B, HIGH);
    Serial.println("GREEN");
  }
  if (d=="BLUE"){
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, 0);
    Serial.println("BLUE");
  }
}


template <typename T>
void Print (T msg) {
    OutputStream->print(msg);
    WebSerial.print(msg);
}
template <typename T>
void Println (T msg) {
    OutputStream->println(msg);
    WebSerial.println(msg);
}

template <typename T>
void Print (T msg, int8_t desim) {
    OutputStream->print(msg, desim);
    WebSerial.print(msg);
}

void Println() {
    OutputStream->println();
    WebSerial.println();
}


void setup()
{

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    digitalWrite(LED_B, 0);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_R, HIGH);

    Serial.begin(115200);
    delay(500);

    
    WiFi.mode(WIFI_STA);
    WiFi.begin("Sensario_guests", "hello-boat");

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    // WebSerial is accessible at "<IP Address>/webserial" in browser
    WebSerial.begin(&server);
    WebSerial.msgCallback(recvMsg);
    server.begin();

    OutputStream = &SerialBT;
    //   while (!Serial)

    //  NMEA2000.SetN2kCANReceiveFrameBufSize(50);
    // Do not forward bus messages at all
    NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
    NMEA2000.SetForwardStream(OutputStream);
    // Set false below, if you do not want to see messages parsed to HEX withing library
    NMEA2000.EnableForward(false);
    NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
    //  NMEA2000.SetN2kCANMsgBufSize(2);
    NMEA2000.Open();
    Print("Running...");

    SerialBT.begin("N2K Insight");
    Serial.println("Bluetooth Started! Ready to pair...");
}

//*****************************************************************************
template <typename T>
void PrintLabelValWithConversionCheckUnDef(const char *label, T val, double (*ConvFunc)(double val) = 0, bool AddLf = false, int8_t Desim = -1)
{
    Print(label);
    if (!N2kIsNA(val))
    {
        if (Desim < 0)
        {
            if (ConvFunc)
            {
                Print(ConvFunc(val));
            }
            else
            {
                Print(val);
                WebSerial.print(val);
            }
        }
        else
        {
            if (ConvFunc)
            {
                Print(ConvFunc(val), Desim);
            }
            else
            {
                Print(val, Desim);
            }
        }
    }
    else
        Print("not available");
    if (AddLf)
        Println();
}

//*****************************************************************************
void SystemTime(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    uint16_t SystemDate;
    double SystemTime;
    tN2kTimeSource TimeSource;

    if (ParseN2kSystemTime(N2kMsg, SID, SystemDate, SystemTime, TimeSource))
    {
        Println("System time:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        PrintLabelValWithConversionCheckUnDef("  days since 1.1.1970: ", SystemDate, 0, true);
        PrintLabelValWithConversionCheckUnDef("  seconds since midnight: ", SystemTime, 0, true);
        Print("  time source: ");
        PrintN2kEnumType(TimeSource, OutputStream);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Rudder(const tN2kMsg &N2kMsg)
{
    unsigned char Instance;
    tN2kRudderDirectionOrder RudderDirectionOrder;
    double RudderPosition;
    double AngleOrder;

    if (ParseN2kRudder(N2kMsg, RudderPosition, Instance, RudderDirectionOrder, AngleOrder))
    {
        PrintLabelValWithConversionCheckUnDef("Rudder: ", Instance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  position (deg): ", RudderPosition, &RadToDeg, true);
        Print("  direction order: ");
        PrintN2kEnumType(RudderDirectionOrder, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  angle order (deg): ", AngleOrder, &RadToDeg, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void EngineRapid(const tN2kMsg &N2kMsg)
{
    unsigned char EngineInstance;
    double EngineSpeed;
    double EngineBoostPressure;
    int8_t EngineTiltTrim;

    if (ParseN2kEngineParamRapid(N2kMsg, EngineInstance, EngineSpeed, EngineBoostPressure, EngineTiltTrim))
    {
        PrintLabelValWithConversionCheckUnDef("Engine rapid params: ", EngineInstance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  RPM: ", EngineSpeed, 0, true);
        PrintLabelValWithConversionCheckUnDef("  boost pressure (Pa): ", EngineBoostPressure, 0, true);
        PrintLabelValWithConversionCheckUnDef("  tilt trim: ", EngineTiltTrim, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void EngineDynamicParameters(const tN2kMsg &N2kMsg)
{
    unsigned char EngineInstance;
    double EngineOilPress;
    double EngineOilTemp;
    double EngineCoolantTemp;
    double AltenatorVoltage;
    double FuelRate;
    double EngineHours;
    double EngineCoolantPress;
    double EngineFuelPress;
    int8_t EngineLoad;
    int8_t EngineTorque;
    tN2kEngineDiscreteStatus1 Status1;
    tN2kEngineDiscreteStatus2 Status2;

    if (ParseN2kEngineDynamicParam(N2kMsg, EngineInstance, EngineOilPress, EngineOilTemp, EngineCoolantTemp,
                                   AltenatorVoltage, FuelRate, EngineHours,
                                   EngineCoolantPress, EngineFuelPress,
                                   EngineLoad, EngineTorque, Status1, Status2))
    {
        PrintLabelValWithConversionCheckUnDef("Engine dynamic params: ", EngineInstance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  oil pressure (Pa): ", EngineOilPress, 0, true);
        PrintLabelValWithConversionCheckUnDef("  oil temp (C): ", EngineOilTemp, &KelvinToC, true);
        PrintLabelValWithConversionCheckUnDef("  coolant temp (C): ", EngineCoolantTemp, &KelvinToC, true);
        PrintLabelValWithConversionCheckUnDef("  altenator voltage (V): ", AltenatorVoltage, 0, true);
        PrintLabelValWithConversionCheckUnDef("  fuel rate (l/h): ", FuelRate, 0, true);
        PrintLabelValWithConversionCheckUnDef("  engine hours (h): ", EngineHours, &SecondsToh, true);
        PrintLabelValWithConversionCheckUnDef("  coolant pressure (Pa): ", EngineCoolantPress, 0, true);
        PrintLabelValWithConversionCheckUnDef("  fuel pressure (Pa): ", EngineFuelPress, 0, true);
        PrintLabelValWithConversionCheckUnDef("  engine load (%): ", EngineLoad, 0, true);
        PrintLabelValWithConversionCheckUnDef("  engine torque (%): ", EngineTorque, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TransmissionParameters(const tN2kMsg &N2kMsg)
{
    unsigned char EngineInstance;
    tN2kTransmissionGear TransmissionGear;
    double OilPressure;
    double OilTemperature;
    unsigned char DiscreteStatus1;

    if (ParseN2kTransmissionParameters(N2kMsg, EngineInstance, TransmissionGear, OilPressure, OilTemperature, DiscreteStatus1))
    {
        PrintLabelValWithConversionCheckUnDef("Transmission params: ", EngineInstance, 0, true);
        Print("  gear: ");
        PrintN2kEnumType(TransmissionGear, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  oil pressure (Pa): ", OilPressure, 0, true);
        PrintLabelValWithConversionCheckUnDef("  oil temperature (C): ", OilTemperature, &KelvinToC, true);
        PrintLabelValWithConversionCheckUnDef("  discrete status: ", DiscreteStatus1, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TripFuelConsumption(const tN2kMsg &N2kMsg)
{
    unsigned char EngineInstance;
    double TripFuelUsed;
    double FuelRateAverage;
    double FuelRateEconomy;
    double InstantaneousFuelEconomy;

    if (ParseN2kEngineTripParameters(N2kMsg, EngineInstance, TripFuelUsed, FuelRateAverage, FuelRateEconomy, InstantaneousFuelEconomy))
    {
        PrintLabelValWithConversionCheckUnDef("Trip fuel consumption: ", EngineInstance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  fuel used (l): ", TripFuelUsed, 0, true);
        PrintLabelValWithConversionCheckUnDef("  average fuel rate (l/h): ", FuelRateAverage, 0, true);
        PrintLabelValWithConversionCheckUnDef("  economy fuel rate (l/h): ", FuelRateEconomy, 0, true);
        PrintLabelValWithConversionCheckUnDef("  instantaneous fuel economy (l/h): ", InstantaneousFuelEconomy, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Heading(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double Heading;
    double Deviation;
    double Variation;

    if (ParseN2kHeading(N2kMsg, SID, Heading, Deviation, Variation, HeadingReference))
    {
        Println("Heading:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        Print("  reference: ");
        PrintN2kEnumType(HeadingReference, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  Heading (deg): ", Heading, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Deviation (deg): ", Deviation, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Variation (deg): ", Variation, &RadToDeg, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void COGSOG(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;

    if (ParseN2kCOGSOGRapid(N2kMsg, SID, HeadingReference, COG, SOG))
    {
        Println("COG/SOG:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        Print("  reference: ");
        PrintN2kEnumType(HeadingReference, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  COG (deg): ", COG, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  SOG (m/s): ", SOG, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void GNSS(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    uint16_t DaysSince1970;
    double SecondsSinceMidnight;
    double Latitude;
    double Longitude;
    double Altitude;
    tN2kGNSStype GNSStype;
    tN2kGNSSmethod GNSSmethod;
    unsigned char nSatellites;
    double HDOP;
    double PDOP;
    double GeoidalSeparation;
    unsigned char nReferenceStations;
    tN2kGNSStype ReferenceStationType;
    uint16_t ReferenceSationID;
    double AgeOfCorrection;

    if (ParseN2kGNSS(N2kMsg, SID, DaysSince1970, SecondsSinceMidnight,
                     Latitude, Longitude, Altitude,
                     GNSStype, GNSSmethod,
                     nSatellites, HDOP, PDOP, GeoidalSeparation,
                     nReferenceStations, ReferenceStationType, ReferenceSationID,
                     AgeOfCorrection))
    {
        Println("GNSS info:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        PrintLabelValWithConversionCheckUnDef("  days since 1.1.1970: ", DaysSince1970, 0, true);
        PrintLabelValWithConversionCheckUnDef("  seconds since midnight: ", SecondsSinceMidnight, 0, true);
        PrintLabelValWithConversionCheckUnDef("  latitude: ", Latitude, 0, true, 9);
        PrintLabelValWithConversionCheckUnDef("  longitude: ", Longitude, 0, true, 9);
        PrintLabelValWithConversionCheckUnDef("  altitude: (m): ", Altitude, 0, true);
        Print("  GNSS type: ");
        PrintN2kEnumType(GNSStype, OutputStream);
        Print("  GNSS method: ");
        PrintN2kEnumType(GNSSmethod, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  satellite count: ", nSatellites, 0, true);
        PrintLabelValWithConversionCheckUnDef("  HDOP: ", HDOP, 0, true);
        PrintLabelValWithConversionCheckUnDef("  PDOP: ", PDOP, 0, true);
        PrintLabelValWithConversionCheckUnDef("  geoidal separation: ", GeoidalSeparation, 0, true);
        PrintLabelValWithConversionCheckUnDef("  reference stations: ", nReferenceStations, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void UserDatumSettings(const tN2kMsg &N2kMsg)
{
    if (N2kMsg.PGN != 129045L)
        return;
    int Index = 0;
    double val;

    Println("User Datum Settings: ");
    val = N2kMsg.Get4ByteDouble(1e-2, Index);
    PrintLabelValWithConversionCheckUnDef("  delta x (m): ", val, 0, true);
    val = N2kMsg.Get4ByteDouble(1e-2, Index);
    PrintLabelValWithConversionCheckUnDef("  delta y (m): ", val, 0, true);
    val = N2kMsg.Get4ByteDouble(1e-2, Index);
    PrintLabelValWithConversionCheckUnDef("  delta z (m): ", val, 0, true);
    val = N2kMsg.GetFloat(Index);
    PrintLabelValWithConversionCheckUnDef("  rotation in x (deg): ", val, &RadToDeg, true, 5);
    val = N2kMsg.GetFloat(Index);
    PrintLabelValWithConversionCheckUnDef("  rotation in y (deg): ", val, &RadToDeg, true, 5);
    val = N2kMsg.GetFloat(Index);
    PrintLabelValWithConversionCheckUnDef("  rotation in z (deg): ", val, &RadToDeg, true, 5);
    val = N2kMsg.GetFloat(Index);
    PrintLabelValWithConversionCheckUnDef("  scale: ", val, 0, true, 3);
}

//*****************************************************************************
void GNSSSatsInView(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    tN2kRangeResidualMode Mode;
    uint8_t NumberOfSVs;
    tSatelliteInfo SatelliteInfo;

    if (ParseN2kPGNSatellitesInView(N2kMsg, SID, Mode, NumberOfSVs))
    {
        Println("Satellites in view: ");
        Print("  mode: ");
        Println(Mode);
        Print("  number of satellites: ");
        Println(NumberOfSVs);
        for (uint8_t i = 0; i < NumberOfSVs && ParseN2kPGNSatellitesInView(N2kMsg, i, SatelliteInfo); i++)
        {
            Print("  Satellite PRN: ");
            Println(SatelliteInfo.PRN);
            PrintLabelValWithConversionCheckUnDef("    elevation: ", SatelliteInfo.Elevation, &RadToDeg, true, 1);
            PrintLabelValWithConversionCheckUnDef("    azimuth:   ", SatelliteInfo.Azimuth, &RadToDeg, true, 1);
            PrintLabelValWithConversionCheckUnDef("    SNR:       ", SatelliteInfo.SNR, 0, true, 1);
            PrintLabelValWithConversionCheckUnDef("    residuals: ", SatelliteInfo.RangeResiduals, 0, true, 1);
            Print("    status: ");
            Println(SatelliteInfo.UsageStatus);
        }
    }
}

//*****************************************************************************
void LocalOffset(const tN2kMsg &N2kMsg)
{
    uint16_t SystemDate;
    double SystemTime;
    int16_t Offset;

    if (ParseN2kLocalOffset(N2kMsg, SystemDate, SystemTime, Offset))
    {
        Println("Date,time and local offset: ");
        PrintLabelValWithConversionCheckUnDef("  days since 1.1.1970: ", SystemDate, 0, true);
        PrintLabelValWithConversionCheckUnDef("  seconds since midnight: ", SystemTime, 0, true);
        PrintLabelValWithConversionCheckUnDef("  local offset (min): ", Offset, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void OutsideEnvironmental(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;

    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg, SID, WaterTemperature, OutsideAmbientAirTemperature, AtmosphericPressure))
    {
        PrintLabelValWithConversionCheckUnDef("Water temp: ", WaterTemperature, &KelvinToC);
        PrintLabelValWithConversionCheckUnDef(", outside ambient temp: ", OutsideAmbientAirTemperature, &KelvinToC);
        PrintLabelValWithConversionCheckUnDef(", pressure: ", AtmosphericPressure, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Temperature(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;

    if (ParseN2kTemperature(N2kMsg, SID, TempInstance, TempSource, ActualTemperature, SetTemperature))
    {
        Print("Temperature source: ");
        PrintN2kEnumType(TempSource, OutputStream, false);
        PrintLabelValWithConversionCheckUnDef(", actual temperature: ", ActualTemperature, &KelvinToC);
        PrintLabelValWithConversionCheckUnDef(", set temperature: ", SetTemperature, &KelvinToC, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Humidity(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    unsigned char Instance;
    tN2kHumiditySource HumiditySource;
    double ActualHumidity, SetHumidity;

    if (ParseN2kHumidity(N2kMsg, SID, Instance, HumiditySource, ActualHumidity, SetHumidity))
    {
        Print("Humidity source: ");
        PrintN2kEnumType(HumiditySource, OutputStream, false);
        PrintLabelValWithConversionCheckUnDef(", humidity: ", ActualHumidity, 0, false);
        PrintLabelValWithConversionCheckUnDef(", set humidity: ", SetHumidity, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Pressure(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    unsigned char Instance;
    tN2kPressureSource PressureSource;
    double ActualPressure;

    if (ParseN2kPressure(N2kMsg, SID, Instance, PressureSource, ActualPressure))
    {
        Print("Pressure source: ");
        PrintN2kEnumType(PressureSource, OutputStream, false);
        PrintLabelValWithConversionCheckUnDef(", pressure: ", ActualPressure, &PascalTomBar, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TemperatureExt(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;

    if (ParseN2kTemperatureExt(N2kMsg, SID, TempInstance, TempSource, ActualTemperature, SetTemperature))
    {
        Print("Temperature source: ");
        PrintN2kEnumType(TempSource, OutputStream, false);
        PrintLabelValWithConversionCheckUnDef(", actual temperature: ", ActualTemperature, &KelvinToC);
        PrintLabelValWithConversionCheckUnDef(", set temperature: ", SetTemperature, &KelvinToC, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void BatteryConfigurationStatus(const tN2kMsg &N2kMsg)
{
    unsigned char BatInstance;
    tN2kBatType BatType;
    tN2kBatEqSupport SupportsEqual;
    tN2kBatNomVolt BatNominalVoltage;
    tN2kBatChem BatChemistry;
    double BatCapacity;
    int8_t BatTemperatureCoefficient;
    double PeukertExponent;
    int8_t ChargeEfficiencyFactor;

    if (ParseN2kBatConf(N2kMsg, BatInstance, BatType, SupportsEqual, BatNominalVoltage, BatChemistry, BatCapacity, BatTemperatureCoefficient, PeukertExponent, ChargeEfficiencyFactor))
    {
        PrintLabelValWithConversionCheckUnDef("Battery instance: ", BatInstance, 0, true);
        Print("  - type: ");
        PrintN2kEnumType(BatType, OutputStream);
        Print("  - support equal.: ");
        PrintN2kEnumType(SupportsEqual, OutputStream);
        Print("  - nominal voltage: ");
        PrintN2kEnumType(BatNominalVoltage, OutputStream);
        Print("  - chemistry: ");
        PrintN2kEnumType(BatChemistry, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  - capacity (Ah): ", BatCapacity, &CoulombToAh, true);
        PrintLabelValWithConversionCheckUnDef("  - temperature coefficient (%): ", BatTemperatureCoefficient, 0, true);
        PrintLabelValWithConversionCheckUnDef("  - peukert exponent: ", PeukertExponent, 0, true);
        PrintLabelValWithConversionCheckUnDef("  - charge efficiency factor (%): ", ChargeEfficiencyFactor, 0, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DCStatus(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    unsigned char DCInstance;
    tN2kDCType DCType;
    unsigned char StateOfCharge;
    unsigned char StateOfHealth;
    double TimeRemaining;
    double RippleVoltage;
    double Capacity;

    if (ParseN2kDCStatus(N2kMsg, SID, DCInstance, DCType, StateOfCharge, StateOfHealth, TimeRemaining, RippleVoltage, Capacity))
    {
        Print("DC instance: ");
        Println(DCInstance);
        Print("  - type: ");
        PrintN2kEnumType(DCType, OutputStream);
        Print("  - state of charge (%): ");
        Println(StateOfCharge);
        Print("  - state of health (%): ");
        Println(StateOfHealth);
        Print("  - time remaining (h): ");
        Println(TimeRemaining / 60);
        Print("  - ripple voltage: ");
        Println(RippleVoltage);
        Print("  - capacity: ");
        Println(Capacity);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Speed(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    double SOW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg, SID, SOW, SOG, SWRT))
    {
        Print("Boat speed:");
        PrintLabelValWithConversionCheckUnDef(" SOW:", N2kIsNA(SOW) ? SOW : msToKnots(SOW));
        PrintLabelValWithConversionCheckUnDef(", SOG:", N2kIsNA(SOG) ? SOG : msToKnots(SOG));
        Print(", ");
        PrintN2kEnumType(SWRT, OutputStream, true);
    }
}

//*****************************************************************************
void WaterDepth(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;

    if (ParseN2kWaterDepth(N2kMsg, SID, DepthBelowTransducer, Offset))
    {
        if (N2kIsNA(Offset) || Offset == 0)
        {
            PrintLabelValWithConversionCheckUnDef("Depth below transducer", DepthBelowTransducer);
            if (N2kIsNA(Offset))
            {
                Println(", offset not available");
            }
            else
            {
                Println(", offset=0");
            }
        }
        else
        {
            if (Offset > 0)
            {
                Print("Water depth:");
            }
            else
            {
                Print("Depth below keel:");
            }
            if (!N2kIsNA(DepthBelowTransducer))
            {
                Println(DepthBelowTransducer + Offset);
            }
            else
            {
                Println(" not available");
            }
        }
    }
}

//*****************************************************************************
void printLLNumber(Stream *OutputStream, unsigned long long n, uint8_t base = 10)
{
    unsigned char buf[16 * sizeof(long)]; // Assumes 8-bit chars.
    unsigned long long i = 0;

    if (n == 0)
    {
        Print('0');
        return;
    }

    while (n > 0)
    {
        buf[i++] = n % base;
        n /= base;
    }

    for (; i > 0; i--)
        Print((char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

//*****************************************************************************
void BinaryStatusFull(const tN2kMsg &N2kMsg)
{
    unsigned char BankInstance;
    tN2kBinaryStatus BankStatus;

    if (ParseN2kBinaryStatus(N2kMsg, BankInstance, BankStatus))
    {
        Print("Binary status for bank ");
        Print(BankInstance);
        Println(":");
        Print("  "); // printLLNumber(OutputStream,BankStatus,16);
        for (uint8_t i = 1; i <= 28; i++)
        {
            if (i > 1)
                Print(",");
            PrintN2kEnumType(N2kGetStatusOnBinaryStatus(BankStatus, i), OutputStream, false);
        }
        Println();
    }
}

//*****************************************************************************
void BinaryStatus(const tN2kMsg &N2kMsg)
{
    unsigned char BankInstance;
    tN2kOnOff Status1, Status2, Status3, Status4;

    if (ParseN2kBinaryStatus(N2kMsg, BankInstance, Status1, Status2, Status3, Status4))
    {
        if (BankInstance > 2)
        { // note that this is only for testing different methods. MessageSender.ini sends 4 status for instace 2
            BinaryStatusFull(N2kMsg);
        }
        else
        {
            Print("Binary status for bank ");
            Print(BankInstance);
            Println(":");
            Print("  Status1=");
            PrintN2kEnumType(Status1, OutputStream, false);
            Print(", Status2=");
            PrintN2kEnumType(Status2, OutputStream, false);
            Print(", Status3=");
            PrintN2kEnumType(Status3, OutputStream, false);
            Print(", Status4=");
            PrintN2kEnumType(Status4, OutputStream, false);
            Println();
        }
    }
}

//*****************************************************************************
void FluidLevel(const tN2kMsg &N2kMsg)
{
    unsigned char Instance;
    tN2kFluidType FluidType;
    double Level = 0;
    double Capacity = 0;

    if (ParseN2kFluidLevel(N2kMsg, Instance, FluidType, Level, Capacity))
    {
        switch (FluidType)
        {
        case N2kft_Fuel:
            Print("Fuel level :");
            break;
        case N2kft_Water:
            Print("Water level :");
            break;
        case N2kft_GrayWater:
            Print("Gray water level :");
            break;
        case N2kft_LiveWell:
            Print("Live well level :");
            break;
        case N2kft_Oil:
            Print("Oil level :");
            break;
        case N2kft_BlackWater:
            Print("Black water level :");
            break;
        case N2kft_FuelGasoline:
            Print("Gasoline level :");
            break;
        case N2kft_Error:
            Print("Error level :");
            break;
        case N2kft_Unavailable:
            Print("Unknown level :");
            break;
        }
        Print(Level);
        Print("%");
        Print(" (");
        Print(Capacity * Level / 100);
        Print("l)");
        Print(" capacity :");
        Println(Capacity);
    }
}

//*****************************************************************************
void Attitude(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    double Yaw;
    double Pitch;
    double Roll;

    if (ParseN2kAttitude(N2kMsg, SID, Yaw, Pitch, Roll))
    {
        Println("Attitude:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        PrintLabelValWithConversionCheckUnDef("  Yaw (deg): ", Yaw, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Pitch (deg): ", Pitch, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Roll (deg): ", Roll, &RadToDeg, true);
    }
    else
    {
        Print("Failed to parse PGN: ");
        Println(N2kMsg.PGN);
    }
}

//*****************************************************************************
// NMEA 2000 message handler
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg)
{
    int iHandler;

    // Find handler
    Print("In Main Handler: ");
    Println(N2kMsg.PGN);
    for (iHandler = 0; NMEA2000Handlers[iHandler].PGN != 0 && !(N2kMsg.PGN == NMEA2000Handlers[iHandler].PGN); iHandler++)
        ;

    if (NMEA2000Handlers[iHandler].PGN != 0)
    {
        NMEA2000Handlers[iHandler].Handler(N2kMsg);
    }
}




//*****************************************************************************
void loop()
{
    NMEA2000.ParseMessages();

    if (Serial.available())
    {
        SerialBT.write(Serial.read());
    }
    if (SerialBT.available())
    {
        Serial.write(SerialBT.read());
    }
}
