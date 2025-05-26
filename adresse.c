#include "adresse.h"

void affiche_ip(ip_addr_t const *ip)
{
    printf("%d",*ip);
    for (size_t i = 1; i < 4; i++)
    {
        printf(".");
        printf("%d",*(ip + i));
    }
}

void affiche_mac(mac_addr_t const *mac)
{
    printf("%X",*mac);
    for (size_t i = 1; i < 6; i++)
    {
        printf(":");
        printf("%02X",*(mac + i));
    }
}