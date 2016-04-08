.globl memr
memr:
	movq  $100000000, %rbx
	movq -8(%rbp), %rax
	movq $64, %r11
	movq $2, %r9
	
loop:
	movq (%rax), %r10
	addq %r11, %rax
	subq %r9, %rbx
	jne loop
	ret
