#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include "imageFilters.h"

int main(){


    FILE *fp2;

    json_t *conf;

    fp2 = fopen("/etc/server/config.conf", "r");

    if(fp2){

        char *fcontent = NULL;
        long size2;

        fseek(fp2, 0, SEEK_END);
        size2 = ftell(fp2);
        fseek(fp2, 0, SEEK_SET);
        fcontent = malloc(size2);
        fread(fcontent, 1, size2, fp2);

        fclose(fp2);

        json_error_t *errors;

        conf = json_loads(fcontent, JSON_DECODE_ANY, errors);

    }


    while(1){

        char direc2[15] = "Images/";
        int processing = 0;

        char smallest[10];
        long size_small = -1;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (direc2)) != NULL) {
        /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                char direc[15] = "Images/";
                strcat(direc,  ent->d_name);
                if(ent->d_type != 4){
                  printf("Entra al if\n");
                  FILE *fp;

                  fp = fopen(direc, "r");

                  long size;

                  fseek(fp, 0, SEEK_END);
                  size = ftell(fp);

                  fclose(fp);

                  if(size_small == -1){
                    strcpy(smallest, ent->d_name);
                    size_small = size;
                    processing = 1;
                  }else{
                    if(size_small > size){
                      strcpy(smallest, ent->d_name);
                      size_small = size;
                      processing = 1;
                    }
                  }
                }
            }
            closedir (dir);
        } else {
        /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }


        if(processing){
          printf("%s\n", smallest);
          printf ("Processing image\n");
          printf("Average\n");
          average_filter("Images/", smallest, json_string_value(json_object_get(conf, "DirAverage")));
          printf("Median\n");
          median_filter("Images/", smallest, json_string_value(json_object_get(conf, "DirMedian")));
          printf("Classifying\n");
          classify("Images/", smallest, json_string_value(json_object_get(conf, "DirColores")));
          strcat(direc2, smallest);
          remove(direc2);
          printf("Image deleted\n");
        }
        sleep(5);
    }
}