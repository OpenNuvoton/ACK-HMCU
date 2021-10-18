/**************************************************************************//**
 * @file     nu_rbuf.c
 * @brief    An Ring-buffer implementation.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "nu_rbuf.h"

#define MIN(a, b) (a)<(b)? (a) : (b)

int nu_rbuf_create(nu_rbuf_t *nu_rbuf, uint8_t *buffer, int length)
{
    if ((nu_rbuf == NULL) || (buffer == NULL))
    {
        return -1;
    }

    memset(nu_rbuf, 0, sizeof(nu_rbuf_t));
    memset(buffer, 0, length);

    nu_rbuf->length = length;
    nu_rbuf->head = 0;
    nu_rbuf->tail = 0;
    nu_rbuf->chunk_idx = 0;
    nu_rbuf->buffer = (uint8_t *)buffer;

    return 0;
}

int nu_rbuf_dump(nu_rbuf_t *nu_rbuf)
{
    if (!nu_rbuf)
    {
        return -1;
    }
    //printf("[%08x, %08x-%d] r=%d, w=%d, ci=%d \n", nu_rbuf, nu_rbuf->buffer, nu_rbuf->length, nu_rbuf->head, nu_rbuf->tail, nu_rbuf->chunk_idx );
    return 0;
}

void nu_rbuf_destroy(nu_rbuf_t *nu_rbuf)
{
    memset(nu_rbuf->buffer, 0, nu_rbuf->length);
    nu_rbuf->length = 0;
    nu_rbuf->head = nu_rbuf->tail = 0;
}

int nu_rbuf_avail_read_space(nu_rbuf_t *nu_rbuf)
{
    int ret = 0;

    uint32_t u32WPos = nu_rbuf->tail;
    uint32_t u32RPos = nu_rbuf->head;

    if (u32RPos == u32WPos)
        ret = 0;
    else if (u32RPos < u32WPos)
        ret = u32WPos - u32RPos;
    else
        ret = nu_rbuf->length - (u32RPos - u32WPos);

    return ret;
}

int nu_rbuf_write(nu_rbuf_t *nu_rbuf, uint8_t *data, uint32_t length)
{
    int i = 0;

    if (nu_rbuf == NULL || data == NULL || length == 0)
    {
        return -1;
    }

    /* if empty reset head and tail to zero */
    if (nu_rbuf_empty(nu_rbuf))
    {
        nu_rbuf_clear(nu_rbuf);
    }

    for (i = 0; i < length; i++)
    {

        if (nu_rbuf_full(nu_rbuf))
        {
            //printf("0x%x ringbuffer %d full\r\n", nu_rbuf->buffer,nu_rbuf->length);
            break;
        }

        nu_rbuf->buffer[nu_rbuf->tail] = data[i];

        nu_rbuf->tail++;
        nu_rbuf->tail %= (nu_rbuf->length);

    }

    /* return real write len */
    return i;
}

int nu_rbuf_dummy_write(nu_rbuf_t *nu_rbuf, uint32_t length)
{
    if (nu_rbuf == NULL || length == 0)
        return -1;

    nu_rbuf->tail += length;
    nu_rbuf->tail %= (nu_rbuf->length);
    return length;
}

int nu_rbuf_dummy_read(nu_rbuf_t *nu_rbuf, uint32_t length)
{
    if (nu_rbuf == NULL || length == 0)
        return -1;

    nu_rbuf->head += length;
    nu_rbuf->head %= (nu_rbuf->length);
    return length;
}

int nu_rbuf_read(nu_rbuf_t *nu_rbuf, uint8_t *target, uint32_t amount)
{
    int copy_sz = 0;
    int i;

    if (nu_rbuf == NULL || target == NULL || amount == 0)
    {
        return -1;
    }

    if (nu_rbuf_empty(nu_rbuf))
    {
        goto exit_nu_rbuf_read;
    }

    /* get real read size */
    copy_sz = MIN(amount, nu_rbuf_avail_read_space(nu_rbuf));

    /* cp data to user buffer */
    for (i = 0; i < copy_sz; i++)
    {
        target[i] = nu_rbuf->buffer[nu_rbuf->head];

        nu_rbuf->head++;
        nu_rbuf->head %= (nu_rbuf->length);
    }

exit_nu_rbuf_read:
    return copy_sz;
}
