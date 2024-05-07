#include "N2K_composed.h"

N2K_composed::N2K_composed(tN2kMsg msg) : PGN(msg.PGN), Source(msg.Source), DataLen(msg.DataLen), MsgTime(msg.MsgTime)
{
    for (int i = 0; i < DataLen; i++)
    {
        Data[i] = msg.Data[i];
    }
}

String N2K_composed::N2KtoJSON()
{
    String json = "{";
    json += "\"PGN\": " + String(PGN) + ",";
    json += "\"Source\": " + String(Source) + ",";
    json += "\"MsgTime\": " + String(MsgTime) + ",";
    json += "\"Data\": ";
    for (int i = 0; i < DataLen; i++)
    {
        json += String(Data[i]);
    };
    json += ",";
    json += "}";
    return json;
};