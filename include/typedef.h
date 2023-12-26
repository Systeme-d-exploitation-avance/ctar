// typedef.h

#ifndef TYPEDEF_H
#define TYPEDEF_H

#define BLOCK_SIZE 512

struct header_tar {
    char name[100];         // Nom du fichier
    char mode[8];           // Permissions du fichier en octal
    char uid[8];            // ID utilisateur en octal
    char gid[8];            // ID groupe en octal
    char size[12];          // Taille du fichier en octal
    char mtime[12];         // Dernière modification en octal
    char checksum[8];       // Somme de contrôle
    char typeflag;          // Type du fichier (regular file, directory, etc.)
    char linkname[100];     // Nom du lien symbolique
    char magic[6];          // "ustar" suivi d'un espace et un zéro final
    char version[2];        // Version du format (00)
    char uname[32];         // Nom de l'utilisateur
    char gname[32];         // Nom du groupe
    char devmajor[8];       // Numéro majeur du périphérique
    char devminor[8];       // Numéro mineur du périphérique
    char prefix[155];       // Préfixe du nom du fichier
    char padding[12];       // Remplissage pour atteindre la taille d'un bloc
};

#endif
