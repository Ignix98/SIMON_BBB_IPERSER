#ifndef STATEMONITOR_HPP_
#define STATEMONITOR_HPP_

#include <pthread.h>

#define MAXSTATES_DEF 10
#define MAXLISTENERS_DEF 10

class StateMonitor {
private:
  static const int MAXSTATES = MAXSTATES_DEF;
  static const int MAXLISTENERS = MAXLISTENERS_DEF;
  int internalState;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  void* (*func_table[MAXSTATES][MAXSTATES][MAXLISTENERS])(int stFrom, int stTo);
  void* (*globalStateChangeHandler)(int, int);

public:
  StateMonitor();

  int waitState(class threadConf *cfg);
  int changeState(int st);
  int getState();
  int addStateChangeListener(int fromState, int toState, void* (*handle)(int,int));
  void setGlobalStateChangeHandler(void* (*handler)(int, int));
};

#endif
