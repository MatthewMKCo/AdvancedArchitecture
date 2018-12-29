addi sp sp -80
sw sp ra 76
sw sp s0 72
addi s0 sp 80
addi a0 zero 10
sw s0 a0 -12
addi a0 zero 9
sw s0 a0 -16
addi a0 zero 8
sw s0 a0 -20
addi a0 zero 7
sw s0 a0 -24
addi a0 zero 6
sw s0 a0 -28
addi a0 zero 5
sw s0 a0 -32
addi a0 zero 4
sw s0 a0 -36
addi a0 zero 3
sw s0 a0 -40
addi a0 zero 2
sw s0 a0 -44
addi a0 zero 1
sw s0 a0 -48
addi a0 zero 0
addi t0 zero 0
addi t1 zero 0
addi t2 zero 0
addi t3 zero 0
addi s1 zero 0
addi a6 zero 4

.LBB0_1:
addi t0 zero 9
blt t0 t1 .LBB0_7

.LBB0_2:
addi s1 zero 0
addi t3 zero 0

.LBB0_3:
addi t2 zero 9
sub t2 t2 t1
blt t2 t3 .LBB0_6

.LBB0_4:
mul a1 t3 a6
addi s2 s0 -12
sub s2 s2 a1
lw a0 s2 0
addi s3 s2 -4
lw a1 s3 0
blt a1 a0 .LBB0_8

.LBB0_5:
beq s1 zero .LBB0_7
addi t3 t3 1
beq zero zero .LBB0_3

.LBB0_6:
addi t1 t1 1
beq zero zero .LBB0_1

.LBB0_8:
sw s2 a1 0
sw s3 a0 0
addi s1 zero 1
beq zero zero .LBB0_5

.LBB0_7:
lw a0 s0 -12
lw a1 s0 -16
lw a2 s0 -20
lw a3 s0 -24
lw a4 s0 -28
lw a5 s0 -32
lw a6 s0 -36
lw a7 s0 -40
lw s2 s0 -44
lw s3 s0 -48
jal zero return
