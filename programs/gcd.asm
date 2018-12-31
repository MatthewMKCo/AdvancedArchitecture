addi a0 zero 2135
addi a1 zero 7

.LBB0_1:
beq a0 a1 .LBB0_5

.LBB0_2:
blt a1 a0 .LBB0_4

.LBB0_3:
sub a1 a1 a0
jal zero .LBB0_1

.LBB0_4:
sub a0 a0 a1
jal zero .LBB0_1

.LBB0_5:
jal zero return
