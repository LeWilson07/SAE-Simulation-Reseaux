#pragma once 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "adresse.h"

// Structure de la tram
typedef struct Trame{
    size_t preambule[7];
    size_t sfd;
    mac_addr_t dest;
    mac_addr_t src;
    size_t type[2];
    size_t *data;
    size_t fcs[4];
    size_t tailleData;
}Trame;

void Lire_Message_Trame(Trame const *tr);
void affiche_trame(Trame const *tr);
void liberer_trame(Trame *tr);
uint32_t calculer_fcs(const size_t *data, size_t taille);
int generer_fcs(Trame *tr);

