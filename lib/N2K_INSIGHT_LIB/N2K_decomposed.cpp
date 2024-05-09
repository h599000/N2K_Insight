#include "N2K_decomposed.h"


N2K_decomposed::N2K_decomposed(time_t TimeOfReceipt, int Pgn):
TimeOfReceipt(TimeOfReceipt),
Pgn(Pgn)
{
}

String N2K_decomposed::N2KtoJSON()
{
    return String();
}

PGN127496_d::PGN127496_d(time_t TimeOfReceipt, int Pgn, double TimetoEmpty, double DistancetoEmpty, short EstimatedFuelRemaining, double TripRunTime):
N2K_decomposed(TimeOfReceipt, Pgn), TimetoEmpty(TimetoEmpty), DistancetoEmpty(DistancetoEmpty), EstimatedFuelRemaining(EstimatedFuelRemaining), TripRunTime(TripRunTime)
{
}

String PGN127496_d::N2KtoJSON()
{
    String json = "{";
    json += "\"PGN\": " + String(Pgn) + ",";
    json += "\"TimeToEmpty\": " + String(TimetoEmpty) + ",";
    json += "\"DistanceToEmpty\": " + String(DistancetoEmpty) + ",";
    json += "\"EstimatedFuelRemaining\": " + String(EstimatedFuelRemaining) + ",";
    json += "\"TripRunTime\": " + String(TripRunTime);
    json += "}";
    return json;
}

PGN127489_d::PGN127489_d(time_t TimeOfReceipt, int Pgn, int8_t Instance, double OilPressure, double OilTemperature, double Temperature, 
double AlternatorPotential, double FuelRate, double TotalEngineHours, double CoolantPressure, double FuelPressure, 
int8_t EngineLoad, int8_t EngineTorque, tN2kEngineDiscreteStatus1 Status1, tN2kEngineDiscreteStatus2 Status2):
N2K_decomposed(TimeOfReceipt, Pgn), Instance(Instance), OilPressure(OilPressure), OilTemperature(OilTemperature), Temperature(Temperature), AlternatorPotential(AlternatorPotential), FuelRate(FuelRate), TotalEngineHours(TotalEngineHours), CoolantPressure(CoolantPressure), FuelPressure(FuelPressure), EngineLoad(EngineLoad), EngineTorque(EngineTorque), Status1(Status1), Status2(Status2)
{

}

String PGN127489_d::N2KtoJSON()
{
    String json = "{";
    json += "\"Time Of Receipt\": " + String(TimeOfReceipt) + ",";
    json += "\"PGN\": " + String(Pgn) + ",";
    json += "\"Instance\": " + String(Instance) + ",";
    json += "\"Oil Pressure\": " + String(OilPressure) + ",";
    json += "\"Oil Temperature\": " + String(OilTemperature) + ",";
    json += "\"Temperature\": " + String(Temperature) + ",";
    json += "\"Alternator Potential\": " + String(AlternatorPotential) + ",";
    json += "\"Fuel Rate\": " + String(FuelRate) + ",";
    json += "\"Total Engine Hours\": " + String(TotalEngineHours) + ",";
    json += "\"Coolant Pressure\": " + String(CoolantPressure) + ",";
    json += "\"Fuel Pressure\": " + String(FuelPressure) + ",";
    json += "\"Engine Load\": " + String(EngineLoad); + ",";
    json += "\"Engine Torque\": " + String(EngineTorque) + ",";
    if (Status1.Bits.ChargeIndicator) {
        json += "\"Charge Indicator\": " + String(Status1.Bits.ChargeIndicator) + ",";
    }
    if (Status1.Bits.CheckEngine) {
        json += "\"Check Engine\": " + String(Status1.Bits.CheckEngine) + ",";
    }
    if (Status1.Bits.EGRSystem) {
        json += "\"EGR System\": " + String(Status1.Bits.EGRSystem) + ",";
    }
    if (Status1.Bits.EngineEmergencyStopMode) {
        json += "\"Engine Emergecy Stop Mode\": " + String(Status1.Bits.EngineEmergencyStopMode) + ",";
    }
    if (Status1.Bits.HighBoostPressure) {
        json += "\"High Boost Pressure\": " + String(Status1.Bits.HighBoostPressure) + ",";
    }
    if (Status1.Bits.LowCoolantLevel) {
        json += "\"Low Coolant Level\": " + String(Status1.Bits.LowCoolantLevel) + ",";
    }
    if (Status1.Bits.LowFuelPressure) {
        json += "\"Low Fuel Pressure\": " + String(Status1.Bits.LowFuelPressure) + ",";
    }
    if (Status1.Bits.LowOilLevel) {
        json += "\"Low Oil Level\": " + String(Status1.Bits.LowOilLevel) + ",";
    }
    if (Status1.Bits.LowOilPressure) {
        json += "\"Low Oil Pressure\": " + String(Status1.Bits.LowOilPressure) + ",";
    }
    if (Status1.Bits.LowSystemVoltage) {
        json += "\"Low System Voltage\": " + String(Status1.Bits.LowSystemVoltage) + ",";
    }
    if (Status1.Bits.OverTemperature) {
        json += "\"Over Temperature\": " + String(Status1.Bits.OverTemperature) + ",";
    }
    if (Status1.Bits.PreheatIndicator) {
        json += "\"Preheat Indicator\": " + String(Status1.Bits.PreheatIndicator) + ",";
    }
    if (Status1.Bits.RevLimitExceeded) {
        json += "\"Rev Limit Exceeded\": " + String(Status1.Bits.RevLimitExceeded) + ",";
    }
    if (Status1.Bits.ThrottlePositionSensor) {
        json += "\"Throttle Position Sensor\": " + String(Status1.Bits.ThrottlePositionSensor) + ",";
    }
    if (Status1.Bits.WaterFlow) {
        json += "\"Water Flow\": " + String(Status1.Bits.WaterFlow) + ",";
    }
    if (Status1.Bits.WaterInFuel) {
        json += "\"Water In Fuel\": " + String(Status1.Bits.WaterInFuel) + ",";
    }
    if (Status2.Bits.EngineCommError) {
        json += "\"Engine Comm Error\": " + String(Status2.Bits.EngineCommError);
    }
    if (Status2.Bits.EngineShuttingDown) {
        json += "\"Engine Shutting Down\": " + String(Status2.Bits.EngineShuttingDown);
    }
    if (Status2.Bits.LowOiPowerReduction) {
        json += "\"Low Oi Power Reduction\": " + String(Status2.Bits.LowOiPowerReduction);
    }
    if (Status2.Bits.MaintenanceNeeded) {
        json += "\"Maintenance Needed\": " + String(Status2.Bits.MaintenanceNeeded);
    }
    if (Status2.Bits.Manufacturer1) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer1);
    }
    if (Status2.Bits.Manufacturer2) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer2);
    }
    if (Status2.Bits.Manufacturer3) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer3);
    }
    if (Status2.Bits.Manufacturer4) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer4);
    }
    if (Status2.Bits.Manufacturer5) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer5);
    }
    if (Status2.Bits.Manufacturer6) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer6);
    }
    if (Status2.Bits.Manufacturer7) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer7);
    }
    if (Status2.Bits.Manufacturer8) {
        json += "\"Manufacturer\": " + String(Status2.Bits.Manufacturer8);
    }
    if (Status2.Bits.NeutralStartProtect) {
        json += "\"Neutral Start Protect\": " + String(Status2.Bits.NeutralStartProtect);
    }
    if (Status2.Bits.SubOrSecondaryThrottle) {
        json += "\"Sub Or Secondary Throttle\": " + String(Status2.Bits.SubOrSecondaryThrottle);
    }
    if (Status2.Bits.WarningLevel1) {
        json += "\"Warning Level\": " + String(Status2.Bits.WarningLevel1);
    }
    if (Status2.Bits.WarningLevel2) {
        json += "\"Warning Level\": " + String(Status2.Bits.WarningLevel2);
    }
    json += "}";
    return json; 
}

PGN129026_d::PGN129026_d(time_t TimeOfReceipt, int Pgn, unsigned char SID, tN2kHeadingReference HeadingReference, double SOG, double COG):
N2K_decomposed(TimeOfReceipt, Pgn), SID(SID), HeadingReference(HeadingReference), SOG(SOG), COG(COG)
{

}

String PGN129026_d::N2KtoJSON()
{
    String json = "{";
    json += "\"TimeOfReceipt\": " + String(TimeOfReceipt) + ",";
    json += "\"PGN\": " + String(Pgn) + ","; 
    json += "\"SID\": " + String(SID) + ",";
    json += "\"HeadingReference\": " + String(HeadingReference) + ",";
    json += "\"SOG\": " + String(SOG) + ",";
    json += "\"COG\": " + String(COG);
    json += "}";
    return json;

}
