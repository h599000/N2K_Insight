#ifndef N2K_DECOMPOSED_H
#define N2K_DECOMPOSED_H

#include <N2KMsg.h>
#include <N2kTypes.h>

class N2K_decomposed
{
protected:
    time_t TimeOfReceipt;
    int Pgn;

public:
    N2K_decomposed(time_t TimeOfReceipt, int Pgn);
    virtual String N2KtoJSON();
};

// PGN 127496 - Trip Parameters, Vessel
class PGN127496_d : public N2K_decomposed
{
protected:
    // Secounds
    double TimetoEmpty;
    // Meters
    double DistancetoEmpty;
    // Litres
    short EstimatedFuelRemaining;
    // Meters
    double TripRunTime;

public:
    PGN127496_d(time_t TimeOfReceipt, int Pgn, double TimetoEmpty, double DistancetoEmpty, short EstimatedFuelRemaining, double TripRunTime);
    String N2KtoJSON();
};

// PGN127489 - Engine Parameters, Dynamic
class PGN127489_d : public N2K_decomposed
{
protected:
    // if only one instance 0 will be only engine. If two instances 0 will be port and 1 will be stb. If more engines 0 will be portmost 1 will be secound portmost and so forth.
    int8_t Instance;
    // Pascal
    double OilPressure;
    // Kelvin
    double OilTemperature;
    // Kelvin
    double Temperature;
    // Volt
    double AlternatorPotential;
    // Liter/hour
    double FuelRate;
    // Secounds
    double TotalEngineHours;
    // Pascal
    double CoolantPressure;
    // Pascal
    double FuelPressure;
    // Percent
    int8_t EngineLoad;
    // Percent
    int8_t EngineTorque;

    tN2kEngineDiscreteStatus1 Status1;

    tN2kEngineDiscreteStatus2 Status2;

public:
    PGN127489_d(time_t TimeOfReceipt, int Pg, int8_t Instance, double OilPressure, double OilTemperature, double Temperature, double AlternatorPotential, double FuelRate, double TotalEngineHours, double CoolantPressure, double FuelPressure, int8_t EngineLoad, int8_t EngineTorque, tN2kEngineDiscreteStatus1 Status1, tN2kEngineDiscreteStatus2 Status2);
    String N2KtoJSON();
};

// PGN 129026 - COG & SOG, Rapid Update
class PGN129026_d : public N2K_decomposed   
{
protected:
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;

public:
    PGN129026_d(time_t TimeOfReceipt, int Pg, unsigned char SID, tN2kHeadingReference HeadingReference, double SOG, double COG);
    String N2KtoJSON();
};
#endif