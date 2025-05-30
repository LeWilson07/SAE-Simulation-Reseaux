#include "reseaux.h"
#include "adresse.h"


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
    CHKNULL(g->equipements = malloc(nbEquipement * sizeof(Equipement)));
    for (size_t i = 0; i < nbEquipement; i++)
    {
        CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture d'une ligne
        ajouterEquipement(bufferLigne,g,i);
    }

    //Enregistrement des liens
        
    CHK0(fclose(f)); //Fermeture du fichier
} 

void ajouterEquipement(char *ligne, Graphe *g ,int const index){
    Equipement e;
    int offset;

    //Parse du Type d'équipement
    int type;
    CHKSSCANF(sscanf(ligne, "%d;%n", &type, &offset),1,
        "Erreur : Scan du type d'équipement");
    char *rest = ligne + offset;
    e.type = type - 1; //Correspond au type de l'enum

    //Parse de l'adresse MAC de l'équipement
    mac_addr_t mac;
    CHKSSCANF(sscanf(rest, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &e.mac[0], &e.mac[1], &e.mac[2], &e.mac[3], &e.mac[4], &e.mac[5]),6, 
           "Erreur de lecture de l'adresse MAC\n");
    rest = ligne + offset;
    
    //Parse des autres attributs en fonction du type
    switch (e.type)
    {
    case STATION_TYPE:
        //Parse de l'ip
        ip_addr_t *ip = &e.station.ip;
        CHKSSCANF(sscanf(rest, "%hhd.%hhd.%hhd.%hhd",
        ip[0], ip[1], ip[2], ip[3]),4,
        "Erreur de lecture de l'adresse IP\n");
        break;
    case SWITCH_TYPE:
        //Parse du nombre de port
        //Parse priorité
        break;
    default:
        printf("Erreur : Type d'équipement inconnu\n");
        exit(EXIT_FAILURE);
        break;
    }   
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
        affiche_mac(e->mac);
        printf("\n  IP  : ");
        affiche_ip(e->station.ip);
        printf("\n");
    } else if (e->type == SWITCH_TYPE) {
        printf("Switch\n");
        printf("  MAC : ");
        affiche_mac(e->mac);
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
