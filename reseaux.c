#include "reseaux.h"



void construireReseau(char const *path, Graphe *g) {
    int nbEquipement;
    int nbLiens;
    char bufferLigne[256];
    FILE *f;
    CHKNULL(f = fopen(path, "r")); //Ouverture du fichier
    CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture de la première ligne
    //Affectaion du nombre d'équipement et de lien
    CHKSSCANF(sscanf(bufferLigne, "%d %d", &nbEquipement, &nbLiens), 2, \
        "Erreur : Nb de Equipement ou Nb de Lien");
    //Vérification de la cohérence des valeurs
    if(nbEquipement <= 0 || nbLiens <= 0) {
        printf("Le nombre d'équipements et liens "
            "ne peut pas être négatif ou nul\n");
        exit(EXIT_FAILURE);
    }
    //Enregistrement des stations
    g->nb_equipements = nbEquipement;
    CHKNULL(g->equipements = malloc(nbEquipement * sizeof(*g->equipements)));
    for (size_t i = 0; i < nbEquipement; i++)
    {
        CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture d'une ligne
        ajouterEquipement(bufferLigne,g,i);
    }

    //Enregistrement des liens
        
    CHK0(fclose(f)); //Fermeture du fichier
} 

void ajouterEquipement(char const *ligne, Graphe *g ,int const index){
    int type;
    int offset;
    CHKSSCANF(sscanf(ligne, "%d;%n", &type, &offset),1,"Erreur : Scan du type d'équipement");
    char *rest = ligne + offset;
    switch (type)
    {
    case 1:
        ajouterStation(rest, g, index);
        break;
    case 2:
        ajouterSwitch(rest, g, index);
        break;
    default:
        printf("Erreur : Type d'équipement inconnu\n");
        exit(EXIT_FAILURE);
        break;
    }   
}

void ajouterSwitch(char const * ligne, Graphe *g ,int const index){

}

void ajouterStation(char const * ligne, Graphe *g ,int const index){

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
