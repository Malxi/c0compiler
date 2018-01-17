.data
	#global allocate
	global: .space 1228
	str_0: .asciiz "The result of case i is done!"
	str_1: .asciiz "The result of case c is done!"
	str_2: .asciiz "result one is:"
	str_3: .asciiz "result two is:"
	str_4: .asciiz "The sum of numbers between two results is:"
	str_5: .asciiz "no result!"
	str_6: .asciiz "wrong input!"
	str_7: .asciiz "wrong input!"
.text
	la $gp global
	jal label_main
	nop
	li $a0 0
	li $v0 17
	syscall
label__asfunc_1:
	# start of prologue
	addiu $sp $sp -96
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	lw $t1 0($fp)
	or $t0 $t1 $zero
	sw $t0 40($sp)
	lw $t1 4($fp)
	or $t0 $t1 $zero
	sw $t0 48($sp)
	li $t1 1
	or $t0 $t1 $zero
	sw $t0 44($sp)
	lw $t1 40($sp)
	lw $t2 44($sp)
	subu $t0 $t1 $t2
	blez $t0 label_t1
	nop
	lw $t1 48($sp)
	li $t2 105
	subu $t0 $t1 $t2
	sw $t0 52($sp)
	lw $t1 52($sp)
	li $t2 0
	subu $t0 $t1 $t2
	bne $t0 $zero label_t5
	nop
	lw $t1 40($sp)
	lw $t2 44($sp)
	subu $t0 $t1 $t2
	sw $t0 60($sp)
	li $t0 105
	sw $t0 -4($sp)
	lw $t0 60($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label__asfunc_1
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	sw $v1 64($sp)
	lw $t1 0($fp)
	lw $t2 64($sp)
	mul $t0 $t1 $t2
	sw $t0 68($sp)
	lw $v1 68($sp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 96
	# end of epilogue
	jr $ra
	nop
	j label_t2
	nop
label_t5:
	lw $t1 40($sp)
	li $t2 1
	subu $t0 $t1 $t2
	sw $t0 72($sp)
	li $t0 99
	sw $t0 -4($sp)
	lw $t0 72($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label__asfunc_1
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	sw $v1 76($sp)
	lw $t1 0($fp)
	lw $t2 76($sp)
	mul $t0 $t1 $t2
	sw $t0 80($sp)
	lw $v1 80($sp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 96
	# end of epilogue
	jr $ra
	nop
label_t2:
	j label_t14
	nop
label_t1:
	lw $t1 48($sp)
	li $t2 105
	subu $t0 $t1 $t2
	bne $t0 $zero label_t16
	nop
	li $v0 4
	la $a0 str_0
	syscall
	j label_t15
	nop
label_t16:
	li $v0 4
	la $a0 str_1
	syscall
label_t15:
	li $v1 1
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 96
	# end of epilogue
	jr $ra
	nop
label_t14:
label_min:
	# start of prologue
	addiu $sp $sp -40
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	lw $t1 0($fp)
	lw $t2 4($fp)
	subu $t0 $t1 $t2
	bgez $t0 label_t22
	nop
	lw $v1 0($fp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 40
	# end of epilogue
	jr $ra
	nop
	j label_t23
	nop
label_t22:
	lw $t1 0($fp)
	lw $t2 4($fp)
	subu $t0 $t1 $t2
	bne $t0 $zero label_t24
	nop
	lw $v1 0($fp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 40
	# end of epilogue
	jr $ra
	nop
	j label_t25
	nop
label_t24:
	lw $v1 4($fp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 40
	# end of epilogue
	jr $ra
	nop
label_t25:
label_t23:
label_max:
	# start of prologue
	addiu $sp $sp -40
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	lw $t1 0($fp)
	lw $t2 4($fp)
	subu $t0 $t1 $t2
	blez $t0 label_t26
	nop
	lw $v1 0($fp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 40
	# end of epilogue
	jr $ra
	nop
	j label_t27
	nop
label_t26:
	lw $v1 4($fp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 40
	# end of epilogue
	jr $ra
	nop
label_t27:
label_no_sfc:
	# start of prologue
	addiu $sp $sp -80
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	lw $t1 0($fp)
	or $t0 $t1 $zero
	sw $t0 40($sp)
	lw $t1 4($fp)
	or $t0 $t1 $zero
	sw $t0 44($sp)
	lw $t1 40($sp)
	li $t2 2000
	subu $t0 $t1 $t2
	blez $t0 label_t28
	nop
	li $t1 0
	li $t2 1
	subu $t0 $t1 $t2
	sw $t0 48($sp)
	lw $t1 48($sp)
	or $t0 $t1 $zero
	sw $t0 8($gp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 80
	# end of epilogue
	jr $ra
	nop
	j label_t31
	nop
label_t28:
label_t31:
	lw $t0 44($sp)
	sw $t0 -4($sp)
	lw $t0 40($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_min
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	sw $v1 52($sp)
	lw $t1 52($sp)
	or $t0 $t1 $zero
	sw $t0 0($gp)
	lw $t0 44($sp)
	sw $t0 -4($sp)
	lw $t0 40($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_max
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	sw $v1 56($sp)
	lw $t1 56($sp)
	or $t0 $t1 $zero
	sw $t0 4($gp)
	lw $t1 0($gp)
	lw $t2 4($gp)
	addu $t0 $t1 $t2
	sw $t0 60($sp)
	lw $t1 4($gp)
	lw $t2 0($gp)
	subu $t0 $t1 $t2
	sw $t0 64($sp)
	lw $t1 64($sp)
	li $t2 1
	addu $t0 $t1 $t2
	sw $t0 68($sp)
	lw $t1 60($sp)
	lw $t2 68($sp)
	mul $t0 $t1 $t2
	sw $t0 72($sp)
	lw $t1 72($sp)
	li $t2 2
	div $t0 $t1 $t2
	sw $t0 76($sp)
	lw $t1 76($sp)
	or $t0 $t1 $zero
	sw $t0 8($gp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 80
	# end of epilogue
	jr $ra
	nop
label_print1:
	# start of prologue
	addiu $sp $sp -68
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	li $v0 4
	la $a0 str_2
	syscall
	li $v0 1
	lw $a0 0($fp)
	syscall
	li $t1 0
	lw $t2 4($fp)
	addu $t0 $t1 $t2
	sw $t0 48($sp)
	lw $t1 48($sp)
	li $t2 2
	addu $t0 $t1 $t2
	sw $t0 52($sp)
	lw $t1 52($sp)
	li $t2 1
	subu $t0 $t1 $t2
	sw $t0 56($sp)
	lw $t1 56($sp)
	li $t2 1
	subu $t0 $t1 $t2
	sw $t0 60($sp)
	li $v0 4
	la $a0 str_3
	syscall
	li $v0 1
	lw $a0 60($sp)
	syscall
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 68
	# end of epilogue
	jr $ra
	nop
label_print2:
	# start of prologue
	addiu $sp $sp -52
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	li $t1 0
	li $t2 1
	subu $t0 $t1 $t2
	sw $t0 40($sp)
	lw $t1 8($gp)
	lw $t2 40($sp)
	subu $t0 $t1 $t2
	beq $t0 $zero label_t52
	nop
	li $v0 4
	la $a0 str_4
	syscall
	li $v0 1
	lw $a0 8($gp)
	syscall
	j label_t55
	nop
label_t52:
	li $v0 4
	la $a0 str_5
	syscall
label_t55:
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 52
	# end of epilogue
	jr $ra
	nop
label_main:
	# start of prologue
	addiu $sp $sp -164
	# end of prologue
	sw $ra 0($sp)
	sw $fp 4($sp)
	li $v0 5
	syscall
	sw $v0 40($sp)
	li $v0 5
	syscall
	sw $v0 44($sp)
	li $v0 12
	syscall
	sw $v0 80($sp)
	li $v0 12
	syscall
	sw $v0 84($sp)
	lw $t1 40($sp)
	li $t2 0
	subu $t0 $t1 $t2
	bgez $t0 label_t60
	nop
	li $v0 4
	la $a0 str_6
	syscall
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 164
	# end of epilogue
	jr $ra
	nop
	j label_t63
	nop
label_t60:
	lw $t1 44($sp)
	li $t2 0
	subu $t0 $t1 $t2
	bgez $t0 label_t64
	nop
	li $v0 4
	la $a0 str_7
	syscall
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 164
	# end of epilogue
	jr $ra
	nop
	j label_t67
	nop
label_t64:
label_t67:
label_t63:
	lw $t1 40($sp)
	lw $t2 44($sp)
	subu $t0 $t1 $t2
	bltz $t0 label_t68
	nop
	lw $t1 40($sp)
	or $t0 $t1 $zero
	sw $t0 48($sp)
	j label_t69
	nop
label_t68:
	lw $t1 44($sp)
	or $t0 $t1 $zero
	sw $t0 48($sp)
label_t69:
	lw $t1 48($sp)
	li $t2 0
	subu $t0 $t1 $t2
	bgtz $t0 label_t70
	nop
label_t71:
	li $t1 0
	li $t2 0
	addu $t0 $t1 $t2
	sw $t0 108($sp)
	lw $t1 48($sp)
	lw $t2 108($sp)
	subu $t0 $t1 $t2
	bgtz $t0 label_t74
	nop
	lw $t1 48($sp)
	li $t2 1
	addu $t0 $t1 $t2
	sw $t0 112($sp)
	lw $t1 112($sp)
	or $t0 $t1 $zero
	sw $t0 48($sp)
	j label_t71
	nop
label_t74:
	j label_t76
	nop
label_t70:
	lw $t1 48($sp)
	li $t2 1
	addu $t0 $t1 $t2
	sw $t0 116($sp)
	lw $t1 116($sp)
	or $t0 $t1 $zero
	sw $t0 48($sp)
label_t76:
	li $t1 0
	lw $t2 80($sp)
	addu $t0 $t1 $t2
	sw $t0 120($sp)
	lw $t1 120($sp)
	lw $t2 84($sp)
	subu $t0 $t1 $t2
	sw $t0 124($sp)
	lw $t1 124($sp)
	li $t2 1
	subu $t0 $t1 $t2
	bne $t0 $zero label_t82
	nop
	li $t1 1
	or $t0 $t1 $zero
	sw $t0 52($sp)
	j label_t78
	nop
label_t82:
	lw $t1 124($sp)
	li $t2 2
	subu $t0 $t1 $t2
	bne $t0 $zero label_t84
	nop
	li $t1 2
	or $t0 $t1 $zero
	sw $t0 52($sp)
	j label_t78
	nop
label_t84:
	lw $t1 124($sp)
	li $t2 -1
	subu $t0 $t1 $t2
	bne $t0 $zero label_t86
	nop
	li $t1 3
	or $t0 $t1 $zero
	sw $t0 52($sp)
	j label_t78
	nop
label_t86:
	lw $t1 124($sp)
	li $t2 -2
	subu $t0 $t1 $t2
	bne $t0 $zero label_t89
	nop
	li $t1 4
	or $t0 $t1 $zero
	sw $t0 52($sp)
	j label_t78
	nop
label_t89:
	li $t1 5
	or $t0 $t1 $zero
	sw $t0 52($sp)
label_t78:
	li $t0 105
	sw $t0 -4($sp)
	lw $t0 48($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label__asfunc_1
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	sw $v1 144($sp)
	lw $t1 144($sp)
	or $t0 $t1 $zero
	sw $t0 56($sp)
	li $t0 99
	sw $t0 -4($sp)
	lw $t0 52($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label__asfunc_1
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	sw $v1 148($sp)
	lw $t1 148($sp)
	or $t0 $t1 $zero
	sw $t0 60($sp)
	lw $t0 60($sp)
	sw $t0 -4($sp)
	lw $t0 56($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_print1
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	lw $t0 60($sp)
	sw $t0 -4($sp)
	lw $t0 56($sp)
	sw $t0 -8($sp)
	addiu $sp $sp -8
	or $fp $sp $zero
	jal label_no_sfc
	nop
	addiu $sp $sp 8
	lw $fp 4($sp)
	or $fp $sp $zero
	jal label_print2
	nop
	lw $fp 4($sp)
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 164
	# end of epilogue
	jr $ra
	nop
	lw $ra 0($sp)
	lw $fp 4($sp)
	# start of epilogue
	addiu $sp $sp 164
	# end of epilogue
	jr $ra
	nop
