#pragma once

#include <stdint.h>
#include <stdio.h>

//Adresse MAC
typedef uint8_t mac_addr_t[6];
//Adresse IP
typedef uint8_t ip_addr_t[4];

void affiche_ip(ip_addr_t const *ip);
void affiche_mac(mac_addr_t const *mac);