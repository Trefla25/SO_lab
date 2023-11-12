#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>

# define FS 200 // Field Size 
# define BS 200 // Buffer Size

typedef struct {
    char numeFisier[FS], drUser[FS], drGrup[FS], drOthers[FS];  //  dr -> drepturi 
    int dimensiune, h , L , userId, countLegaturi;
    time_t lastChange;
}myStats; 

void getData(int in, myStats *stat, char *numeFisier) {
    struct stat fileStats;
    int buffer;

    strcpy(stat->numeFisier, numeFisier);

    lseek(in, 18, SEEK_SET);
    read(in, &buffer, 4);
    stat->dimensiune = buffer;

    read(in, &buffer, 4);
    stat->L = buffer;

    read(in, &buffer, 4);
    stat->h = buffer;

    fstat(in, &fileStats);
    stat->countLegaturi = fileStats.st_nlink;
    stat->userId = fileStats.st_uid;
    stat->lastChange = fileStats.st_mtime;

    (S_ISDIR(fileStats.st_mode)) ? strcpy(stat->drUser, "d") : strcpy(stat->drUser, "-");

    (fileStats.st_mode & S_IRUSR) ? strcat(stat->drUser, "r") : strcat(stat->drUser, "-");
    (fileStats.st_mode & S_IWUSR) ? strcat(stat->drUser, "w") : strcat(stat->drUser, "-");
    (fileStats.st_mode & S_IXUSR) ? strcat(stat->drUser, "x") : strcat(stat->drUser, "-");

    (fileStats.st_mode & S_IRGRP) ? strcpy(stat->drGrup, "r") : strcpy(stat->drGrup, "-");
    (fileStats.st_mode & S_IRGRP) ? strcat(stat->drGrup, "w") : strcat(stat->drGrup, "-");
    (fileStats.st_mode & S_IXGRP) ? strcat(stat->drGrup, "x") : strcat(stat->drGrup, "-");

    (fileStats.st_mode & S_IROTH) ? strcpy(stat->drOthers, "r") : strcpy(stat->drOthers, "-");
    (fileStats.st_mode & S_IWOTH) ? strcat(stat->drOthers, "w") : strcat(stat->drOthers, "-");
    (fileStats.st_mode & S_IXOTH) ? strcat(stat->drOthers, "x") : strcat(stat->drOthers, "-");

}

void fPrint(myStat stat, int out) {    // print Stats from file
    char buffer[BS];

    sprintf(buffer, "Nume fisier: %s\n", stat.numeFisier);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Dimensiune fisier: %d\n", stat.dimensiune);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Inaltime imagine: %d\n", stat.h);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Lungime imagine: %d\n", stat.L);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Identificatorul utilizatorului: %d\n", stat.userId);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Contor de legaturi: %d\n", stat.countLegaturi);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Timpul ultimei modificari: %s", ctime(&stat.lastChange));
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s\n", stat.drUser);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s\n", stat.drGrup);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces altii: %s\n", stat.drOthers);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");
}


int main(int argc, char **argv){
    myStats stats; 

    if (argc != 2 || strstr(argv[1], ".bmp") == NULL) {
        printf("Usage ./program <fisier_intrare>\n");
        exit(1);
    }

    int in_file = open("image.bmp", O_RDONLY);

    if (in_file == -1) {
        printf("Couldn t open input file!\n");
        exit(1);
    }

    int out_file = open("statistica.txt", O_WRONLY);

    if (out_file == -1) {
        printf("Couldn t open output file!\n");
        exit(1);
    }

    getData(in_file, &stats, argv[1]);
    fPrint(stats, out_file);

    close(in_file);
    close(out_file);
    return 0;
}