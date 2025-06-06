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
        printf("Erreur : Le nombre d'équipements et liens "
            "ne peut pas être négatif ou nul\n");
        exit(EXIT_FAILURE);
    }
    //Enregistrement des stations
    g->nb_equipements = nbEquipement;
    CHKNULL(g->equipements = calloc(nbEquipement, sizeof(Equipement)));
    for (size_t i = 0; i < nbEquipement; i++)
    {
        CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture d'une ligne
        ajouterEquipement(bufferLigne,g,i);
    }
    //Création de la matrice d'ajacence
    CHKNULL(g->matrice_adjacence = malloc(nbEquipement*nbEquipement * sizeof(uint8_t)));
    //Inisialisation à la valeur max (Laisse les poids de 0 possible)
    for (size_t i = 0; i < nbEquipement*nbEquipement; i++)
        *(g->matrice_adjacence + i) = UINT8_MAX;
    
    //Enregistrement des liens
    for (size_t i = 0; i < nbLiens; i++)
    {
        int s1, s2, poids;
        CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture d'une ligne
        CHKSSCANF(sscanf(bufferLigne,"%d;%d;%d", &s1, &s2, &poids),3,
            "Erreur de lecture du lien"); //Lecture des valeurs
        // MAJ de la matrice d'adjacence dans le deux sens
        *(g->matrice_adjacence + s1 * nbEquipement + s2) = poids;
        *(g->matrice_adjacence + s2 * nbEquipement + s1) = poids;
    }
        
    CHK0(fclose(f)); //Fermeture du fichier
} 

void libererReseau(Graphe *g){
    //Libère tout les champs allouer de réseau et mets le reste à 0
    g->nb_equipements = 0;
    free(g->equipements);
    g->equipements = NULL;
    free(g->matrice_adjacence);
    g->matrice_adjacence = NULL;
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
    CHKSSCANF(sscanf(rest, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx;%n",
           &e.mac[0], &e.mac[1], &e.mac[2], &e.mac[3], &e.mac[4], &e.mac[5], 
           &offset), 6, "Erreur de lecture de l'adresse MAC\n");
    rest = rest + offset;

    //Parse des autres attributs en fonction du type
    switch (e.type)
    {
    case STATION_TYPE:
        //Parse de l'ip
        //ip_addr_t *ip = e.station.ip;
        CHKSSCANF(sscanf(rest, "%hhu.%hhu.%hhu.%hhu",
        &e.station.ip[0], &e.station.ip[1], &e.station.ip[2], 
        &e.station.ip[3]),4, "Erreur de lecture de l'adresse IP\n");
        break;

    case SWITCH_TYPE:
        //Parse du nombre de port
        CHKSSCANF(sscanf(rest,"%hhu;%n",&e.sw.nb_port, &offset),1,
        "Erreur dans la lecture du nombre de port");
        rest = rest + offset;
        //Parse priorité
        CHKSSCANF(sscanf(rest,"%hu",&e.sw.priorite),1,
        "Erreur dans la lecture du nombre de la priorité");
        break;

    default:
        printf("Erreur : Type d'équipement inconnu\n");
        exit(EXIT_FAILURE);
        break;
    }
    //Ajout de l'équipement au réseau
    *(g->equipements + index) = e;
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

void afficherEquipement(Equipement const *e, int const index) {
    //S'occupe de l'affichage d'un équipement
    printf("Équipement %d : ", index);
    switch (e->type)
    {
    case STATION_TYPE:
        printf("Station\n");
        printf("  MAC : ");
        affiche_mac(e->mac);
        printf("\n  IP  : ");
        affiche_ip(e->station.ip);
        printf("\n");
        break;

    case SWITCH_TYPE:
        printf("Switch\n");
        printf("  MAC : ");
        affiche_mac(e->mac);
        printf("\n  Nombre de ports : %d\n", e->sw.nb_port);
        printf("  Priorité : %d\n", e->sw.priorite);
        afficherTableCommutation(e->sw, e->sw.nb_port);
        break;

    default:
        printf("Type inconnu.\n");
        exit(EXIT_FAILURE);
        break;
    }
}

void afficherGraphe(Graphe const *g) {
    //S'occupe d'afficher le graphe (donc sa matrice d'adjacence)
    printf("=== Graphe du réseau ===\n");
    printf("Nombre d'équipements : %d\n\n", g->nb_equipements);

    for (int i = 0; i < g->nb_equipements; ++i) {
        afficherEquipement(g->equipements + i, i);
    }
    printf("\n");
    afficherMatriceAdja(g);
}

void afficherMatriceAdja(Graphe const *g){
    int n = g->nb_equipements;
    printf("=== Matrice d'adjacence ===\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d\t", g->matrice_adjacence[i * n + j]);
        }
        printf("\n");
    }
}