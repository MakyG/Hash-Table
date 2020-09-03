/**
 * @file       table.c
 * @author     Tomáš Makyča
 * @date       5/2020
 * @brief      Implementation of functions for HashTable.
 * **************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

/* Private includes -------------------------------------------------------- */
#include "table.h"
#include <stdio.h>
#include <string.h>
#include "hash-private.h"
#include "mymalloc.h"

bool HashTable_Init(HashTable *table, size_t size, bool takeOwnership) {
    if(table == NULL || size == 0)
        return false;

    size_t count = sizeof(HashTableNode*) * size;

    table->buckets = myMalloc(count);

    if(table->buckets == NULL)
        return false;

    table->size = size;
    table->count = 0;
    table->take_ownership = takeOwnership;

    memset(table->buckets, 0, count);
    return true;
}

void HashTable_Destruct(HashTable *table) {
    if(!table)
        return;

    for(unsigned int i = 0; i < table->size; i++){
        HashTableNode *newBucket = table->buckets[i];

        if(table->buckets[i] == NULL)
            continue;
        while(newBucket){
            if(table->take_ownership){
                Data_Destruct(newBucket->key);
                Data_Destruct(newBucket->value);
            }
            HashTableNode *tempPtrNext = newBucket->next;
            myFree(newBucket);
            newBucket = tempPtrNext;
        }
        myFree(table->buckets[i]);
        table->buckets[i] = NULL;
        table->count--;
    }
    myFree(table->buckets);
    table->buckets = NULL;
}

bool HashTable_Insert(HashTable *table, Data_t *key, Data_t *value) {
    if (!table || !key)
        return false;

    unsigned int i = hash(table, key);

    if (HashTable_Find(table, key))
        return false;
    printf("hash: Key: ");
        puts(Data_To_String(key, NULL));
        printf(" -> Hash index: %u\n", i);
    HashTableNode *node = myMalloc(sizeof(HashTableNode));

    if (node == NULL)
        return false;

    node->value = value;
    node->key = key;
    node->next = NULL;

    node->next = table->buckets[i];
    table->buckets[i] = node;

    table->count++;
    return true;
}

bool HashTable_Replace(HashTable *table, Data_t *key, Data_t *value) {
    if(!table || !key || !value)
        return false;

    unsigned int i = hash(table, key);

    if (!HashTable_Find(table, key))
        return false;

    if(!table->buckets[i])
        return false;

    if(table->take_ownership)
        myFree(table->buckets[i]->value);

    table->buckets[i]->value = value;
    return true;
}

bool HashTable_Delete(HashTable *table, Data_t *key) {
    if(!table || !key)
        return false;
    unsigned int i = hash(table, key);

    if (!HashTable_Find(table, key))
        return false;

    HashTableNode *node = table->buckets[i];
    HashTableNode *prevNode = NULL;
    HashTableNode *newNode = NULL;
    while (node) {
        if (Data_Cmp(node->key, key) == 0) {
            newNode = node;
            break;
        }
        prevNode = node;
        node = node->next;
    }
    if (!newNode)
        return false;

    if(table->take_ownership){
        Data_Destruct(newNode->value);
        Data_Destruct(newNode->key);
    }
    if (prevNode)
        prevNode->next = newNode->next;
    else {
        table->buckets[i] = newNode->next;
    }
    myFree(newNode);
    table->count--;
    return true;
}
Data_t *HashTable_Find(HashTable *table, Data_t *key) {
    if (table == NULL || key == NULL)
        return NULL;

    unsigned int index = hash(table, key);

    HashTableNode *node = table->buckets[index];

    while (node) {
        if (Data_Cmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

size_t HashTable_Get_Count(HashTable *table) {
    if(!table)
        return 0;
    return table->count;
}

void HashTable_Clear(HashTable *table) {
    if(!table)
        return;
    for(unsigned int i = 0; i < table->size; i++) {
        HashTableNode *newBucket = table->buckets[i];

        if(table->buckets[i] == NULL)
            continue;
        while(newBucket){
            if(table->take_ownership){
                Data_Destruct(newBucket->key);
                Data_Destruct(newBucket->value);
            }
            HashTableNode *tempPtr = newBucket;
            newBucket = newBucket->next;
            myFree(tempPtr);
        }
        table->buckets[i] = NULL;
        table->count--;
    }
}

void HashTable_Process(HashTable* table, TableNodeProc proc)
{
    HashTableNode* xnode;
    for(unsigned int i = 0; i < table->size; i++)	{

        xnode = table->buckets[i];

        while(xnode != NULL) {
            proc(xnode->key, xnode->value);
            xnode = xnode->next;
        }
    }
}
