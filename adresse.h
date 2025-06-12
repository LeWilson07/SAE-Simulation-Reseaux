#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Adresse MAC
typedef struct {
    size_t addr[6];
} mac_addr_t;

//Adresse IP
typedef struct {
    size_t addr[4];
} ip_addr_t;

void affiche_ip(ip_addr_t const *ip);
void affiche_mac(mac_addr_t const *mac);

char comparer_mac(mac_addr_t const *mac1, mac_addr_t const *mac2);