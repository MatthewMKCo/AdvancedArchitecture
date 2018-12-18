addi sp sp -96
sw sp ra 92
sw sp s0 88
addi s0 sp 96
addi a0 zero 1
sw s0 a0 -12
addi a0 zero 2
sw s0 a0 -16
addi a0 zero 3
sw s0 a0 -20
addi a0 zero 4
sw s0 a0 -24
addi a0 zero 5
sw s0 a0 -28
addi a0 zero 6
sw s0 a0 -32
addi a0 zero 7
sw s0 a0 -36
addi a0 zero 8
sw s0 a0 -40
addi a0 zero 9
sw s0 a0 -44
addi a0 zero 5
sw s0 a0 -48
addi a0 zero 2
sw s0 a0 -52
addi a0 zero 1
sw s0 a0 -56
addi t5 zero 0
sw s0 t5 -60
sw s0 t5 -64
sw s0 t5 -68
addi t5 zero 0
addi a3 zero 0
addi a4 zero 0
addi a5 zero 0

.LBB0_1:
addi a1 zero 3
blt a1 a3 .LBB0_7

.LBB0_2:
addi a4 zero 0

.LBB0_3:
addi a1 zero 2
blt a1 a4 .LBB0_6

.LBB0_4:
addi a2 s0 -12
addi a6 zero 3
addi a7 zero 4
mul a1 a3 a6
mul a1 a1 a7
sub a1 a2 a1
mul a0 a4 a7
sub a1 a1 a0
lw a1 a1 0
add a5 zero a1
addi a2 s0 -48
mul a1 a4 a7
sub a1 a2 a1
lw a1 a1 0
mul a5 a5 a1
addi a2 s0 -60
mul a1 a3 a7
sub a1 a2 a1
lw a0 a1 0
add a0 a0 a5
sw a1 a0 0

.LBB0_5:
addi a4 a4 1
beq zero zero .LBB0_3

.LBB0_6:
addi a3 a3 1
beq zero zero .LBB0_1

.LBB0_7:
lw t0 s0 -60
lw t1 s0 -64
lw t2 s0 -68
jal zero return
