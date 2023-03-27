#pragma once
#include "string.h"
#include "stdio.h"
#include "windows.h"
#include <stdint.h>
#define ERR_MSG_BUF 256
// Find the first occurrence of the byte string s in byte string l.
void *memmem(const void *l, size_t l_len, const void *s, size_t s_len);

// печатает как линуксовый xxd
void print_hex(const unsigned char *data, int size);

// По коду ошибки (из функции GetLastError) возвращает в msgbuf соответствующее текстовое представление
void get_error_string(DWORD err, CHAR msgbuf[]);

void print_guid(const uint8_t *uuid);

void print_hex_chars(const uint8_t *data, int data_l);

__int64 set_file_seek(HANDLE hf, __int64 distance, DWORD MoveMethod);

void print_gpt_header(const uint8_t *data);