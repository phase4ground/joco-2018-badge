/*****************************************************************************
 * (C) Copyright 2017 
 *
 * PROPRIETARY AND CONFIDENTIAL UNTIL FEBRUARY 26TH, 2018 then,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributors:
 * 	@sconklin
 *****************************************************************************/
#include "../system.h"

//#defines
#define MAX_REC_COUNT      1     /**< Maximum records count. */

//Local nfc data
uint8_t m_ndef_msg_buf[32];

/**
 * @brief Function for creating a record in English.
 */
static void en_record_add(nfc_ndef_msg_desc_t * p_ndef_msg_desc)
{
    uint32_t             err_code;
    static const uint8_t en_payload[] =
                  {'J', 'o', 'c', 'o', ' ', '2', '0', '1', '8', '!'};
    static const uint8_t en_code[] = {'e', 'n'};

    NFC_NDEF_TEXT_RECORD_DESC_DEF(en_text_rec,
                                  UTF_8,
                                  en_code,
                                  sizeof(en_code),
                                  en_payload,
                                  sizeof(en_payload));

    err_code = nfc_ndef_msg_record_add(p_ndef_msg_desc,
                                       &NFC_NDEF_TEXT_RECORD_DESC(en_text_rec));
    APP_ERROR_CHECK(err_code);
}


static void welcome_msg_encode(uint8_t * p_buffer, uint32_t * p_len)
{
    NFC_NDEF_MSG_DEF(welcome_msg, MAX_REC_COUNT);

    en_record_add(&NFC_NDEF_MSG(welcome_msg));
 
     uint32_t err_code = nfc_ndef_msg_encode(&NFC_NDEF_MSG(welcome_msg),
                                            p_buffer,
                                            p_len);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the Application's NFC events.
 *
 * @details This function will be called for NFC events which are passed to the application.
 *
 * @param[in] TODO_FILL_THIS_IN  Advertising event.
 */
static void nfc_callback(void * p_context, nfc_t2t_event_t event, const uint8_t * p_data, size_t data_length) {

	switch (event) {
	case HAL_NFC_EVENT_FIELD_ON:
		break; // HAL_NFC_EVENT_FIELD_ON
	case HAL_NFC_EVENT_FIELD_OFF:
		break; // HAL_NFC_EVENT_FIELD_OFF
	case HAL_NFC_EVENT_DATA_RECEIVED:
		break; // HAL_NFC_EVENT_DATA_RECEIVED
	case HAL_NFC_EVENT_DATA_TRANSMITTED:
		break; // HAL_NFC_EVENT_DATA_TRANSMITTED
	default:
		break;
	}
}

/**@brief Function for Setting up NFC
 *
 * @param[in] TBD_CHANGE_THIS
 */
/**@brief Function for starting advertising.
 */
void util_nfc_start() {
	uint32_t err_code;

	err_code = hal_nfc_start();
	APP_ERROR_CHECK(err_code);
}

void util_nfc_init() {
        uint32_t  len = sizeof(m_ndef_msg_buf);
	uint32_t err_code;

        err_code = nfc_t2t_setup(nfc_callback, NULL);
	APP_ERROR_CHECK(err_code);

        /* Encode welcome message */
        welcome_msg_encode(m_ndef_msg_buf, &len);

        /* Set created message as the NFC payload */
        err_code = nfc_t2t_payload_set(m_ndef_msg_buf, len);
        APP_ERROR_CHECK(err_code);

        err_code = nfc_t2t_emulation_start();
        APP_ERROR_CHECK(err_code);
}
