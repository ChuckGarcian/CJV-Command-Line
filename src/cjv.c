#include "cjv.h"

void printUsage(char *argv[]) {
    printf("Usage: %s -f <json file path> -o <output directory file path> \n", argv[0]);
    printf("Options:\n");
    printf("  -h          Print this help message.\n");
    printf("  -o <folder> Output Folder Directory");
    printf("  -f <file>   JSON file path.\n");
    printf("  -n <output>   output folder name.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    int opt;
    char * input_file_path = NULL;
    char * output_file_path = NULL;
    char * folder_name = "outputLectures";
    while ((opt = getopt(argc, argv, "f:h:o:n:")) != -1) {
        switch (opt) {
        case 'f':
            input_file_path = optarg;
            break;
        case 'o':
            output_file_path = optarg;
            break;
        case 'n':
            folder_name = optarg;
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
    /* Make sure that all required command line args were specified */
    if (input_file_path == NULL || output_file_path == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }
    json_Arrays_t * data = json_to_arrays(input_file_path);
    
    // Fatal Error: Jansson was not able to load in the json file
    if (data == NULL) {
        return 1;
    }

    // char * output_file_path = "/home/chuckgarcian/LectureDownloadGoogleExtension/TestExtensions/JSON_VIDEO_CONVERTER";
    // char * folder_name = "testFolder";
    char * final_output = malloc( (strlen(output_file_path) + strlen(folder_name) + 2) * sizeof(char));

    strcpy(final_output, output_file_path);
    strcat(final_output, "/");
    strcat(final_output, folder_name);
    
    mkdir(final_output, 0777);    // Create the output folder with the desired directory   

    char * subtitles_folder = malloc( (strlen(final_output) + strlen("subtitles") + 2) * sizeof(char));

    // Creates a subtitles folder 
    strcpy(subtitles_folder, final_output);
    strcat(subtitles_folder, "/");
    strcat(subtitles_folder, "subtitles");
    mkdir(subtitles_folder, 0777); 

    // Now with the parsed data we can use ffmpeg to download and proccess     
    arrays_to_video(*data, final_output, subtitles_folder);
}
// Debug function to print out the newly extracted links
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
    extracted_data_arrays->size = num_videos;
    // Arrays for the data that we will use later 
   
    // iterates through the json objects and
    // assigns the appriopirate array to the apprioate object property 
    size_t index;
    json_t *value;
    json_array_foreach(root, index, value) {
        extracted_data_arrays->videoURL[index] = json_string_value(json_object_get(value, "videoURL"));
        extracted_data_arrays->captionURL[index] = json_string_value(json_object_get(value, "captionURL"));
        extracted_data_arrays->titles[index] = json_string_value(json_object_get(value, "title"));
       // debug_print_json_element(extracted_data_arrays, index);
        
    }
    return extracted_data_arrays;
}

void replace_spaces_with_underscores(char *str) {
    for (char *c = str; *c != '\0'; c++) {
        if (*c == ' ' || *c == '/') {
            *c = '_';
        }
    }
}

void merge_video_and_captions(char * captionsURL, char * folder_path, char * title) {

   char * subtitles_file = malloc( (strlen(folder_path) + strlen(title) + 4) * sizeof(char));

    strcpy(subtitles_file, folder_path);
    strcat(subtitles_file, "/");
    strcat(subtitles_file, title);
    strcat(subtitles_file, ".srt");
    char command1[2048];
    char *user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3";

    snprintf(command1, sizeof(command1), "ffmpeg -y -user_agent '%s' -i '%s' '%s'", user_agent, captionsURL, subtitles_file);
    system(command1);

    free(subtitles_file);
   // ffmpeg -i input.mp4 -vf "subtitles=https://www.example.com/captions.vtt" output.mp4
  
}

void arrays_to_video(json_Arrays_t data, char * output_path, char * subtitles_folder) {
    // We now need to use ffmpeg for each video url 
    for (int i = 0; i < 1; i++) {
        char *url = data.videoURL[i];

        // We need to create the new output file path 
        char *output = malloc((strlen(output_path) + strlen(data.titles[i]) + 6) * sizeof(char));
        strcpy(output, output_path);    // The folder directory file path
        strcat(output, "/");
        replace_spaces_with_underscores(data.titles[i]);
        strcat(output, data.titles[i]); // the output file name/title
        strcat(output, ".mp4");
        printf("New file path: %s\n", output);

       
        // char subtitles_srt[1024];
        // snprintf(subtitles_srt, sizeof(subtitles_srt), "%s.srt", data.captionURL[i]);
        // //replace_spaces_with_underscores(subtitles_srt);

        // Google Chrome on Windows 10 user agent string
        char *user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3";

        // Google Chrome on Windows 10 user agent string
        char command[1024];
        snprintf(command, sizeof(command), "ffmpeg -y -user_agent '%s' -i '%s' -c copy -bsf:a aac_adtstoasc '%s'", user_agent, url, output);
       // snprintf(command, sizeof(command), "ffmpeg -user_agent '%s' -i '%s' -i '%s' -c copy -scodec mov_text -bsf:a aac_adtstoasc '%s'", user_agent, url, , output);

        int ret = system(command);
        if (ret == -1) {
            perror("system command failed");
            return 1;
        }
        
        // finally we merge the captions with the newly made video
       merge_video_and_captions(data.captionURL[i], subtitles_folder, data.titles[i]);
       free(output);
    }
    printf("DONE\n\n");
}