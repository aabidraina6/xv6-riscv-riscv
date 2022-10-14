# Enhancing XV-6
The team:
```
Soham Korade     2021101131
Abid Khursheed   2021101136
```
___
## Specification 1: System Calls


### System Call 1 : `trace`
Since all the system calls pass through `syscall` in `syscall.c` and we can get all the arguments of a system call , it is a good place to implement trace.
We add a variable `tracing` to `proc` data structure to keep trace of the mask which is given as user input in case `strace` is called.
Inside syscall, we check if a given bit is set and corresponding to the position of the bit we print the required information about the system call.


### System Call 2 : `sigalarm` and `sigreturn`
Whenever there is a timer interrupt , the `usertrap` function in `trap.c` is called. We add variables `nticks` , `ticksleft` , `handler` and `saved_tf` to the `proc` data structure which are required for the following:
a. ``nticks`` -> Stores number of ticks after which the handler function should be called.
b. ``ticksleft`` -> Represents the number of ticks left after which the handler function will be called. Intially it is set to `nticks`.
c. ``handler`` -> Pointer to the function which is to be executed after every 'nticks' ticks.
d. ``saved_tf`` -> Saves the trapframe of the process before it calls the `handler` function.

Inside `sigreturn`, we restore the process's saved trapframe and set all the other variable named above to zero.

___
## Specification 2: Scheduling

### a. FCFS (First Come First Serve)
We keep a `ctime` variable in process state. Everytime a new process is created, it is initialized with `ticks`. We choose the process with minimum creation time, and run it.

### b. LOT (Lottery Based Scheduler)

### c. PBS (Priority Based Scheduler)

### d. MLFQ (Multi Level Feedback Queue)

___
## Specification 3: Copy-on-write fork
___
## Specification 4: Report