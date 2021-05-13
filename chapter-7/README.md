# Homework

`mlfq.py` 重点参数说明

```bash
# 时间切片长度，用于相同优先级的队列中的进程RR调度。
# 进程的切片长度每执行一次，就递减一个值，直到为0。
# 比如JOB0 -q 10，[time 0]开始运行,[time 6] 发出一次I/O，那么此时 JOB0 切片长度还剩3;
# JOB0 再次运行时，切片长度从2开始，直到0才算 JOB0 时间切片用完。
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

答：如果配额为 2，在 JOB0 第 1 个时间片执行完以后，JOB0 还保持在优先级最高的队列中；如果此时发出 I/O，此时 JOB0 处于阻塞状态，JOB0 会移到整个队列的队尾；如果没有发出 I/O，此时 JOB0 处于就绪状态，就说明 JOB0 可以立刻使用 CPU，因此 JOB0 会被移到下一个 JOB 的后面，准备运行。

延申思考2

什么时候开始执行最高优先级的低一级队列中的进程？

答：当最高优先级队列中没有进程，或最高优先级队列中的进程全部让出 CPU 时（比如全部在进行 I/O 操作），就会执行次优先级队列中的进程，类推其他低优先级队列也是如此。

# Questions

1. 只用两个工作和两个队列运行几个随机生成的问题。针对每个工作计算 MLFQ 的执
   行记录。限制每项作业的长度并关闭 I/O，让你的生活更轻松。

   ```bash
   prompt > ./mlfq.py -j 2 -n 2 -m 10 -M 0
   
   Here is the list of inputs:
   OPTIONS jobs 2
   OPTIONS queues 2
   OPTIONS allotments for queue  1 is   1
   OPTIONS quantum length for queue  1 is  10
   OPTIONS allotments for queue  0 is   1
   OPTIONS quantum length for queue  0 is  10
   OPTIONS boost 0
   OPTIONS ioTime 5
   OPTIONS stayAfterIO False
   OPTIONS iobump False
   
   Job List:
     Job  0: startTime   0 - runTime   8 - ioFreq   0
     Job  1: startTime   0 - runTime   4 - ioFreq   0
     
   # 这里自己推算执行记录，然后通过 -c 参数验证。
   Execution Trace:
   [time 0] JOB0 Ready
   [time 0] JOB1 Ready
   [time 0] Run JOB 0 at PRIORITY 1 [ticks 9 allot 1 time 7(of 8)]
   [time 1] Run JOB 0 at PRIORITY 1 [ticks 8 allot 1 time 6(of 8)]
   [time 2] Run JOB 0 at PRIORITY 1 [ticks 7 allot 1 time 5(of 8)]
   [time 3] Run JOB 0 at PRIORITY 1 [ticks 6 allot 1 time 4(of 8)]
   [time 4] Run JOB 0 at PRIORITY 1 [ticks 5 allot 1 time 3(of 8)]
   [time 5] Run JOB 0 at PRIORITY 1 [ticks 4 allot 1 time 2(of 8)]
   [time 6] Run JOB 0 at PRIORITY 1 [ticks 3 allot 1 time 1(of 8)]
   [time 7] Run JOB 0 at PRIORITY 1 [ticks 2 allot 1 time 0(of 8)]
   [time 8] Finished JOB 0 # JOB0已经运行结束，让出CPU。
   [time 8] Run JOB 1 at PRIORITY 1 [ticks 9 allot 1 time 3(of 4)]
   [time 9] Run JOB 1 at PRIORITY 1 [ticks 8 allot 1 time 2(of 4)]
   [time 10] Run JOB 1 at PRIORITY 1 [ticks 7 allot 1 time 1(of 4)]
   [time 11] Run JOB 1 at PRIORITY 1 [ticks 6 allot 1 time 0(of 4)]
   [time 12] Finished JOB 1 # JOB1已经运行结束，让出CPU。
   
   Final statistics:
   JOB0: startTime 0, Response 0, turnaround 8
   JOB1: startTime 0, Response 8, turnaround 12
    Avg: startTime 0, Response 4, turnaround 10
   ```

2. 如何运行调度程序来重现本章中的每个实例？

   ```bash
   实例1：单个长工作
   prompt > ./mlfq.py -j 1 -n 3 -m 30 -M 0
   
   实例2：来了个短工作
   prompt > ./mlfq.py -n 3 -l 0,200,0:100,20,0 -q 10
   
   实例3：如果有I/O呢？
   ./mlfq.py -n 3 -l 0,200,0:0,200,1 -q 10 -S
   # -S 用来模拟进程在时间片即将用完之前，发出一个I/O（让出CPU），但在它恢复执行时，
   # 该进程还在原优先级队列中，而且它的时间片完全没有改变 [TICKS依然是9]。
   # 如果配合-a使用，则在时间配额即将用完之前，发出一个I/O（让出CPU），在它恢复执行时保持原队列优先级。
   
   # 摘取部分Execution Trace:
   # [ time 10 ] Run JOB 1 at PRIORITY 2 [ TICKS 9 ALLOT 1 TIME 199 (of 200) ]
   # [ time 11 ] IO_START by JOB 1
   # ...
   # [ time 16 ] Run JOB 1 at PRIORITY 2 [ TICKS 9 ALLOT 1 TIME 198 (of 200) ]
   # [ time 17 ] IO_START by JOB 1
   # ...
   # [ time 22 ] Run JOB 1 at PRIORITY 2 [ TICKS 9 ALLOT 1 TIME 197 (of 200) ]
   # 可以看到 TICKS 9 一直保持不变。
   # 当然加了 -S 属于愚弄调度程序。
   # MLFQ通过时间配额来优化这种程序长时间占用CPU的问题，杜绝不公平调度，防止恶意欺骗攻击。
   
   实例4：图8.5
   # 避免饥饿问题
   prompt > ./mlfq.py -l 0,200,0:100,200,1:101,200,1 -i 1 -q 10 -S
   # JOB0 一开始运行3个时间配额后，被移入到最低优先级队列中，然后开始运行JOB1和JOB2，JOB0则一直处于饥饿状态，
   # 只有等待JOB1和JOB2运行结束，才会继续运行JOB0。假设JOB1和JOB2无限执行状态，那JOB0将会被饿死。
   # 通过周期性地提升所有进程的优先级（将所有进程移入最高优先级队列中），-B BOOST 参数设置。
   
   prompt > ./mlfq.py -l 0,200,0:100,200,1:101,200,1 -i 1 -q 10 -S -B 50
   # 提升到最高优先级队列中时，时间切片为0 (TICKS 0)，表示进程被调度运行1次就会被移入低一级队列中。
   # [ time 99 ] Run JOB 0 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 100 (of 200) ]
   # [ time 100 ] BOOST ( every 50 )
   # [ time 100 ] Run JOB 0 at PRIORITY 2 [ TICKS 0 ALLOT 1 TIME 99 (of 200) ]
   # ...
   # [ time 103 ] Run JOB 0 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 98 (of 200) ]
   
   prompt > ./mlfq.py -l 0,200,0:100,200,1:101,200,1 -i 1 -q 10 -a 2
   # 另外，在只有时间配额（-a）、没有-S 、-B的条件下，低优先级队列中的进程还是会处于饥饿状态且被饿死。
   # 比如攻击者不断创建进程且都在时间配额内运行结束。
   
   ```

   

3. xx

4. xx

5. xx

6. xx