#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

/**
 * \file mymalloc.h
 * \brief Hlavičkový soubor modulu mymalloc (funkce pro ladění přidělování
 *paměti)
 *
 * Knihovna MYMALLOC slouží k ladění přidělování paměti. Pro ladění potřebujeme
 * vést záznam o každém přiděleném bloku dynamické paměti tak, abychom při jeho
 * uvolňování věděli, jak je velký.\n\n
 * Při každém přidělení paměti pak vypíšeme informaci o velikosti právě
 *přidělované paměti a o celkové přidělené paměti, to samé budeme vypisovat i
 *při uvolňování. Záznamy o přidělených blocích (= ukazatel x velikost bloku, na
 *který ukazuje) budeme ukládat do hashovací tabulky.\n\n Hash tabulka má
 *nejjednodušší možnou implementace - tj. explicitní zřetěžení synonym do
 *lineárního seznamu, jednoduchá hash funkce typu "modulo"
 *******************************************************************************/

#include <stdlib.h>

/** \def DEBUG
 * \brief Zapíná/vypíná výpis ladících informací
 *
 *	Pokud je symbol DEBUG definován, jsou vypisovány ladící informace při
 *přidělování i uvolňování paměti
 */
//#define DEBUG

/************************************************************************/
/** \fn void * myMalloc(size_t size)
 * \brief Alokuje paměť o zadané velikosti.
 * \param size - počet bajtů, které se mají alokovat
 *
 *	Pokud není definován symbol #DEBUG, pak se pouze provede volání fce
 *malloc\n. Pokud #DEBUG je definován, provede volání malloc a získaný ukazatel
 *a velikost přidělené paměti uloží do hash tabulky, přičemž ukazatel bude
 *uložen jako klíč - pro snadné vyhledání při pozdějším uvolňování paměti.\n
 *  Nakonec zvýší a vypíše hodnotu proměnné s celkovou alokovanou pamětí.
 */
void *myMalloc(long size);

/************************************************************************/
/** \fn void myFree(void *memblock)
 * \brief Uvolní alokovanou paměť.
 * \param memblock - ukazatel na paměť, která se má uvolnit
 *
 *	Pokud není definován symbol #DEBUG, pak se pouze provede volání fce
 *free\n. Pokud #DEBUG je definován, použije zadaný ukazatel pro vyhledání
 *velikosti přidělené paměti v hash tabulce, a tuto velikost odečte od proměnné,
 *  ve které je uložena celková velikost alokované paměti.
 */
void myFree(void *memblock);

void *myRealloc(void *ptr, size_t newSize);

#endif  //_MYMALLOC_H_
