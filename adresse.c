#include "adresse.h"

void affiche_ip(const ip_addr_t *ip)
{
    printf("%d",ip->addr[0]);
    for (size_t i = 1; i < 4; i++)
    {
        printf(".");
        printf("%d",ip->addr[i]);
    }
}

void affiche_mac(const mac_addr_t *mac)
{
    printf("%02X",mac->addr[0]);
    for (size_t i = 1; i < 6; i++)
    {
        printf(":");
        printf("%02X",mac->addr[i]);
    }
}

char comparer_mac(const mac_addr_t *mac1, const mac_addr_t *mac2){
    //La fonction retourne 0 si les adresses MAC sont similaires
    //1 Si la mac1 est après mac2 et -1 si l'inverse
    return 0;
}