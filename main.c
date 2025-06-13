#include "trame.h"
#include "adresse.h"
#include "reseaux.h"


int main() {
    
    Graphe g;
    construireReseau("mylan.lan",&g);
    
    //mac_addr_t mac1 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x01}}, mac2 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x02}};
    //printf("Affiche (size_t)-1 :  %ld", comparer_mac(&mac1,&mac2));
    //printf("Affiche  0 :  %ld", comparer_mac(&mac2,&mac2));
    //printf("Affiche  1 :  %ld", comparer_mac(&mac2,&mac1));
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
        printf("4 --> Activer STP Dijkstra\n");
        printf("5 --> Activer STP GodMod (En dev)\n");
        printf("======================================\n");
        printf("Quelle action souhaitez-vous réaliser ? ");
        CHKSSCANF(scanf("%d", &action),1,"Erreur de lecture");

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
                size_t index = -1;
                printf("Entrez l'index de l'équipement à afficher (0 à %ld) : ", g.nb_equipements - 1);
                CHKSSCANF(scanf("%ld", &index),1,"Erreur de lecture");
                if (index < g.nb_equipements) {
                    afficherEquipement(&g.equipements[index], index);
                } else {
                    printf("Index invalide. Veuillez entrer une valeur entre 0 et %ld.\n", g.nb_equipements - 1);
                }
                break;
            }
            case 3:
                // envoyer une trame
                size_t stationSrc;
                size_t stationDest;
                char message[256];  // ou une taille suffisante selon tes besoins

                printf("De quelle machine souhaitez vous envoyer un message ? (0 à %ld) : ", g.nb_equipements - 1);
                CHKSSCANF(scanf("%ld", &stationSrc),1,"Erreur de lecture");
                printf("A quelle machine souhaitez vous envoyer votre message ? (0 à %ld) : ", g.nb_equipements - 1);
                CHKSSCANF(scanf("%ld", &stationDest),1,"Erreur de lecture");
                printf("Quel message souhaitez vous envoyer ?\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Entrez votre message : ");
                if (fgets(message, sizeof(message), stdin) != NULL) {
                    // retirer le \n final éventuel
                    size_t len = strlen(message);
                    if (len > 0 && message[len - 1] == '\n') {
                        message[len - 1] = '\0';
                    }
                }
                envoyerMessage(&g, &t, stationSrc, stationDest, message);
                Lire_Message_Trame(&t);
                //liberer_trame(&t);
                break;
            case 4:
                setupSTPBis(&g);
                printf("STP Dijkstra est actif !");
                break;
            case 5:
                setupSTP(&g);
                printf("STP GodMod est actif !");
                break;
            default:
                printf("Action inconnue. Veuillez choisir une option valide.\n");
                break;
        }
    }
    return 0;
}