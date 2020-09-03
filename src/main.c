/**
 * @file       main.c
 * @author     Ondřej Ševčík
 * @date       6/2019
 * @brief      Main file of HashTable.
 * **************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

/* Private includes -------------------------------------------------------- */
#include <stdio.h>

#include "data.h"
#include "data_cstring.h"
#include "data_person.h"
#include "ioutils.h"
#include "table.h"

/* Private functions ------------------------------------------------------- */
/**
 * @brief menu  Prints the menu to stdout
 */
void menu() {
  printf("1 - HashTable_Init()\n");
  printf("2 - HashTable_Destruct()\n");
  printf("3 - HashTable_Insert()\n");
  printf("4 - HashTable_Delete()\n");
  printf("5 - HashTable_Find()\n");
  printf("6 - HashTable_Get_Count()\n");
  printf("7 - HashTable_Clear()\n");
  printf("8 - HashTable_Process()\n");
  printf("9 - HashTable_Replace()\n");
  printf("M - MENU\n");
  printf("E - END\n");
}

/**
 * @brief process_table_node    Prints the information and table node to stdout
 * @param key   Pointer at key of an node
 * @param value Pointer at value of a node
 */
void process_table_node(Data_t *key, Data_t *value) {
  char strbuf[256];
  printf("Key: ");
  puts(Data_To_String(key, strbuf));
  printf(" -> Value: ");
  puts(Data_To_String(value, strbuf));
}

/* Main -------------------------------------------------------------------- */
int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  printf("Hash table\n");
  printf("----------\n\n");
  bool initialized = false;
  char usersInput = ' ';
  HashTable table;
  Data_Cstring_t data_cstring;
  Data_Cstring_Init(&data_cstring, NULL);
  void *key = &data_cstring;
  menu();

  while (!feof(stdin) && usersInput != 'e' && usersInput != 'E') {
    printf("Your choice:\n");
    usersInput = io_utils_get_char_line();

    switch (usersInput) {
      case '1': {
        if (initialized) {
          printf("Table is already initialized!\n");
          break;
        }

        printf("Enter the width of table (for example 1000):\n");
        long size;
        io_utils_get_long(&size);
        initialized = HashTable_Init(&table, (unsigned long)size, true);
        if (initialized) {
          printf("Table was initialized.\n");
        } else {
          printf("Table not was initialized.\n");
        }
        break;
      }

      case '2':
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        HashTable_Destruct(&table);
        printf("Table destructed.\n");
        initialized = false;
        break;

      case '3': {
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        printf("Please enter a key:\n");
        char strbuf[256];
        io_utils_get_string(strbuf, 256);
        void *key_new = Data_Cstring_New(strbuf);
        // io_utils_get_string(Data_To_String(key_new, NULL), 256);
        printf(
            "Please, enter those values:\n"
            "Name: ");
        io_utils_get_string(strbuf, 256);
        double age, weight, height;
        printf("\nEnter age: ");
        io_utils_get_double(&age);
        printf("\nEnter weight: ");
        io_utils_get_double(&weight);
        printf("\nEnter height: ");
        io_utils_get_double(&height);
        void *value_new = Data_Person_New(strbuf, age, weight, height);

        if (!HashTable_Insert(&table, key_new, value_new)) {
          printf(
              "HashTable_Insert failed. Might be because of duplicate key.\n");
          Data_Destruct(key_new);
          Data_Destruct(value_new);
        }

        break;
      }

      case '4': {
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        printf("Please enter a key of item you want to delete:\n");
        io_utils_get_string(Data_To_String(key, NULL), 256);

        if (!HashTable_Delete(&table, key)) {
          printf(
              "HashTable_Delete failed. Most likely because the key or data "
              "were not found.\n");
        }

        break;
      }

      case '5': {
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        char buffer[256];
        printf("Please, enter a key of item you want to search for:\n");
        io_utils_get_string(Data_To_String(key, NULL), 256);
        Data_Person_t *value = (Data_Person_t *)HashTable_Find(&table, key);

        if (value) {
          puts("The item's value is ");
          puts(Data_To_String(value, buffer));
        } else {
          puts("Item not found!\n");
        }

        break;
      }

      case '6':
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        printf("Table has %ld items.\n",
               (long unsigned int)HashTable_Get_Count(&table));
        break;

      case '7':
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        HashTable_Clear(&table);
        printf("Every item in a table were cleared.\n");
        break;

      case '8':
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }

        HashTable_Process(&table, process_table_node);
        break;
      case '9':
        if (!initialized) {
          printf("Table is not initialized!\n");
          break;
        }
        char buffer[256];
        printf("Please, enter a key of item you want to replace:\n");
        io_utils_get_string(Data_To_String(key, NULL), 256);
        printf(
            "Please enter asociated values:\n"
            "Name: ");
        io_utils_get_string(buffer, 256);
        double age, weight, height;
        printf("\nEnter age: ");
        io_utils_get_double(&age);
        printf("\nEnter weight: ");
        io_utils_get_double(&weight);
        printf("\nEnter height: ");
        io_utils_get_double(&height);
        void *value_new = Data_Person_New(buffer, age, weight, height);

        if (HashTable_Replace(&table, key, value_new))
          printf("Table updated!\n");
        break;

      case 'k':
        printf("First item %s\n",
               Data_To_String(table.buckets[0]->value, buffer));
        printf("Second item: %s\n",
               Data_To_String(table.buckets[0]->next->value, buffer));
        break;

      case 'm':
      case 'M':
        menu();
        break;

      case 'e':
      case 'E':
        printf("END.\n");
        break;

      default:
        printf("Unknown option!\n");
        break;
    }

    printf("\n");
  }

  if (initialized) {
    HashTable_Clear(&table);
    HashTable_Destruct(&table);
  }

  return 0;
}
