#ifndef N2K_I2C_API_H
#define N2K_I2C_API_H

#include <Arduino.h>
#include <Wire.h>
#include <N2K_decomposed.h>
#include <N2K_composed.h>

extern PGN129026_d* n2kCOGSOG;

typedef struct {
  unsigned int request;
  void (*Handler)(const long PGN); 
} I2CRequestHandler;


extern I2CRequestHandler I2CRequestHandlers[];



void requestData();
void receiveData(int byteCount);
void status(long pgn);
void newPGN(long pgn);
void spesificPGN(long pgn);

#endif