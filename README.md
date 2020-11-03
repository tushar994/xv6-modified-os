# xv6-public-modified

## Usage

```bash
$ make clean
$ make qemu-nox SCHEDULER=<FLAG>
```
`SCHEDULER` can take values `RR`, `FCFS`, `PBS`, and `MLFQ`
This sets the scheduler algorithm
By default it is `RR`

## System calls

### waitx

- `waitx` is a system call that  takes pointers to two integers and fills them with certain values and returns the pid of the process who's values have been given, and returns -1 if any error
- it waits until it finds a child process of the parent process that has a state value of `ZOMBIE`
```c
int
waitx(int * wtime, int* rtime);
```
- `wtime` is the total time that a process has waited in the queue
- `rtime` is the total time the process spent running

### ps

- The `ps` system call gives us certain information about all the processes
```c
int
ps(void)
```
- it gives the `PID`,`Priority`,`State`,`r_time`,`w_time`,`n_run`,`cur_q`,`q0`,`q1`,`q2`,`q3`,`q4`
- `PID`,`Priority`,`State`, are the pid , priority queue number and the state of a process respectively
- `r_time`,`w_time` are the number of complete ticks that the rocess ran for and has been waiting for
- `n_run` is the number of times the cpu chose the process to run it
- `cur_q` is the current queue that the process is in
- `q0,q1,q2,q3...` are the number of times a process was allocated a cpu cycle in the respective queue

### set_priority
- The `set_priority` system call sets the priority of a certain process to a certain value
- It returns the value of the old priority of the process
```c
int
set_priority(int new_priority, int pid)
```

## User Programs

### time command

```bash
$ time <command>
```
This prints the wtime and rtime that the `command` took to execute, as defined by `waitx`
This uses the system call `waitx`

### ps

```bash 
$ ps
```
This calls the system call `ps` and prints the appropriate information

### setPriority

```bash
$ setPriority <new_priority> <pid>
```
This changes the priority of the process with the given pid

## Scheduling Algorithms

### FIRST COME - FIRST SERVE (FCFS)
- We simply look for the process that was created first, and if multiple processes were created at the same time, we choose based on a certain flag explained below.
- We make sure that the same process runs until quit or sleep by use of a flag that is initially zero, and set to 1 if it has been selected by the cpu. If it sleeps we set it back to zero. While selecting, if we find a process with this flag set to 1, then we automatically select that process, as that is the process that ran in the previous clock cycle.
- We then run the found process until it quits or sleeps.

### PRIORITY BASED SCHEDULER
- We always choose the process with lowest priority value (which indicates highest priority).
- If we have two processes with same priority then we execute then in RR fashion.
- We set default priority to 60.
- We have a system call to change priority of a certain process.
- If a process comes with higher priority than the current running process, we start executing that process in the next cpu cycle.
- To execute this, we create the system call set_priority, and in our scheduler we find any one of the processes with highest priority (if there are any processes) and we execute that.
- This does Round Robin as, if there are multiple process with same priority, as after every cpu cycle, any one of them will be chosen.
- If we there are no processes in the scheduler we simply wait (busy wait) for a process to come.

### MULTI-LEVEL FEEDBACK QUEUE SCHEDULING
- Each process has a queue number that tells us which queue it is a part of. There are 5 queues.
- We choose the process from the highest priority queue, and if there are multiple processes in the top queue, then we choose the one that was first put into the queue.
- We put processes by default into the highest queue.
- If a process does an I/O call, then it is put to sleep, and once that is done, it is put at the end of the queue it was in before the I/O call.
- Each queue has a certain number of the CPU cycles associated with it, let us call it `t`. Then, if a process doesn't sleep or exit before it uses up `t` clock cycles, then it's execution is paused, and it is put at the end of the queue with priority right below it.
- The above can be exploited by a process, in the following manner. Every time it is about to finish up it's `t` clock cycles, it can do a small I/O call. This will keep this process in the top priority queue, which will lead to it getting much more priority when it really shouldn't get such priority.
- We also do aging. In this, if a process in a certain queue waits for more than a specified amount of time, then it is pushed to a higher queue.
- We do Round Robin for the 5th queue. We stop the execution of a process if it completes 16 cpu cycles, and put it at the back of the 5th queue.

## Performance Comparison Between The Schedulers
- We do the following time comparison by running 10 processes all with varying levels of I/O-boundness. The processes start running one by one, and increase in CPU-boundness as they are sequentially created. Hence the given values are only applicable to the given case, and the analysis here will also be restricted to only this particular case. The values will also depend on our implementation, as there are many ways to implement these schedulers. Note that the values change because of the fact that each process runs on a machine with a different state. I.e. the state of machine is different for every time these processes are run.
- We will do analysis by comparing values of total wait time for the 10 processes we run. We measure it in terms of cpu cycles
-  For FCFS the values are -
    - 14960
    - 14578
    - 14496
    - 15643
    - 14235
- For MLFQ the values are - 
    - 12564
    - 11234
    - 10545
    - 10987
    - 11542
- For RR the values are - 
    - 11365
    - 11167
    - 12145
    - 10765
    - 11998
- For PBS we will give values for cases where we give priorities in different ways -
    - When all processes have the same priority
        - 12123
        - 10598
        - 11769
        - 12268
        - 11925
    - When more IO-Bound processes are given a higher priority
        - 6754
        - 7625
        - 6139
        - 7462
        - 8365
    - When more CPU-Bound processes are given higher priority
        - 13768
        - 14906
        - 13874
        - 13998
        - 14508
## Analysis of MULTI-LEVEL FEEDBACK QUEUE SCHEDULING Using Graphs
### Four processes that are IO-Bound
![Alt text](graphs/onlyIO.png?raw=true "Title")
- In Multi-Level Feedback Queue Scheduling if a process executes from a certain queue, and doesn't go into IO or finish within a certain number of clock cycles, then it will be demoted to a queue of lower priority.
- Here, since all processes are IO bound, i.e. since they spend most their time in IO calls, they wont execute for too long in each cpu burst, and hence wont be demoted too much.
### Four processes that are CPU-Bound
![Alt text](graphs/only_cpu.png?raw=true "Title")
- Here, since all processes are CPU bound, i.e. They spend most their time executing, They will be demoted to lower queues as they will not go into IO or exit before the cpu cycle limit per cpu burst.
### Two IO-Bound Processes and Two CPU-Bound Processes
![Alt text](graphs/two_io_two_cpu.png?raw=true "Title")
- As expected, the two cpu bound processes will be demoted to very low levels, while the IO-bound processes will not be demoted to very low levels.
- You can notice aging here.
### Four Processes that have varying (all different) levels of IO/CPU-boundness
![Alt text](graphs/all_diff.png?raw=true "Title")
- The factors that influenced if and if so how quickly a process was demoted to low levels are -
    - How cpu-intensive the process is.
    - When it was created.
- The cpu-intensiveness required to be demoted to lower levels depends on -
    - Amount of time that a queue allows a process to run for before demoting it.
    - Amount of time that a process waits before it gets promoted due to aging.
- The values of these factors, along with details of the processes themselves, and the machine state, cause variations in demotion and promotion times. Due to variations in the machine state, it is rare to see that same graph even when running the same processes at the same times with the same scheduler policies.
