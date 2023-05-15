#include <stdlib.h>
#include <getopt.h>

void printUsage(char *argv[]) {
    printf("Usage: %s -f <json file path> \n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -f <file>  JSON file path.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
        case 'f':
            printf("This is the Test output for -m: %s \n", optarg);
            break;
        
        case 'h':
            printUsage(argv);
            break;
         default:
            printUsage(argv);
            exit(1);
        }
    }

    for (int i = optind; i < argc; i++) {
        printf("Non-option argument: %s\n", argv[i]);
    }
   // printf("\n Final test printing out argv[0]: %s \n", argv[0]);
}