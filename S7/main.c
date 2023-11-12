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

        // getData();
        // close(in_file);
        // close(out_file);

    }

    closedir(dir);

    return 0;
}
