#include "trame.h"
#include "adresse.h"
#include "reseaux.h"


int main() {
    Graphe g;
    construireReseau("test.lan",&g);
    libererReseau(&g);
    /*
    trame t;

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

    // Affichage
    affiche_trame(t);

    // Libération mémoire
    free(t.data);
    */
    return 0;
}