/*******************************************************************************

    Copyright 2014-2015, 2021 Ben Wojtowicz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************

    File: liblte_pdcp.cc

    Description: Contains all the implementations for the LTE Packet Data
                 Convergence Protocol Layer library.

    Revision History
    ----------    -------------    --------------------------------------------
    08/03/2014    Ben Wojtowicz    Created file.
    11/01/2014    Ben Wojtowicz    Added integrity protection of messages.
    11/29/2014    Ben Wojtowicz    Using the byte message struct for everything
                                   except RRC SDUs and added user plane data
                                   processing.
    03/11/2015    Ben Wojtowicz    Added data PDU with short SN support.
    12/06/2015    Ben Wojtowicz    Added control PDU for interspersed ROHC
                                   feedback and RN user plane data PDU with
                                   integrity protection support.
    02/14/2021    Ben Wojtowicz    Massive reformat.

*******************************************************************************/

/*******************************************************************************
                              INCLUDES
*******************************************************************************/

#include "liblte_pdcp.h"
#include "liblte_security.h"

/*******************************************************************************
                              DEFINES
*******************************************************************************/


/*******************************************************************************
                              TYPEDEFS
*******************************************************************************/


/*******************************************************************************
                              GLOBAL VARIABLES
*******************************************************************************/


/*******************************************************************************
                              PDU FUNCTIONS
*******************************************************************************/

/*********************************************************************
    PDU Type: Control Plane PDCP Data PDU

    Document Reference: 36.323 v10.1.0 Section 6.2.2
*********************************************************************/
LIBLTE_ERROR_ENUM liblte_pdcp_pack_control_pdu(LIBLTE_PDCP_CONTROL_PDU_STRUCT *contents,
                                               LIBLTE_BYTE_MSG_STRUCT         *pdu)
{
    return liblte_pdcp_pack_control_pdu(contents, &contents->data, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_control_pdu(LIBLTE_PDCP_CONTROL_PDU_STRUCT *contents,
                                               LIBLTE_BIT_MSG_STRUCT          *data,
                                               LIBLTE_BYTE_MSG_STRUCT         *pdu)
{
    return liblte_pdcp_pack_control_pdu(contents, data, NULL, 0, 0, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_control_pdu(LIBLTE_PDCP_CONTROL_PDU_STRUCT *contents,
                                               uint8                          *key_256,
                                               uint8                           direction,
                                               uint8                           rb_id,
                                               LIBLTE_BYTE_MSG_STRUCT         *pdu)
{
    return liblte_pdcp_pack_control_pdu(contents, &contents->data, key_256, direction, rb_id, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_control_pdu(LIBLTE_PDCP_CONTROL_PDU_STRUCT *contents,
                                               LIBLTE_BIT_MSG_STRUCT          *data,
                                               uint8                          *key_256,
                                               uint8                           direction,
                                               uint8                           rb_id,
                                               LIBLTE_BYTE_MSG_STRUCT         *pdu)
{
    if(contents == NULL || data == NULL || pdu == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    *pdu_ptr = contents->count & 0x1F;
    pdu_ptr++;

    // Byte align data
    if((data->N_bits % 8) != 0)
    {
        for(uint32 i=0; i<8-(data->N_bits % 8); i++)
            data->msg[data->N_bits + i] = 0;
        data->N_bits += 8 - (data->N_bits % 8);
    }

    // Data
    uint8 *data_ptr = data->msg;
    for(uint32 i=0; i<data->N_bits/8; i++)
    {
        *pdu_ptr = liblte_bits_2_value(&data_ptr, 8);
        pdu_ptr++;
    }

    // MAC
    if(NULL == key_256)
    {
        *pdu_ptr = (LIBLTE_PDCP_CONTROL_MAC_I >> 24) & 0xFF;
        pdu_ptr++;
        *pdu_ptr = (LIBLTE_PDCP_CONTROL_MAC_I >> 16) & 0xFF;
        pdu_ptr++;
        *pdu_ptr = (LIBLTE_PDCP_CONTROL_MAC_I >> 8) & 0xFF;
        pdu_ptr++;
        *pdu_ptr = LIBLTE_PDCP_CONTROL_MAC_I & 0xFF;
        pdu_ptr++;
    }else{
        pdu->N_bytes = pdu_ptr - pdu->msg;
        liblte_security_128_eia2(&key_256[16],
                                 contents->count,
                                 rb_id,
                                 direction,
                                 pdu->msg,
                                 pdu->N_bytes,
                                 pdu_ptr);
        pdu_ptr += 4;
    }

    // Fill in the number of bytes used
    pdu->N_bytes = pdu_ptr - pdu->msg;

    return LIBLTE_SUCCESS;
}
LIBLTE_ERROR_ENUM liblte_pdcp_unpack_control_pdu(LIBLTE_BYTE_MSG_STRUCT         *pdu,
                                                 LIBLTE_PDCP_CONTROL_PDU_STRUCT *contents)
{
    if(pdu == NULL || contents == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    contents->count = *pdu_ptr & 0x1F;
    pdu_ptr++;

    // Data
    uint8 *data_ptr = contents->data.msg;
    for(uint32 i=0; i<pdu->N_bytes-5; i++)
    {
        liblte_value_2_bits(*pdu_ptr, &data_ptr, 8);
        pdu_ptr++;
    }
    contents->data.N_bits = data_ptr - contents->data.msg;

    return LIBLTE_SUCCESS;
}

/*********************************************************************
    PDU Type: User Plane PDCP Data PDU with long PDCP SN

    Document Reference: 36.323 v10.1.0 Section 6.2.3
*********************************************************************/
LIBLTE_ERROR_ENUM liblte_pdcp_pack_data_pdu_with_long_sn(LIBLTE_PDCP_DATA_PDU_WITH_LONG_SN_STRUCT *contents,
                                                         LIBLTE_BYTE_MSG_STRUCT                   *pdu)
{
    return liblte_pdcp_pack_data_pdu_with_long_sn(contents, &contents->data, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_data_pdu_with_long_sn(LIBLTE_PDCP_DATA_PDU_WITH_LONG_SN_STRUCT *contents,
                                                         LIBLTE_BYTE_MSG_STRUCT                   *data,
                                                         LIBLTE_BYTE_MSG_STRUCT                   *pdu)
{
    if(contents == NULL || data == NULL || pdu == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    *pdu_ptr = (LIBLTE_PDCP_D_C_DATA_PDU << 7) | ((contents->count >> 8) & 0x0F);
    pdu_ptr++;
    *pdu_ptr = contents->count & 0xFF;
    pdu_ptr++;

    // Data
    memcpy(pdu_ptr, data->msg, data->N_bytes);
    pdu_ptr += data->N_bytes;

    // Fill in the number of bytes used
    pdu->N_bytes = pdu_ptr - pdu->msg;

    return LIBLTE_SUCCESS;
}
LIBLTE_ERROR_ENUM liblte_pdcp_unpack_data_pdu_with_long_sn(LIBLTE_BYTE_MSG_STRUCT                   *pdu,
                                                           LIBLTE_PDCP_DATA_PDU_WITH_LONG_SN_STRUCT *contents)
{
    if(pdu == NULL || contents == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    if(LIBLTE_PDCP_D_C_DATA_PDU != (LIBLTE_PDCP_D_C_ENUM)((*pdu_ptr >> 7) & 0x01))
        return LIBLTE_ERROR_INVALID_INPUTS;
    contents->count = (*pdu_ptr & 0x0F) << 8;
    pdu_ptr++;
    contents->count |= *pdu_ptr;
    pdu_ptr++;

    // Data
    memcpy(contents->data.msg, pdu_ptr, pdu->N_bytes-2);
    contents->data.N_bytes = pdu->N_bytes-2;

    return LIBLTE_SUCCESS;
}

/*********************************************************************
    PDU Type: User Plane PDCP Data PDU with short PDCP SN

    Document Reference: 36.323 v10.1.0 Section 6.2.4
*********************************************************************/
LIBLTE_ERROR_ENUM liblte_pdcp_pack_data_pdu_with_short_sn(LIBLTE_PDCP_DATA_PDU_WITH_SHORT_SN_STRUCT *contents,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *pdu)
{
    return liblte_pdcp_pack_data_pdu_with_short_sn(contents, &contents->data, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_data_pdu_with_short_sn(LIBLTE_PDCP_DATA_PDU_WITH_SHORT_SN_STRUCT *contents,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *data,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *pdu)
{
    if(contents == NULL || data == NULL || pdu == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    *pdu_ptr = (LIBLTE_PDCP_D_C_DATA_PDU << 7) | (contents->count & 0x7F);
    pdu_ptr++;

    // Data
    memcpy(pdu_ptr, data->msg, data->N_bytes);
    pdu_ptr += data->N_bytes;

    // Fill in the number of bytes used
    pdu->N_bytes = pdu_ptr - pdu->msg;

    return LIBLTE_SUCCESS;
}
LIBLTE_ERROR_ENUM liblte_pdcp_unpack_data_pdu_with_short_sn(LIBLTE_BYTE_MSG_STRUCT                    *pdu,
                                                            LIBLTE_PDCP_DATA_PDU_WITH_SHORT_SN_STRUCT *contents)
{
    if(pdu == NULL || contents == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    if(LIBLTE_PDCP_D_C_DATA_PDU != (LIBLTE_PDCP_D_C_ENUM)((*pdu_ptr >> 7) & 0x01))
        return LIBLTE_ERROR_INVALID_INPUTS;
    contents->count = *pdu_ptr & 0x7F;
    pdu_ptr++;

    // Data
    memcpy(contents->data.msg, pdu_ptr, pdu->N_bytes-1);
    contents->data.N_bytes = pdu->N_bytes-1;

    return LIBLTE_SUCCESS;
}

/*********************************************************************
    PDU Type: PDCP Control PDU for interspersed ROHC feedback packet

    Document Reference: 36.323 v10.1.0 Section 6.2.5
*********************************************************************/
LIBLTE_ERROR_ENUM liblte_pdcp_pack_control_pdu_for_interspersed_rohc_feedback(LIBLTE_PDCP_CONTROL_PDU_FOR_INTERSPERSED_ROHC_FEEDBACK_STRUCT *contents,
                                                                              LIBLTE_BYTE_MSG_STRUCT                                        *pdu)
{
    return liblte_pdcp_pack_control_pdu_for_interspersed_rohc_feedback(contents, &contents->rohc_feedback_packet, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_control_pdu_for_interspersed_rohc_feedback(LIBLTE_PDCP_CONTROL_PDU_FOR_INTERSPERSED_ROHC_FEEDBACK_STRUCT *contents,
                                                                              LIBLTE_BYTE_MSG_STRUCT                                        *rohc_feedback_packet,
                                                                              LIBLTE_BYTE_MSG_STRUCT                                        *pdu)
{
    if(contents == NULL || rohc_feedback_packet == NULL || pdu == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    *pdu_ptr = (LIBLTE_PDCP_D_C_CONTROL_PDU << 7) | ((contents->pdu_type << 4) & 0x70);
    pdu_ptr++;

    // Interspersed ROHC feedback packet
    memcpy(pdu_ptr, rohc_feedback_packet->msg, rohc_feedback_packet->N_bytes);
    pdu_ptr += rohc_feedback_packet->N_bytes;

    // Fill in the number of bytes used
    pdu->N_bytes = pdu_ptr - pdu->msg;

    return LIBLTE_SUCCESS;
}
LIBLTE_ERROR_ENUM liblte_pdcp_unpack_control_pdu_for_interspersed_rohc_feedback(LIBLTE_BYTE_MSG_STRUCT                                        *pdu,
                                                                                LIBLTE_PDCP_CONTROL_PDU_FOR_INTERSPERSED_ROHC_FEEDBACK_STRUCT *contents)
{
    if(pdu == NULL || contents == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    if(LIBLTE_PDCP_D_C_CONTROL_PDU != (LIBLTE_PDCP_D_C_ENUM)((*pdu_ptr >> 7) & 0x01))
        return LIBLTE_ERROR_INVALID_INPUTS;
    contents->pdu_type = (*pdu_ptr >> 4) & 0x03;
    pdu_ptr++;

    // Interspersed ROHC feedback packet
    memcpy(contents->rohc_feedback_packet.msg, pdu_ptr, pdu->N_bytes-1);
    contents->rohc_feedback_packet.N_bytes = pdu->N_bytes-1;

    return LIBLTE_SUCCESS;
}

/*********************************************************************
    PDU Type: PDCP Control PDU for PDCP status report

    Document Reference: 36.323 v10.1.0 Section 6.2.6
*********************************************************************/
// FIXME

/*********************************************************************
    PDU Type: RN User Plane PDCP Data PDU with integrity protection

    Document Reference: 36.323 v10.1.0 Section 6.2.8
*********************************************************************/
LIBLTE_ERROR_ENUM liblte_pdcp_pack_rn_user_plane_data_pdu(LIBLTE_PDCP_RN_USER_PLANE_DATA_PDU_STRUCT *contents,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *pdu)
{
    return liblte_pdcp_pack_rn_user_plane_data_pdu(contents, &contents->data, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_rn_user_plane_data_pdu(LIBLTE_PDCP_RN_USER_PLANE_DATA_PDU_STRUCT *contents,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *data,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *pdu)
{
    return liblte_pdcp_pack_rn_user_plane_data_pdu(contents, data, NULL, 0, 0, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_rn_user_plane_data_pdu(LIBLTE_PDCP_RN_USER_PLANE_DATA_PDU_STRUCT *contents,
                                                          uint8                                     *key_256,
                                                          uint8                                      direction,
                                                          uint8                                      rb_id,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *pdu)
{
    return liblte_pdcp_pack_rn_user_plane_data_pdu(contents, &contents->data, key_256, direction, rb_id, pdu);
}
LIBLTE_ERROR_ENUM liblte_pdcp_pack_rn_user_plane_data_pdu(LIBLTE_PDCP_RN_USER_PLANE_DATA_PDU_STRUCT *contents,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *data,
                                                          uint8                                     *key_256,
                                                          uint8                                      direction,
                                                          uint8                                      rb_id,
                                                          LIBLTE_BYTE_MSG_STRUCT                    *pdu)
{
    if(contents == NULL || data == NULL || pdu == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    *pdu_ptr = (LIBLTE_PDCP_D_C_DATA_PDU << 7) | ((contents->count >> 8) & 0x0F);
    pdu_ptr++;
    *pdu_ptr = contents->count & 0xFF;
    pdu_ptr++;

    // Data
    memcpy(pdu_ptr, data->msg, data->N_bytes);
    pdu_ptr += data->N_bytes;

    // MAC
    if(NULL == key_256)
    {
        *pdu_ptr = (LIBLTE_PDCP_CONTROL_MAC_I >> 24) & 0xFF;
        pdu_ptr++;
        *pdu_ptr = (LIBLTE_PDCP_CONTROL_MAC_I >> 16) & 0xFF;
        pdu_ptr++;
        *pdu_ptr = (LIBLTE_PDCP_CONTROL_MAC_I >> 8) & 0xFF;
        pdu_ptr++;
        *pdu_ptr = LIBLTE_PDCP_CONTROL_MAC_I & 0xFF;
        pdu_ptr++;
    }else{
        pdu->N_bytes = pdu_ptr - pdu->msg;
        liblte_security_128_eia2(&key_256[16],
                                 contents->count,
                                 rb_id,
                                 direction,
                                 pdu->msg,
                                 pdu->N_bytes,
                                 pdu_ptr);
        pdu_ptr += 4;
    }

    // Fill in the number of bytes used
    pdu->N_bytes = pdu_ptr - pdu->msg;

    return LIBLTE_SUCCESS;
}
LIBLTE_ERROR_ENUM liblte_pdcp_unpack_rn_user_plane_data_pdu(LIBLTE_BYTE_MSG_STRUCT                    *pdu,
                                                            LIBLTE_PDCP_RN_USER_PLANE_DATA_PDU_STRUCT *contents)
{
    if(pdu == NULL || contents == NULL)
        return LIBLTE_ERROR_INVALID_INPUTS;

    uint8 *pdu_ptr = pdu->msg;

    // Header
    if(LIBLTE_PDCP_D_C_DATA_PDU != (LIBLTE_PDCP_D_C_ENUM)((*pdu_ptr >> 7) & 0x01))
        return LIBLTE_ERROR_INVALID_INPUTS;
    contents->count = (*pdu_ptr & 0x0F) << 8;
    pdu_ptr++;
    contents->count |= *pdu_ptr;
    pdu_ptr++;

    // Data
    memcpy(contents->data.msg, pdu_ptr, pdu->N_bytes-6);
    contents->data.N_bytes = pdu->N_bytes-6;

    return LIBLTE_SUCCESS;
}
