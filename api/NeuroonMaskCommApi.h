/**
 * @file    NeuroonMaskCommApi.h
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

typedef enum {
  LEGACY_NEUROON_FIRMWARE = 0x00,
  NEW_NEUROON_FIRMWARE = 0x01
}e_firmwareMilestone;


/********************************** COMMON  **********************************/
/**
 * @brief
 * Sink for data from D19E0201-D97F-E2D3-840C-A11CB81C0886 characteristic
 *
 * @param in_frame
 * @param in_len
 *
 * @return
 */
bool data_stream0_sink(char *in_frame, size_t in_len);

/**
 * @brief
 * Sink for data from D19E0202-D97F-E2D3-840C-A11CB81C0886 characteristic
 *
 * @param in_frame
 * @param in_len
 *
 * @return
 */
bool data_stream1_sink(char *in_frame, size_t in_len);

/**
 * @brief
 * Sink for data from D19E0301-D97F-E2D3-840C-A11CB81C0886 characteristic
 *
 * @param in_frame
 * @param in_len
 *
 * @return
 */
bool data_stream2_sink(char *in_frame, size_t in_len);

/**
 * @brief
 * Sink for responses from D19E0302-D97F-E2D3-840C-A11CB81C0886 characteristic
 *
 * @param in_frame
 * @param in_len
 *
 * @return
 */
bool response_sink(char *in_frame, size_t in_len);
/*****************************************************************************/


/************************************ DFU ************************************/
/**
 * @brief
 * Generate "go to dfu" command
 *
 * @param out_frame
 * @param out_len
 *
 * @return
 */
bool goto_dfu(char *out_frame, size_t *out_len, e_firmwareMilestone firmware);

/**
 * @brief 
 *
 * @param in_frame
 * @param in_len
 * @param out_frame
 * @param out_len
 *
 * @return 
 */
bool dfu_response_sink(char *in_frame, size_t in_len, char* out_frame, size_t *out_len);

/**
 * @brief 
 * Generate start update command for dfu
 *
 * @param out_frame
 * @param out_len
 *
 * @return 
 */
bool dfu_start_update(char *out_frame, size_t *out_len);
/*****************************************************************************/
#endif /* !NEUROONMASKCOMMAPI_H */
