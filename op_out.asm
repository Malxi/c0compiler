.data
	#global allocate
	global: .space 140
	str_0: .asciiz " "
	str_1: .asciiz "expre:"
	str_2: .asciiz "fibo:"
	str_3: .asciiz "gcd:"
	str_4: .asciiz "string:!#$%&'()*+,-./:;<=>?@[\]^_`{|}~"
	str_5: .asciiz "Test Not Return"
.text
	la $gp global
	jal label_main
	li $a0 0
	li $v0 17
	syscall
label_testdefine:
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
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $t0 0
	li $t1 97
	sll $t2 $t0 2
	addu $t2 $t2 $gp
	sw $t1 36($t2)
	li $t2 1
	li $t3 98
	sll $t4 $t2 2
	addu $t4 $t4 $gp
	sw $t3 36($t4)
	li $t4 2
	li $t5 99
	sll $t6 $t4 2
	addu $t6 $t6 $gp
	sw $t5 36($t6)
	li $t6 3
	li $t7 100
	sll $t8 $t6 2
	addu $t8 $t8 $gp
	sw $t7 36($t8)
	li $t8 4
	li $t9 101
	sll $t0 $t8 2
	addu $t0 $t0 $gp
	sw $t9 36($t0)
	li $t0 5
	li $t2 102
	sll $t4 $t0 2
	addu $t4 $t4 $gp
	sw $t2 36($t4)
	li $t4 6
	li $t0 103
	sll $t6 $t4 2
	addu $t6 $t6 $gp
	sw $t0 36($t6)
	li $t6 7
	li $t4 104
	sll $t8 $t6 2
	addu $t8 $t8 $gp
	sw $t4 36($t8)
	li $t8 8
	li $t6 105
	sll $t0 $t8 2
	addu $t0 $t0 $gp
	sw $t6 36($t0)
	li $t0 9
	li $t8 106
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	sw $t8 36($t1)
	li $t1 10
	li $t0 107
	sll $t2 $t1 2
	addu $t2 $t2 $gp
	sw $t0 36($t2)
	li $t2 11
	li $t1 108
	sll $t0 $t2 2
	addu $t0 $t0 $gp
	sw $t1 36($t0)
	li $t0 12
	li $t2 109
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	sw $t2 36($t1)
	li $t1 13
	li $t0 110
	sll $t2 $t1 2
	addu $t2 $t2 $gp
	sw $t0 36($t2)
	li $t2 14
	li $t1 111
	sll $t0 $t2 2
	addu $t0 $t0 $gp
	sw $t1 36($t0)
	li $t0 15
	li $t2 112
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	sw $t2 36($t1)
	li $t1 16
	li $t0 113
	sll $t2 $t1 2
	addu $t2 $t2 $gp
	sw $t0 36($t2)
	li $t2 17
	li $t1 114
	sll $t0 $t2 2
	addu $t0 $t0 $gp
	sw $t1 36($t0)
	li $t0 18
	li $t2 115
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	sw $t2 36($t1)
	li $t1 19
	li $t0 116
	sll $t2 $t1 2
	addu $t2 $t2 $gp
	sw $t0 36($t2)
	li $t2 20
	li $t1 117
	sll $t0 $t2 2
	addu $t0 $t0 $gp
	sw $t1 36($t0)
	li $t0 21
	li $t2 118
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	sw $t2 36($t1)
	li $t1 22
	li $t0 119
	sll $t2 $t1 2
	addu $t2 $t2 $gp
	sw $t0 36($t2)
	li $t2 23
	li $t1 120
	sll $t0 $t2 2
	addu $t0 $t0 $gp
	sw $t1 36($t0)
	li $t0 24
	li $t2 121
	sll $t1 $t0 2
	addu $t1 $t1 $gp
	sw $t2 36($t1)
	li $t1 25
	li $t0 122
	sll $t2 $t1 2
	addu $t2 $t2 $gp
	sw $t0 36($t2)
	#Base block 1 clear start
	#Base block 1 clear end
label_testdefine_end:
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
	addiu $sp $sp 100
	# end of epilogue
	jr $ra
label_fibo:
	# start of prologue
	addiu $sp $sp -68
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
	li $t0 0
	subu $t1 $a1 $t0
	#Base block 1 save start
	#Base block 1 save end
	bgez $t1 label_t78
	#Base block 2 clear start
	#Base block 2 clear end
	li $v1 -1
	#Base block 2 clear start
	#Base block 2 clear end
	j label_fibo_end
	#Base block 2 save start
	#Base block 2 save end
	j label_t81
	#Base block 3 clear start
	#Base block 3 clear end
label_t78:
	li $t0 0
	subu $t1 $a1 $t0
	#Base block 3 save start
	#Base block 3 save end
	bne $t1 $zero label_t82
	#Base block 4 clear start
	#Base block 4 clear end
	li $v1 0
	#Base block 4 clear start
	#Base block 4 clear end
	j label_fibo_end
	#Base block 4 save start
	#Base block 4 save end
	j label_t83
	#Base block 5 clear start
	#Base block 5 clear end
label_t82:
	li $t0 1
	subu $t1 $a1 $t0
	#Base block 5 save start
	#Base block 5 save end
	bne $t1 $zero label_t84
	#Base block 6 clear start
	#Base block 6 clear end
	li $v1 1
	#Base block 6 clear start
	#Base block 6 clear end
	j label_fibo_end
	#Base block 6 save start
	#Base block 6 save end
	j label_t85
	#Base block 7 clear start
	#Base block 7 clear end
label_t84:
	li $t0 10
	subu $t1 $a1 $t0
	#Base block 7 save start
	#Base block 7 save end
	blez $t1 label_t86
	#Base block 8 clear start
	#Base block 8 clear end
	li $v1 -2
	#Base block 8 clear start
	#Base block 8 clear end
	j label_fibo_end
	#Base block 8 save start
	#Base block 8 save end
	j label_t89
	#Base block 9 clear start
	#Base block 9 clear end
label_t86:
	li $t0 1
	subu $s0 $a1 $t0
	# Caller save reg start
	sw $a1 0($fp)
	# Caller save reg end
	or $a1 $s0 $zero
	#Base block 9 clear start
	#Base block 9 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_fibo
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	# Caller pop reg end
	or $s1 $v1 $zero
	li $t0 2
	subu $s2 $a1 $t0
	# Caller save reg start
	sw $a1 0($fp)
	# Caller save reg end
	or $a1 $s2 $zero
	#Base block 9 clear start
	#Base block 9 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_fibo
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	# Caller pop reg end
	or $s3 $v1 $zero
	addu $s4 $s1 $s3
	or $v1 $s4 $zero
	#Base block 9 clear start
	#Base block 9 clear end
	j label_fibo_end
	#Base block 10 clear start
	#Base block 10 clear end
label_t89:
	#Base block 11 clear start
	#Base block 11 clear end
label_t85:
	#Base block 12 clear start
	#Base block 12 clear end
label_t83:
	#Base block 13 clear start
	#Base block 13 clear end
label_t81:
	#Base block 13 clear start
	#Base block 13 clear end
label_fibo_end:
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
	addiu $sp $sp 68
	# end of epilogue
	jr $ra
label_printspace:
	# start of prologue
	addiu $sp $sp -44
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $v0 4
	la $a0 str_0
	syscall
	#Base block 1 clear start
	#Base block 1 clear end
label_printspace_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 44
	# end of epilogue
	jr $ra
label_display:
	# start of prologue
	addiu $sp $sp -64
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $t0 0
	subu $t1 $a1 $t0
	#Base block 1 save start
	#Base block 1 save end
	bgtz $t1 label_t97
	#Base block 2 clear start
	#Base block 2 clear end
	#Base block 2 clear start
	#Base block 2 clear end
	j label_display_end
	#Base block 2 save start
	#Base block 2 save end
	j label_t98
	#Base block 3 clear start
	#Base block 3 clear end
label_t97:
	li $t0 26
	subu $t1 $a1 $t0
	#Base block 3 save start
	#Base block 3 save end
	blez $t1 label_t100
	#Base block 4 clear start
	#Base block 4 clear end
	li $a1 26
	#Base block 4 save start
	#Base block 4 save end
	j label_t101
	#Base block 5 clear start
	#Base block 5 clear end
label_t100:
	li $s0 0
	#Base block 6 clear start
	#Base block 6 clear end
label_t102:
	subu $t0 $s0 $a1
	#Base block 6 save start
	#Base block 6 save end
	bgez $t0 label_t103
	#Base block 7 clear start
	#Base block 7 clear end
	sll $s1 $s0 2
	addu $s1 $s1 $gp
	lw $s1 36($s1)
	li $v0 11
	or $a0 $s1 $zero
	syscall
	li $t0 1
	addu $s0 $s0 $t0
	#Base block 7 save start
	#Base block 7 save end
	j label_t102
	#Base block 8 clear start
	#Base block 8 clear end
label_t103:
	# Caller save reg start
	# Caller save reg end
	#Base block 8 clear start
	#Base block 8 clear end
	or $fp $sp $zero
	jal label_printspace
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 9 clear start
	#Base block 9 clear end
label_t101:
	#Base block 10 clear start
	#Base block 10 clear end
label_t98:
	#Base block 10 clear start
	#Base block 10 clear end
label_display_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 64
	# end of epilogue
	jr $ra
label_expre:
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
	li $v0 5
	syscall
	or $s6 $v0 $zero
	li $v0 5
	syscall
	or $t0 $v0 $zero
	addu $s0 $s6 $t0
	li $t1 -2
	mul $s1 $t0 $t1
	li $t2 0
	subu $s2 $t2 $s1
	subu $s3 $s0 $s2
	li $t3 0
	addu $s4 $s3 $t3
	li $t4 0
	addu $s7 $t4 $s4
	li $v0 4
	la $a0 str_1
	syscall
	li $v0 1
	or $a0 $s7 $zero
	syscall
	# Caller save reg start
	# Caller save reg end
	#Base block 1 clear start
	sw $t0 44($sp)
	sw $t1 -8($sp)
	sw $t3 0($sp)
	#Base block 1 clear end
	or $fp $sp $zero
	jal label_printspace
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 1 clear start
	#Base block 1 clear end
label_expre_end:
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
label_mod:
	# start of prologue
	addiu $sp $sp -56
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	div $s0 $a1 $a2
	mul $s1 $s0 $a2
	subu $s2 $a1 $s1
	or $v1 $s2 $zero
	#Base block 1 clear start
	#Base block 1 clear end
	j label_mod_end
	#Base block 1 clear start
	#Base block 1 clear end
label_mod_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 56
	# end of epilogue
	jr $ra
label_gcd:
	# start of prologue
	addiu $sp $sp -52
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	sw $s0 8($sp)
	sw $s1 12($sp)
	sw $s2 16($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $t0 0
	subu $t1 $a2 $t0
	#Base block 1 save start
	#Base block 1 save end
	bne $t1 $zero label_t128
	#Base block 2 clear start
	#Base block 2 clear end
	li $v1 0
	#Base block 2 clear start
	#Base block 2 clear end
	j label_gcd_end
	#Base block 2 save start
	#Base block 2 save end
	j label_t129
	#Base block 3 clear start
	#Base block 3 clear end
label_t128:
	# Caller save reg start
	sw $a1 0($fp)
	sw $a2 4($fp)
	# Caller save reg end
	or $a1 $a1 $zero
	or $a2 $a2 $zero
	#Base block 3 clear start
	#Base block 3 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_mod
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	lw $a2 4($fp)
	# Caller pop reg end
	or $s0 $v1 $zero
	li $t0 0
	subu $t1 $s0 $t0
	#Base block 3 save start
	#Base block 3 save end
	bne $t1 $zero label_t131
	#Base block 4 clear start
	#Base block 4 clear end
	or $v1 $a2 $zero
	#Base block 4 clear start
	#Base block 4 clear end
	j label_gcd_end
	#Base block 4 save start
	#Base block 4 save end
	j label_t132
	#Base block 5 clear start
	#Base block 5 clear end
label_t131:
	# Caller save reg start
	sw $a1 0($fp)
	sw $a2 4($fp)
	# Caller save reg end
	or $a1 $a1 $zero
	or $a2 $a2 $zero
	#Base block 5 clear start
	#Base block 5 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_mod
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	lw $a2 4($fp)
	# Caller pop reg end
	or $s1 $v1 $zero
	# Caller save reg start
	sw $a1 0($fp)
	sw $a2 4($fp)
	# Caller save reg end
	or $a1 $a2 $zero
	or $a2 $s1 $zero
	#Base block 5 clear start
	#Base block 5 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_gcd
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	lw $a1 0($fp)
	lw $a2 4($fp)
	# Caller pop reg end
	or $s2 $v1 $zero
	or $v1 $s2 $zero
	#Base block 5 clear start
	#Base block 5 clear end
	j label_gcd_end
	#Base block 6 clear start
	#Base block 6 clear end
label_t132:
	#Base block 7 clear start
	#Base block 7 clear end
label_t129:
	#Base block 7 clear start
	#Base block 7 clear end
label_gcd_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	lw $s0 8($sp)
	lw $s1 12($sp)
	lw $s2 16($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 52
	# end of epilogue
	jr $ra
label_process:
	# start of prologue
	addiu $sp $sp -116
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
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $v0 12
	syscall
	or $s6 $v0 $zero
	li $t0 97
	subu $t1 $s6 $t0
	#Base block 1 save start
	#Base block 1 save end
	bne $t1 $zero label_t137
	#Base block 2 clear start
	#Base block 2 clear end
	# Caller save reg start
	# Caller save reg end
	#Base block 2 clear start
	#Base block 2 clear end
	or $fp $sp $zero
	jal label_expre
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 2 save start
	#Base block 2 save end
	j label_t136
	#Base block 3 clear start
	#Base block 3 clear end
label_t137:
	li $t0 98
	subu $t1 $s6 $t0
	#Base block 3 save start
	#Base block 3 save end
	bne $t1 $zero label_t140
	#Base block 4 clear start
	#Base block 4 clear end
	li $v0 5
	syscall
	or $t0 $v0 $zero
	# Caller save reg start
	# Caller save reg end
	or $a1 $t0 $zero
	#Base block 4 clear start
	sw $t0 0($gp)
	#Base block 4 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_fibo
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s0 $v1 $zero
	or $t0 $s0 $zero
	li $v0 4
	la $a0 str_2
	syscall
	li $v0 1
	or $a0 $t0 $zero
	syscall
	# Caller save reg start
	# Caller save reg end
	#Base block 4 clear start
	sw $t0 20($gp)
	#Base block 4 clear end
	or $fp $sp $zero
	jal label_printspace
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 4 save start
	#Base block 4 save end
	j label_t136
	#Base block 5 clear start
	#Base block 5 clear end
label_t140:
	li $t0 100
	subu $t1 $s6 $t0
	#Base block 5 save start
	#Base block 5 save end
	bne $t1 $zero label_t147
	#Base block 6 clear start
	#Base block 6 clear end
	li $v0 5
	syscall
	or $t0 $v0 $zero
	# Caller save reg start
	# Caller save reg end
	or $a1 $t0 $zero
	#Base block 6 clear start
	sw $t0 16($gp)
	#Base block 6 clear end
	addiu $sp $sp -4
	or $fp $sp $zero
	jal label_display
	addiu $sp $sp 4
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 6 save start
	#Base block 6 save end
	j label_t136
	#Base block 7 clear start
	#Base block 7 clear end
label_t147:
	li $t0 101
	subu $t1 $s6 $t0
	#Base block 7 save start
	#Base block 7 save end
	bne $t1 $zero label_t151
	#Base block 8 clear start
	#Base block 8 clear end
	li $v0 5
	syscall
	or $t0 $v0 $zero
	li $v0 5
	syscall
	or $t1 $v0 $zero
	# Caller save reg start
	# Caller save reg end
	or $a1 $t0 $zero
	or $a2 $t1 $zero
	#Base block 8 clear start
	sw $t0 24($gp)
	sw $t1 28($gp)
	#Base block 8 clear end
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_gcd
	addiu $sp $sp 8
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	or $s1 $v1 $zero
	li $v0 4
	la $a0 str_3
	syscall
	li $v0 1
	or $a0 $s1 $zero
	syscall
	# Caller save reg start
	# Caller save reg end
	#Base block 8 clear start
	#Base block 8 clear end
	or $fp $sp $zero
	jal label_printspace
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 8 save start
	#Base block 8 save end
	j label_t136
	#Base block 9 clear start
	#Base block 9 clear end
label_t151:
	li $t0 102
	subu $t1 $s6 $t0
	#Base block 9 save start
	#Base block 9 save end
	bne $t1 $zero label_t158
	#Base block 10 clear start
	#Base block 10 clear end
	li $v0 4
	la $a0 str_4
	syscall
	#Base block 10 save start
	#Base block 10 save end
	j label_t136
	#Base block 11 clear start
	#Base block 11 clear end
label_t158:
	#Base block 12 clear start
	#Base block 12 clear end
label_t136:
	#Base block 12 clear start
	#Base block 12 clear end
label_process_end:
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
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 116
	# end of epilogue
	jr $ra
label_main:
	# start of prologue
	addiu $sp $sp -68
	# end of prologue
	# Callee save frame start
	sw $ra 0($sp)
	sw $fp 4($sp)
	# Callee save frame end
	#Base block 1 clear start
	#Base block 1 clear end
	li $s0 0
	# Caller save reg start
	# Caller save reg end
	#Base block 1 clear start
	#Base block 1 clear end
	or $fp $sp $zero
	jal label_testdefine
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	li $v0 5
	syscall
	or $s2 $v0 $zero
	#Base block 2 clear start
	#Base block 2 clear end
label_t164:
	subu $t0 $s0 $s2
	#Base block 2 save start
	#Base block 2 save end
	bgez $t0 label_t165
	#Base block 3 clear start
	#Base block 3 clear end
	li $t0 1
	addu $s0 $s0 $t0
	# Caller save reg start
	# Caller save reg end
	#Base block 3 clear start
	#Base block 3 clear end
	or $fp $sp $zero
	jal label_process
	lw $fp 4($sp)
	# Caller pop reg start
	# Caller pop reg end
	#Base block 3 save start
	#Base block 3 save end
	j label_t164
	#Base block 4 clear start
	#Base block 4 clear end
label_t165:
	#Base block 4 clear start
	#Base block 4 clear end
	j label_main_end
	li $v0 4
	la $a0 str_5
	syscall
	#Base block 4 clear start
	#Base block 4 clear end
label_main_end:
	# Callee pop frame start
	lw $ra 0($sp)
	lw $fp 4($sp)
	# Callee pop frame end
	# start of epilogue
	addiu $sp $sp 68
	# end of epilogue
	jr $ra
