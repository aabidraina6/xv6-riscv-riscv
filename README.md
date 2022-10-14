# Enhancing XV-6
The team:
```
Soham Korade     2021101131
Abid Khursheed   2021101136
```
___
## Specification 1: System Calls


### System Call 1 : `trace`
We implement tracing in the `syscall` function as every syscall lands here.
We add a variable `tracing` to the process state to keep tracemask which is given as user input in case `strace` is called. Inside syscall, we check if a given bit is set in the tracemask and only for the corresponding syscalls we display the required information about the system call, such as arguments and the return value.


### System Call 2 : `sigalarm` and `sigreturn`
The `usertrap` function is called whenever there is a timer interrupt. We store variables `nticks` , `ticksleft` , `handler` and `saved_tf` in the process state which are used as follows:
1. `nticks`: Stores number of ticks after which the handler function should be called.
1. `ticksleft`: Represents the number of ticks left after which the handler function will be called. Intially it is set to `nticks`.
1. `handler`: Pointer to the function which is to be executed after every 'nticks' ticks.
1. `saved_tf`: Saves the trapframe of the process before it calls the `handler` function.

Inside `sigreturn`, we restore the process's saved trapframe and reset all the above variables.

___
## Specification 2: Scheduling

### a. FCFS (First Come First Serve)
This algorithm simply schedules the jobs according to their creation time. We keep a `ctime` variable in process state. Everytime a new process is created, it is initialized with `ticks`. We choose the available process with minimum creation time, and run it.

### b. LOT (Lottery Based Scheduler)
We keep a `tickets` variable in process state. We initialize each process with 1 lottery. Processes can change their number of tickets via the `settickets` syscall. These tickets represent the share of processes. A random lottery ticket is chosen from the range $[0, \text{total tickets}]$. The corresponding process will be chosen. Giving each process at least one lottery ticket guarantees that it has non-zero probability of being selected at each scheduling operation, hence avoiding starvation of processes.

### c. PBS (Priority Based Scheduler)
`setpriority` syscall changes the static priority of a process. We track runtime, created time and wait time for each process. We calculate dynamic priority based on static priority and niceness. The formula for niceness is given by
$$\text{niceness} = \left\lfloor \frac{10 * \text{wait time}}{\text{wait time} + \text{run time}} \right\rfloor$$
In the scheduler, we run the process with highest dynamic priority.

### d. MLFQ (Multi Level Feedback Queue)
We create 5 queues with different priorities. We allot time slices as follows:
1. For queue 0: 1 timer tick
1. For queue 1: 2 timer ticks
1. For queue 2: 4 timer ticks
1. For queue 3: 8 timer ticks
1. For queue 4: 16 timer ticks

If the process uses the complete time slice assigned for its current priority queue, it
is preempted and inserted at the end of the next lower level queue.  A round-robin scheduler is be used for processes within the queue. We implement aging of the process by tracking wait time of the processes.

If a process voluntarily relinquishes control of the CPU(eg. For doing I/O), it leaves
the queuing network, and when the process becomes ready again after the I/O, it is
inserted at the tail of the same queue, from which it is relinquished earlier. This can be exploited by a process by yielding the CPU before its allotted time ends. Thus, it can preserve its priority and run continuously. To prevent this, we have implemented ageing to allow lower priority processes to run.

___
## Specification 3: Copy-on-write fork
___
## Specification 4: Report