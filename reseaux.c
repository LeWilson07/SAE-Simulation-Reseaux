#include "reseaux.h"



void construireReseau(char const *path, Graphe *g) {
    int nbEquipement;
    int nbLiens;
    char bufferLigne[256];
    FILE *f;
    CHKNULL(f = fopen(path, "r"));
    CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f));
    CHKSSCANF(sscanf(bufferLigne, "%d %d", &nbEquipement, &nbLiens), 2, \
        "Erreur : Nb de Equipement ou Nb de Lien");
    printf("%d %d\n", nbEquipement, nbLiens);
    CHK0(fclose(f));
} 

void afficherTableCommutation(Switch sw, int taille) {
    //S'occupe de l'affichage d'une table de commutation
    printf("Table de commutation (%d entrées) :\n", taille);
    printf("----------------------------------\n");
    for (int i = 0; i < taille; ++i) {
        printf("MAC : ");
        affiche_mac(sw.tableCommu->adresse_mac); 
        printf(" → Port : %d\n", sw.tableCommu->port);
    }
    printf("----------------------------------\n");
}

void afficherEquipement(Equipement *e, int const index) {
    //S'occupe de l'affichage d'un équipement
    printf("Équipement %d : ", index);
    if (e->type == STATION_TYPE) {
        printf("Station\n");
        printf("  MAC : ");
        affiche_mac(e->station.mac);
        printf("\n  IP  : ");
        affiche_ip(e->station.ip);
        printf("\n");
    } else if (e->type == SWITCH_TYPE) {
        printf("Switch\n");
        printf("  MAC : ");
        affiche_mac(e->sw.mac);
        printf("\n  Nombre de ports : %d\n", e->sw.nb_port);
        printf("  Priorité : %d\n", e->sw.priorite);
        afficherTableCommutation(e->sw, e->sw.nb_port);
    } else {
        printf("Type inconnu.\n");
    }
}

void afficherGraphe(Graphe g) {
    //S'occupe d'afficher le graphe (donc sa matrice d'adjacence)
    printf("=== Graphe du réseau ===\n");
    printf("Nombre d'équipements : %d\n\n", g.nb_equipements);

    for (int i = 0; i < g.nb_equipements; ++i) {
        afficherEquipement(&g.equipements[i], i);
    }

    printf("\n=== Matrice d'adjacence ===\n");
    for (int i = 0; i < g.nb_equipements; ++i) {
        for (int j = 0; j < g.nb_equipements; ++j) {
            printf("%d ", g.matrice_adjacence[i][j]);
        }
        printf("\n");
    }
}
