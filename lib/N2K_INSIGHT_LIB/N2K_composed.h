#ifndef N2K_COMPOSED_H
#define N2K_COMPOSED_H

#include <N2kMsg.h>

class N2K_composed
{
public:
    /** \brief Maximum number of bytes that can be stored in the data buffer */
    static const int MaxDataLen = 223;
    /** \brief Parameter Group Number (PGN) of the NMEA2000 message*/
    long PGN;
    /** \brief Source of the NMEA2000 message*/
    mutable unsigned char Source;
    /** \brief Number of bytes already stored in \ref tN2kMsg::Data of this message*/
    int DataLen;
    /** \brief Byte array which carries all the data of the NMEA2000 message*/
    unsigned char Data[MaxDataLen];
    /** \brief timestamp (ms since start [max 49days]) of the NMEA2000 message*/
    unsigned long MsgTime;
    N2K_composed(tN2kMsg msg);
    String N2KtoJSON();
};

#endif