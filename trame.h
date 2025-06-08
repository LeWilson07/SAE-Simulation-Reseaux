#pragma once 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "adresse.h"

// Structure de la tram
typedef struct Trame{
    uint8_t preambule[7];
    uint8_t sfd;
    mac_addr_t dest;
    mac_addr_t src;
    uint8_t type[2];
    uint8_t *data;
    uint8_t fcs[4];
    uint16_t tailleData;
}Trame;

void affiche_trame(Trame const *tr);
void liberer_trame(Trame *tr);