/**
 * test.c
 * Small Hello World! example
 * to compile with gcc, run the following command
 * gcc -o test test.c -lulfius
 */
#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8080

int imagecount = 0;

char filenameC[15];

char *logDir;


/**
 * upload a file
 */
int callback_upload_file (const struct _u_request * request, struct _u_response * response, void * user_data) {

  char * string_body = msprintf(filenameC);

  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  char log[120];

  //sprintf(log, "%s%s\n", json_string_value(json_object_get(conf, "DirLog")) ,filenameC);


  FILE *logfile;

  logfile = fopen(logDir,"a");

  sprintf(log, "web  %s  en cola  %s\n", filenameC, asctime (timeinfo));

  int results = fputs(log, logfile);

  fclose(logfile);

  imagecount++;

  ulfius_set_string_body_response(response, 200, string_body);
  o_free(string_body);
  return U_CALLBACK_CONTINUE;
}

/**
 * File upload callback function
 */
int file_upload_callback (const struct _u_request * request, 
                          const char * key, 
                          const char * filename, 
                          const char * content_type, 
                          const char * transfer_encoding, 
                          const char * data, 
                          uint64_t off, 
                          size_t size, 
                          void * cls) {
  y_log_message(Y_LOG_LEVEL_DEBUG, "Got from file '%s' of the key '%s', offset %llu, size %zu, cls is '%s'", filename, key, off, size, cls);
  
  strcpy(filenameC, filename);


  FILE * file;

  char dir[50] = "Images/";

  char number[10];

  sprintf(number, "%d", imagecount);

  strcat(number, ".png");

  strcat(dir, number);

  //strcat(dir, filename);

  file=fopen(dir,"a");
  fwrite(data, size, 1, file);
  fclose(file);

  return U_OK;
}


int callback_zawarudo (const struct _u_request * request, struct _u_response * response, void * user_data) {

  char * string_body = msprintf("Zawarudo!");
  ulfius_set_string_body_response(response, 200, string_body);
  o_free(string_body);
  return U_CALLBACK_CONTINUE;
}

/**
 * main function
 */
int main(void) {


    int port = 1234;

    FILE *fp;

    json_t *conf;    

    fp = fopen("/etc/server/config.conf", "r");

    if(fp){

        char *fcontent = NULL;
        long size;

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fcontent = malloc(size);
        fread(fcontent, 1, size, fp);

        fclose(fp);

        json_error_t *errors;

        conf = json_loads(fcontent, JSON_DECODE_ANY, errors);

        port = json_integer_value(json_object_get(conf, "Port"));

        logDir = json_string_value(json_object_get(conf, "DirLog"));

        strcat(logDir, "log.txt");
    }

    struct _u_instance instance;


    // Initialize instance with the port number
    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return(1);
    }

    // Max post param size is 500 Kb, which means an uploaded file is no more than 16 Kb
    instance.max_post_param_size = 500*1024;
  
    if (ulfius_set_upload_file_callback_function(&instance, &file_upload_callback, "my cls") != U_OK) {
        y_log_message(Y_LOG_LEVEL_ERROR, "Error ulfius_set_upload_file_callback_function");
    }

    // Endpoint list declaration
    ulfius_add_endpoint_by_val(&instance, "*", "/upload", NULL, 1, &callback_upload_file, NULL);
    ulfius_add_endpoint_by_val(&instance, "get", "/", NULL, 1, &callback_zawarudo, NULL);


    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);
    }

    while(1);

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
}