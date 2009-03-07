#include "searcher.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>

double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}
void *process(void *arg);
void *counter(void *arg);

typedef struct {
  pthread_t id;
  uint32_t num_procs;
  uint32_t proc_id;
} thread_arg_t;

uint32_t num_threads;

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "%s num_threads\n", argv[0]);
    exit(1);
  }
  num_threads = atoi(argv[1]);

  thread_arg_t *targs = new thread_arg_t[num_threads];

  pthread_t id;
  for (int i = 0; i < (int) num_threads; ++i) {
    targs[i].num_procs = 0;
    targs[i].proc_id = (int) i;
    pthread_create(&(targs[i].id), NULL, &process, &targs[i]);
  }
  pthread_create(&id, NULL, &counter, targs);
  for (int i = 0; i < (int) num_threads; ++i) {
    pthread_join(targs[i].id, NULL);
  }
  pthread_join(id, NULL);
  return 0;
}

void *process(void *arg)
{
  thread_arg_t *targ = (thread_arg_t *) arg;

  std::ifstream fin;
  char file[128];
  memset(file, 0, 128);
  sprintf(file, "./query/queries.%d", targ->proc_id);
  fin.open(file, std::ios::in);
  if (!fin) {
    std::cerr << "file not found." << std::endl;
    return NULL;
  }

  Lux::Dist::Searcher s("blogs");
  Lux::SortCondition scond(Lux::SORT_SCORE, Lux::DESC);
  Lux::Paging paging(5);
  Lux::Condition cond(scond, paging);

  std::string line;
  while (getline(fin, line)) {
    //std::cout << line << std::endl;
    Lux::ResultSet rs = s.search(line.c_str(), cond); 
    ++targ->num_procs;
  }
  std::cout << "thread " << targ->proc_id << " finished" << std::endl;

  return NULL;
}

void *counter(void *arg)
{
  thread_arg_t *targs = (thread_arg_t *) arg;

  double t1 = gettimeofday_sec();
  while (1) {
    double t2 = gettimeofday_sec();
    
    uint32_t total = 0;
    for (int i = 0; i < (int) num_threads; ++i) {
      total += targs[i].num_procs;
    }

    printf("total: %d, time: %lf, proc/s: %6.2f\r", total, t2-t1, total / (t2-t1));

    fflush(stdout);
    usleep(100);
  }

  return NULL;
}

