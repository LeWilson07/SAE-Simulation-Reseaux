#include "trame.h"


void affiche_octets(const uint8_t *octets, size_t taille) {
    for (size_t i = 0; i < taille; i++) {
        printf("%02X ", octets[i]);
    }
    printf("\n");
}


void affiche_trame(Trame const *tr){
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
void Lire_Message_Trame(Trame const *tr){
    //permet de lire le message d'une trame
    printf("Données (Message) : ");
    if (tr->data != NULL && tr->tailleData > 0) {
        // Ajout d’un caractère nul pour imprimer en tant que chaîne
        char *message = malloc(tr->tailleData + 1);
        if (message == NULL) {
            printf("Erreur d'allocation mémoire pour le message.\n");
            return;
        }
        memcpy(message, tr->data, tr->tailleData);
        message[tr->tailleData] = '\0';
        printf("%s\n", message);
        free(message);
    } else {
        printf("(aucune donnée)\n");
    }
}

void liberer_trame(Trame *tr){
    free(tr->data);
}