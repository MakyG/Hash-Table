/**
 * @file       table.h
 * @author     Ondřej Ševčík
 * @date       6/2019
 * @brief      Header file for Hashtable.
 * **********************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

#ifndef _TABLE_H_
#define _TABLE_H_

/* Public includes -------------------------------------------------------- */
#include <stdbool.h>
#include <stddef.h>

#include "data.h"

/*!
 * @brief Item of hashtable
 */
typedef struct _HashTableNode {
  Data_t *key;                 /**< Key of an item */
  Data_t *value;               /**< Value of an item */
  struct _HashTableNode *next; /**< Pointer at next item with the same index */
} HashTableNode;

/*!
 * @brief Hashtable stucture
 */
typedef struct _HashTable {
  bool take_ownership; /**< If true, de-initialization tries to free the data */
  HashTableNode **buckets; /**< Items of a table (Array of pointers on items =
                              items with same hash) */
  size_t size;             /**< Size (Number of items) of an bucket array */
  size_t count;            /**< Number of items saved in hash-table */

} HashTable;

/*!
 * @brief Callback function for processing of hash-table items.
 * @param key Item's key
 * @param value Item's value
 */
typedef void (*TableNodeProc)(Data_t *key, Data_t *value);

/* Public HashTable API ---------------------------------------------------- */

/*!
 * @brief Initialize the table (creates array of pointes on items of the table)
 * and sets the table's size and the count of items. Sets every bucket to NULL
 * @param[out] table Pointer at initialized hash-table.
 * @param[in] size Our desired size of table
 * @param[in] takeOwnership If the argument takeOwnership is true, the value of
 * item will be deleted as well
 * @return Returns true if tree was initialized
 */
bool HashTable_Init(HashTable *table, size_t size, bool takeOwnership);

/*!
 * @brief Destruct the table(Free's the pointer array at nodes and sets the size
 * to NULL)
 * @param[in] table Pointer at table we want to destruct
 */
void HashTable_Destruct(HashTable *table);

/*!
 * @brief Creates a new item and puts it into table at the start of counted
 * index with hash() function. If there's an duplicate key, the insert won't
 * happen. Keys are unique
 * @param[in] table	Pointer at table
 * @param[in] key Pointer at item's key
 * @param[in] value Pointer at values that are inserted
 * @return Returns true if item was inserted, return false otherwise
 */
bool HashTable_Insert(HashTable *table, Data_t *key, Data_t *value);

/*!
 * @brief Replaces item value on key's position. If takeOwnership is true, the
 * original value is freed.
 * @param[in] table	Pointer at table
 * @param[in] key Pointer at item's key
 * @param[in] value Pointer at value that is item replaced with
 * @return Returns true if the item's data were REPLACED, otherwise return false
 */
bool HashTable_Replace(HashTable *table, Data_t *key, Data_t *value);

/*!
 * @brief Deletes an item that is defined with key. If there's no such an item,
 * nothings happens. if takeOwnership is true, remove the data from item as
 * well. To remove data use Data_Destruct function defined in data.h
 * @param[in] table Pointer at table
 * @param[in] key Pointer at item's key
 * @return Return true if the item was deleted, return false otherwise
 */
bool HashTable_Delete(HashTable *table, Data_t *key);

/*!
 * @brief Returns the data from an item that is identified with key.
 * @param[in] table Pointer at table
 * @param[in] key Pointer at item's key
 * @return Returns the aress of item's value according to given key, returns
 * NULL otherwise.
 */
Data_t *HashTable_Find(HashTable *table, Data_t *key);

/*!
 * @brief Returns the number of items in table
 * @param[in] table Pointer at table
 * @return Returns number of items in table
 */
size_t HashTable_Get_Count(HashTable *table);

/*!
 * @brief Returns every item from table. If takeOwnership is true, remove data.
 * To remove data use Data_Destruct function defined in data.h as well.
 * @param[in] table Pointer at table
 */
void HashTable_Clear(HashTable *table);

/*!
 * @brief Process every item in a table with 'proc' function.
 * @param[in] table Pointer at table
 * @param[in] proc Pointer at callback function TableNodeProc
 */
void HashTable_Process(HashTable *table, TableNodeProc proc);

#endif  //_TABLE_H_
