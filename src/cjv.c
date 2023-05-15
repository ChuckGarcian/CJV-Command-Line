#include "cjv.h"

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

    json_Arrays_t * data = json_to_arrays(file_path);
    
    // Fatal Error: Jansson was not able to load in the json file
    if (data == NULL) {
        return 1;
    }

     
    
}

void debug_print_json_element(json_Arrays_t * data, size_t index) {
    printf("Proccessed Video: %ld \n", index);
    printf("Video URL: %s \n", data->videoURL[index]);
    printf("title: %s \n", data->titles[index]);
    printf("Caption URL: %s \n", data->captionURL[index]);
}


/**
 * Takes in a file path to a json file and returns a jsonns_Array struct pointer
 * with all of the data populated in
*/
json_Arrays_t * json_to_arrays(char * file_path) {
    json_error_t error;
    // Uses janson to load in the json file from the given file path
    json_t *root = json_load_file(file_path, 0, &error);

    if (!root) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return NULL;
    }

    
    // we need the number of videos so that we can 
    // initilize arrays of the appriorite size
    size_t num_videos = json_array_size(root);

    json_Arrays_t* extracted_data_arrays = malloc(sizeof(json_Arrays_t));
    extracted_data_arrays->videoURL = malloc(num_videos * sizeof(char*));
    extracted_data_arrays->captionURL = malloc(num_videos * sizeof(char*));
    extracted_data_arrays->titles = malloc(num_videos * sizeof(char*));
    // Arrays for the data that we will use later 
   

    size_t index;
    json_t *value;
    json_array_foreach(root, index, value) {
        extracted_data_arrays->videoURL[index] = json_string_value(json_object_get(value, "videoURL"));
        extracted_data_arrays->captionURL[index] = json_string_value(json_object_get(value, "captionURL"));
        extracted_data_arrays->titles[index] = json_string_value(json_object_get(value, "title"));
        debug_print_json_element(extracted_data_arrays, index);
        
    }
    return extracted_data_arrays;
}