/*
 * fizmasoft_fingerprint.h
 *
 * Created: Tue 01.05.18 12:34:41 PM
 * Authors: Azamjon Rakhimjonov, Sherzodbek Tojimahammatov, Elbek Khoshimjonov
 */ 
#ifndef fizmasoft_fingerprint_H
#define fizmasoft_fingerprint_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

#define FFP_CHAR_BUFFER_1 0x01
#define FFP_CHAR_BUFFER_2 0x02

#define FFP_OK 0x00
#define FFP_ERROR_RECEIVE_PACKET 0x01
#define FFP_ERROR_NO_FINGER 0x02
#define FFP_ERROR_FAIL_COLLECT_FINGER 0x03
#define FFP_ERROR_IMG2TZ_OVER_DISORDER 0x06
#define FFP_ERROR_IMG2TZ_LACKNESS 0x07
#define FFP_ERROR_MISSMATCH 0x08
#define FFP_ERROR_NOTFOUND 0x09
#define FFP_ERROR_COMBINE_FAIL 0x0A
#define FFP_ERROR_PAGEID_ADDRESS 0x0B
#define FFP_ERROR_READ_TEMPLATE 0x0C
#define FFP_ERROR_UPLOAD_TEMPLATE 0x0D
#define FFP_ERROR_WRONG_DATA_PACKAGE 0x0E
#define FFP_ERROR_UPLOAD_IMAGE 0x0F
#define FFP_ERROR_DELETE_TEMPLATE 0x10
#define FFP_ERROR_FAIL_CLEAR 0x11
#define FFP_ERROR_WRONG_PASSWORD 0x13
#define FFP_ERROR_LACKNESS_VALID 0x15
#define FFP_ERROR_WRITING_FLASH 0x18
#define FFP_ERROR_UNKNOWN 0x19
#define FFP_ERROR_INVALID_REGISTER 0x1A
#define FFP_ERROR_REGISTER_CONFIGURE 0x1B
#define FFP_ERROR_WRONG_NOTEPAD_PAGE_NUMBER 0x1C
#define FFP_ERROR_COMMUNICATION_PORT 0x1D

typedef struct _ffp
{
	uart_t *uart_f;
	uint8_t response[20];
	uint16_t matchScore;
	uint16_t uid;
	uint16_t numberOfFingers;
} ffp;

extern void ffp_send(ffp *ff, uint8_t *data, unsigned int length);
extern void ffp_get(ffp *ff, unsigned int length);

extern ffp *ffp_init(char*name, unsigned int baudrate);
extern uint8_t ffp_verifyPassword(ffp *ff);
extern uint8_t ffp_getImage(ffp *ff);
extern uint8_t ffp_image2Tz(ffp *ff, uint8_t charBufferID);
extern uint8_t ffp_search(ffp *ff);
extern uint8_t ffp_emptyDatabase(ffp *ff);
extern uint8_t ffp_match(ffp *ff);
extern uint8_t ffp_store(ffp *ff, uint16_t uid);
extern uint8_t ffp_creatModel(ffp *ff);
extern uint8_t ffp_countTemplates(ffp *ff);
#endif