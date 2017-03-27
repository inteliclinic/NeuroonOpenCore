/**
 * @file    NeuroonDfuApi.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef NEUROONMASKCOMMAPI_H
#define NEUROONMASKCOMMAPI_H

#ifdef __cplusplus
#include <climits> // CHAR_BIT macro
#include <cstddef> // size_t type
#include <cstdint>
extern "C" {
#else
#include <limits.h> // CHAR_BIT macro
#include <stddef.h> // size_t type
#include <stdbool.h>
#include <stdint.h>
#endif

#if CHAR_BIT != 8
#error "Char is not 8 bit!"
#endif

/**
 * @brief Firmaware major release
 *
 * Encodes major releases.
 * */
typedef enum{
  LEGACY_NEUROON_FW = 0x00, /**< firmware 2.0.x.x */
  NEW_NEUROON_FW = 0x01     /**< firmware 2.1.x.x */
}ncFirmwareMilestone;

/**
 * @brief Next step order
 *
 * Encoded orders for application to execute
 */
typedef enum{
  DFU_SEND_NEXT = 0x00, /**< send data stored in output frame */
  DFU_RESEND,           /**< resend previous data set */
  DFU_TERM,             /**< Mask encoutered critical error in DFU mode */
  DFU_FINISHED          /**< Update finished (send last frame)*/
}ncDfuAction;

/**
 * @brief Firmware type
 */
typedef enum{
  SD_FIRM   = 0x00,  /**< Nordic Soft Device */
  APP_FIRM  = 0x01,  /**< Neuroon firmware */
  DFU_FIRM  = 0x02   /**< DFU */
}ncFirmwareType;

/**
 * @brief Generate "go to dfu" command
 *
 * @param[out]  frame     pointer to 20 bytes array where frame will be stored
 * @param[out]  len       pointer to size_t value where function will put length of array
 * @param[in]   firmware  choose firmware type
 *
 * @return characteristic index. Value of 0xFF means an error occurred
 *
 */
int ncGotoDfu(char *frame, size_t *len, ncFirmwareMilestone firmware);

/**
 * @brief Receive data from rdfu response characteristic
 *
 * @param[out]  frame         pointer to 20 bytes array where binary data will be stored
 * @param[out]  len           pointer to size_t value where function will put length of array
 * @param[in]   responseFrame 20 byte array with response frame
 * @param[in]   responseLen   response array length
 * @param[out]  action        pointer to a memory where desired action will be storred
 *                            @ref ncDfuAction
 *
 * @return characteristic index. Value of 0xFF means an error occurred
 *
 */
int ncDfuResponseSink(char* frame, size_t *len, char *responseFrame, size_t responseLen,
    ncDfuAction *action);

/**
 * @brief Generate start update command for dfu
 *
 * @param[out]  frame     pointer to 20 bytes array where frame will be stored
 * @param[out]  len       pointer to size_t value where function will put lenght of array
 * @param[in]   fb        pointer to buffer with binary file
 * @param[in]   file_len  binary file length
 * @param[in]   firm      type of firmware @ref ncFirmwareType
 * @param[in]   version   binary version build from 4 bytes. Ex 16777985 = 1.0.3.1
 *
 * @return characteristic index. Value of 0xFF means an error occurred
 *
 */
int ncDfuStartUpdate(char *frame, size_t *len, char *fb, size_t file_len, ncFirmwareType firm,
    uint32_t version);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !NEUROONMASKCOMMAPI_H */
