/**
 * @file       tests.c
 * @author     Ond�ej �ev��k
 * @date       6/2019
 * @brief      Testing functions for a hashtable.
 * **********************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

/* Private includes -------------------------------------------------------- */
#include <inttypes.h>
#include <string.h>

#include "../src/data_cstring.h"
#include "../src/data_person.h"
#include "../src/hash-private.h"
#include "../src/mymalloc.h"
#include "../src/table.h"
#include "minunit.h"

////////////////////////////// IMPORTANT //////////////////////////////////////
/////////// Source repository: https://github.com/siu/minunit /////////////////
/////////// For more info consult with this page //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/* Private variables ------------------------------------------------------- */
static int gIndex = 0;
static Data_t * testDataArray[5];

/* Private function -------------------------------------------------------- */
void process_table_node(Data_t *key, Data_t *value) {
  testDataArray[gIndex] = key;
  gIndex++;
  (void)value;
}

MU_TEST(test_hashtable_init) {
  HashTable table;
  HashTable_Init(&table, 10, false);
  mu_assert_int_eq(10, (int)table.size);
  mu_assert_int_eq(0, (int)table.count);
  mu_assert(
      table.take_ownership == false,
      "In HashTable_Init takeownership should be false, was true instead.");

  for (int i = 0; i < 10; i++)
    mu_assert(table.buckets[i] == NULL,
              "Table is not initialized correctly, all buckets should be NULL");

  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_init_nulls) { HashTable_Init(NULL, 1000, false); }

MU_TEST(test_hashtable_insert) {
  HashTable table;
  HashTable_Init(&table, 10, true);
  char testBuffer[256] = {"Test Name"};
  void *testingKey = Data_Cstring_New("testKey");
  size_t i = hash(&table, testingKey);
  void *personDifferentTestValue = Data_Person_New(testBuffer, 12, 13, 15);

  mu_assert(HashTable_Insert(&table, testingKey, personDifferentTestValue),
            "HashTable_Insert expected true, got false instead.");
  mu_assert(table.buckets[i]->key == testingKey, "Key values were different!");
  mu_assert(table.buckets[i]->value == personDifferentTestValue,
            "Values were different!");

  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_insert_one_bucket) {
  HashTable table;
  HashTable_Init(&table, 10, true);
  char testBuffer[256] = {"Test Name"};
  void *testingKey = Data_Cstring_New("testKey");
  size_t i = hash(&table, testingKey);
  void *personDifferentTestValue = Data_Person_New(testBuffer, 12, 13, 15);

  void *personTestValue = Data_Person_New(testBuffer, 16, 17, 18);

  /* Inserting item with unique key and unique data ---------------------- */
  mu_assert(HashTable_Insert(&table, testingKey, personDifferentTestValue),
            "HashTable_Insert expected true, got false instead.");
  mu_assert(table.buckets[i]->key == testingKey, "Key values were different!");
  mu_assert(table.buckets[i]->value == personDifferentTestValue,
            "Values were different!");

  testingKey = Data_Cstring_New("tostKey");
  mu_assert(HashTable_Insert(&table, testingKey, personTestValue),
            "HashTable_Insert expected true, got false instead.");
  mu_assert(table.buckets[i]->key == testingKey, "Key values were different!");
  mu_assert(table.buckets[i]->value == personTestValue,
            "Values were different!");
  mu_assert(table.buckets[i]->next->value == personDifferentTestValue,
            "The second item in the list is on bad position.");

  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_insert_non_unique_data) {
  HashTable table;
  HashTable_Init(&table, 10, true);

  {
    char testBuffer[256] = {"Test Name"};
    void *testingKey = Data_Cstring_New("testKey");
    void *personTestValue = Data_Person_New(testBuffer, 16, 17, 18);
    HashTable_Insert(&table, testingKey, personTestValue);
  }
  {
    char testBuffer[256] = {"Test Name2"};
    void *testingKey = Data_Cstring_New("testKey");
    void *personTestValue = Data_Person_New(testBuffer, 136, 147, 185);
    /* Inserting item with non-unique key, unique data -----------------------
     */
    mu_assert(!HashTable_Insert(&table, testingKey, personTestValue),
              "HashTable_Insert with non-unique key false, got true.");
    Data_Destruct(testingKey);
    Data_Destruct(personTestValue);
  }
  {
    char testBuffer[256] = {"Test Name"};
    void *testingKey = Data_Cstring_New("testKey");
    void *personTestValue = Data_Person_New(testBuffer, 16, 17, 18);
    /* Inserting item with non-unique key and data ---------------------------
     */
    mu_assert(
        !HashTable_Insert(&table, testingKey, personTestValue),
        "HashTable_Insert with non-unique data and key expected false, got "
        "true instead!");
    Data_Destruct(testingKey);
    Data_Destruct(personTestValue);
  }

  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_insert_nulls) {
  HashTable table;
  HashTable_Init(&table, 10, true);

  char testBuffer[256] = {"Test Name"};
  void *testingKey = Data_Cstring_New("testKey");
  void *personTestValue = Data_Person_New(testBuffer, 16, 17, 18);

  mu_assert(!HashTable_Insert(NULL, testingKey, personTestValue),
            "HashTable_Insert with NULL pointer at table expected false, got "
            "true instead!");
  mu_assert(!HashTable_Insert(&table, NULL, personTestValue),
            "HashTable_Insert with NULL pointer at key expected false, got "
            "true instead.");
  mu_assert(
      !HashTable_Insert(NULL, NULL, NULL),
      "HashTable_Insert with NULL pointers expected false, got true instead.");
  Data_Destruct(testingKey);
  Data_Destruct(personTestValue);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_replace_ownership) {
  HashTable table;

  HashTable_Init(&table, 10, true);
  char testBuffer[256] = {"Test Name"};
  void *testingKey = Data_Cstring_New("testKey");
  size_t i = hash(&table, testingKey);
  void *personDifferentTestValue = Data_Person_New(testBuffer, 12, 13, 15);
  HashTable_Insert(&table, testingKey, personDifferentTestValue);
  strcpy(testBuffer, "Test Name2");

  void *personDifferentTestValue2 = Data_Person_New(testBuffer, 12, 13, 15);
  mu_assert(HashTable_Replace(&table, testingKey, personDifferentTestValue2),
            "HashTable_Replace expected true, got false instead.");
  mu_assert(table.buckets[i]->value == personDifferentTestValue2,
            "HashTable_Replace is not working properly!");
  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_replace_no_ownership) {
  HashTable table;
  HashTable_Init(&table, 10, false);
  char testBuffer[256] = {"Test Name"};
  void *testingKey = Data_Cstring_New("testKey");
  size_t i = hash(&table, testingKey);
  void *personDifferentTestValue = Data_Person_New(testBuffer, 12, 13, 15);
  HashTable_Insert(&table, testingKey, personDifferentTestValue);
  strcpy(testBuffer, "Test Name2");
  void *personDifferentTestValue2 = Data_Person_New(testBuffer, 12, 13, 15);
  mu_assert(HashTable_Replace(&table, testingKey, personDifferentTestValue2),
            "HashTable_Replace expected true, got false instead.");
  mu_assert(table.buckets[i]->value == personDifferentTestValue2,
            "HashTable_Replace is not working properly!");
  Data_Destruct(personDifferentTestValue2);
  Data_Destruct(personDifferentTestValue);
  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_replace_nulls) { HashTable_Replace(NULL, NULL, NULL); }

MU_TEST(test_hashtable_find) {
  HashTable table;
  HashTable_Init(&table, 10, true);

  char testBuffer[256] = {"Test Name"};
  void *testingKey[3] = {Data_Cstring_New("testKey"),
                         Data_Cstring_New("testingKey"),
                         Data_Cstring_New("nonExisting")};
  void *personDifferentTestValue = Data_Person_New(testBuffer, 12, 13, 15);
  HashTable_Insert(&table, testingKey[0], personDifferentTestValue);
  strcpy(testBuffer, "Johny");
  void *personDifferentTestValue2 = Data_Person_New(testBuffer, 12, 13, 15);
  HashTable_Insert(&table, testingKey[1], personDifferentTestValue2);

  mu_assert(HashTable_Find(&table, testingKey[0]) == personDifferentTestValue,
            "HashTable_Find should found an item.");
  mu_assert(
      HashTable_Find(&table, testingKey[2]) == NULL,
      "HashTable_Find should return NULL when non-saved key was in argument.");

  Data_Destruct(testingKey[2]);
  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_find_nulls) {
  mu_assert(!HashTable_Find(NULL, NULL), "HashTable_Find Expected NULL");
}

MU_TEST(test_hashtable_delete) {
  HashTable table;
  HashTable_Init(&table, 10, true);

  char testBuffer[2][256] = {{"Test Name"}, {"Test Name2"}};
  void *testingKey[2] = {Data_Cstring_New("testKey"),
                         Data_Cstring_New("tostKey")};
  void *personDifferentTestValue[2] = {
      Data_Person_New(testBuffer[0], 12, 13, 15),
      Data_Person_New(testBuffer[1], 16, 17, 18)};

  for (int i = 0; i < 2; i++) {
    HashTable_Insert(&table, testingKey[i], personDifferentTestValue[i]);
  }

  mu_assert_int_eq(2, (int)HashTable_Get_Count(&table));
  HashTable_Delete(&table, testingKey[0]);
  mu_assert_int_eq(1, (int)HashTable_Get_Count(&table));

  size_t i = hash(&table, testingKey[1]);

  mu_assert(table.buckets[i], "There is no value at first index.");
  mu_assert(table.buckets[i]->value == personDifferentTestValue[1],
            "Data's in table are different.");

  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_delete_nulls) {
  HashTable table;
  HashTable_Init(&table, 10, true);
  void *testingKey = Data_Cstring_New("testKey");
  mu_assert(!HashTable_Delete(NULL, testingKey),
            "HashTable_Delete with null pointer at table expected output "
            "false, got true instead.");
  mu_assert(!HashTable_Delete(&table, NULL),
            "HashTable_Delete with null pointer at key expected output false, "
            "got true instead.");
  mu_assert(!HashTable_Delete(NULL, NULL),
            "HashTable_Delete with null pointer at key expected output false, "
            "got true instead.");
  Data_Destruct(testingKey);
  HashTable_Destruct(&table);
}

MU_TEST(test_hashtable_proc) {
  HashTable table;
  HashTable_Init(&table, 10, true);
  {
    char testBuffer[256] = {"Name1"};
    void *testingKey = Data_Cstring_New("testKey");
    void *personDifferentTestValue = Data_Person_New(testBuffer, 5, 5, 5);
    HashTable_Insert(&table, testingKey, personDifferentTestValue);
  }
  {
    char testBuffer[256] = {"Name2"};
    void *testingKey = Data_Cstring_New("tostKey");
    void *personDifferentTestValue = Data_Person_New(testBuffer, 4, 4, 4);
    HashTable_Insert(&table, testingKey, personDifferentTestValue);
  }
  {
    char testBuffer[256] = {"Name3"};
    void *testingKey = Data_Cstring_New("trike");
    void *personDifferentTestValue = Data_Person_New(testBuffer, 3, 3, 3);
    HashTable_Insert(&table, testingKey, personDifferentTestValue);
  }
  {
    char testBuffer[256] = {"Name4"};
    void *testingKey = Data_Cstring_New("bike");
    void *personDifferentTestValue = Data_Person_New(testBuffer, 2, 2, 2);
    HashTable_Insert(&table, testingKey, personDifferentTestValue);
  }
  {
    char testBuffer[256] = {"Name5"};
    void *testingKey = Data_Cstring_New("car");
    void *personDifferentTestValue = Data_Person_New(testBuffer, 1, 1, 1);
    HashTable_Insert(&table, testingKey, personDifferentTestValue);
  }
  gIndex = 0;
  HashTable_Process(&table, process_table_node);

  char buffer[256];
  mu_assert_string_eq("car", Data_To_String(testDataArray[0], buffer));
  Data_To_String(testDataArray[1], buffer);
  mu_assert_string_eq("bike", buffer);
  mu_assert_string_eq("trike", Data_To_String(testDataArray[2], buffer));
  mu_assert_string_eq("tostKey", Data_To_String(testDataArray[3], buffer));
  mu_assert_string_eq("testKey", Data_To_String(testDataArray[4], buffer));

  HashTable_Clear(&table);
  HashTable_Destruct(&table);
}

MU_TEST_SUITE(test_suite) {
  MU_RUN_TEST(test_hashtable_init);
  MU_RUN_TEST(test_hashtable_init_nulls);
  MU_RUN_TEST(test_hashtable_insert);
  MU_RUN_TEST(test_hashtable_insert_one_bucket);
  MU_RUN_TEST(test_hashtable_insert_non_unique_data);
  MU_RUN_TEST(test_hashtable_insert_nulls);
  MU_RUN_TEST(test_hashtable_replace_ownership);
  MU_RUN_TEST(test_hashtable_replace_no_ownership);
  MU_RUN_TEST(test_hashtable_replace_nulls);
  MU_RUN_TEST(test_hashtable_find);
  MU_RUN_TEST(test_hashtable_find_nulls);
  MU_RUN_TEST(test_hashtable_delete);
  MU_RUN_TEST(test_hashtable_delete_nulls);
  MU_RUN_TEST(test_hashtable_proc);
}

int main(void) {
  MU_RUN_SUITE(test_suite);
  MU_REPORT();

  return 0;
}
