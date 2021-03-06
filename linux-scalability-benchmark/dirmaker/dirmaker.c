#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>

#include "config.h"
#include "bench.h"

volatile uint64_t count = 0;
int nr_threads = 2;

static int
create_dir(const char *name)
{
    int fd;
    
    if ((fd = open(name, O_RDONLY)) < 0) {
        if ((fd = mkdir(name, S_IRUSR | S_IWUSR | S_IXUSR)) < 0) {
            /* perror(""); */
            fd = open(name, O_RDONLY);
        } else {
            /* printf("%s created.\n", name); */
            count ++;
        }
    } else {
    }

    close(fd);

    return fd;
}

void *
worker(void *args)
{
    int id = (long) args;
    int l1n, l2n, l3n;
    char dirname[1024];

    affinity_set(id);

    sprintf(dirname, "%s%d", PATH_PREFIX, id);
    create_dir(dirname);
    
    for (l1n = 0; l1n < NR_SUBDIRS; l1n++) {
        sprintf(dirname, "%s%d/%d", PATH_PREFIX, id, l1n);
        create_dir(dirname);
        for (l2n = 0; l2n < NR_SUBDIRS; l2n++) {
            sprintf(dirname, "%s%d/%d/%d", PATH_PREFIX, id, l1n, l2n);
            create_dir(dirname);
            for (l3n = 0; l3n < NR_SUBDIRS; l3n++) {
                sprintf(dirname, "%s%d/%d/%d/%d", PATH_PREFIX, id, l1n, l2n, l3n);
                create_dir(dirname);
            }
        }
    }

    return NULL;
}

int
main(int argc, char **argv)
{
    pthread_t tid[32];
    int i;
    uint64_t start, end, usec;

    if (argc > 1) {
        nr_threads = atoi(argv[1]);
    }
    
    printf("Create directories...\n");
    start = read_tsc();
    create_dir(PATH_PREFIX);
    for (i = 0; i < nr_threads; i++) {
        pthread_create(&tid[i], NULL, worker, (void *) (long) i);
    }

    for (i = 0; i < nr_threads; i++) {
        pthread_join(tid[i], NULL);
    }
    
    end = read_tsc();
    usec = (end - start) * 1000000 / get_cpu_freq();
    printf("usec: %ld\t\n", usec);

    printf("Cleanup directories...\n");
    system("rm -rf /tmp/_dirs");
    
    return 0;
}
