#include "stateMonitor.hpp"
#include "threadConf.hpp"
#include <pthread.h>

StateMonitor::StateMonitor() {
  internalState = 0;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&condition, NULL);
  globalStateChangeHandler = NULL;

  for(int i=0;i<MAXSTATES;i++)
    for(int j=0;j<MAXSTATES;j++)
      for(int k=0;k<MAXLISTENERS;k++)
        func_table[i][j][k] = NULL;
}

int StateMonitor::waitState(threadConf *cfg) {
  pthread_mutex_lock(&mutex);
  while (!cfg->containsState(internalState)) {
    pthread_cond_wait(&condition, &mutex);
  }
  int auxState = internalState;
  pthread_mutex_unlock(&mutex);
  return(auxState);
}

int StateMonitor::changeState(int st) {
  pthread_mutex_lock(&mutex);

  if (globalStateChangeHandler != NULL) {
    (*globalStateChangeHandler)(internalState, st);
  }

  for(int i=0; i<MAXLISTENERS; i++) {
    if (func_table[internalState][st][i] != NULL)
      (*func_table[internalState][st][i])(internalState, st);
  }

  internalState = st;
  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&mutex);
  return(0);
}

int StateMonitor::getState() {
  int aux;
  pthread_mutex_lock(&mutex);
  aux = internalState;
  pthread_mutex_unlock(&mutex);
  return(aux);
}

int StateMonitor::addStateChangeListener(int fromState, int toState, void* (*handle)(int,int)) {
  pthread_mutex_lock(&mutex);
  for(int i=0; i<MAXLISTENERS; i++) {
    if (func_table[fromState][toState][i] == NULL) {
      func_table[fromState][toState][i] = handle;
      pthread_mutex_unlock(&mutex);
      return 0;
    }
  }
  pthread_mutex_unlock(&mutex);
  return 1;
}

void StateMonitor::setGlobalStateChangeHandler(void* (*handler)(int, int)) {
  pthread_mutex_lock(&mutex);
  globalStateChangeHandler = handler;
  pthread_mutex_unlock(&mutex);
}
