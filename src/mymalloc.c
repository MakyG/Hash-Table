#include "mymalloc.h"
#include <stdio.h>
/**
 * \file mymalloc.c
 * \brief Modul mymalloc obsahuje funkce pro ladění pridělování paměti
 *
 * Podrobnější dokumentaci najdete v souboru mymalloc.h.
 */

/* Pozn.: vnitřnosti modulu mymalloc záměrně nejsou dokumentovány ve formátu
 * doxygen, aby zbytečně nezaplevelovaly výslednou dokumentaci a nemátly vás
 */
#define HASH_TABLE_SIZE 101 /* počet položek pole hash, musí být prvočislo */
typedef void *tHTableKey;   /* typ vyhledávacího klice = ukazatel */
typedef unsigned int tHTableData;  /* typ dat uloženych pod hledanym klicem =
                                      velikost alokované pameti*/
typedef unsigned int tHTableIndex; /* index do vnitřního pole hash tabulky */
typedef unsigned long int tDword;
#define compEQ(a, b) (a == b) /* funkce pro porovnani 2 klíčů */

typedef struct tHTableNode_s {
  tHTableKey key;             /* vyhledávací klíč */
  tHTableData data;           /* data */
  struct tHTableNode_s *next; /* next node */
} tHTableNode;

tHTableNode *hashTable[HASH_TABLE_SIZE]; /* jednoduchá hashovaci tabulka=pole
                                            ukazatelu na uzel */

/***************************************************************************
 *  funkce hash převádí klíč na index do pole hash tabulky
 ***************************************************************************/
tHTableIndex hashFn(tHTableKey key) { return ((tDword)key % HASH_TABLE_SIZE); }

/***************************************************************************
 * funkce insertHTableNode vytvoří nový uzel se zadaným klíčem a daty a vloží
 *jej do tabulky na první místo lin. seznamu synonym
 ***************************************************************************/
tHTableNode *insertHTableNode(tHTableKey key, tHTableData data) {
  tHTableNode *first, *ukTmp;
  tHTableIndex index;
  index = hashFn(key); /* hash fce vrátí index do pole hash tab. */

  /* nyní vytvoříme v dyn. paměti nový uzel */
  if ((first = malloc(sizeof(tHTableNode))) == NULL) {
    fprintf(stderr, "out of memory (insertHTableNode)\n");
    exit(1); /* pokud se alokace nepodařila, konec */
  }

  /* Zde uděláme klasický list_insert_first jako v lineárním seznamu */
  ukTmp =
      hashTable[index]; /* schováme si ukazatel na začátek seznamu synonym */
  hashTable[index] = first; /* a přepíšeme jej ukazatelem na nový 1. uzel */
  first->next = ukTmp;      /* na nový první uzel navážeme starý 1. uzel */
  first->key = key;         /* a naplníme klíč a data hodnotami */
  first->data = data;
  return first;
}

/***************************************************************************
 * Funkce deleteNode smaže zadaný klíč z tabulky a vrátí jeho hodnotu.
 * Klíč je potřeba nejdřív najít v lin. seznamu synonym, nalezený uzel pak
 * smazat. Operace je ekvivalentní list_delete_next v lineárním seznamu.
 ***************************************************************************/

tHTableData deleteNode(tHTableKey key) {
  tHTableNode *ukAct, *ukPrev = NULL;
  tHTableIndex index;
  tHTableData data;
  /* nejdříve spočítáme index, kde začíná lin. seznam synonym */
  index = hashFn(key);
  ukAct = hashTable[index];

  while (ukAct && !compEQ(ukAct->key, key)) {
    ukPrev = ukAct;
    ukAct = ukAct->next;
  }

  if (ukAct == NULL) {
    return -1;
  }

  if (ukPrev == NULL) /* mažeme první uzel seznamu synonym ? */
  {
    hashTable[index] = ukAct->next;
  }    /* ano: nastav ukazatel na první prvek na další za mazaným */
  else /* ne: propoj prvek před mazaným s prvkem za mazaným */
  {
    ukPrev->next = ukAct->next;
  }

  data = ukAct->data;
  free(ukAct);
  return data;
}

/*******************************
 *  find node containing key  *
 *******************************/

tHTableData findNode(tHTableKey key) {
  tHTableNode *uk;
  uk = hashTable[hashFn(key)];

  while (uk && !compEQ(uk->key, key)) {
    uk = uk->next;
  }

  return uk->data;
}

tHTableData *findNodeDataRef(tHTableKey key) {
  tHTableNode *uk;
  uk = hashTable[hashFn(key)];

  while (uk && !compEQ(uk->key, key)) {
    uk = uk->next;
  }

  return &uk->data;
}

/****************************************************************************************
 * Funkce myMalloc
 * hash tabulka s jednoduchou hash funkci typu "modulo"
 *****************************************************************************************/

long int alokaceCelkem = 0;
void *myMalloc(long size) {
  void *tmpUk = malloc(size);
#ifdef DEBUG

  if (tmpUk != NULL) {
    insertHTableNode(tmpUk, size);
    alokaceCelkem += size;
    printf("myMalloc: allocating %ld bytes, memory allocated %ld bytes\n", size,
           alokaceCelkem);
  }

#endif
  return tmpUk;
}

void myFree(void *memblock) {
#ifdef DEBUG
  long size = 0;

  if (memblock != NULL) {
    size = deleteNode(memblock);
    alokaceCelkem -= size;
    printf("myFree: releasing %ld bytes, memory allocated %ld bytes\n", size,
           alokaceCelkem);
  }

#endif
  free(memblock);
}

void *myRealloc(void *ptr, size_t newSize) {
  void *tPtr = realloc(ptr, newSize);
#ifdef DEBUG

  // ukazatel se nezmenil. Nova pamet se vlezla do volneho mista za nebo doslo
  // ke zmenseni pameti
  if (tPtr == ptr) {
    tHTableData *size = findNodeDataRef(ptr);
    alokaceCelkem += newSize - *size;

    if (*size > newSize) {
      printf("myRealloc: releasing %ld bytes, memory allocated %ld bytes\n",
             (long unsigned int)(*size - newSize),
             (long unsigned int)alokaceCelkem);
    } else {
      printf("myRealloc: allocating %ld bytes, memory allocated %ld bytes\n",
             (long unsigned int)(newSize - *size),
             (long unsigned int)alokaceCelkem);
    }

    *size = newSize;
  } else if (ptr == NULL &&
             tPtr != NULL) {  // kdyz je realloc volan s null na vstupu
    insertHTableNode(tPtr, newSize);
    alokaceCelkem += newSize;
    printf("myRealloc: allocating %ld bytes, memory allocated %ld bytes\n",
           (long unsigned int)newSize, (long unsigned int)alokaceCelkem);
  } else if (tPtr != NULL) {  // uspesne alokovano nove misto
    long size = newSize - deleteNode(ptr);
    insertHTableNode(tPtr, newSize);
    alokaceCelkem += size;
    printf("myRealloc: allocating %ld bytes, memory allocated %ld bytes\n",
           (long unsigned int)size, (long unsigned int)alokaceCelkem);
  }

#endif
  return tPtr;
}
