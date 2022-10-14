# Enhancing XV-6
The team:
```
Soham Korade     2021101131
Abid Khursheed   2021101136
```
___
## Specification 1: System Calls


### System Call 1 : `trace`

### System Call 2 : `sigalarm` and `sigreturn`

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