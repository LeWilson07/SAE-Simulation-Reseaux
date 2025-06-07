#include "trame.h"


void affiche_octets(const uint8_t *octets, size_t taille) {
    for (size_t i = 0; i < taille; i++) {
        printf("%02X ", octets[i]);
    }
    printf("\n");
}


void affiche_trame(trame const *tr){
    printf("=== TRAME ===\n");

    printf("Preambule     : ");
    affiche_octets(tr->preambule, 7);

    printf("Dest MAC      : ");
    affiche_mac(&tr->dest);
    printf("\n");

    printf("Src MAC       : ");
    affiche_mac(&tr->src);
    printf("\n");  
     
    printf("Type          : ");
    affiche_octets(tr->type, 2);

    printf("Données (%d octets) : ", tr->tailleData);
    affiche_octets(tr->data, tr->tailleData);

    printf("FCS           : ");
    affiche_octets(tr->fcs, 4);
}

void liberer_trame(trame *tr){
    free(tr->data);
}