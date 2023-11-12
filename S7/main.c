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


int main(int argc, char **argv){
    myStats stats; 
    struct stat fStat;
    struct dirent *dirent;
    // DIR *dir

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

  //  getData(in_file, &stats, argv[1]);
  //  fPrint(stats, out_file);

    close(in_file);
    close(out_file);
    return 0;
}
