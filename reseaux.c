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
        if (s1 >= nbEquipement || s2 >= nbEquipement){
            printf("Ce lien possède un index supérieur au nombre de machine du réseau");
            exit(EXIT_FAILURE);
        }
        // MAJ de la matrice d'adjacence dans le deux sens
        *(g->matrice_adjacence + s1 * nbEquipement + s2) = poids;
        *(g->matrice_adjacence + s2 * nbEquipement + s1) = poids;
        // MAJ des ports des equipement concernés
        construirePort(g,s1,s2);
        construirePort(g,s2,s1);
    }
        
    CHK0(fclose(f)); //Fermeture du fichier
} 

void construirePort(Graphe *g, int s1, int s2){
    //On va mettre sur le premier port disponible de l'équipement l'index l'autre.
    Equipement e = g->equipements[s1];
    switch (e.type)
    {
    case STATION_TYPE:
        if (e.station.port.indexEquipement == -1)
            e.station.port.indexEquipement = s2;
        else printf("L'équipement %d n'a pas assez "
            "de port pour cette configuration\n", s1);
        break;

    case SWITCH_TYPE:
        int i = 0;
        while (i < e.sw.nb_port && e.sw.ports[i].indexEquipement != -1) i++;
        if(i < e.sw.nb_port)
            e.sw.ports[i].indexEquipement = s2;
        else printf("L'équipement %d n'a pas assez "
            "de port pour cette configuration\n", s1);
        break;

    default:
        printf("Erreur : Type d'équipement inconnu\n");
        exit(EXIT_FAILURE);
        break; 
    }
}

void libererReseau(Graphe *g){
    //Libère la table de commutation des switch
    for (size_t i = 0; i < g->nb_equipements; i++)
    {
        if (g->equipements[i].type == SWITCH_TYPE){
            free(g->equipements[i].sw.tableCommu);
            free(g->equipements[i].sw.ports);
        } 
    }
    
    //Libère tout les champs allouer de réseau et mets le reste à 0
    g->nb_equipements = 0;
    free(g->equipements);
    g->equipements = NULL;
    free(g->matrice_adjacence);
    g->matrice_adjacence = NULL;
}

void ajouterEquipement(char *ligne, Graphe *g ,int const index){
    Equipement e = {0};
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
        &e.mac.addr[0], &e.mac.addr[1], &e.mac.addr[2], &e.mac.addr[3],
        &e.mac.addr[4], &e.mac.addr[5], &offset),
        6, "Erreur de lecture de l'adresse MAC\n");
    rest = rest + offset;

    //Parse des autres attributs en fonction du type
    switch (e.type)
    {
    case STATION_TYPE:
        //Parse de l'ip
        //ip_addr_t *ip = e.station.ip;
        CHKSSCANF(sscanf(rest, "%hhu.%hhu.%hhu.%hhu",
            &e.station.ip.addr[0], &e.station.ip.addr[1],
            &e.station.ip.addr[2], &e.station.ip.addr[3]),
            4, "Erreur de lecture de l'adresse IP\n");
        //Mise en place du port
        e.station.port.indexEquipement = -1;
        break;

    case SWITCH_TYPE:
        //Parse du nombre de port
        CHKSSCANF(sscanf(rest,"%hhu;%n",&e.sw.nb_port, &offset),1,
        "Erreur dans la lecture du nombre de port\n");
        rest = rest + offset;
        //Parse priorité
        CHKSSCANF(sscanf(rest,"%hu",&e.sw.priorite),1,
        "Erreur dans la lecture du nombre de la priorité\n");
        //Allocation de la table de commutation
        e.sw.nb_commu = 0;
        e.sw.commu_capacite = 4; //Valeur par défaut
        CHKNULL(e.sw.tableCommu = calloc(NOMBRE_COMMUTATION_DEFAUT, \
            sizeof(Commutation)));
        //Allocation de tous les ports du switch
        CHKNULL(e.sw.ports = calloc(e.sw.nb_port,sizeof(Port)));
        for (size_t i = 0; i < e.sw.nb_port; i++) {
            e.sw.ports[i].num = i+1;
            e.sw.ports[i].indexEquipement = -1;
        }        
        break;

    default:
        printf("Erreur : Type d'équipement inconnu\n");
        exit(EXIT_FAILURE);
        break;
    }
    //Ajout de l'équipement au réseau
    e.index = index;
    *(g->equipements + index) = e;
}

void ajouterCommutation(Switch *sw,  mac_addr_t const *mac, uint8_t indexPort){
    //Ajout de place dans la table de commutation si nécessaire
    if (sw->nb_commu == sw->commu_capacite){
        CHKNULL(sw->tableCommu = realloc(sw->tableCommu,sw->commu_capacite * \
            2 * sizeof(Commutation)));
        sw->commu_capacite *= 2;
    }
    //Enregistrement de la commutation
    (sw->tableCommu + sw->nb_commu)->port = indexPort;
    (sw->tableCommu + sw->nb_commu)->adresse_mac = *mac;
}

void afficherPortSwitch(Switch const *sw){
    printf("Etat des ports :\n");
    printf("----------------\n");
    for (size_t i = 0; i < sw->nb_port; i++)
    {
        printf("Port : %d → Machine : %d\n", sw->ports[i].num, 
            sw->ports[i].indexEquipement);
    }
    printf("----------------\n");
}

void afficherTableCommutation(Switch const *sw) {
    //S'occupe de l'affichage d'une table de commutation
    printf("Table de commutation (%d entrées) :\n", sw->nb_commu);
    printf("----------------------------------\n");
    for (int i = 0; i < sw->nb_commu; ++i) {
        printf("MAC : ");
        affiche_mac(&sw->tableCommu->adresse_mac); //Régler le +i car affiche tjr la même
        printf(" → Port : %d\n", sw->tableCommu->port);
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
        affiche_mac(&e->mac);
        printf("\n  IP  : ");
        affiche_ip(&e->station.ip);
        printf("\n");
        break;

    case SWITCH_TYPE:
        printf("Switch\n");
        printf("  MAC : ");
        affiche_mac(&e->mac);
        printf("\n  Nombre de ports : %d\n", e->sw.nb_port);
        printf("  Priorité : %d\n", e->sw.priorite);
        afficherTableCommutation(&e->sw);
        afficherPortSwitch(&e->sw);
        break;

    default:
        printf("Type inconnu.\n");
        exit(EXIT_FAILURE);
        break;
    }
    printf("\n");
}

void afficherGraphe(Graphe const *g) {
    //S'occupe d'afficher le graphe
    printf("=== Graphe du réseau ===\n");
    printf("Nombre d'équipements : %d\n\n", g->nb_equipements);

    for (int i = 0; i < g->nb_equipements; i++) {
        afficherEquipement(g->equipements + i, i);
    }
    printf("\n");
    //afficherMatriceAdja(g);
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

int adresseDansTabCommu(Switch const *sw, mac_addr_t const *mac){
    //Retourne l'index si elle existe sinon -1
}

uint8_t numPortIndexEquipment(Switch const *sw, int index){
    //Retourne le numéro du port avec l'index en paramètre, -1 si introuvable
    for (size_t i = 0; i < sw->nb_port; i++)
        if (sw->ports[i].indexEquipement == index) return sw->ports[i].num;
    return -1;
}

int indexEquipmentNumPort(Switch const *sw, uint8_t numPort){
    //Retourne l'index de l'equipement derrière le port en paramètre, -1 si introuvable
    for (size_t i = 0; i < sw->nb_port; i++)
        if (sw->ports[i].num == numPort) return sw->ports[i].indexEquipement;
    return -1;
}

void transmettreTrame(Graphe *g, Trame const *tr, int indexSrc, int indexCourant){
    Equipement e = g->equipements[indexCourant];
    // Vérifier si l'adresse MAC Courant est celle de destination
    if (comparer_mac(&e.mac,&tr->dest) == 0) {
        printf("La trame est arrivé à destination");
        return;
    }
    // On se prépare à transmettre (Si c'est un switch)
    if (e.type == SWITCH_TYPE)
    {
        //On récupère le port d'arriver
        uint8_t num = numPortIndexEquipment(&e.sw,indexSrc);
        //On sauvegarde l'adresse MAC source si elle n'est pas connue dans la table de commutation
        if (adresseDansTabCommu(&e.sw,&tr->src) == -1) {
            //On ajout l'adresse dans la table de commu
            ajouterCommutation(&e.sw,&tr->src,num);
        }
        //On regarde si la destination est connue pour transmettre
        int indexPortDest = adresseDansTabCommu(&e.sw,&tr->dest);
        if (indexPortDest != -1) {
            //On transmet à la machine sur le bon port
            uint8_t numPortDest = e.sw.tableCommu[indexPortDest].port;
            transmettreTrame(g,tr,e.index,indexEquipmentNumPort(&e.sw,numPortDest));
        }
        else {
            //Sinon Broadcast sur tout les ports excepté celui d'arrivé
            for (size_t i = 0; i < e.sw.nb_port; i++)
            {
                int indexEqTransmission = e.sw.ports[i].indexEquipement;
                if (indexEqTransmission != 0 && indexEqTransmission != indexSrc) {
                    transmettreTrame(g,tr,e.index,indexEqTransmission);
                }   
            }
        }  
    }  
}