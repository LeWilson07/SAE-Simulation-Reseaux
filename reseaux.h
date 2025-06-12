#pragma once

#include "adresse.h"
#include "trame.h"
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

typedef enum {
    RACINE,
    DESGIGNE,
    BLOQUE
} EtatPort;

typedef struct {
    mac_addr_t RootID;
    size_t cout;
    mac_addr_t mac;
} BPDU;


typedef struct {
    size_t num;
    size_t indexEquipement;
    EtatPort etat;
    BPDU bpdu;
} Port;

typedef struct {
    ip_addr_t ip;
    Port port;
} Station;

typedef struct{
    size_t port;
    mac_addr_t adresse_mac;
}Commutation;

typedef struct{
    size_t nb_port;
    Port *ports;
    size_t priorite;
    size_t commu_capacite;
    size_t nb_commu; 
    Commutation* tableCommu;
    BPDU meilleur_bpdu;
}Switch;

typedef enum {
    STATION_TYPE,
    SWITCH_TYPE
} EquipementType;

typedef struct {
    size_t index;
    EquipementType type;
    mac_addr_t mac;
    union {
        Station station;
        Switch sw;
    };
} Equipement;

typedef struct {
    size_t nb_equipements;
    Equipement *equipements;
    size_t *matrice_adjacence;
} Graphe;

void construireReseau(char const *path, Graphe *g);
void construirePort(Graphe *g, size_t s1, size_t s2);
void libererReseau(Graphe *g);
void ajouterEquipement(char *ligne, Graphe *g ,int const index);
void ajouterCommutation(Switch *sw,  mac_addr_t const *mac, size_t indexPort);
void afficherPortSwitch(Switch const *sw);
void afficherTableCommutation(Switch const *sw);
void afficherEquipement(Equipement const *e, size_t const index);
void afficherGraphe(Graphe const *g);
void afficherMatriceAdja(Graphe const *g);
size_t adresseDansTabCommu(Switch const *sw, mac_addr_t const *mac);
size_t numPortIndexEquipment(Switch const *sw, size_t index);
size_t indexEquipmentNumPort(Switch const *sw, size_t numPort);
void transmettreTrame(Graphe *g, Trame const *tr, size_t indexSrc, size_t indexCourant, size_t PTL);
void envoyerMessage(Graphe *g, Trame *t, size_t stationSrc, size_t stationDest);