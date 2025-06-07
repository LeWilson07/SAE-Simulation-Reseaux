#pragma once

#include <stdint.h>
#include <stdio.h>

//Adresse MAC
typedef struct {
    uint8_t addr[6];
} mac_addr_t;

//Adresse IP
typedef struct {
    uint8_t addr[4];
} ip_addr_t;

void affiche_ip(const ip_addr_t *ip);
void affiche_mac(const mac_addr_t *mac);

char comparer_mac(const mac_addr_t *mac1, const mac_addr_t *mac2);