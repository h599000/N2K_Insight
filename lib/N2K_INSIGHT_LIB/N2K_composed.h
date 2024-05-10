#ifndef N2K_COMPOSED_H
#define N2K_COMPOSED_H

#include <N2kMsg.h>

class N2K_composed
{
public:
    /** 
     * @brief Maximum number of bytes that can be stored in the data buffer 
    */
    static const int MaxDataLen = 223;

    /** 
     * @brief Parameter Group Number (PGN) of the NMEA2000 message
    */
    long PGN;

    /** 
     * @brief Source of the NMEA2000 message
    */
    mutable unsigned char Source;

    /**
     * @brief 
     * 
     * @ref tN2kMsg::Data of this message
     */
    int DataLen;

    /** 
     * @brief Byte array which carries all the data of the NMEA2000 message
    */
    unsigned char Data[MaxDataLen];

    /** 
     * @brief timestamp (ms since start [max 49days]) of the NMEA2000 message
    */
    unsigned long MsgTime;

    /**
    * @class N2K_composed
    * @brief Constructor for the N2K_composed class, which initializes an instance with data from a tN2kMsg.
    *
    * @param msg The tN2kMsg from which to initialize the N2K_composed object.
    *            It contains the raw data and metadata for an NMEA2000 message.
    */
    N2K_composed(tN2kMsg msg);

    /**
     * @brief Converts the NMEA 2000 message data to a JSON formatted string.
     * 
     * @return A String containing the JSON formatted representation of the N2K_composed object.
     *         This string includes the PGN, Source, MsgTime, and Data fields.
     */
    String N2KtoJSON();
};

#endif