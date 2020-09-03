/*
 * ioutils.h
 *
 *  Created on: 16.2.2012
 *      Author: dulik
 */

#ifndef _IOUTILS_H_
#define _IOUTILS_H_

void io_utils_get_string(char* line, int len);
char io_utils_get_char_line();
int io_utils_get_long(long* val);
int io_utils_get_double(double* val);
void io_utils_clear_stdin();

#endif  //_IOUTILS_H_
