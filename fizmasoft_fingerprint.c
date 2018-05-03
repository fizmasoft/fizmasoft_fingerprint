/*
 * fizmasoft_fingerprint.c
 *
 * Created: Tue 01.05.18 9:42:36 AM
 * Authors: Azamjon Rakhimjonov, Sherzodbek Tojimahammatov, Elbek Khoshimjonov
 */ 

#include "fizmasoft_fingerprint.h"

uint8_t _ffp_verify_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x07, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B};
unsigned int _ffp_verify_data_length = 16;
unsigned int _ffp_verify_resp_length = 12;

uint8_t _ffp_getImage_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
unsigned int _ffp_getImage_data_length = 12;
unsigned int _ffp_getImage_resp_length = 12;

uint8_t _ffp_img2Tz_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08};
unsigned int _ffp_img2Tz_data_length = 13;
unsigned int _ffp_img2Tz_resp_length = 12;

uint8_t _ffp_search_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x04, 0x01, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xB1};
unsigned int _ffp_search_data_length = 17;
unsigned int _ffp_search_resp_length = 16;

uint8_t _ffp_empty_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x0D, 0x00, 0x11};
unsigned int _ffp_empty_data_length = 12;
unsigned int _ffp_empty_resp_length = 12;

uint8_t _ffp_match_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x03, 0x00, 0x07};
unsigned int _ffp_match_data_length = 12;
unsigned int _ffp_match_resp_length = 12;

uint8_t _ffp_store_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x01, 0x00, 0x0F};
unsigned int _ffp_store_data_length = 15;
unsigned int _ffp_store_resp_length = 12;

uint8_t _ffp_createModel_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09};
unsigned int _ffp_createModel_data_length = 12;
unsigned int _ffp_createModel_resp_length = 12;

uint8_t _ffp_count_data[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x1D, 0x00, 0x21};
unsigned int _ffp_count_data_length = 12;
unsigned int _ffp_count_resp_length = 14;

ffp *ffp_init(char*name, unsigned int baudrate){
	if (baudrate == 0)
		baudrate = 57600;
	ffp *ff;
	ff = malloc(sizeof(ffp));
	_delay_ms(1000);
	sei();
	ff->uart_f = uart_init(name, UART_BAUD_SELECT(baudrate, F_CPU));
	return ff;
}

uint8_t ffp_verifyPassword(ffp *ff) {
	ffp_send(ff, _ffp_verify_data, _ffp_verify_data_length);
	ffp_get(ff, _ffp_verify_resp_length);
	_delay_ms(100);
	return ff->response[9];
}

uint8_t ffp_getImage(ffp *ff) {
	ffp_send(ff, _ffp_getImage_data, _ffp_getImage_data_length);
	ffp_get(ff, _ffp_getImage_resp_length);
	return ff->response[9];
}

uint8_t ffp_image2Tz(ffp *ff, uint8_t charBufferID) {
	_ffp_img2Tz_data[10] = charBufferID;
	if (charBufferID == FFP_CHAR_BUFFER_1)
		_ffp_img2Tz_data[12] = 0x08;
	else
		_ffp_img2Tz_data[12] = 0x09;
	ffp_send(ff, _ffp_img2Tz_data, _ffp_img2Tz_data_length);
	ffp_get(ff, _ffp_img2Tz_resp_length);
	return ff->response[9];
}

uint8_t ffp_search(ffp *ff) {
	ffp_send(ff, _ffp_search_data, _ffp_search_data_length);
	ffp_get(ff, _ffp_search_resp_length);
	ff->uid = ((uint16_t)ff->response[10] << 8) | ff->response[11];
	ff->matchScore = ((uint16_t)ff->response[12] << 8) | ff->response[13];
	return ff->response[9];
}

uint8_t ffp_emptyDatabase(ffp *ff) {
	ffp_send(ff, _ffp_empty_data, _ffp_empty_data_length);
	ffp_get(ff, _ffp_empty_resp_length);
	return ff->response[9];
}

uint8_t ffp_match(ffp *ff) {
	ffp_send(ff, _ffp_match_data, _ffp_match_data_length);
	ffp_get(ff, _ffp_match_resp_length);
	ff->matchScore = ((uint16_t)ff->response[10] << 8) | ff->response[11];
	return ff->response[9];
} // 10

uint8_t ffp_store(ffp *ff, uint16_t uid) {
	_ffp_store_data[12] = (uint8_t)uid;
	_ffp_store_data[11] = (uint8_t)(uid >> 8);
	uint16_t cs = 0x000E + uid;
	_ffp_store_data[14] = (uint8_t)cs; 
	_ffp_store_data[13] = (uint8_t)(cs >> 8);
	ffp_send(ff, _ffp_store_data, _ffp_store_data_length);
	ffp_get(ff, _ffp_store_resp_length);
	return ff->response[9];
}

uint8_t ffp_creatModel(ffp *ff) {
	ffp_send(ff, _ffp_createModel_data, _ffp_createModel_data_length);
	ffp_get(ff, _ffp_createModel_resp_length);
	return ff->response[9];
}

uint8_t ffp_countTemplates(ffp *ff) {
	ffp_send(ff, _ffp_count_data, _ffp_count_data_length);
	ffp_get(ff, _ffp_count_resp_length);
	ff->numberOfFingers = ((uint16_t)ff->response[10] << 8) | ff->response[11];
	return ff->response[9];
}

void ffp_send(ffp *ff, uint8_t *data, unsigned int length) {
	for (unsigned int i = 0; i < length; i++)
		uart_putc(ff->uart_f, data[i]);
}

void ffp_get(ffp *ff, unsigned int length) {
	uint16_t d;
	for (unsigned int i = 0; i < length;)
	{
		d = uart_getc(ff->uart_f);
		if (d != UART_NO_DATA)
			ff->response[i++] = (uint8_t)d;
		else
		_delay_ms(1);
	}
}