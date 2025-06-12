#include "reseaux.h"

void construireReseau(char const *path, Graphe *g) {
    size_t nbEquipement;
    size_t nbLiens;
    char bufferLigne[256];
    FILE *f;
    CHKNULL(f = fopen(path, "r")); //Ouverture du fichier
    CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture de la première ligne
    //Affectaion du nombre d'équipement et de lien
    CHKSSCANF(sscanf(bufferLigne, "%ld %ld", &nbEquipement, &nbLiens), 2, \
        "Erreur : Nb de Equipement ou Nb de Lien\n");
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
    CHKNULL(g->matrice_adjacence = malloc(nbEquipement*nbEquipement * sizeof(size_t)));
    //Inisialisation à la valeur max (Laisse les poids de 0 possible)
    for (size_t i = 0; i < nbEquipement*nbEquipement; i++)
        *(g->matrice_adjacence + i) = UINT8_MAX;
    
    //Enregistrement des liens
    for (size_t i = 0; i < nbLiens; i++)
    {
        size_t s1, s2, poids;
        CHKNULL(fgets(bufferLigne, sizeof(bufferLigne), f)); //Lecture d'une ligne
        CHKSSCANF(sscanf(bufferLigne," %ld; %ld; %ld", &s1, &s2, &poids),3,
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

void construirePort(Graphe *g, size_t s1, size_t s2){
    //On va mettre sur le premier port disponible de l'équipement l'index l'autre.
    Equipement *e = &g->equipements[s1];
    switch (e->type)
    {
    case STATION_TYPE:
        if (e->station.port.indexEquipement == (size_t)-1){
            e->station.port.indexEquipement = s2;
        }
        else printf("L'équipement  %ld n'a pas assez "
            "de port pour cette configuration\n", s1);
        break;

    case SWITCH_TYPE:
        size_t i = 0;
        while (i < e->sw.nb_port && e->sw.ports[i].indexEquipement != (size_t)-1) i++;
        if(i < e->sw.nb_port)
            e->sw.ports[i].indexEquipement = s2;
        else printf("L'équipement  %ld n'a pas assez "
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
    size_t offset;

    //Parse du Type d'équipement
    size_t type;
    CHKSSCANF(sscanf(ligne, " %ld;%ln", &type, &offset),1,
        "Erreur : Scan du type d'équipement");
    char *rest = ligne + offset;
    e.type = type - 1; //Correspond au type de l'enum

    //Parse de l'adresse MAC de l'équipement
    CHKSSCANF(sscanf(rest, "%lx:%lx:%lx:%lx:%lx:%lx;%ln",
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
        CHKSSCANF(sscanf(rest, "%zu.%zu.%zu.%zu",
            &e.station.ip.addr[0], &e.station.ip.addr[1],
            &e.station.ip.addr[2], &e.station.ip.addr[3]),
            4, "Erreur de lecture de l'adresse IP\n");
        //Mise en place du port
        e.station.port.indexEquipement = (size_t)-1;
        break;

    case SWITCH_TYPE:
        //Parse du nombre de port
        CHKSSCANF(sscanf(rest,"%zu;%ln",&e.sw.nb_port, &offset),1,
        "Erreur dans la lecture du nombre de port\n");
        rest = rest + offset;
        //Parse priorité
        CHKSSCANF(sscanf(rest,"%zu",&e.sw.priorite),1,
        "Erreur dans la lecture du nombre de la priorité\n");
        //Allocation de la table de commutation
        e.sw.nb_commu = 0;
        e.sw.commu_capacite = NOMBRE_COMMUTATION_DEFAUT; //Valeur par défaut
        CHKNULL(e.sw.tableCommu = calloc(NOMBRE_COMMUTATION_DEFAUT, \
            sizeof(Commutation)));
        //Allocation de tous les ports du switch
        CHKNULL(e.sw.ports = calloc(e.sw.nb_port,sizeof(Port)));
        for (size_t i = 0; i < e.sw.nb_port; i++) {
            e.sw.ports[i].num = i+1;
            e.sw.ports[i].indexEquipement = (size_t)-1;
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

void ajouterCommutation(Switch *sw,  mac_addr_t const *mac, size_t indexPort){
    //Ajout de place dans la table de commutation si nécessaire
    if (sw->nb_commu == sw->commu_capacite){
        CHKNULL(sw->tableCommu = realloc(sw->tableCommu,sw->commu_capacite * \
            2 * sizeof(Commutation)));
        sw->commu_capacite *= 2;
    }
    //Enregistrement de la commutation
    (sw->tableCommu + sw->nb_commu)->port = indexPort;
    (sw->tableCommu + sw->nb_commu)->adresse_mac = *mac;
    sw->nb_commu++;
}

void afficherPortSwitch(Switch const *sw){
    printf("\e[32m├─────────────────────────────────────┤\e[0m\n");
    printf("\e[32m│\e[0m           \e[35mEtat des ports\e[0m            \e[32m│\e[0m\n");
    printf("\e[32m├─────────────────────────────────────┤\e[0m\n");

    for (size_t i = 0; i < sw->nb_port; i++)
    {
        //if(sw->ports[i].indexEquipement > 9 || sw->ports[i].indexEquipement < 0)
        if(sw->ports[i].indexEquipement > 9){
            printf("\e[32m│\e[0m  \e[34mPort\e[0m : %ld → Machine : %ld            \e[32m│\e[0m\n", sw->ports[i].num, 
            sw->ports[i].indexEquipement);
        }
        else{
            printf("\e[32m│\e[0m  \e[34mPort\e[0m : %ld → Machine : %ld             \e[32m│\e[0m\n", sw->ports[i].num, 
            sw->ports[i].indexEquipement);
        }
        
    }
    printf("\e[32m└─────────────────────────────────────┘\e[0m\n");
    
}

void afficherTableCommutation(Switch const *sw) {
    // S'occupe de l'affichage d'une table de commutation
    printf("\e[32m├─────────────────────────────────────┤\e[0m\n");
    printf("\e[32m│\e[0m  \e[35mTable de commutation (%ld entrées)\e[0m   \e[32m│\e[0m\n", sw->nb_commu);
    printf("\e[32m├─────────────────────────────────────┤\e[0m\n");

    for (size_t i = 0; i < sw->nb_commu; i++) {
        printf("\e[32m│\e[0m  \e[93mMAC\e[0m : ");
        affiche_mac(&sw->tableCommu[i].adresse_mac);
        printf(" → Port : %ld \e[32m│\e[0m\n", sw->tableCommu[i].port);
    }
    printf("\e[32m│                                     │\e[0m\n");

}

void afficherEquipement(Equipement const *e, size_t const index) {
    //S'occupe de l'affichage d'un équipement
    printf("\e[32m┌─────────────────────────────────────┐\e[0m\n");
    printf("\e[32m│\e[0m    \e[35mÉquipement %ld :\e[0m ", index);
    switch (e->type)
    {
    case STATION_TYPE:
        if(index > 9){
            printf("\e[36mStation\e[0m          \e[32m│\e[0m\n");
        }
        else{
            printf("Station           \e[32m│\e[0m\n");
        }
        printf("\e[32m├─────────────────────────────────────┤\e[0m\n");
        printf("\e[32m│\e[0m  \e[93mMAC\e[0m : ");
        affiche_mac(&e->mac);
        printf("            \e[32m│\e[0m");
        printf("\n\e[32m│\e[0m  \e[91mIP\e[0m  : ");
        affiche_ip(&e->station.ip);
        printf("                  \e[32m│\e[0m\n");
        printf("\e[32m└─────────────────────────────────────┘\e[0m\n");
        break;

    case SWITCH_TYPE:
        printf("\e[33mSwitch\e[0m            \e[32m│\e[0m\n");
        printf("\e[32m├─────────────────────────────────────┤\e[0m\n");
        printf("\e[32m│\e[0m  \e[93mMAC\e[0m : ");
        affiche_mac(&e->mac);
        printf("            \e[32m│\e[0m");
        printf("\n\e[32m│\e[0m  \e[94mNombre de ports\e[0m : %ld                \e[32m│\e[0m\n", e->sw.nb_port);
        printf("\e[32m│\e[0m  \e[95mPriorité\e[0m : %ld                    \e[32m│\e[0m\n", e->sw.priorite);
        printf("\e[32m│\e[0m                                     \e[32m│\e[0m\n");
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
    printf("Nombre d'équipements :  %ld\n\n", g->nb_equipements);

    for (size_t i = 0; i < g->nb_equipements; i++) {
        afficherEquipement(g->equipements + i, i);
    }
    printf("\n");
    //afficherMatriceAdja(g);
}

void afficherMatriceAdja(Graphe const *g){
    size_t n = g->nb_equipements;
    printf("=== Matrice d'adjacence ===\n");
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            printf(" %ld\t", g->matrice_adjacence[i * n + j]);
        }
        printf("\n");
    }
}



size_t adresseDansTabCommu(Switch const *sw, mac_addr_t const *mac){
    //Retourne l'index si elle existe sinon (size_t)-1
    for (size_t i = 0; i < sw->nb_commu; i++) {
        if (comparer_mac(&sw->tableCommu[i].adresse_mac, mac) == 0) {
            return i; // Adresse trouvée, on retourne son index
        }
    }
    return (size_t)-1; // Adresse non trouvée
}

size_t numPortIndexEquipment(Switch const *sw, size_t index){
    //Retourne le numéro du port avec l'index en paramètre, (size_t)-1 si size_trouvable
    for (size_t i = 0; i < sw->nb_port; i++)
        if (sw->ports[i].indexEquipement == index) return sw->ports[i].num;
    return (size_t)-1;
}

size_t indexEquipmentNumPort(Switch const *sw, size_t numPort){
    //Retourne l'index de l'equipement derrière le port en paramètre, (size_t)-1 si size_trouvable
    for (size_t i = 0; i < sw->nb_port; i++)
        if (sw->ports[i].num == numPort) return sw->ports[i].indexEquipement;
    return (size_t)-1;
}

void transmettreTrame(Graphe *g, Trame const *tr, size_t indexSrc, size_t indexCourant, size_t PTL){ //PTL Profondeur to live
    // Vérifier 
    Equipement* e = &g->equipements[indexCourant];
    // Vérifier si l'adresse MAC Courant est celle de destination
    if (comparer_mac(&e->mac,&tr->dest) == 0) {
        printf("\033[1;31mLa trame est arrivé à destination\033[0m\n") ;
        return;
    }
    // On se prépare à transmettre (Si c'est un switch)
    if (e->type == SWITCH_TYPE)
    {
        
        //On récupère le port d'arriver
        size_t num = numPortIndexEquipment(&e->sw,indexSrc);
        //On sauvegarde l'adresse MAC source si elle n'est pas connue dans la table de commutation
        if (adresseDansTabCommu(&e->sw,&tr->src) == (size_t)-1) {
            //On ajout l'adresse dans la table de commu
            ajouterCommutation(&e->sw,&tr->src,num);
        }
        //On regarde si la destination est connue pour transmettre
        size_t indexPortDest = adresseDansTabCommu(&e->sw,&tr->dest);
        if (indexPortDest != (size_t)-1) {
            //On transmet à la machine sur le bon port
            size_t numPortDest = e->sw.tableCommu[indexPortDest].port;
            transmettreTrame(g,tr,e->index,indexEquipmentNumPort(&e->sw,numPortDest), PTL--);
        }
        else {
            //Sinon Broadcast sur tout les ports excepté celui d'arrivé
            for (size_t i = 0; i < e->sw.nb_port; i++)
            {
                size_t indexEqTransmission = e->sw.ports[i].indexEquipement;
                if (indexEqTransmission != (size_t)-1 && indexEqTransmission != indexSrc) {
                    transmettreTrame(g,tr,e->index,indexEqTransmission, PTL--);
                }   
            }
        }  
    }  
}

void envoyerMessage(Graphe *g, Trame *t, size_t stationSrc, size_t stationDest){
    if (stationSrc >= g->nb_equipements || stationDest >= g->nb_equipements) {
        printf("La station n'existe pas\n");
        return;
    }

    if (g->equipements[stationSrc].type != STATION_TYPE){
        printf("L'équipement source n'est pas une station !\n");
        return;
    }

    t->src = g->equipements[stationSrc].mac;
    t->dest = g->equipements[stationDest].mac;

    for (size_t i = 0; i < 7; i++) {
        t->preambule[i] = 0xAA;
    }
    t->sfd = 0xAB;

    t->type[0] = 0x08;
    t->type[1] = 0x06;

    t->fcs[0] = 0xDE;
    t->fcs[1] = 0xAD;
    t->fcs[2] = 0xBE;
    t->fcs[3] = 0xEF;
    
    //Transmission de la trame
    size_t indexEquipPort = g->equipements[stationSrc].station.port.indexEquipement;
    transmettreTrame(g,t,stationSrc,indexEquipPort,g->nb_equipements);
}

void transmettreBPDU(Graphe *g, size_t indexSrc, size_t indexDest, BPDU bpdu){
    Equipement* e = &g->equipements[indexDest];
    if (e->type == SWITCH_TYPE){
        size_t num = numPortIndexEquipment(&e->sw,indexSrc);
        e->sw.ports[num-1].bpdu = bpdu;
    }
}

int comparer_BPDU(BPDU bpdu1, BPDU bpdu2)
{
    int cmp = comparer_mac(&bpdu1.RootID, &bpdu2.RootID);
    if (cmp != 0){
        return cmp;
    } 

    if (bpdu1.cout != bpdu2.cout){
        return (bpdu1.cout < bpdu2.cout) ? -1 : 1;
    }
    return comparer_mac(&bpdu1.mac, &bpdu2.mac);
}


/**/
char comparerBPDU(BPDU const bpdu1, BPDU const bpdu2){
    //Retourne (size_t)-1 si bpdu1 est moins bon que bpdu sinon 1
    //Pas besoins de traiter le cas égal car il n'existe pas
    //Utiliser comparer mac
    (void)bpdu1;
    (void)bpdu2;
    // TODO: implement comparison
    return 0;
}

void setupSTP(Graphe *g){
    //Initialisation du protocole STP (Création et envoie des BPDU par chaque Switch)
    for (size_t i = 0; i < g->nb_equipements; i++){
        Equipement *e = &g->equipements[i];
        if (e->type == SWITCH_TYPE){
            //BPDU par défaut (Chaque Switch se croit racine)
            e->sw.meilleur_bpdu.RootID = e->mac;
            e->sw.meilleur_bpdu.cout = 0;
            e->sw.meilleur_bpdu.mac = e->mac;
            //Puis envoi son BPDU sur tout les ports
            for (size_t i = 0; i < e->sw.nb_port; i++)
            {
                if (e->sw.ports[i].indexEquipement != (size_t)-1){
                    transmettreBPDU(g,e->index,e->sw.ports[i].indexEquipement, e->sw.meilleur_bpdu);
                }
            }
        }
    }
    //Trouver la convergence de l'arbre STP
    char changement;
    do
    {
        changement = 0;
        for (size_t i = 0; i < g->nb_equipements; i++) //Ajouter une Macro Sympa style foreach
        {
            if (g->equipements[i].type == SWITCH_TYPE)
            {
                Switch * sw = &g->equipements[i].sw;
                for (size_t i = 0; i < sw->nb_port; i++) //Enlever les ports avec de sations
                {
                    if (comparerBPDU(sw->meilleur_bpdu, sw->ports[i].bpdu) == -1){
                        sw->meilleur_bpdu = sw->ports[i].bpdu;
                        changement = 1;
                        //Propager le changement ?
                    }
                    
                }
            }      
        }
        //Mise à jour des ports des switch
        for (size_t i = 0; i < g->nb_equipements; i++)
        {
            Equipement *e = &g->equipements[i];
            if (e->type == SWITCH_TYPE)
            {
                /* code */
            }
            
        }
    } while (changement);
}