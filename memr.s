.globl memr
memr:
	mov $6000000, %rbx
	leaq -64(%rbp), %r12
	leaq -64000064(%rbp), %r13
	leaq -128000064(%rbp), %r14
	leaq -192000064(%rbp), %r15
	leaq -256000064(%rbp), %r10
	leaq -320000064(%rbp), %r9
	leaq -384000064(%rbp), %r8
	leaq -448000064(%rbp), %r11

loop:
	movaps (%r12), %xmm0
	movaps (%r13), %xmm1
	movaps (%r14), %xmm2
	movaps (%r15), %xmm3
	movaps (%r10), %xmm4
	movaps (%r9), %xmm5
	movaps (%r8), %xmm6
	movaps (%r11), %xmm7
	sub $64, %r12
	sub $64, %r13
	sub $64, %r14
	sub $64, %r15
	sub $64, %r10
	sub $64, %r9
	sub $64, %r8
	sub $64, %r11
	sub $8, %rbx
	jne loop
	ret
