/*
 * main.c
 *
 *  Created on: Aug 11, 2016
 *      Author: shawn
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>



pthread_t tid[100];

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp() {
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void *gotomax(void *data)
{
	int a = 0;
	while (a != INT32_MAX) {
		a++;
//		if (a % 300000000 == 0)
//			printf("tick\n");
	}
//	printf("#%d reached to MAX: %d\n", (int)data, a);
}

void sequential(int numofthread)
{
	for (int id = 1; id <= numofthread; id++) {
		pthread_create(&tid[id], NULL, gotomax, id);
		pthread_join(tid[id], NULL);
	}
}

void simultaneous(int numofthread)
{
	for (int id = 1; id <= numofthread; id++) {
		pthread_create(&tid[id], NULL, gotomax, id);
	}
	for (int id = 1; id <= numofthread; id++) {
		pthread_join(tid[id], NULL);
	}
}

void setaffinity(int numofthread, int numofcore)
{
	cpu_set_t cpuset;
//	printf(" CPU_SIZE: %d\n", CPU_SETSIZE);

	for (int id = 1; id <= numofthread; id++) {
		CPU_ZERO(&cpuset);
		CPU_SET((id - 1) % numofcore, &cpuset);
		pthread_create(&tid[id], NULL, gotomax, id);
		if (pthread_setaffinity_np(tid[id], sizeof(cpu_set_t), &cpuset) != 0)
			perror("pthread_setaffinity_np");
		if (pthread_getaffinity_np(tid[id], sizeof(cpu_set_t), &cpuset) != 0)
			perror("pthread_getaffinity_np");

		/*
		 * For debugging
		printf("Set returned by pthread_getaffinity_np() contained:\n");
		for (int j = 0; j < CPU_SETSIZE; j++)
			if (CPU_ISSET(j, &cpuset))
				printf("   CPU %d\n", j);
		*/
	}
	for (int id = 1; id <= numofthread; id++) {
		pthread_join(tid[id], NULL);
	}
}

int main()
{
	int cpu = sysconf(_SC_NPROCESSORS_ONLN);
	int thread_max = sysconf(_SC_THREAD_THREADS_MAX);
	double time_taken;
	char str[100];
	int n;

	printf("Num of CPU: %d\nThreads max: %d\n", cpu, thread_max);
	printf("type 'q' to quit\n");

	while (strcmp(str, "q")) {
		printf("[Shawn]$> ");
		scanf("%s", str);
		n = atoi(str);

		timestamp_t start = get_timestamp();
		simultaneous(n);
		timestamp_t end = get_timestamp();
		time_taken = (end - start) / 1000000.0L;
		printf("%d threads took %f seconds to execute\n", n, time_taken);

		start = get_timestamp();
		setaffinity(n, cpu);
		end = get_timestamp();
		time_taken = (end - start) / 1000000.0L;
		printf("%d threads took %f seconds to execute\n", n, time_taken);

//		sequential(n);
	}

	return 0;
}
