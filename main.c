    #include "trame.h"
    #include "adresse.h"
    #include "reseaux.h"


<<<<<<< HEAD
    int main() {
        
        Graphe g;
        construireReseau("mylan_no_cycle.lan",&g);
        
=======
int main() {
    
    Graphe g;
    construireReseau("mylan_no_cycle.lan",&g);
>>>>>>> 87a8be8ab13956f037ca72bfcc0bf519683e1a75

        //mac_addr_t mac1 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x01}}, mac2 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x02}};

        //printf("Affiche -1 : %d", comparer_mac(&mac1,&mac2));
        //printf("Affiche  0 : %d", comparer_mac(&mac2,&mac2));
        //printf("Affiche  1 : %d", comparer_mac(&mac2,&mac1));

        Trame t;
        char *message = "hello";
        t.tailleData = strlen(message);
        t.data = malloc(t.tailleData);
        memcpy(t.data, message, t.tailleData);

        Lire_Message_Trame(&t);
        envoyerMessage(&g,&t,7,14);
        envoyerMessage(&g,&t,14,7);

        // Affichage
        //affiche_trame(&t);
        afficherGraphe(&g);

        // Libération mémoire
        liberer_trame(&t);
        libererReseau(&g);
        return 0;
    }