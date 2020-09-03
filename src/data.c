/**
 * @file    data.c
 * @author  Tomas Dulik
 * @date    9/3/2015
 * @brief   Implementation of void abstract type functions
 * Jak vidíte níže, void nemá definován žádný konstruktor - funkci typu Init,
 * Create nebo Construct. void je totiž čistě abstraktní typ - nikdy nebudete
 * tvořit jeho instance.
 */

#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mymalloc.h"

int Data_Cmp(void *d1, void *d2) {
  Data_t *dd1 = d1, *dd2 = d2;  // typecasting aliases...
  return dd1->fncs->cmp(dd1, dd2);
}
unsigned Data_Hash(void *data) {
  Data_t *d = data;
  return d->fncs->hash(d);
}

char *Data_To_String(void *data, char *str) {
  Data_t *d = data;
  return d->fncs->to_string(d, str);
}

void Data_From_String(char *str, void *data) {
  Data_t *d = data;
  d->fncs->from_string(str, d);
}

char *Data_Get_Type(void *data) {
  Data_t *d = data;
  return d->fncs->get_type();
}

void Data_Destruct(void *data) {
  Data_t *d = data;
  if (d->fncs->destruct != NULL)
    d->fncs->destruct(data);
  else
    myFree(d);
}
