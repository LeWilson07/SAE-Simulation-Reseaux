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

void calculer_fcs(const uint8_t *data, size_t taille)
{
    //pour l'algorithme jme suis basé sur la page wikipedia anglaise "Ethernet Frame", plus simplement le CRC32
    uint32_t calculer_fcs(const uint8_t *data, size_t taille) {
    uint32_t crc = 0xFFFFFFFF; // Valeur initiale du CRC
    for (size_t i = 0; i < taille; i++) {
        uint8_t byte = data[i];
        crc ^= byte;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320; // Polynôme CRC32 standard
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc; // Complément à un du CRC
    }
    
int generer_fcs(Trame *tr) {
    if (tr == NULL) {
        fprintf(stderr, "Erreur: Trame NULL\n");
        return -1; // Indique une erreur
    }

    // Calcul de la taille des données à partir du début de la trame jusqu'aux données.
    size_t taille_entete = sizeof(tr->preambule) + sizeof(tr->sfd) + sizeof(tr->dest) + sizeof(tr->src) + sizeof(tr->type);

    // Allocation d'un buffer temporaire pour contenir l'entête et les données.
    uint8_t *buffer_crc = (uint8_t*)malloc(taille_entete + tr->tailleData);
    if (buffer_crc == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return -1;
    }

    // Copie de l'entête dans le buffer,avec un offset pour savoir ou commencerx.
    size_t offset = 0;
    memcpy(buffer_crc + offset, tr->preambule, sizeof(tr->preambule));
    offset += sizeof(tr->preambule);
    memcpy(buffer_crc + offset, &tr->sfd, sizeof(tr->sfd));
    offset += sizeof(tr->sfd);
    memcpy(buffer_crc + offset, &tr->dest, sizeof(tr->dest));
    offset += sizeof(tr->dest);
    memcpy(buffer_crc + offset, &tr->src, sizeof(tr->src));
    offset += sizeof(tr->src);
    memcpy(buffer_crc + offset, tr->type, sizeof(tr->type));
    offset += sizeof(tr->type);

    // Copie des données dans le buffer.
    memcpy(buffer_crc + offset, tr->data, tr->tailleData);

    // Calcul du FCS sur l'entête et les données.
    uint32_t fcs = calculer_fcs(buffer_crc, taille_entete + tr->tailleData);

    // Libération du buffer temporaire.
    free(buffer_crc);
    buffer_crc = NULL;

    // Insertion du FCS dans la trame.
    tr->fcs[0] = (uint8_t)(fcs & 0xFF);
    tr->fcs[1] = (uint8_t)((fcs >> 8) & 0xFF);
    tr->fcs[2] = (uint8_t)((fcs >> 16) & 0xFF);
    tr->fcs[3] = (uint8_t)((fcs >> 24) & 0xFF);

    return 0; // Indique le succès
}   
}
