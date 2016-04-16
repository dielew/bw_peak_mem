.globl memr
memr:
	movq  $225000000, %rbx
	movq -152(%rbp), %rax
	movq (%rax), %rax
	movq $64, %r11
	movq $4, %r9
	
loop:
	movq (%rax), %r10
	addq %r11, %rax
	subq %r9, %rbx
	jne loop
	ret
