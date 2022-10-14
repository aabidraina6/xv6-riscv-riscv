#ifdef MLFQ

  #include "types.h"
  #include "param.h"
  #include "memlayout.h"
  #include "riscv.h"
  #include "spinlock.h"
  #include "defs.h"
  #include "proc.h"

  // multi-level feedback queue structure
  struct mlfq_t q_data[5];

  void
  q_init()
  {
    for(int i = 0; i < 5; i++){
      q_data[i].max_ticks = 1<<i; // 2^i
      q_data[i].size = 0;
      for(int j = 0; j < NPROC; j++)
        q_data[i].process[j] = NULL;
    }
  }

  void
  q_push(struct proc *p, int q_index)
  {
    if(q_index < 0)
      panic("mlfq: queue underflow");
    if(q_index > 4 || q_data[q_index].size >= NPROC)
      panic("mlfq: queue overflow");
    
    q_data[q_index].process[q_data[q_index].size] = p;
    q_data[q_index].size++;

    // update process state
    p->q_index = q_index;
    p->q_waiting = 0;
    p->q_entered = ticks;
    p->q_ticks = 0;
    p->q_within = 1;
  }

  // Pops pointer to process from front of queue. 
  struct proc*
  q_pop(int q_index)
  {
    if(q_index < 0)
      panic("mlfq: queue underflow");
    if(q_index > 4 || q_data[q_index].size <= 0)
      panic("mlfq: queue overflow");

    q_data[q_index].size--;

    struct proc *temp = q_data[q_index].process[0];

    // slide back
    q_data[q_index].process[0] = NULL;
    for(int i = 1; i < NPROC && q_data[q_index].process[i]; i++)
      q_data[q_index].process[i-1] = q_data[q_index].process[i];

    // update process state
    temp->q_within = 0;
    return temp;
  }

  void 
  q_del(struct proc *p, int q_index)
  {
    int found = -1;
    for(int i = 0; i < NPROC; i++)
      if(q_data[q_index].process[i] == p) found = i;
    if(found == -1) return;

    // slide back
    q_data[q_index].process[found] = NULL;
    for(int i = found + 1; i < NPROC && q_data[q_index].process[i]; i++)
      q_data[q_index].process[i-1] = q_data[q_index].process[i];
  }

#endif