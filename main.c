    #include "trame.h"
    #include "adresse.h"
    #include "reseaux.h"


    int main() {
        
        Graphe g;
        construireReseau("mylan_no_cycle.lan",&g);
        

        //mac_addr_t mac1 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x01}}, mac2 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x02}};

        //printf("Affiche -1 : %d", comparer_mac(&mac1,&mac2));
        //printf("Affiche  0 : %d", comparer_mac(&mac2,&mac2));
        //printf("Affiche  1 : %d", comparer_mac(&mac2,&mac1));

       Trame t;
        /*char *message = "hello";
        t.tailleData = strlen(message);
        t.data = malloc(t.tailleData);
        memcpy(t.data, message, t.tailleData);*/

        //Lire_Message_Trame(&t);
        //envoyerMessage(&g,&t,7,14);
        //envoyerMessage(&g,&t,14,7);

        // Affichage
        //affiche_trame(&t);
       

        // Libération mémoire
        //liberer_trame(&t);
        //libererReseau(&g); 

        bool menu = true;
    while (menu) {
        int action = 0;
        printf("\n================ MENU =================\n");
        printf("0 --> Quitter\n");
        printf("1 --> Afficher tous les éléments du réseau\n");
        printf("2 --> Afficher les informations d'un équipement en particulier\n");
        printf("3 --> Envoyer une trame\n");
        printf("======================================\n");
        printf("Quelle action souhaitez-vous réaliser ? ");
        scanf("%d", &action);

        switch (action) {
            case 0:
                // Fermer le programme
                printf("Fermeture du programme...\n");
                liberer_trame(&t);
                libererReseau(&g);
                menu = false;
                break;

            case 1:
                // Afficher tous les éléments du réseau
                afficherGraphe(&g);
                break;

            case 2: {
                // Afficher les informations d’un équipement en particulier
                int index = -1;
                printf("Entrez l'index de l'équipement à afficher (0 à %d) : ", g.nb_equipements - 1);
                scanf("%d", &index);
                if (index >= 0 && index < g.nb_equipements) {
                    afficherEquipement(&g.equipements[index], index);
                } else {
                    printf("Index invalide. Veuillez entrer une valeur entre 0 et %d.\n", g.nb_equipements - 1);
                }
                break;
            }
            case 3:
                // envoyer une trame
                int stationSrc = 0;
                int stationDest = 0;
                char *message = "";
                printf("De quelle machine souhaitez vous envoyer un message ? (0 à %d) : ", g.nb_equipements - 1);
                scanf("%d", &stationSrc);
                printf("A quelle machine souhaitez vous envoyer votre message ? (0 à %d) : ", g.nb_equipements - 1);
                scanf("%d", &stationDest);
                printf("Quel message souhaitez vous envoyer ?");
                scanf("%s", &message);
                envoyerMessage(&g, &t, stationSrc, stationDest, &message);
                Lire_Message_Trame(&t);
                liberer_trame(&t);
                break;

            default:
                printf("Action inconnue. Veuillez choisir une option valide.\n");
                break;
        }
    }

        return 0;
    }


