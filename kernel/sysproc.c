#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_waitx(void)
{
  uint64 addr, addr1, addr2;
  uint wtime, rtime;
  argaddr(0, &addr);
  argaddr(1, &addr1); // user virtual memory
  argaddr(2, &addr2);
  int ret = waitx(addr, &rtime, &wtime);
  struct proc* p = myproc();
  if (copyout(p->pagetable, addr1,(char*)&rtime, sizeof(int)) < 0)
    return -1;
  if (copyout(p->pagetable, addr2,(char*)&wtime, sizeof(int)) < 0)
    return -1;
  return ret;
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// trace implementation

uint64
sys_trace(void)
{
  int n;
  argint(0, &n);
  myproc()->tracing = n; // saving the argument
  return 0;
}

// sigalarm implementation

uint64
sys_sigalarm(void)
{
  int nticks;
  uint64 handler;
  argint(0,&nticks);
  argaddr(1,&handler);
  struct proc *p = myproc();
  p->nticks = nticks;
  p->ticksleft = nticks;
  p->handler = handler;
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc* p = myproc();
  memmove(p->trapframe , p->saved_tf , sizeof(struct trapframe));
  kfree(p->saved_tf);
  p->sigalarm_flag = 0;
  p->handler = 0;
  p->nticks = 0;
  p->ticksleft = 0;
  return p->trapframe->a0;
}

uint64
sys_setpriority(void){
#ifdef PBS
  int priority, pid;
  argint(0, &priority);
  argint(1, &pid);
  return setpriority(priority, pid);
#else
  printf("setpriority is available only when using priority-based scheduler\n");
#endif
  return 0;
}

uint64
sys_settickets(void){
#ifdef LOT
  int n;
  argint(0, &n);
  myproc()->tickets = n;
#else
  printf("settickets is available only when using lottery-based scheduler\n");
#endif
  return 0;
}