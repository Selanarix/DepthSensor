*version 9.1 1624748972
u 149
V? 6
R? 5
Q? 2
? 2
DSTM? 2
@libraries
@analysis
.DC 1 0 0 0 1 1
+ 0 0 Vb
+ 0 4 0.9V
+ 0 5 2.0V
+ 0 6 0.05V
.OP 0 
.STMLIB amplifier_npn.stl
@targets
@attributes
@translators
a 0 u 13 0 0 0 hln 100 PCBOARDS=PCB
a 0 u 13 0 0 0 hln 100 PSPICE=PSPICE
a 0 u 13 0 0 0 hln 100 XILINX=XILINX
@setup
unconnectedPins 0
connectViaLabel 0
connectViaLocalLabels 0
NoStim4ExtIFPortsWarnings 1
AutoGenStim4ExtIFPorts 1
@index
pageloc 1 0 3670 
@status
n 0 114:00:27:14:14:20;1390832060 e 
s 2832 114:00:27:14:14:27;1390832067 e 
e 0 114:00:27:12:04:19;1390824259 e 
*page 1 0 970 720 iA
@ports
port 9 GND_EARTH 320 530 h
@parts
part 138 VDC 180 410 h
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 x 0:13 0 0 0 hln 100 PKGREF=Vb
a 1 xp 9 0 24 7 hcn 100 REFDES=Vb
part 8 Q2N3904 350 330 h
a 0 sp 11 0 25 40 hln 100 PART=Q2N3904
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=TO-92
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=Q1
a 0 ap 9 0 5 5 hln 100 REFDES=Q1
part 2 VDC 410 100 v
a 1 u 13 0 -11 18 hcn 100 DC=5V
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 x 0:13 0 0 0 hln 100 PKGREF=Vin
a 1 xp 9 0 24 7 hcn 100 REFDES=Vin
part 7 r 370 460 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 x 0:13 0 0 0 hln 100 PKGREF=Re
a 0 xp 9 0 15 0 hln 100 REFDES=Re
a 0 u 13 0 15 31 hln 100 VALUE=100
part 5 r 370 230 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 x 0:13 0 0 0 hln 100 PKGREF=Rc
a 0 xp 9 0 15 0 hln 100 REFDES=Rc
a 0 u 13 0 15 41 hln 100 VALUE=220
part 6 r 270 450 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 x 0:13 0 0 0 hln 100 PKGREF=R2
a 0 xp 9 0 15 0 hln 100 REFDES=R2
a 0 u 13 0 13 39 hln 100 VALUE=1.8k
part 4 r 270 220 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R1
a 0 ap 9 0 15 0 hln 100 REFDES=R1
a 0 u 13 0 17 37 hln 100 VALUE=6.8k
part 1 titleblk 970 720 h
a 1 s 13 0 350 10 hcn 100 PAGESIZE=A
a 1 s 13 0 180 60 hcn 100 PAGETITLE=
a 1 s 13 0 300 95 hrn 100 PAGENO=1
a 1 s 13 0 340 95 hrn 100 PAGECOUNT=1
part 115 VIEWPOINT 370 400 h
a 0 s 0 0 0 0 hln 100 MARKERTYPE=VIEWPOINT
a 0 s 0 0 0 0 hln 100 PROBEVAR=
a 1 s 13 0 14 8 hcn 100 BIASVALUE=534.49pV
a 0 s 0:13 0 0 0 hln 100 NODE=emitter
@conn
w 23
a 0 up 0:33 0 0 0 hln 100 V=
a 0 sr 0 0 0 0 hln 100 LABEL=Va
s 370 310 370 230 108
a 0 up 33 0 372 270 hlt 100 V=
a 0 sr 3 0 372 260 hln 100 LABEL=Va
s 370 310 370 320 24
w 88
a 0 sr 0 0 0 0 hln 100 LABEL=basis
a 0 up 0:33 0 0 0 hln 100 V=
s 350 330 270 330 31
a 0 sr 3 0 310 328 hcn 100 LABEL=basis
s 270 220 270 330 18
a 0 up 33 0 272 275 hlt 100 V=
s 270 330 270 410 33
s 180 330 180 410 143
s 270 330 180 330 114
w 28
a 0 sr 0 0 0 0 hln 100 LABEL=emitter
a 0 up 0:33 0 0 0 hln 100 V=
s 370 350 370 400 27
a 0 sr 3 0 372 375 hln 100 LABEL=emitter
a 0 up 33 0 372 385 hlt 100 V=
s 370 400 370 420 116
w 68
a 0 sr 0 0 0 0 hln 100 LABEL=source
a 0 up 0:33 0 0 0 hln 100 V=
s 370 100 270 100 112
a 0 sr 3 0 320 98 hcn 100 LABEL=source
a 0 up 33 0 320 99 hct 100 V=
s 370 100 370 190 12
s 270 100 270 180 16
s 410 100 370 100 10
w 52
a 0 sr 0 0 0 0 hln 100 LABEL=ground
a 0 up 0:33 0 0 0 hln 100 V=
s 480 100 480 530 51
a 0 sr 3 0 482 315 hln 100 LABEL=ground
a 0 up 33 0 482 315 hlt 100 V=
s 480 100 450 100 110
s 270 530 320 530 78
s 270 450 270 530 40
s 180 530 270 530 42
s 370 460 370 530 43
s 320 530 370 530 83
s 480 530 370 530 53
s 180 450 180 530 147
@junction
j 370 190
+ p 5 2
+ w 68
j 370 310
+ p 8 c
+ w 23
j 370 230
+ p 5 1
+ w 23
j 270 530
+ w 52
+ w 52
j 370 100
+ w 68
+ w 68
j 350 330
+ p 8 b
+ w 88
j 270 330
+ w 88
+ w 88
j 270 410
+ p 6 2
+ w 88
j 370 350
+ p 8 e
+ w 28
j 370 400
+ p 115 +
+ w 28
j 370 420
+ p 7 2
+ w 28
j 270 450
+ p 6 1
+ w 52
j 320 530
+ s 9
+ w 52
j 370 460
+ p 7 1
+ w 52
j 370 530
+ w 52
+ w 52
j 180 410
+ p 138 +
+ w 88
j 180 450
+ p 138 -
+ w 52
j 410 100
+ p 2 +
+ w 68
j 450 100
+ p 2 -
+ w 52
j 270 180
+ p 4 2
+ w 68
j 270 220
+ p 4 1
+ w 88
@attributes
a 0 s 0:13 0 0 0 hln 100 PAGETITLE=
a 0 s 0:13 0 0 0 hln 100 PAGENO=1
a 0 s 0:13 0 0 0 hln 100 PAGESIZE=A
a 0 s 0:13 0 0 0 hln 100 PAGECOUNT=1
@graphics
