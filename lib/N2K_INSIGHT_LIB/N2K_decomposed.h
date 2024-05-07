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
    /**
     * @brief Constructor for the N2K_decomposed class, which initializes an instance with given parameters.
     *
     * This constructor initializes an N2K_decomposed object with the specific time of receipt
     * and Parameter Group Number (PGN). These values are used to represent the metadata associated
     * with a decomposed NMEA 2000 message.
     *
     * @param TimeOfReceipt The time at which the message was received.
     * @param Pgn The PGN of this NMEA 2000 message.
     */
    N2K_decomposed(time_t TimeOfReceipt, int Pgn);

    /**
     * @brief Converts the N2K_decomposed object data to a JSON formatted string.
     *
     * This member function is intended to serialize the contents of the N2K_decomposed object
     * into a JSON string. The JSON string should include relevant data such as TimeOfReceipt, Pgn, and
     * possibly other details that describe the decomposed NMEA 2000 message.
     *
     * @note Currently, this function is a stub and does not perform any serialization. It returns
     * an empty string. Proper implementation is required to achieve the intended functionality.
     *
     * @return An empty String object, indicating that serialization is not yet implemented.
     */
    virtual String N2KtoJSON();
};

/** @brief PGN 127496 - Trip Parameters, Vessel */
class PGN127496_d : public N2K_decomposed
{
protected:
    // Seconds
    double TimetoEmpty;
    // Meters
    double DistancetoEmpty;
    // Litres
    short EstimatedFuelRemaining;
    // Meters
    double TripRunTime;

public:
    /**
     * @brief Constructor for PGN127496_d class, initializing data specific to PGN 127496.
     *
     * The PGN127496_d class is derived from N2K_decomposed and represents the NMEA 2000 message
     * with PGN 127496, which typically contains information about the engine parameters
     * like time to empty, distance to empty, estimated fuel remaining, and trip run time.
     *
     * @param TimeOfReceipt The time at which the message was received
     * @param Pgn The PGN of this NMEA 2000 message.
     * @param TimetoEmpty The calculated time remaining before the fuel tank is empty, based on current usage.
     * @param DistancetoEmpty The calculated distance that can be covered before the fuel tank is empty, based on current usage.
     * @param EstimatedFuelRemaining An estimate of the amount of fuel remaining in the tank.
     * @param TripRunTime The cumulative time the trip has been running.
     */
    PGN127496_d(time_t TimeOfReceipt, int Pgn, double TimetoEmpty, double DistancetoEmpty, short EstimatedFuelRemaining, double TripRunTime);

    /**
     * @brief Converts the N2K_decomposed object data to a JSON formatted string.
     *
     * @return A JSON formatted string.
     */
    String N2KtoJSON();
};

/** @brief PGN127489 - Engine Parameters, Dynamic */
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
    // Seconds
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
    /**
     * @brief Constructor for PGN127489_d class, initializing data specific to PGN 127489.
     *
     * @param TimeOfReceipt The time at which the message was received
     * @param Pgn The PGN of this NMEA 2000 message.
     * @param Instance Instance of the engine to which this data message belongs.
     * @param OilPressure The current engine oil pressure reading.
     * @param OilTemperature The current engine oil temperature reading.
     * @param Temperature The current temperature of the engine.
     * @param AlternatorPotential The current potential (voltage) of the alternator.
     * @param FuelRate The current rate of fuel consumption.
     * @param TotalEngineHours The total number of hours the engine has been operating.
     * @param CoolantPressure The current pressure of the engine coolant.
     * @param FuelPressure The current fuel pressure.
     * @param EngineLoad The current percentage of maximum achievable engine load.
     * @param EngineTorque The current percentage of maximum achievable engine torque.
     * @param Status1 A discrete set of status flags indicating various binary engine conditions.
     * @param Status2 A second discrete set of status flags complementing Status1.
     */
    PGN127489_d(time_t TimeOfReceipt, int Pgn, int8_t Instance, double OilPressure, double OilTemperature, double Temperature, double AlternatorPotential, double FuelRate, double TotalEngineHours, double CoolantPressure, double FuelPressure, int8_t EngineLoad, int8_t EngineTorque, tN2kEngineDiscreteStatus1 Status1, tN2kEngineDiscreteStatus2 Status2);

    /**
     * @brief Converts the N2K_decomposed object data to a JSON formatted string.
     *
     * @return A JSON formatted string.
     */
    String N2KtoJSON();
};

/** @brief PGN 129026 - COG & SOG, Rapid Update */
class PGN129026_d : public N2K_decomposed
{
protected:
    // Letter
    unsigned char SID;
    // Heading reference 
    tN2kHeadingReference HeadingReference;
    // Radians
    double COG;
    // Meters per second
    double SOG;
public:
    /**
     * @brief Constructor for PGN129026_d class, initializing data specific to PGN 129026.
     * 
     * @param TimeOfReceipt The time at which the NMEA 2000 message was received
     * @param Pgn The PGN of this NMEA 2000 message.
     * @param SID The sequence identifier of the data source (unique identifier for the message)
     *        used for matching related PGNs.
     * @param HeadingReference The reference used for COG data (e.g., true north, magnetic north).
     * @param SOG The speed over ground (SOG) in meters per second, representing the vessel's ground speed.
     * @param COG The course over ground (COG) in radians, representing the vessel's ground track angle.
     */
    PGN129026_d(time_t TimeOfReceipt, int Pgn, unsigned char SID, tN2kHeadingReference HeadingReference, double SOG, double COG);

    /**
     * @brief Converts the N2K_decomposed object data to a JSON formatted string.
     *
     * @return A JSON formatted string.
     */
    String N2KtoJSON();
};
#endif