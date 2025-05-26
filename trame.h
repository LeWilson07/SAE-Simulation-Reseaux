#pragma once 
#include <stdio.h>
#include <stdint.h>
#include <adresse.h>
#include <reseaux.h>


// Structure de la tram
typedef struct trame{
    uint8_t preambule[7];
    mac_addr_t dest;
    mac_addr_t src;
    uint8_t type[2];
    uint8_t *data;
    uint8_t fcs[4];
    uint16_t tailleData;
}trame;

void affiche_trame(trame tr);