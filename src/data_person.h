/**
 * @file    data_person.h
 * @author  Tomas Dulik
 * @date    9/3/2015
 * @brief   Headers of Data_Person_t specialization of Data_t abstract type
 */

#ifndef DATA_PERSON_H_
#define DATA_PERSON_H_

#include "data.h"
/*!
 * \brief typ Data_Person_t je datový typ s konstantní velikostí a Data_t
 * rozhraním
 */
typedef struct {
  const Data_Functions_t* fncs; /**< ukazatel na tabulku virtuálních funkcí */
  char name[255];               /**< jméno člověka */
  double age, weight, height;   /**< další osobní data */
} Data_Person_t;

void Data_Person_Init(Data_Person_t* d, char* str, double age, double weight,
                      double height);

Data_Person_t* Data_Person_New(char* str, double age, double weight,
                               double height);

Data_Person_t Data_Person_Create(char* str, double age, double weight,
                                 double height);

#endif /* DATA_VALUE_H_ */
