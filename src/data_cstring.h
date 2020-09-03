/**
 * @file    data_cstring.h
 * @author  Tomas Dulik
 * @date    9/3/2015
 * @brief   Headers of Data_Cstring_t specialization of Data_t abstract type
 */

#ifndef DATA_CSTRING_H_
#define DATA_CSTRING_H_

#include "data.h"

/*!
 * @brief typ Data_Cstring_t je C-style řetězec s konstantní velikostí a Data_t
 * rozhraním
 */
typedef struct {
  const Data_Functions_t* fncs; /**< ukazatel na tabulku virtuálních funkcí */
  char cstring[256];            /**< pole typu char o konstatní velikosti */
} Data_Cstring_t;

void Data_Cstring_Init(Data_Cstring_t* d, char* str);

Data_Cstring_t* Data_Cstring_New(char* str);

Data_Cstring_t Data_Cstring_Create(char* str);

#endif /* DATA_VALUE_H_ */
