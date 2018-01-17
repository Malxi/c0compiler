.data
	#global allocate
	global: .space 44
	str_0: .asciiz " "
	str_1: .asciiz "="
	str_2: .asciiz "ERROR:Divided by ZERO. "
	str_3: .asciiz "Undefined operation:"
	str_4: .asciiz "Please select one from set {+,-,*,/}. "
	str_5: .asciiz "Please input a smaller one:"
	str_6: .asciiz "True"
	str_7: .asciiz "False"
	str_8: .asciiz "Wrong number:"
	str_9: .asciiz "    "
.text
	la $gp global
	jal label_main
	li $a0 0
	li $v0 17
	syscall
label_fact:
	# start of prologue
	addiu $sp $sp -100
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	sw $s3 20($sp)
	sw $s4 24($sp)
	sw $s5 28($sp)
	sw $s6 32($sp)
	sw $s7 36($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $t0 0
	subu $t1 $a1 $t0
	#Base block 1 save start
	#Base block 1 save end
	bne $t1 $zero label_t1
	#Base block 2 clear start
	#Base block 2 clear end
	li $v1 1
	#Base block 2 clear start
	#Base block 2 clear end
	j label_fact_end
	#Base block 2 save start
	#Base block 2 save end
	j label_t21
	#Base block 3 clear start
	#Base block 3 clear end
label_t1:
	li $t0 1
	subu $s0 $a1 $t0
	# Caller save reg start
	sw $a1 0($fp)
	# Caller save reg end
	or $a1 $s0 $zero
	#Base block 3 clear start
	#Base block 3 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_fact
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	# Caller pop reg end
	or $s1 $v1 $zero
	mul $s2 $a1 $s1
	or $v1 $s2 $zero
	#Base block 3 clear start
	#Base block 3 clear end
	j label_fact_end
	#Base block 4 clear start
	#Base block 4 clear end
label_t21:
	#Base block 4 clear start
	#Base block 4 clear end
label_fact_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	lw $s3 20($sp)
	lw $s4 24($sp)
	lw $s5 28($sp)
	lw $s6 32($sp)
	lw $s7 36($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 100
	# end of epilogue
	jr $ra
label_isletter:
	# start of prologue
	addiu $sp $sp -40
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $t0 95
	subu $t1 $a1 $t0
	#Base block 1 save start
	#Base block 1 save end
	bne $t1 $zero label_t26
	#Base block 2 clear start
	#Base block 2 clear end
	li $v1 1
	#Base block 2 clear start
	#Base block 2 clear end
	j label_isletter_end
	#Base block 2 save start
	#Base block 2 save end
	j label_t27
	#Base block 3 clear start
	#Base block 3 clear end
label_t26:
	li $t0 97
	subu $t1 $a1 $t0
	#Base block 3 save start
	#Base block 3 save end
	bltz $t1 label_t29
	#Base block 4 clear start
	#Base block 4 clear end
	li $t0 122
	subu $t1 $a1 $t0
	#Base block 4 save start
	#Base block 4 save end
	bgtz $t1 label_t31
	#Base block 5 clear start
	#Base block 5 clear end
	li $v1 1
	#Base block 5 clear start
	#Base block 5 clear end
	j label_isletter_end
	#Base block 5 save start
	#Base block 5 save end
	j label_t32
	#Base block 6 clear start
	#Base block 6 clear end
label_t31:
	li $v1 0
	#Base block 6 clear start
	#Base block 6 clear end
	j label_isletter_end
	#Base block 7 clear start
	#Base block 7 clear end
label_t32:
	#Base block 7 save start
	#Base block 7 save end
	j label_t33
	#Base block 8 clear start
	#Base block 8 clear end
label_t29:
	li $t0 65
	subu $t1 $a1 $t0
	#Base block 8 save start
	#Base block 8 save end
	bltz $t1 label_t35
	#Base block 9 clear start
	#Base block 9 clear end
	li $t0 90
	subu $t1 $a1 $t0
	#Base block 9 save start
	#Base block 9 save end
	bgtz $t1 label_t37
	#Base block 10 clear start
	#Base block 10 clear end
	li $v1 1
	#Base block 10 clear start
	#Base block 10 clear end
	j label_isletter_end
	#Base block 10 save start
	#Base block 10 save end
	j label_t38
	#Base block 11 clear start
	#Base block 11 clear end
label_t37:
	#Base block 12 clear start
	#Base block 12 clear end
label_t38:
	#Base block 12 save start
	#Base block 12 save end
	j label_t39
	#Base block 13 clear start
	#Base block 13 clear end
label_t35:
	#Base block 14 clear start
	#Base block 14 clear end
label_t39:
	#Base block 15 clear start
	#Base block 15 clear end
label_t33:
	#Base block 16 clear start
	#Base block 16 clear end
label_t27:
	li $v1 0
	#Base block 16 clear start
	#Base block 16 clear end
	j label_isletter_end
	#Base block 16 clear start
	#Base block 16 clear end
label_isletter_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 40
	# end of epilogue
	jr $ra
label_choose_pivot:
	# start of prologue
	addiu $sp $sp -48
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	addu $s0 $a1 $a2
	li $t0 2
	div $s1 $s0 $t0
	or $v1 $s1 $zero
	#Base block 1 clear start
	#Base block 1 clear end
	j label_choose_pivot_end
	#Base block 1 clear start
	#Base block 1 clear end
label_choose_pivot_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 48
	# end of epilogue
	jr $ra
label_printlist:
	# start of prologue
	addiu $sp $sp -56
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $s0 0
	#Base block 2 clear start
	#Base block 2 clear end
label_t42:
	subu $t0 $s0 $a1
	#Base block 2 save start
	#Base block 2 save end
	bgez $t0 label_t43
	#Base block 3 clear start
	#Base block 3 clear end
	sll $s1 $s0 2
	addu $s1 $s1 $gp
	lw $s1 0($s1)
	li $v0 4
	la $a0 str_0
	syscall
	li $v0 1
	or $a0 $s1 $zero
	syscall
	li $t0 1
	addu $s0 $s0 $t0
	#Base block 3 save start
	#Base block 3 save end
	j label_t42
	#Base block 4 clear start
	#Base block 4 clear end
label_t43:
	#Base block 4 clear start
	#Base block 4 clear end
label_printlist_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 56
	# end of epilogue
	jr $ra
label_quicksort:
	# start of prologue
	addiu $sp $sp -108
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	sw $s3 20($sp)
	sw $s4 24($sp)
	sw $s5 28($sp)
	sw $s6 32($sp)
	sw $s7 36($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	# Caller save reg start
	sw $a1 0($fp)
	sw $a2 4($fp)
	# Caller save reg end
	or $a1 $a1 $zero
	or $a2 $a2 $zero
	#Base block 1 clear start
	#Base block 1 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_choose_pivot
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	lw $a2 4($fp)
	# Caller pop reg end
	or $t0 $v1 $zero
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	lw $t1 0($t1)
	or $s4 $t1 $zero
	or $s0 $a1 $zero
	or $s1 $a2 $zero
	#Base block 2 clear start
	sw $t0 60($sp)
	sw $t1 64($sp)
	#Base block 2 clear end
label_t50:
	subu $t0 $s0 $s1
	#Base block 2 save start
	#Base block 2 save end
	bgtz $t0 label_t51
	#Base block 3 clear start
	#Base block 3 clear end
label_t52:
	sll $s2 $s1 2
	addu $s2 $s2 $gp
	lw $s2 0($s2)
	subu $t0 $s4 $s2
	#Base block 3 save start
	#Base block 3 save end
	bgez $t0 label_t54
	#Base block 4 clear start
	#Base block 4 clear end
	li $t0 1
	subu $s1 $s1 $t0
	#Base block 4 save start
	#Base block 4 save end
	j label_t52
	#Base block 5 clear start
	#Base block 5 clear end
label_t54:
	#Base block 6 clear start
	#Base block 6 clear end
label_t56:
	sll $s3 $s0 2
	addu $s3 $s3 $gp
	lw $s3 0($s3)
	subu $t0 $s4 $s3
	#Base block 6 save start
	#Base block 6 save end
	blez $t0 label_t58
	#Base block 7 clear start
	#Base block 7 clear end
	li $t0 1
	addu $s0 $s0 $t0
	#Base block 7 save start
	#Base block 7 save end
	j label_t56
	#Base block 8 clear start
	#Base block 8 clear end
label_t58:
	subu $t0 $s0 $s1
	#Base block 8 save start
	#Base block 8 save end
	bgtz $t0 label_t60
	#Base block 9 clear start
	#Base block 9 clear end
	sll $s5 $s0 2
	addu $s5 $s5 $gp
	lw $s5 0($s5)
	or $s7 $s5 $zero
	sll $s6 $s1 2
	addu $s6 $s6 $gp
	lw $s6 0($s6)
	sll $t0 $s0 2
	addu $t0 $t0 $gp
	sw $s6 0($t0)
	sll $t0 $s1 2
	addu $t0 $t0 $gp
	sw $s7 0($t0)
	li $t0 1
	addu $s0 $s0 $t0
	subu $s1 $s1 $t0
	#Base block 9 save start
	#Base block 9 save end
	j label_t67
	#Base block 10 clear start
	#Base block 10 clear end
label_t60:
	#Base block 11 clear start
	#Base block 11 clear end
label_t67:
	#Base block 11 save start
	#Base block 11 save end
	j label_t50
	#Base block 12 clear start
	#Base block 12 clear end
label_t51:
	subu $t0 $s0 $a2
	#Base block 12 save start
	#Base block 12 save end
	bgez $t0 label_t68
	#Base block 13 clear start
	#Base block 13 clear end
	# Caller save reg start
	sw $a1 0($fp)
	sw $a2 4($fp)
	# Caller save reg end
	or $a1 $s0 $zero
	or $a2 $a2 $zero
	#Base block 13 clear start
	#Base block 13 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_quicksort
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	lw $a2 4($fp)
	# Caller pop reg end
	#Base block 13 save start
	#Base block 13 save end
	j label_t70
	#Base block 14 clear start
	#Base block 14 clear end
label_t68:
	#Base block 15 clear start
	#Base block 15 clear end
label_t70:
	subu $t0 $a1 $s1
	#Base block 15 save start
	#Base block 15 save end
	bgez $t0 label_t71
	#Base block 16 clear start
	#Base block 16 clear end
	# Caller save reg start
	sw $a1 0($fp)
	sw $a2 4($fp)
	# Caller save reg end
	or $a1 $a1 $zero
	or $a2 $s1 $zero
	#Base block 16 clear start
	#Base block 16 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_quicksort
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	lw $a2 4($fp)
	# Caller pop reg end
	#Base block 16 save start
	#Base block 16 save end
	j label_t73
	#Base block 17 clear start
	#Base block 17 clear end
label_t71:
	#Base block 18 clear start
	#Base block 18 clear end
label_t73:
	#Base block 18 clear start
	#Base block 18 clear end
label_quicksort_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	lw $s3 20($sp)
	lw $s4 24($sp)
	lw $s5 28($sp)
	lw $s6 32($sp)
	lw $s7 36($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 108
	# end of epilogue
	jr $ra
label_exprcheck:
	# start of prologue
	addiu $sp $sp -140
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	sw $s3 20($sp)
	sw $s4 24($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $v0 5
	syscall
	or $s0 $v0 $zero
	li $v0 12
	syscall
	or $s1 $v0 $zero
	li $v0 5
	syscall
	or $s2 $v0 $zero
	li $v0 1
	or $a0 $s0 $zero
	syscall
	li $v0 11
	or $a0 $s1 $zero
	syscall
	li $v0 1
	or $a0 $s2 $zero
	syscall
	li $v0 4
	la $a0 str_1
	syscall
	li $s3 1
	li $t0 43
	subu $t1 $s1 $t0
	#Base block 1 save start
	#Base block 1 save end
	bne $t1 $zero label_t81
	#Base block 2 clear start
	#Base block 2 clear end
	addu $s4 $s0 $s2
	li $v0 1
	or $a0 $s4 $zero
	syscall
	li $s3 0
	#Base block 2 save start
	#Base block 2 save end
	j label_t80
	#Base block 3 clear start
	#Base block 3 clear end
label_t81:
	li $t0 45
	subu $t1 $s1 $t0
	#Base block 3 save start
	#Base block 3 save end
	bne $t1 $zero label_t85
	#Base block 4 clear start
	#Base block 4 clear end
	subu $s4 $s0 $s2
	li $v0 1
	or $a0 $s4 $zero
	syscall
	li $s3 0
	#Base block 4 save start
	#Base block 4 save end
	j label_t80
	#Base block 5 clear start
	#Base block 5 clear end
label_t85:
	li $t0 42
	subu $t1 $s1 $t0
	#Base block 5 save start
	#Base block 5 save end
	bne $t1 $zero label_t89
	#Base block 6 clear start
	#Base block 6 clear end
	mul $s4 $s0 $s2
	li $v0 1
	or $a0 $s4 $zero
	syscall
	li $s3 0
	#Base block 6 save start
	#Base block 6 save end
	j label_t80
	#Base block 7 clear start
	#Base block 7 clear end
label_t89:
	li $t0 47
	subu $t1 $s1 $t0
	#Base block 7 save start
	#Base block 7 save end
	bne $t1 $zero label_t93
	#Base block 8 clear start
	#Base block 8 clear end
	li $s3 0
	li $t0 0
	subu $t1 $s2 $t0
	#Base block 8 save start
	#Base block 8 save end
	bne $t1 $zero label_t95
	#Base block 9 clear start
	#Base block 9 clear end
	li $v0 4
	la $a0 str_2
	syscall
	#Base block 9 save start
	#Base block 9 save end
	j label_t98
	#Base block 10 clear start
	#Base block 10 clear end
label_t95:
	div $s4 $s0 $s2
	li $v0 1
	or $a0 $s4 $zero
	syscall
	#Base block 11 clear start
	#Base block 11 clear end
label_t98:
	#Base block 11 save start
	#Base block 11 save end
	j label_t80
	#Base block 12 clear start
	#Base block 12 clear end
label_t93:
	#Base block 13 clear start
	#Base block 13 clear end
label_t80:
	#Base block 13 save start
	#Base block 13 save end
	beq $s3 $zero label_t101
	#Base block 14 clear start
	#Base block 14 clear end
	li $v0 4
	la $a0 str_3
	syscall
	li $v0 11
	or $a0 $s1 $zero
	syscall
	li $v0 4
	la $a0 str_4
	syscall
	#Base block 14 save start
	#Base block 14 save end
	j label_t106
	#Base block 15 clear start
	#Base block 15 clear end
label_t101:
	#Base block 16 clear start
	#Base block 16 clear end
label_t106:
	#Base block 16 clear start
	#Base block 16 clear end
label_exprcheck_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	lw $s3 20($sp)
	lw $s4 24($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 140
	# end of epilogue
	jr $ra
label_arraychek:
	# start of prologue
	addiu $sp $sp -88
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	sw $s3 20($sp)
	sw $s4 24($sp)
	sw $s5 28($sp)
	sw $s6 32($sp)
	sw $s7 36($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $s0 0
	#Base block 2 clear start
	#Base block 2 clear end
label_t107:
	li $t0 10
	subu $t1 $s0 $t0
	#Base block 2 save start
	#Base block 2 save end
	bgez $t1 label_t109
	#Base block 3 clear start
	#Base block 3 clear end
	li $t0 10
	subu $s1 $t0 $s0
	sll $t1 $s0 2
	addu $t1 $t1 $gp
	sw $s1 0($t1)
	li $t1 1
	addu $s0 $s0 $t1
	#Base block 3 save start
	#Base block 3 save end
	j label_t107
	#Base block 4 clear start
	#Base block 4 clear end
label_t109:
	li $t0 1
	li $t1 16
	sll $t2 $t0 2
	addu $t2 $t2 $gp
	sw $t1 0($t2)
	li $t2 2
	li $t3 3
	sll $t4 $t2 2
	addu $t4 $t4 $gp
	sw $t3 0($t4)
	li $t4 -8
	sll $t5 $t3 2
	addu $t5 $t5 $gp
	sw $t4 0($t5)
	li $t5 4
	li $t6 40
	sll $t7 $t5 2
	addu $t7 $t7 $gp
	sw $t6 0($t7)
	li $t7 5
	li $t8 -1
	sll $t9 $t7 2
	addu $t9 $t9 $gp
	sw $t8 0($t9)
	li $t9 6
	sll $t1 $t9 2
	addu $t1 $t1 $gp
	sw $t2 0($t1)
	li $t1 7
	li $t2 -10
	sll $t3 $t1 2
	addu $t3 $t3 $gp
	sw $t2 0($t3)
	or $s0 $t0 $zero
	# Caller save reg start
	# Caller save reg end
	or $a1 $t0 $zero
	or $a2 $t0 $zero
	#Base block 4 clear start
	sw $t2 -40($sp)
	sw $t4 -32($sp)
	sw $t8 -4($sp)
	#Base block 4 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_choose_pivot
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s2 $v1 $zero
	subu $s3 $s2 $s0
	# Caller save reg start
	# Caller save reg end
	li $a1 0
	or $a2 $s3 $zero
	#Base block 4 clear start
	#Base block 4 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_choose_pivot
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s4 $v1 $zero
	# Caller save reg start
	# Caller save reg end
	li $a1 9
	li $a2 9
	#Base block 4 clear start
	#Base block 4 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_choose_pivot
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s5 $v1 $zero
	# Caller save reg start
	# Caller save reg end
	or $a1 $s4 $zero
	or $a2 $s5 $zero
	#Base block 4 clear start
	#Base block 4 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_quicksort
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	# Caller save reg start
	# Caller save reg end
	li $a1 10
	#Base block 4 clear start
	#Base block 4 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_printlist
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 4 clear start
	#Base block 4 clear end
label_arraychek_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	lw $s3 20($sp)
	lw $s4 24($sp)
	lw $s5 28($sp)
	lw $s6 32($sp)
	lw $s7 36($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 88
	# end of epilogue
	jr $ra
label_recurcheck:
	# start of prologue
	addiu $sp $sp -64
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $v0 5
	syscall
	or $s0 $v0 $zero
	#Base block 2 clear start
	#Base block 2 clear end
label_t142:
	li $t0 13
	subu $t1 $t0 $s0
	#Base block 2 save start
	#Base block 2 save end
	bgtz $t1 label_t144
	#Base block 3 clear start
	#Base block 3 clear end
	li $v0 4
	la $a0 str_5
	syscall
	li $v0 5
	syscall
	or $s0 $v0 $zero
	#Base block 3 save start
	#Base block 3 save end
	j label_t142
	#Base block 4 clear start
	#Base block 4 clear end
label_t144:
	# Caller save reg start
	# Caller save reg end
	or $a1 $s0 $zero
	#Base block 4 clear start
	#Base block 4 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_fact
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s1 $v1 $zero
	li $v0 1
	or $a0 $s1 $zero
	syscall
	#Base block 4 clear start
	#Base block 4 clear end
label_recurcheck_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 64
	# end of epilogue
	jr $ra
label_lettercheck:
	# start of prologue
	addiu $sp $sp -60
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $v0 12
	syscall
	or $s1 $v0 $zero
	# Caller save reg start
	# Caller save reg end
	or $a1 $s1 $zero
	#Base block 1 clear start
	#Base block 1 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_isletter
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s0 $v1 $zero
	#Base block 1 save start
	#Base block 1 save end
	beq $s0 $zero label_t152
	#Base block 2 clear start
	#Base block 2 clear end
	li $v0 4
	la $a0 str_6
	syscall
	#Base block 2 save start
	#Base block 2 save end
	j label_t155
	#Base block 3 clear start
	#Base block 3 clear end
label_t152:
	li $v0 4
	la $a0 str_7
	syscall
	#Base block 4 clear start
	#Base block 4 clear end
label_t155:
	#Base block 4 clear start
	#Base block 4 clear end
label_lettercheck_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 60
	# end of epilogue
	jr $ra
label_main:
	# start of prologue
	addiu $sp $sp -116
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $v0 5
	syscall
	or $s5 $v0 $zero
	#Base block 2 clear start
	#Base block 2 clear end
label_t159:
	li $t0 0
	subu $t1 $s5 $t0
	#Base block 2 save start
	#Base block 2 save end
	beq $t1 $zero label_t160
	#Base block 3 clear start
	#Base block 3 clear end
	li $t0 0
	addu $s4 $s5 $t0
	li $t1 4
	mul $s1 $s4 $t1
	div $s2 $s1 $t1
	li $t2 19
	addu $s3 $s2 $t2
	subu $s0 $s3 $t2
	li $t3 1
	subu $t4 $s0 $t3
	#Base block 3 save start
	sw $t0 0($sp)
	#Base block 3 save end
	bne $t4 $zero label_t169
	#Base block 4 clear start
	#Base block 4 clear end
	# Caller save reg start
	# Caller save reg end
	#Base block 4 clear start
	#Base block 4 clear end
	or $fp $sp $zero
	jal label_exprcheck
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 4 save start
	#Base block 4 save end
	j label_t161
	#Base block 5 clear start
	#Base block 5 clear end
label_t169:
	li $t0 2
	subu $t1 $s0 $t0
	#Base block 5 save start
	#Base block 5 save end
	bne $t1 $zero label_t172
	#Base block 6 clear start
	#Base block 6 clear end
	# Caller save reg start
	# Caller save reg end
	#Base block 6 clear start
	#Base block 6 clear end
	or $fp $sp $zero
	jal label_arraychek
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 6 save start
	#Base block 6 save end
	j label_t161
	#Base block 7 clear start
	#Base block 7 clear end
label_t172:
	li $t0 3
	subu $t1 $s0 $t0
	#Base block 7 save start
	#Base block 7 save end
	bne $t1 $zero label_t175
	#Base block 8 clear start
	#Base block 8 clear end
	# Caller save reg start
	# Caller save reg end
	#Base block 8 clear start
	#Base block 8 clear end
	or $fp $sp $zero
	jal label_recurcheck
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 8 save start
	#Base block 8 save end
	j label_t161
	#Base block 9 clear start
	#Base block 9 clear end
label_t175:
	li $t0 4
	subu $t1 $s0 $t0
	#Base block 9 save start
	#Base block 9 save end
	bne $t1 $zero label_t178
	#Base block 10 clear start
	#Base block 10 clear end
	# Caller save reg start
	# Caller save reg end
	#Base block 10 clear start
	#Base block 10 clear end
	or $fp $sp $zero
	jal label_lettercheck
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 10 save start
	#Base block 10 save end
	j label_t161
	#Base block 11 clear start
	#Base block 11 clear end
label_t178:
	li $v0 4
	la $a0 str_8
	syscall
	li $v0 1
	or $a0 $s5 $zero
	syscall
	#Base block 12 clear start
	#Base block 12 clear end
label_t161:
	li $v0 4
	la $a0 str_9
	syscall
	li $v0 5
	syscall
	or $s5 $v0 $zero
	#Base block 12 save start
	#Base block 12 save end
	j label_t159
	#Base block 13 clear start
	#Base block 13 clear end
label_t160:
	#Base block 13 clear start
	#Base block 13 clear end
label_main_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 116
	# end of epilogue
	jr $ra
