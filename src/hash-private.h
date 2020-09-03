#ifndef HASH_PRIVATE_H
#define HASH_PRIVATE_H

#include "data.h"
#include "table.h"

/*!
 * @brief Vypočte index v tabulce na základě hodnoty zadaného klíče.
 * Index je vypočítán jako součet hodnot bytů v klíči modulo rozměr tabulky
 * kde každý byte v hodnotě klíče je navíc rotován o počet bitů daných jeho
 * pozicí, tj. byte na pozici 0 o 0 bitů, byte na pozici 1 o 1 bit, atd. Počet
 * bytů v hodnotě klíče je sizeof(T) kde T je datový typ klíče pro všechny typy
 * kromě vtSTRING, nebo strlen(data) pro vtSTRING.
 * @param table Ukazatel na tabulku
 * @param key Klíč
 * @return Vypočtený index
 */
unsigned int hash(HashTable* table, Data_t* key);

#endif
