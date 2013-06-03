#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
static void usage(void)
{
    fprintf(stderr,
        "getopt_long -v|-h|-o targtefile\n"
        );
}

struct option long_options[] = {
    {"help", 2, NULL, 'v'},
    {0,      0,    0,  0}
};
int main(int argc, char **argv) 
{
    int opt           = 0;
    int options_index = 0;

    while ((opt = getopt_long(argc, argv, "vho:", long_options, &options_index)) != EOF) {
        switch (opt) {
	    case 'v':
	        printf("verison is 0.1\n");
		break;
	    case '?':
	    case 'h':
	        usage();
		break;
	    case 'o':
	        printf("taget is %s\n", optarg);
		break;
	}
    }


}
