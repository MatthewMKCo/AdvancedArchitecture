addi t0 zero 0
addi t1 zero 15
addi a0 zero 1
addi a1 zero 1
addi a2 zero 1
addi a3 zero 1
addi a4 zero 1

.LBB0_1:
add a0 a0 a1
add a1 a1 a2
add a2 a2 a3
add a3 a3 a4
add a4 a4 a0
addi t0 t0 1
blt t0 t1 .LBB0_1

.LBB0_2:
jal zero return
