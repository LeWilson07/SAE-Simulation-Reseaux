#pragma once

#include "adresse.h"
<<<<<<< HEAD
=======
#include "trame.h"

>>>>>>> 487072a8043cf02b25f25bd479e2b0c23f0ed03d
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define NOMBRE_COMMUTATION_DEFAUT 4

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
    uint8_t num;
    int indexEquipement;
} Port;


typedef struct {
    ip_addr_t ip;
    Port port;
} Station;

typedef struct{
    uint8_t port;
    mac_addr_t adresse_mac;
<<<<<<< HEAD
    // mettre un enume bloqué, designer et racine

=======
>>>>>>> 487072a8043cf02b25f25bd479e2b0c23f0ed03d
}Commutation;

typedef struct{
    uint8_t nb_port;
    Port *ports;
    uint16_t priorite;
    uint16_t commu_capacite;
    uint16_t nb_commu; 
    Commutation* tableCommu;
}Switch;

typedef enum {
    STATION_TYPE,
    SWITCH_TYPE
} EquipementType;

typedef struct {
    int index;
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
void construirePort(Graphe *g, int s1, int s2);
void libererReseau(Graphe *g);
void ajouterEquipement(char *ligne, Graphe *g ,int const index);
void ajouterCommutation(Switch *sw,  mac_addr_t const *mac, uint8_t indexPort);
void afficherPortSwitch(Switch const *sw);
void afficherTableCommutation(Switch const *sw);
void afficherEquipement(Equipement const *e,int const index);
void afficherGraphe(Graphe const *g);
void afficherMatriceAdja(Graphe const *g);
int  adresseDansTabCommu(Switch const *sw, mac_addr_t const *mac);
void transmettreTrame(Graphe *g, Trame const *tr, int indexSrc, int indexCourant);