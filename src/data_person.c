/**
 * @file    data_person.c
 * @author  Tomas Dulik
 * @date    9/3/2015
 * @brief   Implementation of Data_Person_t specialization of Data_t abstract
 * type
 */

#include "data_person.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "mymalloc.h"

char *data_person_get_type() { return "Data_Person"; }

int data_person_cmp(void *d1, void *d2) {
  Data_Person_t *p1 = d1, *p2 = d2;  // just a "shortcut" - typecasting alias...
  int scmp = strcmp(p1->name, p2->name);

  if (scmp != 0) {
    return scmp;
  } else {
    int acmp = p1->age - p1->age;

    if (acmp != 0) {
      return acmp;  // you can continue with other comparison criteria..
    } else {
      return 0;
    }
  }
}

unsigned data_person_hash(void *data) {
  Data_Person_t *p = data;  // just a "shortcut" - typecasting alias...
  char *ptr = p->name;
  char *end =
      (char *)&(p->height) +
      sizeof(p->height);  // points right behind the end of the data structure
  unsigned result = 0;

  while (ptr < end) {  // the hash is a simple sum of all bytes of the structure
    result += *ptr++;
  }

  return result;
}

char *data_person_to_string(void *data, char *str) {
  Data_Person_t *d = data;

  if (str != NULL) {
    sprintf(str, "%.255s, %0.1lf, %0.1lf, %0.1lf", d->name, d->age, d->weight,
            d->height);
    return str;
  } else {
    return "data_person_to_string: error - null pointer. ";
  }
}
void data_person_from_string(char *str, void *data) {
  (void)str;
  (void)data;
  /**
   * TODO finish this!!
   */
}

static const Data_Functions_t data_person_fncs = {
  cmp : data_person_cmp,
  hash : data_person_hash,
  to_string : data_person_to_string,
  from_string : data_person_from_string,
  get_type : data_person_get_type,
  destruct :
      NULL  // no need for special destructor => Data_Destruct will call free()
};

void Data_Person_Init(Data_Person_t *data, char *str, double age, double weight,
                      double height) {
  assert(data != NULL);
  data->fncs = &data_person_fncs;

  if (str != NULL) {
    strncpy(data->name, str,
            255);  // trim the string to 255 char, d.cstring has only 256 bytes
    data->name[254] = '\0';  // if trimmed, insert the terminating 0.
  } else {
    data->name[0] = '\0';
  }

  data->age = age;
  data->height = height;
  data->weight = weight;
}

Data_Person_t *Data_Person_New(char *str, double age, double weight,
                               double height) {
  Data_Person_t *data = myMalloc(sizeof(Data_Person_t));
  Data_Person_Init(data, str, age, weight, height);
  return data;
}

Data_Person_t Data_Person_Create(char *str, double age, double weight,
                                 double height) {
  Data_Person_t new_data;
  Data_Person_Init(&new_data, str, age, weight, height);
  return new_data;
}
