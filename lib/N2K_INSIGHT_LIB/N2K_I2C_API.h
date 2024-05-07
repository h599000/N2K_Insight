#ifndef N2K_I2C_API_H
#define N2K_I2C_API_H

#include <Arduino.h>
#include <Wire.h>
#include <N2K_decomposed.h>
#include <N2K_composed.h>

extern PGN129026_d* n2kCOGSOG;

/**
 * @brief An array of I2CRequestHandler structures used for handling I2C requests.
 * 
 */
typedef struct {
  unsigned int request;
  void (*Handler)(const long PGN); 
} I2CRequestHandler;

/**
 * @brief This function finds the correct handler function and calls it with the given PGN.
 * 
 * @param request A pointer to the message received from controller
 */
extern I2CRequestHandler I2CRequestHandlers[];

/**
 * @brief The function handling an I2C request event
 * 
 */
void requestData();

/**
 * @brief The function handling an I2C receive event\n 
 * 
 * Calls the function specified in the messagen with the parameter specified.
 * 
 * @param byteCount The number of bytes in received message
 */
void receiveData(int byteCount);

/**
 * @brief Writes the adress of the I2C device to the controller 
 * 
 * @param pgn Not used
 */
void status(long pgn);

/**
 * @brief Writes a list of new PGNs to controller 
 * 
 * @param pgn Not used
 * 
 * @note NOT IMPLEMENTED
 */
void newPGN(long pgn);

/**
 * @brief Writes the last message of the given PGN to the controller 
 * 
 * @param pgn PGN
 * 
 * @note Only PGN 129026 implemented
 */
void spesificPGN(long pgn);

#endif