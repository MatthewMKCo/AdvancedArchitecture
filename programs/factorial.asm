addi sp sp -80
sw sp ra 76
sw sp s0 72
addi s0 sp 80
addi a0 zero 1
addi a1 zero 12

.LBB0_1:
addi t0 zero 1
blt a1 t0 .LBB0_4

.LBB0_2:
mul a0 a0 a1

.LBB0_3:
addi a1 a1 -1
jal zero .LBB0_1

.LBB0_4:
jal zero return
