/**************************************************************************//**
 * @brief    Microcontroller Library
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>

typedef struct
{
    uint8_t  *buffer;
    uint32_t length;
    uint32_t head;
    uint32_t tail;
    uint32_t chunk_idx;
} nu_rbuf_t;

/**
 * @brief  create a ring buffer
 * @param[in] buffer pointer of the ring buffer
 * @param[in] length length space of the ring buffer
 * @return  pointer to ring buffer
 */
int nu_rbuf_create(nu_rbuf_t *nu_rbuf, uint8_t *buffer, int length);
int nu_rbuf_dump(nu_rbuf_t *nu_rbuf);
/**
 * @brief   destroy the ring buffer
 * @param[in] buffer pointer to the ring buffer
 * @return  null
 */
void nu_rbuf_destroy(nu_rbuf_t *buffer);

/**
 * @brief   read data from ring buffer.
 * @param[in] buffer pointer to the ring buffer
 * @param[in] target pointer to buffer for data to read from ring buffer
 * @param[in] amount amount of data items to read
 * @return  number of actual read data
 */
int nu_rbuf_read(nu_rbuf_t *buffer, uint8_t *target, uint32_t amount);

/**
 * @brief   dummy read
 * @param[in] buffer pointer to the ring buffer
 * @param[in] length length of data items to write
 * @return  0 for success
 */
int nu_rbuf_dummy_read(nu_rbuf_t *buffer, uint32_t length);

/**
 * @brief   write data to ring buffer
 * @param[in] buffer pointer to the ring buffer
 * @param[in] data pointer to buffer for data to write to ring buffer
 * @param[in] length length of data items to write
 * @return  0 for success
 */
int nu_rbuf_write(nu_rbuf_t *buffer, uint8_t *data, uint32_t length);

/**
 * @brief   dummy write data to ring buffer
 * @param[in] buffer pointer to the ring buffer
 * @param[in] length length of data items to write
 * @return  0 for success
 */
int nu_rbuf_dummy_write(nu_rbuf_t *buffer, uint32_t length);

/**
 * @brief   is the ring buffer empty?
 * @param[in] buffer pointer to the ring buffer
 * @return  0 for success
 */
int nu_rbuf_empty(nu_rbuf_t *buffer);

/**
 * @brief   is the ring buffer full?
 * @param[in] buffer pointer to the ring buffer
 * @return  0 for success
 */
int nu_rbuf_full(nu_rbuf_t *buffer);

/**
 * @brief   available write space to ring buffer
 * @param[in] buffer pointer to the ring buffer
 * @return   number of write spcae
 */
int nu_rbuf_avail_write_space(nu_rbuf_t *buffer);

/**
 * @brief    available read space to ring buffer
 * @param[in] buffer pointer to the ring buffer
 * @return  number of read spcae
 */
int nu_rbuf_avail_read_space(nu_rbuf_t *buffer);

#define nu_rbuf_avail_write_space(B) ((B)->length - nu_rbuf_avail_read_space(B))

#define nu_rbuf_full(B) (nu_rbuf_avail_write_space(B) == 0)

#define nu_rbuf_empty(B) (nu_rbuf_avail_read_space((B)) == 0)

#define nu_rbuf_clear(B) (B->tail = B->head = 0)
