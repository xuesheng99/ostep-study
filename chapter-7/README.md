# Homework

`mlfq.py` 重点参数说明

```bash
# 时间切片长度，用于相同优先级的队列中的进程RR调度。
# 进程的切片长度每执行一次，就递减一个值，直到为0。
# 比如JOB0 -q 10，[time 0-6] 发出一次I/O，那么此时 JOB0 切片长度还剩3，JOB0 再次运行时，切片长度从2开始，直到0才算 JOB0 时间切片用完。
-q QUANTUM, --quantum=QUANTUM
                        length of time slice (if not using -Q)
# 时间配额长度，进程在某级队列的配额用完后，就进入低一级队列。
# 每一级队列中的进程可以设置不同的时间配额。
# 这里配额是按1个切片长度为1个配额，比如： -a 2 -q 10 表示某一进程用完2个时间切片后，额度用完，该进程移入下一级队列。
# 以此来控制一个进程在某一级队列中运行的总时间。
-a ALLOTMENT, --allotment=ALLOTMENT
                        length of allotment (if not using -A)
# 设置多久执行一次提升进程优先级到最高。                        
-B BOOST, --boost=BOOST
                        how often to boost the priority of all jobs back to
                        high priority
```

用例分析

```bash
prompt > ./mlfq.py -j 3
Here is the list of inputs:
# jobs 3 表示3个进程
OPTIONS jobs 3
# queues 3 表示3个不同优先级的队列
OPTIONS queues 3
OPTIONS allotments for queue  2 is   1
OPTIONS quantum length for queue  2 is  10
OPTIONS allotments for queue  1 is   1
OPTIONS quantum length for queue  1 is  10
OPTIONS allotments for queue  0 is   1
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False
# ioFreq 表示每隔ioFreq时间，进程就发出一次I/O（该I/O花费ioTime时间完成）。
# ioFreq 7，ioTime 5 表示进程正常运行7ms后，发出一次I/O，5ms后I/O完成。 
For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime  84 - ioFreq   7
  Job  1: startTime   0 - runTime  42 - ioFreq   3
  Job  2: startTime   0 - runTime  51 - ioFreq   4
```

用例结果分析

```bash
# 摘取JOB0部分运行结果
Execution Trace:
[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] JOB BEGINS by JOB 1
[ time 0 ] JOB BEGINS by JOB 2
[ time 0 ] Run JOB 0 at PRIORITY 2 [ TICKS 9 ALLOT 1 TIME 83 (of 84) ]
[ time 1 ] Run JOB 0 at PRIORITY 2 [ TICKS 8 ALLOT 1 TIME 82 (of 84) ]
[ time 2 ] Run JOB 0 at PRIORITY 2 [ TICKS 7 ALLOT 1 TIME 81 (of 84) ]
[ time 3 ] Run JOB 0 at PRIORITY 2 [ TICKS 6 ALLOT 1 TIME 80 (of 84) ]
[ time 4 ] Run JOB 0 at PRIORITY 2 [ TICKS 5 ALLOT 1 TIME 79 (of 84) ]
[ time 5 ] Run JOB 0 at PRIORITY 2 [ TICKS 4 ALLOT 1 TIME 78 (of 84) ]
[ time 6 ] Run JOB 0 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 77 (of 84) ]
[ time 7 ] IO_START by JOB 0 # 7个时间过去后，JOB0 发出 I/O，让出 CPU，调度程序开始运行 JOB1。
[ time 7 ] Run JOB 1 at PRIORITY 2 [ TICKS 9 ALLOT 1 TIME 41 (of 42) ]
[ time 10 ] Run JOB 2 at PRIORITY 2 [ TICKS 9 ALLOT 1 TIME 50 (of 51) ]
...
[ time 12 ] IO_DONE by JOB 0 # 5个时间后，JOB0 的 I/O 完成。
# 可以发现优先级为 2 的队列中的 JOB0 在没有用完一个切片长度时，发出了 I/O，仍然保留了原队列优先级。
# 并且 JOB0 在第一次让出 CPU 后[time 7]，被移到了队列尾，所以[time 14]再次运行 JOB0 之前，依次运行过JOB1，JOB2。
[ time 14 ] Run JOB 0 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 76 (of 84) ]
[ time 15 ] IO_DONE by JOB 1
[ time 15 ] Run JOB 0 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 75 (of 84) ]
[ time 16 ] Run JOB 0 at PRIORITY 2 [ TICKS 0 ALLOT 1 TIME 74 (of 84) ]
# [time 16] JOB0 在此时已经用完了一个时间切片[TICKS 0]，开始轮转调度执行 JOB1。
# 由于[ALLOT]为 1，所以配额也用完了，所以在[time 24]可以看到 JOB0 被移入下一级队列[PRIORITY 1]中运行。
# 请延申思考1
[ time 17 ] Run JOB 1 at PRIORITY 2 [ TICKS 6 ALLOT 1 TIME 38 (of 42) ]
...
[ time 24 ] Run JOB 0 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 73 (of 84) ]
```

延申思考1

假如 [ALLOT] 为 2，JOB0 会保留在原来优先级队列中，那么 JOB0 什么时候会再次运行呢？

答：如果配额为 2，在 JOB0 第 1 个时间片执行完以后，JOB0 还保持在优先级最高的队列中；如果此时发出 I/O，JOB0 会移到整个队列的队尾；如果没有发出 I/O，就说明 JOB0 可以立刻使用 CPU，因此 JOB0 会被移到下一个 JOB 的后面，等待运行。

延申思考2

什么时候开始执行次优先级队列中的进程？

答：当最高优先级队列中没有进程，或最高优先级队列中的进程全部让出 CPU 时（比如全部在进行 I/O 操作），就会执行次优先级队列中的进程。

