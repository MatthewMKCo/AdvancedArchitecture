addi t0 zero 0
addi t1 zero 384
addi t2 zero 1
addi s0 zero 1
addi s1 zero 1
addi a0 zero 1
addi a1 zero 1
addi a2 zero 1
addi a3 zero 1
addi a4 zero 1
addi a5 zero 1
addi a6 zero 1
addi a7 zero 1
addi s2 zero 1
addi s3 zero 1

.LBB0_1:
bge t0 t1 .LBB0_3

.LBB0_2:
addi t2 t2 1
addi s0 s0 1
addi s1 s1 1
addi a0 a0 1
addi a1 a1 1
addi a2 a2 1
addi a3 a3 1
addi a4 a4 1
addi a5 a5 1
addi a6 a6 1
addi a7 a7 1
addi s2 s2 1
addi s3 s3 1
addi t0 t0 1
jal zero .LBB0_1

.LBB0_3:
jal zero return
