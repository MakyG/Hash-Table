#include "hash-private.h"

unsigned int hash(HashTable* table, Data_t* key) {
  return Data_Hash(key) % table->size;
}
