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
    mac_addr_t mac;
    ip_addr_t ip;
} Station;

typedef struct{
    uint8_t port;
    mac_addr_t adresse_mac;

}Commutation;

typedef struct{
    mac_addr_t mac;
    uint8_t nb_port;
    int priorite; 
    Commutation* tableCommu;
}Switch;

typedef enum {
    STATION_TYPE,
    SWITCH_TYPE
} EquipementType;

typedef struct {
    EquipementType type;
    union {
        Station station;
        Switch sw;
    };
} Equipement;

typedef struct {
    int nb_equipements;
    Equipement *equipements;
    int** matrice_adjacence;
} Graphe;

void construireReseau(char const *path, Graphe *g);
void ajouterEquipement(char const *ligne, Graphe *g ,int const index);
void ajouterSwitch(char const * ligne, Graphe *g ,int const index);
void ajouterStation(char const * ligne, Graphe *g ,int const index);
void afficherTableCommutation(Switch sw,int const taille);
void afficherEquipement(Equipement *e,int const index);
void afficherGraphe(Graphe g);



