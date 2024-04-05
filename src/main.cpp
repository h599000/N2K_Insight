#include <N2kMsg.h>
#include <NMEA2000.h>
#include "BluetoothSerial.h"
#define ESP32_CAN_TX_PIN GPIO_NUM_25 // Uncomment this and set right CAN TX pin definition, if you use ESP32 and do not have TX on default IO 16
#define ESP32_CAN_RX_PIN GPIO_NUM_26 // Uncomment this and set right CAN RX pin definition, if you use ESP32 and do not have RX on default IO 4
#define I2C_ADDRESS 0x0a
#include <NMEA2000_esp32.h> // https://github.com/ttlappalainen/NMEA2000_esp32
#include <Wire.h>

//
tNMEA2000_esp32 NMEA2000;
#include <Arduino.h>
//#define N2k_SPI_CS_PIN 53    // Pin for SPI select for mcp_can
//#define N2k_CAN_INT_PIN 21   // Interrupt pin for mcp_can
//#define USE_MCP_CAN_CLOCK_SET 8  // Uncomment this, if your mcp_can shield has 8MHz chrystal

//#define NMEA2000_ARDUINO_DUE_CAN_BUS tNMEA2000_due::CANDevice1    // Uncomment this, if you want to use CAN bus 1 instead of 0 for Arduino DUE
//#include <NMEA2000_CAN.h>
#include <N2kMessages.h>
#include <N2kMessagesEnumToStr.h>
int LED_R = 2;
int LED_B = 4;
// int LED_G = 15;
#include "N2K_decomposed.h"
#include "N2K_composed.h"
#include <N2K_I2C_API.h>

PGN129026_d* n2kCOGSOG;
N2K_composed* composedN2KCOGSOG;


//#define N2k_CAN_INT_PIN 21
// Demo: NMEA2000 library. 
// This demo reads messages from NMEA 2000 bus and
// sends them translated to clear text to Serial.

// Note! If you use this on Arduino Mega, I prefer to also connect interrupt line
// for MCP2515 and define N2k_CAN_INT_PIN to related line. E.g. MessageSender
// sends so many messages that Mega can not handle them. If you only use
// received messages internally without slow operations, then youmay survive
// without interrupt.

//#define N2k_SPI_CS_PIN 53    // Pin for SPI select for mcp_can
//#define N2k_CAN_INT_PIN 21   // Interrupt pin for mcp_can
//#define USE_MCP_CAN_CLOCK_SET 8  // Uncomment this, if your mcp_can shield has 8MHz chrystal
//#define ESP32_CAN_TX_PIN GPIO_NUM_16 // Uncomment this and set right CAN TX pin definition, if you use ESP32 and do not have TX on default IO 16
//#define ESP32_CAN_RX_PIN GPIO_NUM_17 // Uncomment this and set right CAN RX pin definition, if you use ESP32 and do not have RX on default IO 4
//#define NMEA2000_ARDUINO_DUE_CAN_BUS tNMEA2000_due::CANDevice1    // Uncomment this, if you want to use CAN bus 1 instead of 0 for Arduino DUE
//#include <NMEA2000_CAN.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

typedef struct {
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

tNMEA2000Handler NMEA2000Handlers[]={
  {126992L,&SystemTime},
  {127245L,&Rudder },
  {127250L,&Heading},
  {127257L,&Attitude},
  {127488L,&EngineRapid},
  {127489L,&EngineDynamicParameters},
  {127493L,&TransmissionParameters},
  {127497L,&TripFuelConsumption},
  {127501L,&BinaryStatus},
  {127505L,&FluidLevel},
  {127506L,&DCStatus},
  {127513L,&BatteryConfigurationStatus},
  {128259L,&Speed},
  {128267L,&WaterDepth},
  {129026L,&COGSOG},
  {129029L,&GNSS},
  {129033L,&LocalOffset},
  {129045L,&UserDatumSettings},
  {129540L,&GNSSSatsInView},
  {130310L,&OutsideEnvironmental},
  {130312L,&Temperature},
  {130313L,&Humidity},
  {130314L,&Pressure},
  {130316L,&TemperatureExt},
  {0,0}
};

Stream *OutputStream;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);

void setup() {

  pinMode(LED_R,OUTPUT);
  // pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_B, 0);
  // digitalWrite(LED_G, HIGH);
  digitalWrite(LED_R, HIGH);


  Serial.begin(115200); delay(500);
  OutputStream=&Serial;
//   while (!Serial) 
  
//  NMEA2000.SetN2kCANReceiveFrameBufSize(50);
  // Do not forward bus messages at all
  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetForwardStream(OutputStream);
  NMEA2000.SetMode(tNMEA2000::N2km_ListenOnly);
  // Set false below, if you do not want to see messages parsed to HEX withing library
  NMEA2000.EnableForward(false);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
//  NMEA2000.SetN2kCANMsgBufSize(2);
  NMEA2000.Open();
  OutputStream->print("Running...");

  SerialBT.begin("N2K Insight");
  Serial.println("Bluetooth Started! Ready to pair...");

  Wire.setPins(23,22);
  Wire.onReceive(receiveData);
  Wire.onRequest(requestData);
  Wire.begin();
}

//*****************************************************************************
template<typename T> void PrintLabelValWithConversionCheckUnDef(const char* label, T val, double (*ConvFunc)(double val)=0, bool AddLf=false, int8_t Desim=-1 ) {
  OutputStream->print(label);
  if (!N2kIsNA(val)) {
    if ( Desim<0 ) {
      if (ConvFunc) { OutputStream->print(ConvFunc(val)); } else { OutputStream->print(val); }
    } else {
      if (ConvFunc) { OutputStream->print(ConvFunc(val),Desim); } else { OutputStream->print(val,Desim); }
    }
  } else OutputStream->print("not available");
  if (AddLf) OutputStream->println();
}

//*****************************************************************************
void SystemTime(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    uint16_t SystemDate;
    double SystemTime;
    tN2kTimeSource TimeSource;
    
    if (ParseN2kSystemTime(N2kMsg,SID,SystemDate,SystemTime,TimeSource) ) {
                      
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Rudder(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kRudderDirectionOrder RudderDirectionOrder;
    double RudderPosition;
    double AngleOrder;
    
    if (ParseN2kRudder(N2kMsg,RudderPosition,Instance,RudderDirectionOrder,AngleOrder) ) {

    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void EngineRapid(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineSpeed;
    double EngineBoostPressure;
    int8_t EngineTiltTrim;
    
    if (ParseN2kEngineParamRapid(N2kMsg,EngineInstance,EngineSpeed,EngineBoostPressure,EngineTiltTrim) ) {

    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void EngineDynamicParameters(const tN2kMsg &N2kMsg) {
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
    
    if (ParseN2kEngineDynamicParam(N2kMsg,EngineInstance,EngineOilPress,EngineOilTemp,EngineCoolantTemp,
                                 AltenatorVoltage,FuelRate,EngineHours,
                                 EngineCoolantPress,EngineFuelPress,
                                 EngineLoad,EngineTorque,Status1,Status2) ) {

    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TransmissionParameters(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    tN2kTransmissionGear TransmissionGear;
    double OilPressure;
    double OilTemperature;
    unsigned char DiscreteStatus1;
    
    if (ParseN2kTransmissionParameters(N2kMsg,EngineInstance, TransmissionGear, OilPressure, OilTemperature, DiscreteStatus1) ) {

    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TripFuelConsumption(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double TripFuelUsed;
    double FuelRateAverage; 
    double FuelRateEconomy; 
    double InstantaneousFuelEconomy; 
    
    if (ParseN2kEngineTripParameters(N2kMsg,EngineInstance, TripFuelUsed, FuelRateAverage, FuelRateEconomy, InstantaneousFuelEconomy) ) {
      
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Heading(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double Heading;
    double Deviation;
    double Variation;
    
    if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,HeadingReference) ) {
                      OutputStream->println("Heading:");
      
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void COGSOG(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;
    
    if (ParseN2kCOGSOGRapid(N2kMsg,SID,HeadingReference,COG,SOG) ) {
                      

    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void GNSS(const tN2kMsg &N2kMsg) {
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

    if (ParseN2kGNSS(N2kMsg,SID,DaysSince1970,SecondsSinceMidnight,
                  Latitude,Longitude,Altitude,
                  GNSStype,GNSSmethod,
                  nSatellites,HDOP,PDOP,GeoidalSeparation,
                  nReferenceStations,ReferenceStationType,ReferenceSationID,
                  AgeOfCorrection) ) {
                      
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void UserDatumSettings(const tN2kMsg &N2kMsg) {
  if (N2kMsg.PGN!=129045L) return;
  int Index=0;
  double val;

  
}

//*****************************************************************************
void GNSSSatsInView(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  tN2kRangeResidualMode Mode;
  uint8_t NumberOfSVs;
  tSatelliteInfo SatelliteInfo;

  if (ParseN2kPGNSatellitesInView(N2kMsg,SID,Mode,NumberOfSVs) ) {
    OutputStream->println("Satellites in view: ");
                      OutputStream->print("  mode: "); OutputStream->println(Mode);
                      OutputStream->print("  number of satellites: ");  OutputStream->println(NumberOfSVs);
    for ( uint8_t i=0; i<NumberOfSVs && ParseN2kPGNSatellitesInView(N2kMsg,i,SatelliteInfo); i++) {
                        OutputStream->print("  Satellite PRN: ");  OutputStream->println(SatelliteInfo.PRN);
      PrintLabelValWithConversionCheckUnDef("    elevation: ",SatelliteInfo.Elevation,&RadToDeg,true,1);
      PrintLabelValWithConversionCheckUnDef("    azimuth:   ",SatelliteInfo.Azimuth,&RadToDeg,true,1);
      PrintLabelValWithConversionCheckUnDef("    SNR:       ",SatelliteInfo.SNR,0,true,1);
      PrintLabelValWithConversionCheckUnDef("    residuals: ",SatelliteInfo.RangeResiduals,0,true,1);
                        OutputStream->print("    status: "); OutputStream->println(SatelliteInfo.UsageStatus);
    }
  }
}

//*****************************************************************************
void LocalOffset(const tN2kMsg &N2kMsg) {
    uint16_t SystemDate;
    double SystemTime;
    int16_t Offset;
    
    if (ParseN2kLocalOffset(N2kMsg,SystemDate,SystemTime,Offset) ) {
                     
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void OutsideEnvironmental(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;
    
    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg,SID,WaterTemperature,OutsideAmbientAirTemperature,AtmosphericPressure) ) {
      
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Temperature(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperature(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Humidity(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kHumiditySource HumiditySource;
    double ActualHumidity,SetHumidity;
    
    if ( ParseN2kHumidity(N2kMsg,SID,Instance,HumiditySource,ActualHumidity,SetHumidity) ) {
                        
      
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Pressure(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kPressureSource PressureSource;
    double ActualPressure;
    
    if ( ParseN2kPressure(N2kMsg,SID,Instance,PressureSource,ActualPressure) ) {
                       
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TemperatureExt(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperatureExt(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void BatteryConfigurationStatus(const tN2kMsg &N2kMsg) {
    unsigned char BatInstance;
    tN2kBatType BatType;
    tN2kBatEqSupport SupportsEqual;
    tN2kBatNomVolt BatNominalVoltage;
    tN2kBatChem BatChemistry;
    double BatCapacity;
    int8_t BatTemperatureCoefficient;
    double PeukertExponent; 
    int8_t ChargeEfficiencyFactor;

    if (ParseN2kBatConf(N2kMsg,BatInstance,BatType,SupportsEqual,BatNominalVoltage,BatChemistry,BatCapacity,BatTemperatureCoefficient,PeukertExponent,ChargeEfficiencyFactor) ) {
     
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DCStatus(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char DCInstance;
    tN2kDCType DCType;
    unsigned char StateOfCharge;
    unsigned char StateOfHealth;
    double TimeRemaining;
    double RippleVoltage;
    double Capacity;

    if (ParseN2kDCStatus(N2kMsg,SID,DCInstance,DCType,StateOfCharge,StateOfHealth,TimeRemaining,RippleVoltage,Capacity) ) {
      
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void Speed(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double SOW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg,SID,SOW,SOG,SWRT) ) {

    }
}

//*****************************************************************************
void WaterDepth(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;

    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) {
      if ( N2kIsNA(Offset) || Offset == 0 ) {
        PrintLabelValWithConversionCheckUnDef("Depth below transducer",DepthBelowTransducer);
        if ( N2kIsNA(Offset) ) {
          OutputStream->println(", offset not available");
        } else {
          OutputStream->println(", offset=0");
        }
      } else {
        if (Offset>0) {
          OutputStream->print("Water depth:");
        } else {
          OutputStream->print("Depth below keel:");
        }
        if ( !N2kIsNA(DepthBelowTransducer) ) { 
          OutputStream->println(DepthBelowTransducer+Offset); 
        } else {  OutputStream->println(" not available"); }
      }
    }
}

//*****************************************************************************
void printLLNumber(Stream *OutputStream, unsigned long long n, uint8_t base=10)
{
  unsigned char buf[16 * sizeof(long)]; // Assumes 8-bit chars.
  unsigned long long i = 0;

  if (n == 0) {
    OutputStream->print('0');
    return;
  }

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    OutputStream->print((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}

//*****************************************************************************
void BinaryStatusFull(const tN2kMsg &N2kMsg) {
    unsigned char BankInstance;
    tN2kBinaryStatus BankStatus;

    if (ParseN2kBinaryStatus(N2kMsg,BankInstance,BankStatus) ) {
      OutputStream->print("Binary status for bank "); OutputStream->print(BankInstance); OutputStream->println(":");
      OutputStream->print("  "); //printLLNumber(OutputStream,BankStatus,16);
      for (uint8_t i=1; i<=28; i++) {
        if (i>1) OutputStream->print(",");
        PrintN2kEnumType(N2kGetStatusOnBinaryStatus(BankStatus,i),OutputStream,false);
      }
      OutputStream->println();
    }
}

//*****************************************************************************
void BinaryStatus(const tN2kMsg &N2kMsg) {
    unsigned char BankInstance;
    tN2kOnOff Status1,Status2,Status3,Status4;

    if (ParseN2kBinaryStatus(N2kMsg,BankInstance,Status1,Status2,Status3,Status4) ) {
      if (BankInstance>2) { // note that this is only for testing different methods. MessageSender.ini sends 4 status for instace 2
        BinaryStatusFull(N2kMsg);
      } else {
        OutputStream->print("Binary status for bank "); OutputStream->print(BankInstance); OutputStream->println(":");
        OutputStream->print("  Status1=");PrintN2kEnumType(Status1,OutputStream,false);
        OutputStream->print(", Status2=");PrintN2kEnumType(Status2,OutputStream,false);
        OutputStream->print(", Status3=");PrintN2kEnumType(Status3,OutputStream,false);
        OutputStream->print(", Status4=");PrintN2kEnumType(Status4,OutputStream,false);
        OutputStream->println();
      }
    }
}

//*****************************************************************************
void FluidLevel(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kFluidType FluidType;
    double Level=0;
    double Capacity=0;

    if (ParseN2kFluidLevel(N2kMsg,Instance,FluidType,Level,Capacity) ) {
      switch (FluidType) {
        case N2kft_Fuel:
          OutputStream->print("Fuel level :");
          break;
        case N2kft_Water:
          OutputStream->print("Water level :");
          break;
        case N2kft_GrayWater:
          OutputStream->print("Gray water level :");
          break;
        case N2kft_LiveWell:
          OutputStream->print("Live well level :");
          break;
        case N2kft_Oil:
          OutputStream->print("Oil level :");
          break;
        case N2kft_BlackWater:
          OutputStream->print("Black water level :");
          break;
        case N2kft_FuelGasoline:
          OutputStream->print("Gasoline level :");
          break;
        case N2kft_Error:
          OutputStream->print("Error level :");
          break;
        case N2kft_Unavailable:
          OutputStream->print("Unknown level :");
          break;
      }
      OutputStream->print(Level); OutputStream->print("%"); 
      OutputStream->print(" ("); OutputStream->print(Capacity*Level/100); OutputStream->print("l)");
      OutputStream->print(" capacity :"); OutputStream->println(Capacity);
    }
}

//*****************************************************************************
void Attitude(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double Yaw;
    double Pitch;
    double Roll;
    
    if (ParseN2kAttitude(N2kMsg,SID,Yaw,Pitch,Roll) ) {
                      OutputStream->println("Attitude:");
      PrintLabelValWithConversionCheckUnDef("  SID: ",SID,0,true);
      PrintLabelValWithConversionCheckUnDef("  Yaw (deg): ",Yaw,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef("  Pitch (deg): ",Pitch,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef("  Roll (deg): ",Roll,&RadToDeg,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
//NMEA 2000 message handler
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  int iHandler;
  
  // Find handler
  OutputStream->print("In Main Handler: "); OutputStream->println(N2kMsg.PGN);
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  
  if (NMEA2000Handlers[iHandler].PGN!=0) {
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
    char inputChar = (char)SerialBT.read();

    switch(inputChar) {
      case '1':
        SerialBT.println(n2kCOGSOG->N2KtoJSON());
        break;
      case '2':
        SerialBT.println(composedN2KCOGSOG->N2KtoJSON());
        break;
      default:
        SerialBT.println("Unexpected input. Press 1 or 2");
        break;
    }

    // Serial.write(SerialBT.read());

  }
  
}