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

#include <limits.h> // CHAR_BIT macro
#include <stddef.h> // size_t type

#if CHAR_BIT != 8
#error "Char is not 8 bit!"
#endif

/**
 * @brief Firmaware major release
 *
 * Encodes major releases.
 * */
typedef enum{
  LEGACY_NEUROON_FIRMWARE = 0x00, /**< firmware 2.0.x.x */
  NEW_NEUROON_FIRMWARE = 0x01     /**< firmware 2.1.x.x */
}e_firmwareMilestone;

/**
 * @brief Next step order
 *
 * Encoded orders for application to execute
 */
typedef enum{
  DFU_SEND_NEXT_DATASET = 0x00, /**< send data stored in output frame */
  DFU_RESEND_DATASET,           /**< resend previous data set */
  DFU_TERMINATE,                /**< Mask encoutered critical error in DFU mode */
  DFU_END                       /**< Update finished */
}e_dfuAction;

/**
 * @brief Generate "go to dfu" command
 *
 * @param[out]  frame     pointer to 20 bytes array where frame will be
 * stored
 * @param[out]  len       pointer to size_t value where function will put length
 * of array
 * @param[in]   firmware  choose firmware type
 *
 * @return true if function was able to generate frame, false otherwise(ex.
 * wrong frame len or unsupported firmware)
 */
bool goto_dfu(char *frame, size_t *len, e_firmwareMilestone firmware);

/**
 * @brief Receive data from rdfu response characteristic
 *
 * @param[in]   response_frame  20 byte array with response frame
 * @param[in]   response_len    response array length
 * @param[out]  frame           pointer to 20 bytes array where binary data will
 * be stored
 * @param[out]  len             pointer to size_t value where function will pit
 * output frame length
 *
 * @return returns next step for update
 */
e_dfuAction dfu_response_sink(char *response_frame, size_t response_len, char* frame, size_t *len);

/**
 * @brief Generate start update command for dfu
 *
 * @param[out]  frame pointer to 20 bytes array where frame will be stored
 * @param[out]  len   pointer to size_t value where function will put lenght of
 * array
 *
 * @return true if function was able to generate frame, false otherwise(ex.
 * wrong frame len)
 */
bool dfu_start_update(char *frame, size_t *len);

#endif /* !NEUROONMASKCOMMAPI_H */
