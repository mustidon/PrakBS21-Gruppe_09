#include <string.h>
#include <stdlib.h>
#include "keyValStore.h"

// Einfache Key-Value-Datenstruktur (Beispiel)
#define MAX_ENTRIES 100
static struct {
    char key[50];
    char value[50];
} store[MAX_ENTRIES];
static int store_size = 0;

// Fügt einen Key-Value-Eintrag hinzu
void put(const char* key, const char* value) {
    if (store_size < MAX_ENTRIES) {
        strcpy(store[store_size].key, key);
        strcpy(store[store_size].value, value);
        store_size++;
    }
}

// Holt einen Wert anhand des Keys
char* get(const char* key) {
    for (int i = 0; i < store_size; i++) {
        if (strcmp(store[i].key, key) == 0) {
            return store[i].value;
        }
    }
    return "Key not found!";
}