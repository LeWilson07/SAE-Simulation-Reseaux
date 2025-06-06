#pragma once 

#include <stdlib.h>
#include <stdbool.h>
#include  "adresse.h"
#include "trame.h"


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

void afficherTableCommutation(Switch sw,int const taille);
//void afficherEquipement(Equipement *e,int const index);
void afficherGraphe(Graphe g);



