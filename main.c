#include "trame.h"
#include "adresse.h"
#include "reseaux.h"


int main() {
    
    Graphe g;
    construireReseau("mylan_no_cycle.lan",&g);
    //construireReseau("mylan_no_cycle.lan",&g);
    

    //mac_addr_t mac1 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x01}}, mac2 = {{0x01, 0x45, 0x23, 0xA6, 0xF7, 0x02}};

    //printf("Affiche -1 : %d", comparer_mac(&mac1,&mac2));
    //printf("Affiche  0 : %d", comparer_mac(&mac2,&mac2));
    //printf("Affiche  1 : %d", comparer_mac(&mac2,&mac1));

    Trame t;

    // Remplir le préambule avec une valeur fixe
    for (int i = 0; i < 7; i++) {
        t.preambule[i] = 0xAA;
    }

    // Type arbitraire
    t.type[0] = 0x08;
    t.type[1] = 0x06;

    // Données : chaîne de texte transformée en octets
    const char* message = "Hello, world!";
    t.tailleData = strlen(message);
    t.data = malloc(t.tailleData);
    memcpy(t.data, message, t.tailleData);

    // FCS arbitraire
    t.fcs[0] = 0xDE;
    t.fcs[1] = 0xAD;
    t.fcs[2] = 0xBE;
    t.fcs[3] = 0xEF;

    //Ajout des MAC
    // 54:d6:a6:82:c5:02
    // 54:d6:a6:82:c5:07
    //mac_addr_t mac1 = {{0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x02}};
    //mac_addr_t mac2 = {{0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x07}};


    // 54:d6:a6:82:c5:23
    // c8:69:72:5e:43:af
    mac_addr_t mac1 = {{0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x23}};
    mac_addr_t mac2 = {{0xC8, 0x69, 0x72, 0x5E, 0x43, 0xAF}};

    t.src = mac1;
    t.dest = mac2;

    //Envoyer trame
    transmettreTrame(&g, &t, 1, 0);

    // Affichage
    //affiche_trame(&t);
    afficherGraphe(&g);

    // Libération mémoire
    liberer_trame(&t);
    libererReseau(&g);
    return 0;
}