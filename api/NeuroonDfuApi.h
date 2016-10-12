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

/**
 * @brief Generate "go to dfu" command
 *
 * @param[out] frame
 * @param[out] len
 * @param[in] firmware
 *
 * @return
 */
bool goto_dfu(char *frame, size_t *len, e_firmwareMilestone firmware);

/**
 * @brief
 *
 * @param[in]   frame
 * @param[in]   len
 * @param[out]  frame
 * @param[out]  len
 *
 * @return
 */
bool dfu_response_sink(char *in_frame, size_t in_len, char* out_frame, size_t *out_len);

/**
 * @brief Generate start update command for dfu
 *
 * @param[out]  frame
 * @param[out]  len
 *
 * @return
 */
bool dfu_start_update(char *frame, size_t *len);

#endif /* !NEUROONMASKCOMMAPI_H */
