#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>

# define FS 200
# define BS 200 

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

     if (strstr(numeFisier, ".bmp")) {
        read(in, &buffer, 4);
        stat->L = buffer;

        read(in, &buffer, 4);
        stat->h = buffer;
    }
    else {
        stat->L = -1;
        stat->h = -1;
    }

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

     if (stat.h != -1 || stat.L != -1) {
        sprintf(buffer, "h image: %d\n", stat.h);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");

        sprintf(buffer, "L image: %d\n", stat.L);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");
    }

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

    sprintf(buffer, "Drepturi de acces others: %s\n", stat.drOthers);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");
}


int main(int argc, char **argv){
    myStats stats; 
    struct stat fStat;
    struct dirent *dirent;
    DIR *dir

    if (argc != 2) {
        printf("Usage ./program <director intrare>\n");
        exit(1);
    }

    dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Error\n");
        exit(1);
    }

    while ((dirent = readdir(dir)) != NULL) {
        char path[50];
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirent->d_name);
        
        if ((stat(path, &fStat) == -1)) {
            printf("Error\n");
            exit(1);
        }

        if (S_ISREG(fStat.st_mode)) {
            int in = open(path, O_RDONLY);
            
            if (in == -1) {
                printf("Cant open file!\n");
                exit(1);
            }

        getData(in, &stats, dirent->d_name);
        fPrint(stats, out);

        close(in);

        }
        else if (S_ISDIR(fStat.st_mode)) {
            int out = open("statistica.txt", O_WRONLY | O_APPEND);

           //
        }

        close(out);
    }

    closedir(dir);

    return 0;
}
