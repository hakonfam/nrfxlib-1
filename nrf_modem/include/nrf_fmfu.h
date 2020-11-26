/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file nrf_fmfu.h
 *
 */
#ifndef NRF_FMFU_H__
#define NRF_FMFU_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__GNUC__) || (__GNUC__ == 0)
typedef int32_t ssize_t;
#else
#include <sys/types.h>
#ifdef __SES_ARM
typedef int32_t ssize_t;
#endif
#endif

/**@addtogroup nrf_mfu_api_utils
 *@{
 */

/**@brief Modem	states */
#define NRF_FMFU_MODEM_STATE_UNINITIALIZED	0 /**< Modem is not initialized. */
#define NRF_FMFU_MODEM_STATE_WAITING		1 /**< Modem is waiting the booloader. */
#define NRF_FMFU_MODEM_STATE_READY			2 /**< Modem is ready for firmware upload. */
#define NRF_FMFU_MODEM_STATE_ERROR			3 /**< Modem is in error state. */

/**@brief Buffer length for digest and uuid data. */
#define NRF_FMFU_DIGEST_BUFFER_LEN 32
#define NRF_FMFU_UUID_BUFFER_LEN   36

/**@brief Size of modem communication buffer to be reserved. */
#define NRF_FMFU_MODEM_BUFFER_SIZE 0x201c

/**@brief Structure for storing 256-bit digest/hash replies. Endianess not converted. */
struct nrf_fmfu_digest {
	uint8_t data[NRF_FMFU_DIGEST_BUFFER_LEN];
};

/**@brief Structure for storing	modem UUID response. */
struct nrf_fmfu_uuid {
	uint8_t data[NRF_FMFU_UUID_BUFFER_LEN];
};

/**@brief
 * Function to set modem in DFU/RPC mode.
 *
 * @note Call once before MFU operation. If the modem goes to a error state, this can be called again to re-initialize.
 *       The root key digest response of the modem is put in the digest_buffer structure.
 *       NRF_FMFU_MODEM_BUFFER_SIZE shared memory buffer must be reserved for MFU operation to communicate with modem.
 *       If success, the modem will be in NRF_FMFU_RPC_MODEM_STATE_WAITING state.
 *
 * @param[out]  digest_buffer       Pointer to the buffer to store digest hash. If NULL digest skipped.
 * @param[in]   modem_buffer_len    Length of a modem buffer.
 * @param[in]   modem_buffer        Pointer to RPC buffer.
 *
 * @return 0 if the procedure succeeds, else, -1 and errno set in case of error.
 */
int nrf_fmfu_init(struct nrf_fmfu_digest *digest_buffer,
				uint32_t modem_buffer_len,
				void *modem_buffer);

/**@brief
 * Function for writing a memory chunk to the modem.
 *
 * @note Call after nrf_mfu_init to upload modem firmware segments.
 *       Bootloader segment must be uploaded first which sets the modem to
 *       NRF_FMFU_MODEM_STATE_READY state. Firmware segments can be
 *       uploaded after successful bootloader upload.
 *       Use nrf_fmfu_transfer_start and nrf_fmfu_transfer_end at the start
 *       and end of each segment upload.
 *
 * @param[in]   addr   Address where the data to be written.
 * @param[in]   len    Length of data to be written.
 * @param[in]   src    Pointer to the buffer where chunk data is stored.
 *
 * @return 0 if the procedure succeeds, else, -1 and errno set in case of error.
 */
int nrf_fmfu_memory_chunk_write(uint32_t addr, uint32_t len, void *src);

/**@brief
 * Function for starting a data chunk download.
 *
 * @return 0 if the procedure succeeds, else, -1 and errno set in case of error.
 */
int nrf_fmfu_transfer_start(void);

/**@brief
 * Function for ending a data chunk download.
 *
 * @return 0 if the procedure succeeds, else, -1 and errno set in case of error.
 */
int nrf_fmfu_transfer_end(void);

/**@brief
 * Function for reading a digest hash data from the modem.
 *
 * @param[in]   start_address   Start address.
 * @param[in]   end_address     End address.
 * @param[out]  digest_buffer   Pointer to the buffer to store digest hash data.
 *
 * @return 0 if the procedure succeeds, else, -1 and errno set in case of error.
 */
int nrf_fmfu_memory_hash_get(uint32_t addr, uint32_t size,
			     struct nrf_fmfu_digest *digest_buffer);

/**@brief
 * Function for reading an uuid data from the modem.
 *
 * @param[out]  modem_uuid      Pointer to the buffer to store uuid data.
 *
 * @return 0 if the procedure succeeds, else, -1 and errno set in case of error.
 */
int nrf_fmfu_uuid_get(struct nrf_fmfu_uuid *modem_uuid);

/**@brief
 * Function for reading modem state.
 *
 * @retval  NRF_FMFU_MODEM_STATE_UNINITIALIZED When modem is not initialized.
 * @retval  NRF_FMFU_MODEM_STATE_WAITING When modem is ready to receive bootloader.
 * @retval  NRF_FMFU_MODEM_STATE_READY When modem is ready to receive firmware.
 * @retval  NRF_FMFU_MODEM_STATE_ERROR When modem is in error state.
 */
int nrf_fmfu_modem_state_get(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRF_FMFU_H__

/**@} */
