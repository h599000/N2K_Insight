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

}

PGN129026_d::PGN129026_d(time_t TimeOfReceipt, int Pgn, unsigned char SID, tN2kHeadingReference HeadingReference, double SOG, double COG):
N2K_decomposed(TimeOfReceipt, Pgn), SID(SID), HeadingReference(HeadingReference), SOG(SOG), COG(COG)
{

}

String PGN129026_d::N2KtoJSON()
{
    String json = "{";
    json += "\"TimeOfReceipt\"" + String(TimeOfReceipt) + ",";
    json += "\"PGN\": " + String(Pgn) + ","; 
    json += "\"SID\": " + String(SID) + ",";
    json += "\"HeadingReference\": " + String(HeadingReference) + ",";
    json += "\"SOG\": " + String(SOG) + ",";
    json += "\"COG\": " + String(COG);
    json += "}";
    return json;

}
