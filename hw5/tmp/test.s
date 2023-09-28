	.file	"test.c"
	.option nopic
	.text
	.align	1
	.globl	sum
	.type	sum, @function
sum:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	li	a5,0
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	sum, .-sum
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-80
	sw	ra,76(sp)
	sw	s0,72(sp)
	addi	s0,sp,80
	addi	a5,s0,-68
	mv	a0,a5
	call	sum
	sw	a0,-20(s0)
	li	a5,0
	mv	a0,a5
	lw	ra,76(sp)
	lw	s0,72(sp)
	addi	sp,sp,80
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
