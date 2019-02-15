/***************************************************************************//**
* \file cy_crypto_core_mem_v2.c
* \version 2.20
*
* \brief
*  This file provides the source code to the API for the PRNG
*  in the Crypto block driver.
*
********************************************************************************
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#include "cy_crypto_common.h"
#include "cy_crypto_core_mem_v2.h"
#include "cy_crypto_core_hw_v2.h"
#include "cy_syslib.h"


#if defined(CY_IP_MXCRYPTO)


/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_MemCpy
****************************************************************************//**
*
* Function MemCpy uses Crypto HW.
* Memory structures should not overlap!
* There is no alignment restriction.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param src
* The pointer to the source of MemCpy.
*
* \param dst
* The pointer to the destination of MemCpy.
*
* \param size
* The size in bytes of the copy operation.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_MemCpy(CRYPTO_Type *base, void* dst, void const *src, uint16_t size)
{
    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, (const uint8_t*)src, (uint32_t)size);
    Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, (const uint8_t*)dst, (uint32_t)size);

    while (size >= CY_CRYPTO_V2_DATA_FIFODEPTH)
    {
        Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_V2_DATA_FIFODEPTH);
        size -= CY_CRYPTO_V2_DATA_FIFODEPTH;
    }

    if (size != 0u)
    {
        Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_FF_LOAD0, (uint32_t)size);
    }

    Cy_Crypto_Core_V2_Sync(base);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_MemSet
****************************************************************************//**
*
* Function MemSet uses Crypto HW.
* There is no alignment restriction.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param dst
* The pointer to the destination of MemSet.

* \param data
* The value to be set.

* \param size
* The size in bytes of the set operation.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_MemSet(CRYPTO_Type *base, void* dst, uint8_t data, uint16_t size)
{
    Cy_Crypto_Core_V2_FFStart(base, CY_CRYPTO_V2_RB_FF_STORE, dst, (uint32_t)size);

    while (size >= CY_CRYPTO_V2_DATA_FIFODEPTH)
    {
        Cy_Crypto_Core_V2_BlockSet(base, CY_CRYPTO_V2_RB_FF_STORE, data, CY_CRYPTO_V2_DATA_FIFODEPTH);
        size -= CY_CRYPTO_V2_DATA_FIFODEPTH;
    }

    if (size != 0u)
    {
        Cy_Crypto_Core_V2_BlockSet(base, CY_CRYPTO_V2_RB_FF_STORE, data, (uint32_t)size);
    }

    Cy_Crypto_Core_V2_Sync(base);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_MemCmp
****************************************************************************//**
*
* Function MemCmp uses Crypto HW.
* There is no alignment restriction.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param src0
* The pointer to the first source of MemCmp.

* \param src1
* The pointer to the second source of MemCmp.

* \param size
* the size in bytes of the compare operation.
*
* \return
* 0 - if Source 1 = Source 2, 1 - if not.
*
*******************************************************************************/
uint32_t Cy_Crypto_Core_V2_MemCmp(CRYPTO_Type *base, void const *src0, void const *src1, uint16_t size)
{
    REG_CRYPTO_RESULT(base) = 0ul;

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, (const uint8_t*)src0, (uint32_t)size);
    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD1, (const uint8_t*)src1, (uint32_t)size);

    while (size >= CY_CRYPTO_V2_DATA_FIFODEPTH)
    {
        Cy_Crypto_Core_V2_BlockCmp(base, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_V2_RB_FF_LOAD1, CY_CRYPTO_V2_DATA_FIFODEPTH);
        size -= CY_CRYPTO_V2_DATA_FIFODEPTH;
    }

    if (size != 0u)
    {
        Cy_Crypto_Core_V2_BlockCmp(base, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_V2_RB_FF_LOAD1, (uint32_t)size);
    }

    Cy_Crypto_Core_V2_Sync(base);

    return((uint32_t)(REG_CRYPTO_RESULT(base)));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_MemXor
****************************************************************************//**
*
* Function MemXor uses Crypto HW.
* Memory structures should not overlap!
* There is no alignment restriction.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param src0
* The pointer to the first source of MemXor.

* \param src1
* The pointer to the second source of MemXor.

* \param dst
* The pointer to the destination of MemXor.
*
* \param size
* The size in bytes of the compare operation.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_MemXor(CRYPTO_Type *base,
                               void* dst, void const *src0, void const *src1, uint16_t size)
{
    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, (const uint8_t*)src0, (uint32_t)size);
    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD1, (const uint8_t*)src1, (uint32_t)size);
    Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, dst, (uint32_t)size);

    while (size >= CY_CRYPTO_V2_DATA_FIFODEPTH)
    {
        Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_FF_LOAD0,
                                         CY_CRYPTO_V2_RB_FF_LOAD1, CY_CRYPTO_V2_DATA_FIFODEPTH);

        size -= CY_CRYPTO_V2_DATA_FIFODEPTH;
    }

    if (size != 0u)
    {
        Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_FF_LOAD0,
                                         CY_CRYPTO_V2_RB_FF_LOAD1, (uint32_t)size);
    }

    Cy_Crypto_Core_V2_Sync(base);
}


#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */