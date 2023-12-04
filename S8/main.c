#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>

# define FS 200 // Field Size 
# define BS 200 // Buffer Size

# define DATA_OFFSET 10

typedef struct {
    char numeFisier[FS], drUser[FS], drGrup[FS], drOthers[FS];  //  dr -> drepturi 
    int dimensiune, h , L , userId, countLegaturi;
    time_t lastChange;
}myStat;

void getData(int in, myStat *stat, char *numeFisier) {
    struct stat fileStat;
    int buffer;

    strcpy(stat->numeFisier, numeFisier);
    lseek(in, 18, SEEK_SET);


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

    fstat(in, &fileStat);
    stat->countLegaturi = fileStat.st_nlink;
    stat->userId = fileStat.st_uid;
    stat->lastChange = fileStat.st_mtime;
    stat->dimensiune = fileStat.st_size;

    (S_ISDIR(fileStat.st_mode)) ? strcpy(stat->drUser, "d") : strcpy(stat->drUser, "-");
    (fileStat.st_mode & S_IRUSR) ? strcat(stat->drUser, "r") : strcat(stat->drUser, "-");
    (fileStat.st_mode & S_IWUSR) ? strcat(stat->drUser, "w") : strcat(stat->drUser, "-");
    (fileStat.st_mode & S_IXUSR) ? strcat(stat->drUser, "x") : strcat(stat->drUser, "-");

    (fileStat.st_mode & S_IRGRP) ? strcpy(stat->drGrup, "r") : strcpy(stat->drGrup, "-");
    (fileStat.st_mode & S_IRGRP) ? strcat(stat->drGrup, "w") : strcat(stat->drGrup, "-");
    (fileStat.st_mode & S_IXGRP) ? strcat(stat->drGrup, "x") : strcat(stat->drGrup, "-");

    (fileStat.st_mode & S_IROTH) ? strcpy(stat->drOthers, "r") : strcpy(stat->drOthers, "-");
    (fileStat.st_mode & S_IWOTH) ? strcat(stat->drOthers, "w") : strcat(stat->drOthers, "-");
    (fileStat.st_mode & S_IXOTH) ? strcat(stat->drOthers, "x") : strcat(stat->drOthers, "-");

}

void fprintStat(myStat *stat, int out) {
    char buffer[BS];

    sprintf(buffer, "Nume fisier: %s\n", stat->numeFisier);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Dimensiune fisier: %d\n", stat->dimensiune);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    if (stat->h != -1 || stat->L != -1) {
        sprintf(buffer, "h imagine: %d\n", stat->h);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");

        sprintf(buffer, "L imagine: %d\n", stat->L);
        write(out, buffer, strlen(buffer));
        strcpy(buffer, "");
    }

    sprintf(buffer, "Identificatorul utilizatorului: %d\n", stat->userId);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Contor de legaturi: %d\n", stat->countLegaturi);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Timpul ultimei modificari: %s", ctime(&stat->lastChange));
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s\n", stat->drUser);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s\n", stat->drGrup);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces others: %s\n\n", stat->drOthers);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");
}

void fprintDirStat(int out, struct stat *fileStat, char *dirName) {
    char buffer[BS];

    sprintf(buffer, "Nume director: %s\n", dirName);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Indentificatorul utilizatorului: %d\n", fileStat->st_uid);
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces user: %s%s%s\n", (fileStat->st_mode & S_IRUSR) ? "r" : "-", (fileStat->st_mode & S_IWUSR) ? "w" : "-", (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces grup: %s%s%s\n", (fileStat->st_mode & S_IRGRP) ? "r" : "-", (fileStat->st_mode & S_IWGRP) ? "w" : "-", (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

    sprintf(buffer, "Drepturi de acces altii: %s%s%s\n\n", (fileStat->st_mode & S_IROTH) ? "r" : "-", (fileStat->st_mode & S_IWOTH) ? "w" : "-", (fileStat->st_mode & S_IXOTH) ? "x" : "-");
    write(out, buffer, strlen(buffer));
    strcpy(buffer, "");

}

void fprintSymLnkStat() {
    char buffer[BS];
}

void createDirEntityFilePath(char *entName, char *outputRelativePath, char *outputDir) {
    char fileName[BS];

    if (strcmp(entName, ".") == 0) {
            strcpy(fileName, "current_dir_statistic.txt");
        }
        else if (strcmp(entName, "..") == 0) {
            strcpy(fileName, "previous_dir_statistic.txt");
        }
        else {
            char fileNameCopy[BS];
            strcpy(fileNameCopy, entName);

            char *token = strtok(fileNameCopy, ".");
            strcpy(fileName, token);
            strcat(fileName, "_statistic.txt");
        }

        strcpy(outputRelativePath, outputDir);
        strcat(outputRelativePath, fileName);
}

void wLines(int out, int writtenLines, char *fileName) {
    char buffer[BS];
    sprintf(buffer, "S au scris %d linii in %s.\n", writtenLines, fileName);
    write(out, buffer, strlen(buffer));
}

int countLines(char *filePath) {
    FILE *in;
    if((in = fopen(filePath, "r")) == NULL) {
        printf("countlines in %s failed!\n", filePath);
        exit(1);
    }
    int lines = 0;

    char buffer[BS];
    while(fgets(buffer, BS, in) != NULL) {
        lines++;
    }
    
    return lines - 1;
}

int getRasterDataStartAddress(int bmpIn) {
}

void toBlackAndWhite(int bmpIn, int width, int heigth) {
    unsigned char bP, gP, rP; // blue , green and red Pixel
    
    int rasterStart = getRasterDataStartAddress(bmpIn);
    lseek(bmpIn, rasterStart, SEEK_SET);

    for (int i = 0; i < (width * heigth); i++){
        read(bmpIn, &bP, 1) ? : printf("error\n");
        read(bmpIn, &gP, 1) ? : printf("error\n");
        read(bmpIn, &rP, 1) ? : printf("error\n");

        unsigned char greyVal = 0.114 * bP + 0.587 * gP + 0.299 * rP;

        lseek(bmpIn, -3, SEEK_CUR);

        write(bmpIn, &greyVal, 1) ? : printf("error\n");
        write(bmpIn, &greyVal, 1) ? : printf("error\n");
        write(bmpIn, &greyVal, 1) ? : printf("error\n");
    }
}

void parseDir() {
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage ./a.out <director_intrare> <director_iesire>\n");
        exit(1);
    }

    struct dirent *dirent = NULL;
    DIR *dir = opendir("input_dir");

    if (dir == NULL) {
        printf("Could not open dir!\n");
        exit(1);
    }

    struct stat fileStat;

    closedir(dir);

    return 0;
}