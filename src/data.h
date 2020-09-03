/**
 * @file    data.h
 * @author  Tomas Dulik
 * @date    9/3/2015
 * @brief   Headers of Data Abstract type
 */

#ifndef DATA_H_
#define DATA_H_

typedef struct { /**< typ pro sadu ukazatelů na funkce ("tabulku virtuálních
                    funkcí") */
  int (*cmp)(void *d1, void *d2); /**< Porovnání velikosti 2 instancí */
  unsigned (*hash)(void *data); /**< Výpočet hash hodnoty pro danou instanci */
  char *(*to_string)(
      void *data,
      char *str); /**< Převod na string. Výsledek se uloží do parametru str */
  void (*from_string)(char *str, void *data); /**< Převod ze stringu na data */
  char *(*get_type)(); /**< Vrátí řetěz, obsahující typ objektu  */
  void (*destruct)(
      void *data); /**< Destrukce objektu = korektní uvolnění všech zdrojů */
} Data_Functions_t;

/*!
 * \brief type void je základní typ, ze kterého jsou odvozeny další typy.
 *
 * Typ se skládá z ukazatele na tabulku virtuálních funkcí a ukazatele na data.
 * Funkce v tabulce virtuálních funkcí mohou být volány přes základní datový
 * void, ale přitom mohou být definovány pro každý odvozený typ zvlášť. Díky
 * tomu je možné uložit (ukazatele na) instance void i všechny odvozené typy do
 * jedné datové struktury.
 */
typedef struct {
  const Data_Functions_t *fncs; /**< ukazatel na tabulku virtuálních funkcí */
  void *data;                   /**< ukazatel na data */
} Data_t;

/**
 * @brief       Funkce Data_Cmp slouží k porovnání dvou objektů typu void
 * @warning     Oba parametry musí být stejným typem! Nelze porovnávat různé
 * odvozeniny typu void.
 * @param       d1      Ukazatel na první položku
 * @param       d2      Ukazatel na druhou položku
 * @return      Funkce vrací záporné číslo pokud je první položka menší než
 * druhá, nulu pokud se rovnají a kladné číslo pokud je první větší než druhá.
 *              Velikost kladného/záporného čísla je úměrná odlišnosti prvků.
 */
int Data_Cmp(void *d1, void *d2);

/**
 * @brief       Funkce Data_Hash vypočítá hash objektu.
 * @param       data    Ukazetel na hashovaný objekt
 * @return      Vrací vypočítaný hash.
 */
unsigned Data_Hash(void *data);

/**
 * @brief       Funkce Data_To_String vytvoří řetězec z dat objektu a uloží ho
 * do připraveného ukazatele.
 * @param       data    Ukazatel na objekt pro který je vytvořen řetězec.
 * @param       str     Ukazatel na paměť kam je uložen vytvořený řetězec.
 * @return      Funkce vrací ukazatel na vytvořený řetězec.
 */
char *Data_To_String(void *data, char *str);

/**
 * @brief       Funkce Data_From_String umožňuje rekonstruovat objekt z řetězce.
 *              Konkrétní formát řetězce závisí na odvozeném typu.
 * @param       str     Ukazetel na řetězec obsahující data objektu.
 * @param       data    Ukazatel na paměť, ve které je vytvořen výsledný objekt.
 */
void Data_From_String(char *str, void *data);

/**
 * @brief       Funkce Data_Get_Type vrací řetězec popisující datový typ
 * objektu.
 * @param       data    Ukazatel na objekt.
 * @return      Vrací ukazatel na řetězec popisující datový typ objektu.
 */
char *Data_Get_Type(void *data);

/**
 * @brief       Korektně uvolní veškeré zdroje (např. alokovanou dynamickou
 * paměť, otevřené soubory) objektu.
 * @param       data    Ukazatel na objekt.
 */
void Data_Destruct(void *data);

#endif /* DATA_VALUE_H_ */
