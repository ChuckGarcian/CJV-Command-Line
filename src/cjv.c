#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>
void printUsage(char *argv[]) {
    printf("Usage: %s -f <json file path> \n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -f <file>  JSON file path.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    int opt;
    char * file_path;
    while ((opt = getopt(argc, argv, "f:h:")) != -1) {
        switch (opt) {
        case 'f':
            file_path = optarg;
            break;
        
        case 'h':
            printUsage(argv);
            break;
         default:
            printUsage(argv);
            exit(1);
            break;
        }
    }
    
    json_error_t error;
    // Uses janson to load in the json file from the given file path
    json_t *root = json_load_file(file_path, 0, &error);

    if (!root) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }

    
    // we need the number of videos so that we can 
    // initilize arrays of the appriorite size
    size_t num_videos = json_array_size(root);
    
    // Arrays for the data that we will use later 
    char **videoURL = malloc(num_videos * sizeof(char *));
    char **captionURL = malloc(num_videos * sizeof(char *));
    char **titles = malloc(num_videos * sizeof(char *));

    size_t index;
    json_t *value;
    json_array_foreach(root, index, value) {
        if (!json_is_object(json_object_get(value, "videoURL"))) {
        fprintf(stderr, "error: root is not an object\n");
        json_decref(root);
        return 1;
    }
        printf("This is index: %ld \n", index);
        printf("This is the videoURL string: %s \n", json_string_value(json_object_get(value, "videoURL")));
    }
}