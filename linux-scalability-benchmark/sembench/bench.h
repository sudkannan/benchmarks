#ifndef _BENCH_H_
#define _BENCH_H_

static int
affinity_set(int cpu)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    return sched_setaffinity(0, sizeof(cpuset), &cpuset);
}

static uint64_t
get_cpu_freq(void)
{
    FILE *fd;
    uint64_t freq = 0;
    float freqf = 0;
    char *line = NULL;
    size_t len = 0;

    fd = fopen("/proc/cpuinfo", "r");
    if (!fd) {
	fprintf(stderr, "failed to get cpu frequecy\n");
	perror(NULL);
	return freq;
    }

    while (getline(&line, &len, fd) != EOF) {
	if (sscanf(line, "cpu MHz\t: %f", &freqf) == 1) {
	    freqf = freqf * 1000000UL;
	    freq = (uint64_t)freqf;
	    break;
	}
    }

    fclose(fd);
    return freq;
}

static uint64_t
read_tsc(void)
{
    uint32_t a, d;
    __asm __volatile("rdtsc" : "=a" (a), "=d" (d));
    return ((uint64_t) a) | (((uint64_t) d) << 32);
}


#endif /* _BENCH_H_ */
