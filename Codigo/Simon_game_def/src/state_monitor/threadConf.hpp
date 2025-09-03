#ifndef THREADCONF_HPP_
#define THREADCONF_HPP_

#define MAXSTATES_DEF 10

class threadConf {
  private:
   int states[MAXSTATES_DEF];
   int nstates;
   void *arg;

  public:
   threadConf() {
     nstates = 0;
     arg = NULL;
   }

   void addState(int s) {
    states[nstates++] = s;
   }

   bool containsState(int s) {
    for (int i = 0; i < nstates; i++) {
       if (states[i] == s) return true;
    }
    return false;
   }

   void setArg(void *pointer) { arg = pointer; }
   void *getArg() { return arg; }
};

#endif
