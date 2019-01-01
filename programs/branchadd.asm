addi t0 zero 1
addi t2 zero 384
addi t3 zero 0
addi a0 zero 0
addi a1 zero 0
addi a2 zero 0
addi a3 zero 0
addi a4 zero 0


.LBB0_0:
blt t2 t3 .LBB0_6
addi t1 zero 1
beq t0 t1 .LBB0_1
addi t1 zero 2
beq t0 t1 .LBB0_2

.LBB0_1:
addi a0 a0 1
addi t0 t0 1
beq zero zero .LBB0_0

.LBB0_2:
addi a1 a1 1
addi t0 zero 1
addi t3 t3 1
beq zero zero .LBB0_0

.LBB0_3:

.LBB0_4:

.LBB0_5:

.LBB0_6:
jal zero return
