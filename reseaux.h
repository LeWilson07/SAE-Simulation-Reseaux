#pragma once 

#include "adresse.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h> 
#include <stdbool.h>

#define CHK0(op) do { \
    if((op) != 0) { \
        perror(#op); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

#define CHKNULL(op) do { \
    if((op) == NULL) { \
        perror(#op); \
        exit(EXIT_FAILURE); \
    }\
} while(0)

#define CHKSSCANF(op,nbVal,desc) do { \
    if ((op) != nbVal) { \
        printf("%s\n",desc); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

typedef struct {
    ip_addr_t ip;
} Station;

typedef struct{
    uint8_t port;
    mac_addr_t adresse_mac;

}Commutation;

typedef struct{
    uint8_t nb_port;
    uint16_t priorite; 
    Commutation* tableCommu;
}Switch;

typedef enum {
    STATION_TYPE,
    SWITCH_TYPE
} EquipementType;

typedef struct {
    EquipementType type;
    mac_addr_t mac;
    union {
        Station station;
        Switch sw;
    };
} Equipement;

typedef struct {
    int nb_equipements;
    Equipement *equipements;
    uint8_t *matrice_adjacence;
} Graphe;

void construireReseau(char const *path, Graphe *g);
void ajouterEquipement(char *ligne, Graphe *g ,int const index);
void afficherTableCommutation(Switch sw,int const taille);
void afficherEquipement(Equipement const *e,int const index);
void afficherGraphe(Graphe const *g);
void afficherMatriceAdja(Graphe const *g);
