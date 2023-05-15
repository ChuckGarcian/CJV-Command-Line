//Convert JSON to Video aka CJV

#include <getopt.h>

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getop(argc, argv, "m:")) != -1) {
        switch (opt) {
        case 'm':
            printf("This is the Test output for -m: %s \n", optarg);
            break;
        
        case '?':
            printf("This is a test for unkown input: %s", optopt);
            break;
        }
    }

    for (int i = optind; i < argc; i++) {
        printf("Non-option argument: %s\n", argv[i]);
    }
    printf("\n Final test printing out argv[0]: %s \n", argv[0]);
}