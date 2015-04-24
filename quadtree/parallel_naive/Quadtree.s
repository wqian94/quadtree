	.file	"Quadtree.c"
	.text
	.type	Marsaglia_rands, @function
Marsaglia_rands:
.LFB50:
	.cfi_startproc
	movl	(%rdi), %eax
	testl	%eax, %eax
	movl	$1, %edx
	cmove	%edx, %eax
	movl	%eax, %edx
	sall	$6, %edx
	xorl	%edx, %eax
	movl	%eax, %edx
	shrl	$21, %edx
	xorl	%edx, %eax
	movl	%eax, %edx
	sall	$7, %edx
	xorl	%edx, %eax
	movl	%eax, (%rdi)
	andl	$2147483647, %eax
	ret
	.cfi_endproc
.LFE50:
	.size	Marsaglia_rands, .-Marsaglia_rands
	.type	pthread_mutex_attr, @function
pthread_mutex_attr:
.LFB54:
	.cfi_startproc
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE54:
	.size	pthread_mutex_attr, .-pthread_mutex_attr
	.type	in_range, @function
in_range:
.LFB56:
	.cfi_startproc
	movsd	16(%rdi), %xmm0
	mulsd	.LC0(%rip), %xmm0
	movq	8(%rdi), %rax
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	ucomisd	%xmm2, %xmm1
	jb	.L16
	movq	8(%rdi), %rax
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm0, %xmm2
	ucomisd	%xmm1, %xmm2
	jbe	.L17
	movq	8(%rdi), %rax
	movss	4(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	ucomisd	%xmm2, %xmm1
	jb	.L18
	movq	8(%rdi), %rax
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	seta	%al
	movzbl	%al, %eax
	ret
.L16:
	movl	$0, %eax
	ret
.L17:
	movl	$0, %eax
	ret
.L18:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE56:
	.size	in_range, .-in_range
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"true"
.LC2:
	.string	"false"
.LC3:
	.string	"NULL"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"pointer = %p, is_square = %s, center = (%.6lf, %.6lf), length = %llu, parent = %p, up = %p, down = %p, children = {%p, %p, %p, %p}, dirty = %s, id = %llu\n"
	.text
	.globl	print
	.type	print, @function
print:
.LFB59:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$72, %rsp
	.cfi_def_cfa_offset 112
	testq	%rdi, %rdi
	jne	.L20
	movl	$.LC3, %edi
	call	puts
	jmp	.L19
.L20:
	movq	128(%rdi), %r12
	movzbl	80(%rdi), %eax
	testb	%al, %al
	movl	$.LC1, %eax
	movl	$.LC2, %ebp
	cmovne	%rax, %rbp
	movq	72(%rdi), %rax
	movl	$0, %ebx
	testq	%rax, %rax
	je	.L23
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L23
	movq	72(%rdi), %rbx
.L23:
	movq	64(%rdi), %rax
	movl	$0, %r11d
	testq	%rax, %rax
	je	.L24
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L24
	movq	64(%rdi), %r11
.L24:
	movq	56(%rdi), %rax
	movl	$0, %r10d
	testq	%rax, %rax
	je	.L25
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L25
	movq	56(%rdi), %r10
.L25:
	movq	48(%rdi), %rax
	movl	$0, %esi
	testq	%rax, %rax
	je	.L26
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L26
	movq	48(%rdi), %rsi
.L26:
	movq	40(%rdi), %rax
	movl	$0, %edx
	testq	%rax, %rax
	je	.L27
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L27
	movq	40(%rdi), %rdx
.L27:
	movq	32(%rdi), %rcx
	movl	$0, %eax
	testq	%rcx, %rcx
	je	.L28
	movzbl	80(%rcx), %ecx
	testb	%cl, %cl
	jne	.L28
	movq	32(%rdi), %rax
.L28:
	movq	24(%rdi), %rcx
	movl	$0, %r9d
	testq	%rcx, %rcx
	je	.L29
	movzbl	80(%rcx), %ecx
	testb	%cl, %cl
	jne	.L29
	movq	24(%rdi), %r9
.L29:
	movsd	16(%rdi), %xmm0
	ucomisd	.LC4(%rip), %xmm0
	jae	.L30
	cvttsd2siq	%xmm0, %r8
	jmp	.L31
.L30:
	subsd	.LC4(%rip), %xmm0
	cvttsd2siq	%xmm0, %r8
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %r8
.L31:
	movq	8(%rdi), %rcx
	movss	4(%rcx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movq	8(%rdi), %rcx
	movss	(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movzbl	(%rdi), %ecx
	testb	%cl, %cl
	movl	$.LC1, %ecx
	movl	$.LC2, %r13d
	cmove	%r13, %rcx
	movq	%r12, 56(%rsp)
	movq	%rbp, 48(%rsp)
	movq	%rbx, 40(%rsp)
	movq	%r11, 32(%rsp)
	movq	%r10, 24(%rsp)
	movq	%rsi, 16(%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rax, (%rsp)
	movq	%rdi, %rdx
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$2, %eax
	call	__printf_chk
.L19:
	addq	$72, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE59:
	.size	print, .-print
	.globl	Quadtree_init
	.type	Quadtree_init, @function
Quadtree_init:
.LFB61:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	movsd	%xmm0, 8(%rsp)
	movq	%xmm1, 16(%rsp)
	movl	$136, %edi
	call	malloc
	movq	%rax, %rbx
	movb	$0, (%rax)
	movsd	8(%rsp), %xmm0
	movsd	%xmm0, 16(%rax)
	movl	$8, %edi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	$0, 24(%rbx)
	movq	$0, 32(%rbx)
	movq	$0, 40(%rbx)
	movq	$0, 48(%rbx)
	movq	$0, 56(%rbx)
	movq	$0, 64(%rbx)
	movq	$0, 72(%rbx)
	movl	$0, %eax
	call	pthread_mutex_attr
	leaq	88(%rbx), %rdi
	movq	%rax, %rsi
	call	pthread_mutex_init
	movb	$0, 80(%rbx)
	movq	8(%rbx), %rsi
	leaq	16(%rsp), %rdi
	call	Point_copy
	movb	$1, (%rbx)
	movq	%rbx, %rax
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE61:
	.size	Quadtree_init, .-Quadtree_init
	.section	.rodata.str1.1
.LC6:
	.string	"locked=%s "
.LC7:
	.string	"Total length: %llu\n"
	.text
	.globl	print_LockSet
	.type	print_LockSet, @function
print_LockSet:
.LFB68:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbx
	movl	$0, %ebp
	testq	%rdi, %rdi
	je	.L51
	movl	$.LC2, %r12d
.L53:
	movzbl	8(%rbx), %eax
	testb	%al, %al
	movl	$.LC1, %edx
	cmove	%r12, %rdx
	movl	$.LC6, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movq	(%rbx), %rdi
	call	print
	movq	16(%rbx), %rbx
	addq	$1, %rbp
	testq	%rbx, %rbx
	jne	.L53
.L51:
	movq	%rbp, %rdx
	movl	$.LC7, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE68:
	.size	print_LockSet, .-print_LockSet
	.globl	Quadtree_search_helper
	.type	Quadtree_search_helper, @function
Quadtree_search_helper:
.LFB69:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	movsd	16(%rdi), %xmm0
	mulsd	.LC0(%rip), %xmm0
	movq	8(%rdi), %rax
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm2, %xmm1
	jb	.L58
	movq	8(%rdi), %rax
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm1, %xmm2
	jbe	.L58
	movq	8(%rdi), %rax
	movss	4(%rsi), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	movl	$0, %eax
	ucomisd	%xmm3, %xmm2
	jb	.L58
	movq	8(%rdi), %rax
	movss	4(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm3, %xmm0
	movl	$0, %eax
	ucomisd	%xmm2, %xmm0
	jbe	.L58
	movq	8(%rdi), %rcx
	movss	(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	.LC8(%rip), %xmm3
	subsd	%xmm3, %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%dl
	movss	4(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm3, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%edx, %eax
	movzbl	%al, %edx
	movslq	%edx, %rax
	addq	$6, %rax
	movq	(%rdi,%rax,8), %rax
	testq	%rax, %rax
	je	.L65
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L65
	movslq	%edx, %rax
	addq	$6, %rax
	movq	(%rdi,%rax,8), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L87
	jmp	.L88
.L68:
	movzbl	80(%rax), %edx
	movl	$0, %eax
	testb	%dl, %dl
	jne	.L58
	movq	40(%rbx), %rdi
	movq	%rbp, %rsi
	call	Quadtree_search_helper
	jmp	.L58
.L88:
	movslq	%edx, %rdx
	addq	$6, %rdx
	movq	(%rdi,%rdx,8), %rdi
	call	Quadtree_search_helper
	jmp	.L58
.L87:
	movslq	%edx, %rdx
	addq	$6, %rdx
	movq	(%rdi,%rdx,8), %rax
	movq	8(%rax), %rdi
	call	Point_equals
	movl	%eax, %edx
	movl	$1, %eax
	testb	%dl, %dl
	jne	.L58
	movq	40(%rbx), %rdx
	movl	$0, %eax
	testq	%rdx, %rdx
	je	.L58
	movzbl	80(%rdx), %edx
	testb	%dl, %dl
	jne	.L58
	movq	40(%rbx), %rdi
	movq	%rbp, %rsi
	call	Quadtree_search_helper
.L58:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L65:
	.cfi_restore_state
	movq	40(%rbx), %rax
	testq	%rax, %rax
	jne	.L68
	jmp	.L58
	.cfi_endproc
.LFE69:
	.size	Quadtree_search_helper, .-Quadtree_search_helper
	.globl	Quadtree_search
	.type	Quadtree_search, @function
Quadtree_search:
.LFB70:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%xmm0, (%rsp)
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L90
	movzbl	80(%rdi), %edx
	testb	%dl, %dl
	jne	.L90
.L95:
	movq	32(%rdi), %rax
	testq	%rax, %rax
	je	.L91
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L91
	movq	32(%rdi), %rdi
	jmp	.L95
.L90:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L91:
	.cfi_restore_state
	movq	%rsp, %rsi
	call	Quadtree_search_helper
	.p2align 4,,2
	jmp	.L90
	.cfi_endproc
.LFE70:
	.size	Quadtree_search, .-Quadtree_search
	.globl	Quadtree_add_find_root
	.type	Quadtree_add_find_root, @function
Quadtree_add_find_root:
.LFB71:
	.cfi_startproc
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rsi), %xmm4
	cvtps2pd	%xmm4, %xmm4
	movl	$0, %r8d
	movsd	.LC8(%rip), %xmm2
	movsd	.LC0(%rip), %xmm5
.L104:
	movq	%rdi, %rax
	jmp	.L103
.L112:
	movq	%rcx, %rax
.L103:
	movq	8(%rax), %rsi
	movss	(%rsi), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%dil
	movss	4(%rsi), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm4
	sbbl	%ecx, %ecx
	notl	%ecx
	andl	$2, %ecx
	orl	%edi, %ecx
	movsbq	%cl, %rcx
	addq	$6, %rcx
	movq	(%rax,%rcx,8), %rcx
	testq	%rcx, %rcx
	je	.L106
	movzbl	80(%rcx), %esi
	testb	%sil, %sil
	jne	.L107
	movzbl	(%rcx), %esi
	testb	%sil, %sil
	je	.L108
	movsd	16(%rcx), %xmm0
	mulsd	%xmm5, %xmm0
	movq	8(%rcx), %rsi
	movss	(%rsi), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm1
	jb	.L118
	movq	8(%rcx), %rsi
	movss	(%rsi), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jbe	.L119
	movq	8(%rcx), %rsi
	movss	4(%rsi), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm4
	jb	.L120
	movq	8(%rcx), %rsi
	movss	4(%rsi), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm3, %xmm0
	ucomisd	%xmm4, %xmm0
	ja	.L112
	movq	%rax, %rcx
	jmp	.L99
.L106:
	movq	%rax, %rcx
	jmp	.L99
.L107:
	movq	%rax, %rcx
	.p2align 4,,2
	jmp	.L99
.L108:
	movq	%rax, %rcx
	.p2align 4,,2
	jmp	.L99
.L118:
	movq	%rax, %rcx
	.p2align 4,,2
	jmp	.L99
.L119:
	movq	%rax, %rcx
	.p2align 4,,2
	jmp	.L99
.L120:
	movq	%rax, %rcx
.L99:
	movq	40(%rcx), %rdi
	addq	$1, %r8
	cmpq	%r8, %rdx
	jae	.L104
	rep
	ret
	.cfi_endproc
.LFE71:
	.size	Quadtree_add_find_root, .-Quadtree_add_find_root
	.globl	Quadtree_add_lock
	.type	Quadtree_add_lock, @function
Quadtree_add_lock:
.LFB72:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	%rsi, %r15
	movq	%rdx, %rbp
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rsi), %xmm4
	cvtps2pd	%xmm4, %xmm4
	movq	%rdi, %r12
	movsd	.LC8(%rip), %xmm2
	movsd	.LC0(%rip), %xmm5
	jmp	.L128
.L184:
	movq	%rbx, %r12
.L128:
	movq	8(%r12), %rdx
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%cl
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm4
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%ecx, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	(%r12,%rax,8), %rbx
	testq	%rbx, %rbx
	je	.L124
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L124
	movzbl	(%rbx), %eax
	testb	%al, %al
	je	.L124
	movsd	16(%rbx), %xmm0
	mulsd	%xmm5, %xmm0
	movq	8(%rbx), %rax
	movss	(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm1
	jb	.L124
	movq	8(%rbx), %rax
	movss	(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jbe	.L124
	movq	8(%rbx), %rax
	movss	4(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm4
	jb	.L124
	movq	8(%rbx), %rax
	movss	4(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm3, %xmm0
	ucomisd	%xmm4, %xmm0
	ja	.L184
.L124:
	movl	$1, %r13d
	testq	%r12, %r12
	je	.L129
	movzbl	80(%r12), %eax
	testb	%al, %al
	jne	.L129
	testq	%rbp, %rbp
	je	.L130
	movq	0(%rbp), %rax
	movl	$0, %r13d
	cmpq	%rax, %r12
	je	.L129
	movq	%rbp, %rax
	jmp	.L131
.L132:
	movq	(%rax), %rdx
	cmpq	%rdx, %r12
	je	.L188
.L131:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L132
	jmp	.L130
.L188:
	movl	$0, %r13d
.L129:
	movq	24(%rbp), %rax
	testq	%rax, %rax
	jne	.L133
	movq	%r12, 0(%rbp)
	movq	$0, 16(%rbp)
	movq	%rbp, 24(%rbp)
	jmp	.L134
.L133:
	movq	0(%rbp), %r14
	movl	$32, %edi
	call	malloc
	movq	%r14, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%rbp), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%rbp)
	movq	%r12, 0(%rbp)
.L134:
	movb	%r13b, 8(%rbp)
	movl	$1, %r13d
	testq	%rbx, %rbx
	je	.L135
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L135
	testq	%rbp, %rbp
	je	.L136
	movq	0(%rbp), %rax
	movl	$0, %r13d
	cmpq	%rax, %rbx
	je	.L135
	movq	%rbp, %rax
	jmp	.L137
.L138:
	movq	(%rax), %rdx
	cmpq	%rdx, %rbx
	je	.L192
.L137:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L138
	jmp	.L136
.L192:
	movl	$0, %r13d
.L135:
	movq	24(%rbp), %rax
	testq	%rax, %rax
	jne	.L139
	movq	%rbx, 0(%rbp)
	movq	$0, 16(%rbp)
	movq	%rbp, 24(%rbp)
	jmp	.L140
.L139:
	movq	0(%rbp), %r14
	movl	$32, %edi
	call	malloc
	movq	%r14, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%rbp), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%rbp)
	movq	%rbx, 0(%rbp)
.L140:
	movb	%r13b, 8(%rbp)
	movl	$0, %eax
	testq	%r12, %r12
	je	.L141
	movzbl	80(%r12), %edx
	testb	%dl, %dl
	jne	.L141
	testq	%rbx, %rbx
	je	.L142
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L142
	movq	8(%rbx), %rcx
	movq	8(%r12), %rdx
	movss	(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movsd	.LC8(%rip), %xmm0
	subsd	%xmm0, %xmm1
	ucomisd	%xmm1, %xmm2
	setae	%sil
	movss	4(%rcx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	ucomisd	%xmm2, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%esi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	(%r12,%rax,8), %rax
	cmpq	%rbx, %rax
	jne	.L145
	movq	24(%rax), %rax
	cmpq	%rax, %r12
	je	.L142
.L145:
	movsd	16(%rbx), %xmm0
	mulsd	.LC0(%rip), %xmm0
	movq	8(%rbx), %rax
	movss	(%r15), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm2, %xmm1
	jb	.L141
	movq	8(%rbx), %rax
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm1, %xmm2
	jbe	.L141
	movq	8(%rbx), %rax
	movss	4(%r15), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm2, %xmm1
	jb	.L141
	movq	8(%rbx), %rax
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm0
	movl	$0, %eax
	ucomisd	%xmm1, %xmm0
	jbe	.L141
.L142:
	movq	40(%r12), %rax
	testq	%rax, %rax
	je	.L150
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L150
	testq	%rbx, %rbx
	je	.L150
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L150
	movq	40(%r12), %r14
	movss	(%r15), %xmm4
	cvtps2pd	%xmm4, %xmm4
	movss	4(%r15), %xmm5
	cvtps2pd	%xmm5, %xmm5
	movsd	.LC8(%rip), %xmm2
	movsd	.LC0(%rip), %xmm6
	movapd	%xmm6, %xmm7
	jmp	.L161
.L201:
	movq	%r13, %r14
.L161:
	movq	8(%r14), %rdx
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm4
	setae	%cl
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm5
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%ecx, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	(%r14,%rax,8), %r13
	testq	%r13, %r13
	je	.L153
	movzbl	80(%r13), %eax
	testb	%al, %al
	jne	.L153
	movzbl	0(%r13), %eax
	testb	%al, %al
	je	.L153
	movq	8(%rbx), %rax
	movsd	16(%r13), %xmm0
	mulsd	%xmm6, %xmm0
	movq	8(%r13), %rdx
	movss	(%rax), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rdx), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm1
	jb	.L153
	movq	8(%r13), %rdx
	movss	(%rdx), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jbe	.L153
	movq	8(%r13), %rdx
	movss	4(%rax), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rdx), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm1
	jb	.L153
	movq	8(%r13), %rax
	movss	4(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm3, %xmm0
	ucomisd	%xmm1, %xmm0
	jbe	.L153
	movsd	16(%r13), %xmm1
	mulsd	%xmm7, %xmm1
	movq	8(%r13), %rax
	movss	(%rax), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm4
	jb	.L153
	movq	8(%r13), %rax
	movss	(%rax), %xmm0
	cvtps2pd	%xmm0, %xmm0
	addsd	%xmm1, %xmm0
	ucomisd	%xmm4, %xmm0
	jbe	.L153
	movq	8(%r13), %rax
	movss	4(%rax), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm5
	jb	.L153
	movq	8(%r13), %rax
	movss	4(%rax), %xmm0
	cvtps2pd	%xmm0, %xmm0
	addsd	%xmm0, %xmm1
	ucomisd	%xmm5, %xmm1
	ja	.L201
.L153:
	movb	$1, 7(%rsp)
	testq	%r14, %r14
	je	.L162
	movzbl	80(%r14), %eax
	testb	%al, %al
	jne	.L162
	testq	%rbp, %rbp
	je	.L163
	movq	0(%rbp), %rax
	movb	$0, 7(%rsp)
	cmpq	%rax, %r14
	je	.L162
	movq	%rbp, %rax
	jmp	.L164
.L165:
	movq	(%rax), %rdx
	cmpq	%rdx, %r14
	je	.L205
.L164:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L165
	jmp	.L163
.L205:
	movb	$0, 7(%rsp)
.L162:
	movq	24(%rbp), %rax
	testq	%rax, %rax
	jne	.L166
	movq	%r14, 0(%rbp)
	movq	$0, 16(%rbp)
	movq	%rbp, 24(%rbp)
	jmp	.L167
.L166:
	movq	0(%rbp), %rax
	movq	%rax, 8(%rsp)
	movl	$32, %edi
	call	malloc
	movq	8(%rsp), %rdx
	movq	%rdx, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%rbp), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%rbp)
	movq	%r14, 0(%rbp)
.L167:
	movzbl	7(%rsp), %eax
	movb	%al, 8(%rbp)
	movsd	16(%r14), %xmm0
	movsd	16(%rbx), %xmm1
	subsd	%xmm1, %xmm0
	movl	$0, %eax
	movsd	.LC9(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L141
	movq	8(%rbx), %rax
	movsd	16(%r14), %xmm0
	mulsd	.LC0(%rip), %xmm0
	movq	8(%r14), %rdx
	movss	(%rax), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	ucomisd	%xmm2, %xmm1
	jb	.L250
	movq	8(%r14), %rdx
	movss	(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm0, %xmm2
	ucomisd	%xmm1, %xmm2
	jbe	.L251
	movq	8(%r14), %rdx
	movss	4(%rax), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm2, %xmm1
	jb	.L141
	movq	8(%r14), %rax
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm0
	movl	$0, %eax
	ucomisd	%xmm1, %xmm0
	jbe	.L141
	movsd	16(%r14), %xmm0
	mulsd	.LC0(%rip), %xmm0
	movq	8(%r14), %rax
	movss	(%r15), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm2, %xmm1
	jb	.L141
	movq	8(%r14), %rax
	movss	(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm1, %xmm2
	jbe	.L141
	movq	8(%r14), %rax
	movss	4(%r15), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm0, %xmm2
	movl	$0, %eax
	ucomisd	%xmm2, %xmm1
	jb	.L141
	movq	8(%r14), %rax
	movss	4(%rax), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm0
	movl	$0, %eax
	ucomisd	%xmm1, %xmm0
	jbe	.L141
	movq	24(%r14), %rax
	testq	%rax, %rax
	je	.L176
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L176
	testq	%r13, %r13
	je	.L176
	movzbl	80(%r13), %eax
	testb	%al, %al
	jne	.L176
	movzbl	0(%r13), %eax
	testb	%al, %al
	je	.L176
	movq	8(%rbx), %rsi
	movq	%r13, %rdi
	call	in_range
	testb	%al, %al
	je	.L176
	movq	%r15, %rsi
	movq	%r13, %rdi
	call	in_range
	testb	%al, %al
	je	.L176
	movq	8(%rbx), %rdx
	movq	8(%r14), %rax
	cvtss2sd	(%rdx), %xmm2
	cvtss2sd	(%rax), %xmm1
	movsd	.LC8(%rip), %xmm0
	subsd	%xmm0, %xmm1
	ucomisd	%xmm1, %xmm2
	setae	%cl
	cvtss2sd	4(%rdx), %xmm1
	cvtss2sd	4(%rax), %xmm2
	subsd	%xmm0, %xmm2
	ucomisd	%xmm2, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%eax, %ecx
	movq	8(%r14), %rdx
	cvtss2sd	(%r15), %xmm2
	cvtss2sd	(%rdx), %xmm1
	movsd	.LC8(%rip), %xmm0
	subsd	%xmm0, %xmm1
	ucomisd	%xmm1, %xmm2
	setae	%sil
	cvtss2sd	4(%r15), %xmm1
	cvtss2sd	4(%rdx), %xmm2
	subsd	%xmm0, %xmm2
	ucomisd	%xmm2, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	movl	%eax, %edx
	orl	%esi, %edx
	movl	$0, %eax
	cmpb	%dl, %cl
	jne	.L176
	jmp	.L141
.L150:
	movq	24(%rbp), %rax
	testq	%rax, %rax
	jne	.L181
	movq	$0, 0(%rbp)
	movq	$0, 16(%rbp)
	movq	%rbp, 24(%rbp)
	jmp	.L182
.L181:
	movq	0(%rbp), %rbx
	movl	$32, %edi
	call	malloc
	movq	%rbx, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%rbp), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%rbp)
	movq	$0, 0(%rbp)
.L182:
	movb	$1, 8(%rbp)
.L176:
	movq	40(%r12), %rdx
	movl	$1, %eax
	testq	%rdx, %rdx
	je	.L141
	movzbl	80(%rdx), %edx
	testb	%dl, %dl
	jne	.L141
	movq	40(%r12), %rdi
	movq	%rbp, %rdx
	movq	%r15, %rsi
	call	Quadtree_add_lock
	jmp	.L141
.L250:
	movl	$0, %eax
	jmp	.L141
.L251:
	movl	$0, %eax
.L141:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L130:
	.cfi_restore_state
	leaq	88(%r12), %rdi
	call	pthread_mutex_lock
	movl	$1, %r13d
	jmp	.L129
.L136:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_lock
	movl	$1, %r13d
	jmp	.L135
.L163:
	leaq	88(%r14), %rdi
	call	pthread_mutex_lock
	movb	$1, 7(%rsp)
	jmp	.L162
	.cfi_endproc
.LFE72:
	.size	Quadtree_add_lock, .-Quadtree_add_lock
	.globl	Quadtree_add_validate
	.type	Quadtree_add_validate, @function
Quadtree_add_validate:
.LFB73:
	.cfi_startproc
	testq	%rdi, %rdi
	je	.L302
	movq	(%rdi), %rax
	movq	16(%rdi), %rdx
	testq	%rdx, %rdx
	je	.L303
	movq	16(%rdi), %rdx
	movq	(%rdx), %rcx
	movq	16(%rdi), %rdx
	movq	16(%rdx), %rdx
	testq	%rdx, %rdx
	jne	.L357
	jmp	.L371
.L300:
	movq	(%rdi), %rax
	movq	16(%rdi), %rdx
	testq	%rdx, %rdx
	je	.L305
	movq	16(%rdi), %rdx
	movq	(%rdx), %rcx
	movq	16(%rdi), %rdx
	movq	16(%rdx), %rdx
	testq	%rdx, %rdx
	jne	.L358
	jmp	.L372
.L357:
	movsd	.LC8(%rip), %xmm5
	movsd	.LC0(%rip), %xmm4
	movapd	%xmm4, %xmm10
	movapd	%xmm5, %xmm7
	movapd	%xmm5, %xmm6
	movapd	%xmm4, %xmm8
	movapd	%xmm4, %xmm11
	movapd	%xmm4, %xmm9
.L358:
	movq	16(%rdi), %rdx
	movq	16(%rdx), %rdx
	movq	(%rdx), %rdx
	movq	16(%rdi), %rdi
	movq	16(%rdi), %rdi
	movq	16(%rdi), %rdi
	testq	%rdx, %rdx
	je	.L307
	movzbl	80(%rdx), %r8d
	testb	%r8b, %r8b
	jne	.L308
	movq	8(%rdx), %r9
	movss	(%rsi), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movss	(%r9), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm5, %xmm1
	ucomisd	%xmm1, %xmm0
	setae	%r10b
	movss	4(%rsi), %xmm3
	cvtps2pd	%xmm3, %xmm3
	movss	4(%r9), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm5, %xmm1
	ucomisd	%xmm1, %xmm3
	sbbl	%r8d, %r8d
	notl	%r8d
	andl	$2, %r8d
	orl	%r10d, %r8d
	movsbq	%r8b, %r8
	addq	$6, %r8
	movq	(%rdx,%r8,8), %r8
	cmpq	%rcx, %r8
	jne	.L310
	testq	%r8, %r8
	je	.L263
	movzbl	80(%r8), %r9d
	testb	%r9b, %r9b
	jne	.L264
	movq	24(%r8), %r8
	cmpq	%rdx, %r8
	je	.L264
	jmp	.L373
.L263:
	testq	%rcx, %rcx
	je	.L265
.L264:
	movzbl	80(%rcx), %r8d
	testb	%r8b, %r8b
	je	.L266
	jmp	.L265
.L301:
	movzbl	80(%rax), %r8d
	testb	%r8b, %r8b
	je	.L312
.L266:
	movq	40(%rdx), %r8
	testq	%r8, %r8
	je	.L267
	movzbl	80(%r8), %r8d
	testb	%r8b, %r8b
	jne	.L267
	testq	%rcx, %rcx
	je	.L267
	movzbl	80(%rcx), %r8d
	testb	%r8b, %r8b
	jne	.L267
	testq	%rax, %rax
	je	.L313
	movzbl	80(%rax), %r8d
	testb	%r8b, %r8b
	jne	.L314
.L267:
	movsd	16(%rdx), %xmm1
	mulsd	%xmm4, %xmm1
	movq	8(%rdx), %r8
	movss	(%r8), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm1, %xmm2
	ucomisd	%xmm2, %xmm0
	jb	.L359
	movq	8(%rdx), %r8
	movss	(%r8), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm1, %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L360
	movq	8(%rdx), %r8
	movss	4(%r8), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm1, %xmm2
	ucomisd	%xmm2, %xmm3
	jb	.L361
	movq	8(%rdx), %r8
	movss	4(%r8), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm1
	ucomisd	%xmm3, %xmm1
	jbe	.L362
	testq	%rcx, %rcx
	je	.L272
	movzbl	80(%rcx), %r8d
	testb	%r8b, %r8b
	jne	.L273
	movq	8(%rcx), %r10
	movq	8(%rdx), %r9
	movss	(%r10), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%r9), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm7, %xmm1
	ucomisd	%xmm1, %xmm2
	setae	%r11b
	movss	4(%r10), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%r9), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm7, %xmm1
	ucomisd	%xmm1, %xmm2
	sbbl	%r8d, %r8d
	notl	%r8d
	andl	$2, %r8d
	orl	%r11d, %r8d
	movq	8(%rdx), %r9
	movss	(%r9), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm6, %xmm1
	ucomisd	%xmm1, %xmm0
	setae	%r10b
	movss	4(%r9), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm6, %xmm1
	ucomisd	%xmm1, %xmm3
	sbbl	%edx, %edx
	notl	%edx
	andl	$2, %edx
	orl	%r10d, %edx
	cmpb	%dl, %r8b
	jne	.L321
.L273:
	movzbl	80(%rcx), %edx
	testb	%dl, %dl
	jne	.L272
	movzbl	(%rcx), %edx
	testb	%dl, %dl
	je	.L272
	movsd	16(%rcx), %xmm1
	mulsd	%xmm10, %xmm1
	movq	8(%rcx), %rdx
	movss	(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm1, %xmm2
	ucomisd	%xmm2, %xmm0
	jb	.L272
	movq	8(%rcx), %rdx
	movss	(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm1, %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L272
	movq	8(%rcx), %rdx
	movss	4(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm1, %xmm2
	ucomisd	%xmm2, %xmm3
	jb	.L272
	movq	8(%rcx), %rdx
	movss	4(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm1
	ucomisd	%xmm3, %xmm1
	ja	.L322
.L272:
	testq	%rax, %rax
	je	.L281
	movzbl	80(%rax), %edx
	testb	%dl, %dl
	jne	.L282
	movq	8(%rcx), %rdx
	movsd	16(%rax), %xmm1
	mulsd	%xmm9, %xmm1
	movq	8(%rax), %r8
	movss	(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%r8), %xmm12
	cvtps2pd	%xmm12, %xmm12
	subsd	%xmm1, %xmm12
	ucomisd	%xmm12, %xmm2
	jb	.L363
	movq	8(%rax), %r8
	movss	(%r8), %xmm12
	cvtps2pd	%xmm12, %xmm12
	addsd	%xmm1, %xmm12
	ucomisd	%xmm2, %xmm12
	jbe	.L364
	movq	8(%rax), %r8
	movss	4(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%r8), %xmm12
	cvtps2pd	%xmm12, %xmm12
	subsd	%xmm1, %xmm12
	ucomisd	%xmm12, %xmm2
	jb	.L365
	movq	8(%rax), %rdx
	movss	4(%rdx), %xmm12
	cvtps2pd	%xmm12, %xmm12
	addsd	%xmm12, %xmm1
	ucomisd	%xmm2, %xmm1
	jbe	.L366
	movsd	16(%rax), %xmm2
	mulsd	.LC0(%rip), %xmm2
	movq	8(%rax), %rdx
	movss	(%rdx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm2, %xmm1
	ucomisd	%xmm1, %xmm0
	jb	.L367
	movq	8(%rax), %rdx
	movss	(%rdx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	addsd	%xmm2, %xmm1
	ucomisd	%xmm0, %xmm1
	jbe	.L368
	movq	8(%rax), %rdx
	movss	4(%rdx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	%xmm2, %xmm1
	ucomisd	%xmm1, %xmm3
	jb	.L369
	movq	8(%rax), %rdx
	movss	4(%rdx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	addsd	%xmm1, %xmm2
	ucomisd	%xmm3, %xmm2
	jbe	.L370
.L282:
	movzbl	80(%rax), %edx
	testb	%dl, %dl
	jne	.L281
	movl	$0, %edx
.L299:
	movslq	%edx, %r8
	addq	$6, %r8
	movq	(%rax,%r8,8), %r8
	testq	%r8, %r8
	je	.L291
	movzbl	80(%r8), %r8d
	testb	%r8b, %r8b
	jne	.L291
	movq	8(%rcx), %r9
	movslq	%edx, %r8
	addq	$6, %r8
	movq	(%rax,%r8,8), %r8
	movsd	16(%r8), %xmm1
	mulsd	%xmm8, %xmm1
	movq	8(%r8), %r10
	movss	(%r9), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%r10), %xmm12
	cvtps2pd	%xmm12, %xmm12
	subsd	%xmm1, %xmm12
	ucomisd	%xmm12, %xmm2
	jb	.L291
	movq	8(%r8), %r10
	movss	(%r10), %xmm12
	cvtps2pd	%xmm12, %xmm12
	addsd	%xmm1, %xmm12
	ucomisd	%xmm2, %xmm12
	jbe	.L291
	movq	8(%r8), %r10
	movss	4(%r9), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%r10), %xmm12
	cvtps2pd	%xmm12, %xmm12
	subsd	%xmm1, %xmm12
	ucomisd	%xmm12, %xmm2
	jb	.L291
	movq	8(%r8), %r8
	movss	4(%r8), %xmm12
	cvtps2pd	%xmm12, %xmm12
	addsd	%xmm12, %xmm1
	ucomisd	%xmm2, %xmm1
	jbe	.L291
	movslq	%edx, %r8
	addq	$6, %r8
	movq	(%rax,%r8,8), %r8
	movsd	16(%r8), %xmm1
	mulsd	%xmm11, %xmm1
	movq	8(%r8), %r9
	movss	(%r9), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm1, %xmm2
	ucomisd	%xmm2, %xmm0
	jb	.L291
	movq	8(%r8), %r9
	movss	(%r9), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm1, %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L291
	movq	8(%r8), %r9
	movss	4(%r9), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	%xmm1, %xmm2
	ucomisd	%xmm2, %xmm3
	jb	.L291
	movq	8(%r8), %r8
	movss	4(%r8), %xmm2
	cvtps2pd	%xmm2, %xmm2
	addsd	%xmm2, %xmm1
	ucomisd	%xmm3, %xmm1
	ja	.L331
.L291:
	addl	$1, %edx
	cmpl	$4, %edx
	jne	.L299
.L281:
	testq	%rdi, %rdi
	jne	.L300
	movl	$1, %eax
	ret
.L302:
	movl	$1, %eax
	ret
.L303:
	movl	$0, %eax
	ret
.L371:
	movl	$0, %eax
	ret
.L305:
	movl	$0, %eax
	ret
.L372:
	movl	$0, %eax
	ret
.L307:
	movl	$0, %eax
	ret
.L308:
	movl	$0, %eax
	ret
.L310:
	movl	$0, %eax
	ret
.L373:
	movl	$0, %eax
	ret
.L312:
	movl	$0, %eax
	ret
.L313:
	movl	$0, %eax
	ret
.L314:
	movl	$0, %eax
	ret
.L359:
	movl	$0, %eax
	ret
.L360:
	movl	$0, %eax
	ret
.L361:
	movl	$0, %eax
	ret
.L362:
	movl	$0, %eax
	ret
.L321:
	movl	$0, %eax
	ret
.L322:
	movl	$0, %eax
	ret
.L363:
	movl	$0, %eax
	ret
.L364:
	movl	$0, %eax
	ret
.L365:
	movl	$0, %eax
	ret
.L366:
	movl	$0, %eax
	ret
.L367:
	movl	$0, %eax
	ret
.L368:
	movl	$0, %eax
	ret
.L369:
	movl	$0, %eax
	ret
.L370:
	movl	$0, %eax
	ret
.L331:
	movl	$0, %eax
	ret
.L265:
	testq	%rax, %rax
	jne	.L301
	.p2align 4,,4
	jmp	.L266
	.cfi_endproc
.LFE73:
	.size	Quadtree_add_validate, .-Quadtree_add_validate
	.globl	Quadtree_add_helper
	.type	Quadtree_add_helper, @function
Quadtree_add_helper:
.LFB74:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	movq	%rdi, %r14
	movq	%rsi, %r12
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L375
	movq	$0, (%rsp)
	movl	$0, %ebx
	jmp	.L405
.L419:
	movq	%rbp, %rbx
.L405:
	movq	16(%r14), %rax
	movq	16(%rax), %rax
	movq	(%rax), %r15
	movq	16(%r14), %rax
	movq	(%rax), %r13
	movq	(%r14), %rax
	movq	%rax, 8(%rsp)
	movq	16(%r14), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %r14
	movq	(%r12), %rax
	movq	%rax, 32(%rsp)
	movl	$136, %edi
	call	malloc
	movq	%rax, %rbp
	movb	$0, (%rax)
	movq	$0, 16(%rax)
	movl	$8, %edi
	call	malloc
	movq	%rax, 8(%rbp)
	movq	$0, 24(%rbp)
	movq	$0, 32(%rbp)
	movq	$0, 40(%rbp)
	movq	$0, 48(%rbp)
	movq	$0, 56(%rbp)
	movq	$0, 64(%rbp)
	movq	$0, 72(%rbp)
	movl	$0, %eax
	call	pthread_mutex_attr
	movq	%rax, 16(%rsp)
	leaq	88(%rbp), %rdi
	movq	%rax, %rsi
	call	pthread_mutex_init
	movb	$0, 80(%rbp)
	movq	8(%rbp), %rsi
	leaq	32(%rsp), %rdi
	call	Point_copy
	movq	%r15, 24(%rbp)
	movq	%rbx, 40(%rbp)
	testq	%r13, %r13
	je	.L376
	movzbl	80(%r13), %eax
	testb	%al, %al
	jne	.L376
	cmpq	$0, 8(%rsp)
	je	.L377
	movq	8(%rsp), %rdx
	movzbl	80(%rdx), %eax
	testb	%al, %al
	jne	.L377
	movq	8(%rdx), %rax
	movsd	16(%rdx), %xmm0
	movsd	%xmm0, 24(%rsp)
	movl	(%rax), %edx
	movl	4(%rax), %eax
	movl	%edx, 32(%rsp)
	movl	%eax, 36(%rsp)
	movl	$136, %edi
	call	malloc
	movq	%rax, %rbx
	movb	$0, (%rax)
	movsd	24(%rsp), %xmm0
	movsd	%xmm0, 16(%rax)
	movl	$8, %edi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	$0, 24(%rbx)
	movq	$0, 32(%rbx)
	movq	$0, 40(%rbx)
	movq	$0, 48(%rbx)
	movq	$0, 56(%rbx)
	movq	$0, 64(%rbx)
	movq	$0, 72(%rbx)
	leaq	88(%rbx), %rdi
	movq	16(%rsp), %rsi
	call	pthread_mutex_init
	movb	$0, 80(%rbx)
	movq	8(%rbx), %rsi
	leaq	32(%rsp), %rdi
	call	Point_copy
	movb	$1, (%rbx)
	jmp	.L381
.L408:
	movq	(%rsp), %rdx
	movzbl	80(%rdx), %eax
	testb	%al, %al
	jne	.L379
	movq	8(%rdx), %rax
	movsd	16(%rdx), %xmm0
	movsd	%xmm0, 24(%rsp)
	movl	(%rax), %edx
	movl	4(%rax), %eax
	movl	%edx, 32(%rsp)
	movl	%eax, 36(%rsp)
	movl	$136, %edi
	call	malloc
	movq	%rax, %rbx
	movb	$0, (%rax)
	movsd	24(%rsp), %xmm0
	movsd	%xmm0, 16(%rax)
	movl	$8, %edi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	$0, 24(%rbx)
	movq	$0, 32(%rbx)
	movq	$0, 40(%rbx)
	movq	$0, 48(%rbx)
	movq	$0, 56(%rbx)
	movq	$0, 64(%rbx)
	movq	$0, 72(%rbx)
	leaq	88(%rbx), %rdi
	movq	16(%rsp), %rsi
	call	pthread_mutex_init
	movb	$0, 80(%rbx)
	movq	8(%rbx), %rsi
	leaq	32(%rsp), %rdi
	call	Point_copy
	movb	$1, (%rbx)
	jmp	.L381
.L409:
	movzbl	80(%rax), %eax
	movl	$0, %ebx
	testb	%al, %al
	je	.L381
	jmp	.L380
.L388:
	movq	8(%rbx), %rdx
	movq	8(%rbx), %rcx
	movss	(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm5, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%sil
	movss	4(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm5, %xmm0
	ucomisd	%xmm0, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%esi, %eax
	movsbl	%al, %eax
	movq	8(%rbx), %rcx
	movsd	16(%rbx), %xmm7
	movq	8(%rbx), %rsi
	movsd	16(%rbx), %xmm2
	movss	4(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movl	%eax, %esi
	andl	$2, %esi
	subl	$1, %esi
	cvtsi2sd	%esi, %xmm0
	mulsd	%xmm6, %xmm0
	mulsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movss	(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	andl	$1, %eax
	cvtsi2sd	%eax, %xmm1
	subsd	%xmm3, %xmm1
	mulsd	%xmm3, %xmm1
	mulsd	%xmm7, %xmm1
	addsd	%xmm2, %xmm1
	unpcklpd	%xmm1, %xmm1
	cvtpd2ps	%xmm1, %xmm1
	movss	%xmm1, (%rdx)
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	movss	%xmm0, 4(%rdx)
	movsd	16(%rbx), %xmm0
	mulsd	%xmm3, %xmm0
	movsd	%xmm0, 16(%rbx)
	jmp	.L432
.L381:
	movsd	.LC8(%rip), %xmm4
	movapd	%xmm4, %xmm5
	movsd	.LC13(%rip), %xmm6
	movsd	.LC0(%rip), %xmm3
.L432:
	movq	8(%r13), %rcx
	movq	8(%rbx), %rdx
	movss	(%rcx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%sil
	movss	4(%rcx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	movl	%eax, %ecx
	orl	%esi, %ecx
	movq	8(%rbx), %rdx
	movss	(%r12), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm4, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%sil
	movss	4(%r12), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm4, %xmm0
	ucomisd	%xmm0, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%esi, %eax
	cmpb	%al, %cl
	je	.L388
	cmpq	$0, (%rsp)
	je	.L389
	movq	(%rsp), %rdx
	movzbl	80(%rdx), %eax
	testb	%al, %al
	jne	.L389
	movsd	16(%rdx), %xmm0
	movsd	16(%rbx), %xmm1
	subsd	%xmm1, %xmm0
	cmpltsd	.LC12(%rip), %xmm0
	movsd	.LC10(%rip), %xmm1
	andpd	%xmm0, %xmm1
	movsd	.LC11(%rip), %xmm2
	andnpd	%xmm2, %xmm0
	orpd	%xmm1, %xmm0
	movsd	16(%rdx), %xmm1
	movsd	16(%rbx), %xmm2
	subsd	%xmm2, %xmm1
	mulsd	%xmm1, %xmm0
	movsd	.LC8(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	jb	.L389
	movq	%rdx, 40(%rbx)
	jmp	.L393
.L389:
	cmpq	$0, 8(%rsp)
	je	.L394
	movq	8(%rsp), %rdx
	movzbl	80(%rdx), %eax
	testb	%al, %al
	jne	.L394
	movsd	16(%rdx), %xmm0
	movsd	16(%rbx), %xmm1
	subsd	%xmm1, %xmm0
	cmpltsd	.LC12(%rip), %xmm0
	movsd	.LC10(%rip), %xmm1
	andpd	%xmm0, %xmm1
	movsd	.LC11(%rip), %xmm2
	andnpd	%xmm2, %xmm0
	orpd	%xmm1, %xmm0
	movsd	16(%rdx), %xmm1
	movsd	16(%rbx), %xmm2
	subsd	%xmm2, %xmm1
	mulsd	%xmm1, %xmm0
	movsd	.LC8(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	jb	.L394
	movq	%rdx, 40(%rbx)
	jmp	.L393
.L394:
	movq	40(%r15), %rax
	testq	%rax, %rax
	je	.L393
	movzbl	80(%rax), %eax
.L393:
	movq	8(%rbp), %rcx
	movq	8(%rbx), %rdx
	movss	(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	.LC8(%rip), %xmm1
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%sil
	movss	4(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%esi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	%rbp, (%rbx,%rax,8)
	movq	8(%r13), %rcx
	movq	8(%rbx), %rdx
	movss	(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%sil
	movss	4(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%esi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	%r13, (%rbx,%rax,8)
	movq	%r15, 24(%rbx)
	movq	%rbx, 24(%rbp)
	movq	%rbx, 24(%r13)
	movq	8(%r15), %rdx
	movss	(%r12), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%cl
	movss	4(%r12), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%ecx, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	%rbx, (%r15,%rax,8)
	movq	%rbx, (%rsp)
.L404:
	testq	%r14, %r14
	jne	.L419
	movq	%rbp, %rax
.L375:
	addq	$56, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L376:
	.cfi_restore_state
	movq	8(%r15), %rdx
	movss	(%r12), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	.LC8(%rip), %xmm1
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%cl
	movss	4(%r12), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%ecx, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	%rbp, (%r15,%rax,8)
	movq	$0, (%rsp)
	jmp	.L404
.L377:
	cmpq	$0, (%rsp)
	jne	.L408
.L379:
	movq	40(%r15), %rax
	testq	%rax, %rax
	jne	.L409
.L380:
	movq	8(%r15), %rax
	movsd	16(%r15), %xmm0
	movsd	%xmm0, 24(%rsp)
	movl	(%rax), %edx
	movl	4(%rax), %eax
	movl	%edx, 32(%rsp)
	movl	%eax, 36(%rsp)
	movl	$136, %edi
	call	malloc
	movq	%rax, %rbx
	movb	$0, (%rax)
	movsd	24(%rsp), %xmm0
	movsd	%xmm0, 16(%rax)
	movl	$8, %edi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	$0, 24(%rbx)
	movq	$0, 32(%rbx)
	movq	$0, 40(%rbx)
	movq	$0, 48(%rbx)
	movq	$0, 56(%rbx)
	movq	$0, 64(%rbx)
	movq	$0, 72(%rbx)
	leaq	88(%rbx), %rdi
	movq	16(%rsp), %rsi
	call	pthread_mutex_init
	movb	$0, 80(%rbx)
	movq	8(%rbx), %rsi
	leaq	32(%rsp), %rdi
	call	Point_copy
	movb	$1, (%rbx)
	jmp	.L381
	.cfi_endproc
.LFE74:
	.size	Quadtree_add_helper, .-Quadtree_add_helper
	.globl	Quadtree_add
	.type	Quadtree_add, @function
Quadtree_add:
.LFB75:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$32, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, %rbx
	movq	%xmm0, (%rsp)
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L434
	movzbl	80(%rdi), %edx
	testb	%dl, %dl
	jne	.L434
	call	clock
	movq	%rax, %rbp
	call	pthread_self
	addl	%eax, %ebp
	movl	%ebp, 28(%rsp)
	leaq	28(%rsp), %rdi
	call	Marsaglia_rands
	movl	%eax, %ecx
	andl	$15, %ecx
	movl	$-1, %edx
	sall	%cl, %edx
	notl	%edx
	andl	%eax, %edx
	sall	%cl, %edx
	shrl	%cl, %eax
	orl	%eax, %edx
	movl	%edx, 28(%rsp)
	movl	$1374389535, %r12d
	jmp	.L459
.L439:
	movq	32(%rbx), %rax
	testq	%rax, %rax
	je	.L436
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L436
	movq	32(%rbx), %rbx
	jmp	.L459
.L452:
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L438
	movq	32(%rbx), %rbp
.L453:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_unlock
	movq	%rbp, %rbx
.L459:
	leaq	28(%rsp), %rdi
	call	Marsaglia_rands
	movl	%eax, %ecx
	mull	%r12d
	shrl	$5, %edx
	imull	$100, %edx, %edx
	subl	%edx, %ecx
	cmpl	$49, %ecx
	jbe	.L439
	movl	$0, %r12d
.L443:
	movq	32(%rbx), %rax
	testq	%rax, %rax
	je	.L440
	movzbl	80(%rax), %eax
	testb	%al, %al
	je	.L441
.L440:
	movl	$10, %r14d
	jmp	.L442
.L441:
	addq	$1, %r12
	movq	32(%rbx), %rbx
	jmp	.L443
.L442:
	movq	%r12, %rdx
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	Quadtree_add_find_root
	movq	%rax, %r13
	movl	$32, %edi
	call	malloc
	movq	$0, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movl	$0, %ebp
	movq	%rbp, %rdx
	movq	%rsp, %rsi
	movq	%r13, %rdi
	call	Quadtree_add_lock
	testb	%al, %al
	je	.L446
	movq	%rsp, %rsi
	movq	%rbp, %rdi
	call	Quadtree_add_validate
	testb	%al, %al
	jne	.L445
.L446:
	subq	$1, %r14
	jne	.L442
	.p2align 4,,5
	jmp	.L460
.L445:
	movq	%rsp, %rsi
	movl	$0, %ebp
	movq	%rbp, %rdi
	call	Quadtree_add_helper
	movq	%rax, %r12
	movl	$0, %ebx
	testq	%rbp, %rbp
	jne	.L451
	jmp	.L449
.L458:
	movq	%rbp, %rbx
.L451:
	movq	16(%rbx), %rbp
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L450
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L450
	movzbl	8(%rbx), %eax
	testb	%al, %al
	je	.L450
	movq	(%rbx), %rdi
	addq	$88, %rdi
	call	pthread_mutex_unlock
.L450:
	movq	%rbx, %rdi
	call	free
	testq	%rbp, %rbp
	jne	.L458
.L449:
	testq	%r12, %r12
	setne	%al
	jmp	.L434
.L460:
	movl	$0, %eax
.L434:
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L436:
	.cfi_restore_state
	leaq	88(%rbx), %rdi
	call	pthread_mutex_lock
	movq	32(%rbx), %rax
	testq	%rax, %rax
	jne	.L452
.L438:
	movq	8(%rbx), %rax
	movq	16(%rbx), %r13
	movl	(%rax), %edx
	movl	4(%rax), %eax
	movl	%edx, 16(%rsp)
	movl	%eax, 20(%rsp)
	movl	$136, %edi
	call	malloc
	movq	%rax, %rbp
	movb	$0, (%rax)
	movq	%r13, 16(%rax)
	movl	$8, %edi
	call	malloc
	movq	%rax, 8(%rbp)
	movq	$0, 24(%rbp)
	movq	$0, 32(%rbp)
	movq	$0, 40(%rbp)
	movq	$0, 48(%rbp)
	movq	$0, 56(%rbp)
	movq	$0, 64(%rbp)
	movq	$0, 72(%rbp)
	movl	$0, %eax
	call	pthread_mutex_attr
	leaq	88(%rbp), %rdi
	movq	%rax, %rsi
	call	pthread_mutex_init
	movb	$0, 80(%rbp)
	movq	8(%rbp), %rsi
	leaq	16(%rsp), %rdi
	call	Point_copy
	movb	$1, 0(%rbp)
	movq	%rbx, 40(%rbp)
	movq	%rbp, 32(%rbx)
	jmp	.L453
	.cfi_endproc
.LFE75:
	.size	Quadtree_add, .-Quadtree_add
	.globl	Quadtree_remove_node
	.type	Quadtree_remove_node, @function
Quadtree_remove_node:
.LFB76:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbx
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L462
	movzbl	80(%rdi), %edx
	testb	%dl, %dl
	jne	.L462
	movq	40(%rdi), %rax
	testq	%rax, %rax
	je	.L463
	movzbl	80(%rax), %eax
	testb	%al, %al
	je	.L464
	jmp	.L463
.L477:
	movzbl	80(%rax), %edx
	movl	$0, %eax
	testb	%dl, %dl
	jne	.L462
.L464:
	movq	24(%rbx), %rax
	testq	%rax, %rax
	je	.L465
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L465
	movq	24(%rbx), %rdi
	addq	$88, %rdi
	call	pthread_mutex_lock
.L465:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_lock
	movzbl	(%rbx), %eax
	testb	%al, %al
	je	.L466
	movl	$0, %eax
	movl	$0, %ebp
	movl	$0, %esi
.L468:
	movslq	%eax, %rdx
	addq	$6, %rdx
	movq	(%rbx,%rdx,8), %rdx
	testq	%rdx, %rdx
	je	.L467
	movzbl	80(%rdx), %edx
	testb	%dl, %dl
	jne	.L467
	addl	$1, %esi
	movslq	%eax, %rcx
	addq	$6, %rcx
	movq	(%rbx,%rcx,8), %rbp
.L467:
	addl	$1, %eax
	cmpl	$4, %eax
	jne	.L468
	cmpb	$1, %sil
	jbe	.L469
	movq	24(%rbx), %rax
	testq	%rax, %rax
	je	.L470
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L470
	movq	24(%rbx), %rdi
	addq	$88, %rdi
	call	pthread_mutex_unlock
.L470:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_unlock
	movl	$0, %eax
	jmp	.L462
.L469:
	cmpb	$1, %sil
	jne	.L466
	movq	24(%rbx), %rax
	testq	%rax, %rax
	je	.L471
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L471
	leaq	88(%rbp), %rdi
	call	pthread_mutex_lock
	movq	24(%rbx), %rax
	movq	%rax, 24(%rbp)
	movq	24(%rbx), %rsi
	movq	8(%rbx), %rcx
	movq	24(%rbx), %rax
	movq	8(%rax), %rdx
	movss	(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	.LC8(%rip), %xmm1
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%dil
	movss	4(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%edi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	%rbp, (%rsi,%rax,8)
	leaq	88(%rbp), %rdi
	call	pthread_mutex_unlock
.L466:
	movb	$1, 80(%rbx)
	leaq	88(%rbx), %rdi
	call	pthread_mutex_unlock
	movq	24(%rbx), %rax
	testq	%rax, %rax
	je	.L474
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L474
	movl	$0, %eax
	movl	$0, %ebp
.L476:
	movq	24(%rbx), %rdx
	movslq	%eax, %rcx
	addq	$6, %rcx
	movq	(%rdx,%rcx,8), %rcx
	movl	$0, %edx
	testq	%rcx, %rcx
	je	.L475
	movzbl	80(%rcx), %edx
	xorl	$1, %edx
	movzbl	%dl, %edx
.L475:
	addl	%edx, %ebp
	addl	$1, %eax
	cmpl	$4, %eax
	jne	.L476
	movq	24(%rbx), %rdi
	addq	$88, %rdi
	call	pthread_mutex_unlock
	cmpb	$1, %bpl
	ja	.L474
	movq	24(%rbx), %rdi
	call	Quadtree_remove_node
.L474:
	movq	40(%rbx), %rdx
	movl	$1, %eax
	testq	%rdx, %rdx
	je	.L462
	movzbl	80(%rdx), %edx
	testb	%dl, %dl
	jne	.L462
	movq	40(%rbx), %rdi
	call	Quadtree_remove_node
	movl	$1, %eax
.L462:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L463:
	.cfi_restore_state
	movq	24(%rbx), %rax
	testq	%rax, %rax
	jne	.L477
	jmp	.L462
.L471:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_unlock
	movl	$0, %eax
	jmp	.L462
	.cfi_endproc
.LFE76:
	.size	Quadtree_remove_node, .-Quadtree_remove_node
	.globl	Quadtree_remove_lock
	.type	Quadtree_remove_lock, @function
Quadtree_remove_lock:
.LFB77:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, %rbx
	movq	%rsi, %r13
	movq	%rdx, %r12
	jmp	.L555
.L556:
	movzbl	80(%rbp), %eax
	testb	%al, %al
	jne	.L490
	movzbl	0(%rbp), %eax
	testb	%al, %al
	je	.L491
	movsd	16(%rbp), %xmm2
	mulsd	.LC0(%rip), %xmm2
	movq	8(%rbp), %rax
	movss	(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	subsd	%xmm2, %xmm3
	ucomisd	%xmm3, %xmm0
	jb	.L492
	movq	8(%rbp), %rax
	movss	(%rax), %xmm3
	cvtps2pd	%xmm3, %xmm3
	addsd	%xmm2, %xmm3
	ucomisd	%xmm0, %xmm3
	jbe	.L492
	movq	8(%rbp), %rax
	movss	4(%rax), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm2, %xmm0
	ucomisd	%xmm0, %xmm1
	jb	.L492
	movq	8(%rbp), %rax
	movss	4(%rax), %xmm0
	cvtps2pd	%xmm0, %xmm0
	addsd	%xmm0, %xmm2
	ucomisd	%xmm1, %xmm2
	jbe	.L492
	jmp	.L557
.L491:
	movq	8(%rbp), %rdi
	movq	%r13, %rsi
	call	Point_equals
	testb	%al, %al
	je	.L497
	movq	24(%rbx), %r14
	movl	$1, %r15d
	testq	%r14, %r14
	je	.L498
	movzbl	80(%r14), %eax
	testb	%al, %al
	jne	.L498
	testq	%r12, %r12
	je	.L499
	movq	(%r12), %rax
	movl	$0, %r15d
	cmpq	%rax, %r14
	je	.L498
	movq	%r12, %rax
	jmp	.L500
.L501:
	movq	(%rax), %rdx
	cmpq	%rdx, %r14
	je	.L533
.L500:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L501
	jmp	.L499
.L533:
	movl	$0, %r15d
.L498:
	movq	24(%r12), %rax
	testq	%rax, %rax
	jne	.L502
	movq	%r14, (%r12)
	movq	$0, 16(%r12)
	movq	%r12, 24(%r12)
	jmp	.L503
.L502:
	movq	(%r12), %rax
	movq	%rax, 8(%rsp)
	movl	$32, %edi
	call	malloc
	movq	8(%rsp), %rdx
	movq	%rdx, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%r12), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%r12), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%r12), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%r12)
	movq	%r14, (%r12)
.L503:
	movb	%r15b, 8(%r12)
	movzbl	80(%rbx), %eax
	movl	$1, %r15d
	testb	%al, %al
	jne	.L504
	testq	%r12, %r12
	je	.L505
	movq	(%r12), %rax
	movl	$0, %r15d
	cmpq	%rax, %rbx
	je	.L504
	movq	%r12, %rax
	jmp	.L506
.L507:
	movq	(%rax), %rdx
	cmpq	%rdx, %rbx
	je	.L536
.L506:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L507
	jmp	.L505
.L536:
	movl	$0, %r15d
.L504:
	movq	24(%r12), %rax
	testq	%rax, %rax
	jne	.L508
	movq	%rbx, (%r12)
	movq	$0, 16(%r12)
	movq	%r12, 24(%r12)
	jmp	.L509
.L508:
	movq	(%r12), %r14
	movl	$32, %edi
	call	malloc
	movq	%r14, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%r12), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%r12), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%r12), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%r12)
	movq	%rbx, (%r12)
.L509:
	movb	%r15b, 8(%r12)
	movzbl	80(%rbp), %eax
	movl	$1, %r14d
	testb	%al, %al
	jne	.L510
	testq	%r12, %r12
	je	.L511
	movq	(%r12), %rax
	movl	$0, %r14d
	cmpq	%rax, %rbp
	je	.L510
	movq	%r12, %rax
	jmp	.L512
.L513:
	movq	(%rax), %rdx
	cmpq	%rdx, %rbp
	je	.L539
.L512:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L513
	jmp	.L511
.L539:
	movl	$0, %r14d
.L510:
	movq	24(%r12), %rax
	testq	%rax, %rax
	jne	.L514
	movq	%rbp, (%r12)
	movq	$0, 16(%r12)
	movq	%r12, 24(%r12)
	jmp	.L515
.L514:
	movq	(%r12), %r15
	movl	$32, %edi
	call	malloc
	movq	%r15, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%r12), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%r12), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%r12), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%r12)
	movq	%rbp, (%r12)
.L515:
	movb	%r14b, 8(%r12)
	movq	24(%rbx), %rax
	testq	%rax, %rax
	je	.L540
	movzbl	80(%rax), %eax
	movl	$0, %r14d
	testb	%al, %al
	jne	.L516
	movl	$0, %eax
.L519:
	movl	%eax, %ecx
	movslq	%eax, %rdx
	addq	$6, %rdx
	movq	(%rbx,%rdx,8), %rdx
	testq	%rdx, %rdx
	je	.L517
	movzbl	80(%rdx), %edx
	testb	%dl, %dl
	jne	.L517
	movslq	%eax, %rdx
	addq	$6, %rdx
	movq	(%rbx,%rdx,8), %rdx
	cmpq	%rbp, %rdx
	je	.L517
	testq	%r14, %r14
	je	.L518
	movzbl	80(%r14), %edx
	testb	%dl, %dl
	je	.L542
	jmp	.L518
.L517:
	addl	$1, %eax
	cmpl	$4, %eax
	jne	.L519
	movq	%r14, %rdi
	testq	%r14, %r14
	je	.L516
	movzbl	80(%r14), %eax
	testb	%al, %al
	jne	.L516
	testq	%r12, %r12
	je	.L520
	movq	(%r12), %rax
	movl	$0, %ebx
	cmpq	%r14, %rax
	je	.L521
	movq	%r12, %rax
	jmp	.L522
.L523:
	movq	(%rax), %rdx
	cmpq	%rdi, %rdx
	je	.L544
.L522:
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L523
	jmp	.L520
.L544:
	movl	$0, %ebx
.L521:
	movq	24(%r12), %rax
	testq	%rax, %rax
	jne	.L524
	movq	%r14, (%r12)
	movq	$0, 16(%r12)
	movq	%r12, 24(%r12)
	jmp	.L525
.L524:
	movq	(%r12), %r15
	movl	$32, %edi
	call	malloc
	movq	%r15, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movzbl	8(%r12), %edx
	movl	$0, %eax
	movb	%dl, 8(%rax)
	movq	16(%r12), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%r12), %rdx
	movq	%rdx, 24(%rax)
	movq	%rax, 16(%r12)
	movq	%r14, (%r12)
.L525:
	movb	%bl, 8(%r12)
	movq	40(%rbp), %rax
	movl	$0, %ebx
	testq	%rax, %rax
	je	.L555
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L555
	movq	40(%rbp), %rax
	movq	24(%rax), %rbx
	jmp	.L555
.L497:
	movq	40(%rbx), %rbx
	jmp	.L555
.L557:
	movq	%rbp, %rbx
.L555:
	testq	%rbx, %rbx
	je	.L526
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L526
	movq	8(%rbx), %rdx
	movss	0(%r13), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movss	(%rdx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	subsd	.LC8(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	setae	%cl
	movss	4(%r13), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rdx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	subsd	.LC8(%rip), %xmm2
	ucomisd	%xmm2, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%ecx, %eax
	andl	$3, %eax
	addq	$6, %rax
	movq	(%rbx,%rax,8), %rbp
	testq	%rbp, %rbp
	jne	.L556
.L490:
	movq	40(%rbx), %rbx
	jmp	.L555
.L492:
	movq	40(%rbx), %rbx
	jmp	.L555
.L499:
	leaq	88(%r14), %rdi
	call	pthread_mutex_lock
	movl	$1, %r15d
	jmp	.L498
.L505:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_lock
	movl	$1, %r15d
	jmp	.L504
.L511:
	leaq	88(%rbp), %rdi
	call	pthread_mutex_lock
	movl	$1, %r14d
	jmp	.L510
.L518:
	movslq	%ecx, %rcx
	addq	$6, %rcx
	movq	(%rbx,%rcx,8), %r14
	jmp	.L517
.L540:
	movl	$0, %r14d
	jmp	.L516
.L542:
	movl	$0, %r14d
.L516:
	movl	$1, %ebx
	jmp	.L521
.L520:
	addq	$88, %rdi
	call	pthread_mutex_lock
	movl	$1, %ebx
	jmp	.L521
.L526:
	movl	$1, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE77:
	.size	Quadtree_remove_lock, .-Quadtree_remove_lock
	.globl	Quadtree_remove_validate
	.type	Quadtree_remove_validate, @function
Quadtree_remove_validate:
.LFB78:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %rbp
	movq	%rsi, %r15
	movq	24(%rdi), %rdx
	movl	$1, %eax
	testq	%rdx, %rdx
	je	.L559
	testq	%rdi, %rdi
	je	.L578
.L601:
	movq	16(%rbp), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %rax
	movq	(%rax), %r12
	movq	16(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rbx
	movq	16(%rbp), %rax
	movq	(%rax), %rdx
	movq	0(%rbp), %r13
	movq	16(%rbp), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %rbp
	testq	%r12, %r12
	je	.L560
	movzbl	80(%r12), %eax
	testb	%al, %al
	jne	.L560
	movq	8(%rbx), %rsi
	movq	8(%r12), %rcx
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%dil
	movss	4(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%edi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	(%r12,%rax,8), %rax
	cmpq	%rbx, %rax
	jne	.L580
	movq	24(%rax), %rax
	cmpq	%r12, %rax
	jne	.L581
.L560:
	movq	8(%rdx), %rsi
	movq	8(%rbx), %rcx
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%dil
	movss	4(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%edi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	(%rbx,%rax,8), %r14
	cmpq	%rdx, %r14
	jne	.L583
	movq	24(%r14), %rax
	cmpq	%rbx, %rax
	jne	.L584
	testq	%r13, %r13
	je	.L565
	movzbl	80(%r13), %edx
	testb	%dl, %dl
	jne	.L565
	movq	8(%r13), %rsi
	movq	8(%rax), %rcx
	movss	(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	setae	%dil
	movss	4(%rsi), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	4(%rcx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC8(%rip), %xmm0
	ucomisd	%xmm0, %xmm1
	sbbl	%edx, %edx
	notl	%edx
	andl	$2, %edx
	orl	%edi, %edx
	movsbq	%dl, %rdx
	addq	$6, %rdx
	movq	(%rax,%rdx,8), %rdx
	cmpq	%r13, %rdx
	jne	.L586
	movq	24(%rdx), %rdx
	cmpq	%rdx, %rax
	jne	.L587
.L565:
	movq	8(%r14), %rdi
	movq	%r15, %rsi
	call	Point_equals
	testb	%al, %al
	je	.L588
	movq	40(%rbx), %rax
	testq	%rax, %rax
	jne	.L589
	movq	40(%r14), %rax
	testq	%rax, %rax
	jne	.L590
	testq	%r12, %r12
	je	.L568
	movzbl	80(%r12), %eax
	xorl	$1, %eax
	movzbl	%al, %eax
.L568:
	movl	%eax, %ecx
	movq	24(%rbx), %rdx
	movl	$0, %eax
	testq	%rdx, %rdx
	je	.L569
	movzbl	80(%rdx), %eax
	xorl	$1, %eax
	movzbl	%al, %eax
.L569:
	cmpb	%al, %cl
	jne	.L593
	testq	%r12, %r12
	je	.L570
	movzbl	80(%r12), %eax
	testb	%al, %al
	jne	.L570
	movl	$0, %eax
	movl	$0, %esi
.L572:
	movslq	%eax, %rdx
	addq	$6, %rdx
	movq	(%rbx,%rdx,8), %rcx
	movl	$0, %edx
	testq	%rcx, %rcx
	je	.L571
	movzbl	80(%rcx), %edx
	xorl	$1, %edx
	movzbl	%dl, %edx
.L571:
	addl	%edx, %esi
	addl	$1, %eax
	cmpl	$4, %eax
	jne	.L572
	cmpb	$2, %sil
	sete	%dl
	movzbl	%dl, %edx
	movb	$0, %al
	testq	%r13, %r13
	je	.L573
	movzbl	80(%r13), %eax
	xorl	$1, %eax
	movzbl	%al, %eax
.L573:
	movzbl	%al, %eax
	cmpl	%eax, %edx
	je	.L574
	jmp	.L604
.L576:
	movzbl	80(%r13), %eax
	testb	%al, %al
	je	.L597
.L574:
	testq	%rbp, %rbp
	jne	.L601
	movl	$1, %eax
	jmp	.L559
.L578:
	movl	$1, %eax
	jmp	.L559
.L580:
	movl	$0, %eax
	jmp	.L559
.L581:
	movl	$0, %eax
	jmp	.L559
.L583:
	movl	$0, %eax
	jmp	.L559
.L584:
	movl	$0, %eax
	jmp	.L559
.L586:
	movl	$0, %eax
	jmp	.L559
.L587:
	movl	$0, %eax
	jmp	.L559
.L588:
	movl	$0, %eax
	jmp	.L559
.L589:
	movl	$0, %eax
	jmp	.L559
.L590:
	movl	$0, %eax
	jmp	.L559
.L593:
	movl	$0, %eax
	jmp	.L559
.L604:
	movl	$0, %eax
	jmp	.L559
.L597:
	movl	$0, %eax
.L559:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L570:
	.cfi_restore_state
	testq	%r13, %r13
	jne	.L576
	jmp	.L574
	.cfi_endproc
.LFE78:
	.size	Quadtree_remove_validate, .-Quadtree_remove_validate
	.globl	Quadtree_remove_helper
	.type	Quadtree_remove_helper, @function
Quadtree_remove_helper:
.LFB79:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset 13, -16
	.cfi_offset 12, -24
	.cfi_offset 6, -32
	.cfi_offset 3, -40
	movq	24(%rdi), %rax
	movl	$0, %edx
	testq	%rax, %rax
	je	.L606
	movq	16(%rdi), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rbp
	movq	16(%rdi), %rax
	movq	16(%rax), %rax
	movq	(%rax), %r13
	movq	16(%rdi), %rax
	movq	(%rax), %r12
	movq	(%rdi), %rbx
	movq	16(%rdi), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %rax
	movq	16(%rax), %rdi
	testq	%rdi, %rdi
	je	.L607
	call	Quadtree_remove_helper
	movl	$0, %edx
	testb	%al, %al
	je	.L606
.L607:
	movb	$1, 80(%r12)
	movl	$1, %edx
	testq	%rbx, %rbx
	je	.L606
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L606
	movq	8(%r13), %rcx
	movq	8(%rbp), %rdx
	movss	(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	.LC8(%rip), %xmm1
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	setae	%sil
	movss	4(%rcx), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movss	4(%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	ucomisd	%xmm0, %xmm2
	sbbl	%eax, %eax
	notl	%eax
	andl	$2, %eax
	orl	%esi, %eax
	movsbq	%al, %rax
	addq	$6, %rax
	movq	%rbx, 0(%rbp,%rax,8)
	movq	%rbp, 24(%rbx)
	movb	$1, 80(%r13)
	movl	$1, %edx
.L606:
	movl	%edx, %eax
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE79:
	.size	Quadtree_remove_helper, .-Quadtree_remove_helper
	.globl	Quadtree_remove
	.type	Quadtree_remove, @function
Quadtree_remove:
.LFB80:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$16, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, %rbx
	movq	%xmm0, (%rsp)
.L620:
	movq	32(%rbx), %rax
	testq	%rax, %rax
	je	.L617
	movzbl	80(%rax), %eax
	testb	%al, %al
	je	.L618
.L617:
	movl	$10, %r12d
	jmp	.L619
.L618:
	movq	32(%rbx), %rbx
	jmp	.L620
.L619:
	movl	$32, %edi
	call	malloc
	movq	$0, (%rax)
	movb	$0, 8(%rax)
	movq	%rax, 24(%rax)
	movq	$0, 16(%rax)
	movl	$0, %ebp
	movq	%rbp, %rdx
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	Quadtree_remove_lock
	testb	%al, %al
	je	.L623
	movq	%rsp, %rsi
	movq	%rbp, %rdi
	call	Quadtree_remove_validate
	testb	%al, %al
	jne	.L622
.L623:
	subq	$1, %r12
	jne	.L619
	.p2align 4,,5
	jmp	.L632
.L622:
	movl	$0, %ebp
	movq	%rbp, %rdi
	call	Quadtree_remove_helper
	movl	%eax, %r12d
	movl	$0, %ebx
	testq	%rbp, %rbp
	jne	.L628
	jmp	.L626
.L631:
	movq	%rbp, %rbx
.L628:
	movq	16(%rbx), %rbp
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L627
	movzbl	80(%rax), %eax
	testb	%al, %al
	jne	.L627
	movzbl	8(%rbx), %eax
	testb	%al, %al
	je	.L627
	movq	(%rbx), %rdi
	addq	$88, %rdi
	call	pthread_mutex_unlock
.L627:
	movq	%rbx, %rdi
	call	free
	testq	%rbp, %rbp
	jne	.L631
	.p2align 4,,2
	jmp	.L626
.L632:
	movl	$0, %r12d
.L626:
	movl	%r12d, %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE80:
	.size	Quadtree_remove, .-Quadtree_remove
	.globl	Quadtree_free_helper
	.type	Quadtree_free_helper, @function
Quadtree_free_helper:
.LFB81:
	.cfi_startproc
	movq	%rbx, -40(%rsp)
	movq	%rbp, -32(%rsp)
	movq	%r12, -24(%rsp)
	movq	%r13, -16(%rsp)
	movq	%r14, -8(%rsp)
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset 14, -16
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 6, -40
	.cfi_offset 3, -48
	movq	%rdi, %rbx
	movq	%rsi, %r12
	movzbl	(%rdi), %eax
	movl	$1, %r13d
	testb	%al, %al
	je	.L634
	movl	$0, %ebp
.L636:
	movslq	%ebp, %rax
	addq	$6, %rax
	movq	(%rbx,%rax,8), %rax
	testq	%rax, %rax
	je	.L635
	movslq	%ebp, %r14
	addq	$6, %r14
	movq	(%rbx,%r14,8), %rdi
	movq	%r12, %rsi
	call	Quadtree_free_helper
	andl	%eax, %r13d
	movq	$0, (%rbx,%r14,8)
.L635:
	addl	$1, %ebp
	cmpl	$4, %ebp
	jne	.L636
.L634:
	movq	32(%rbx), %rax
	testq	%rax, %rax
	je	.L637
	movq	32(%rbx), %rax
	movq	$0, 40(%rax)
.L637:
	movq	40(%rbx), %rax
	testq	%rax, %rax
	je	.L638
	movq	40(%rbx), %rax
	movq	$0, 32(%rax)
.L638:
	addq	$1, (%r12)
	movzbl	80(%rbx), %eax
	testb	%al, %al
	jne	.L639
	addq	$1, 8(%r12)
	movzbl	(%rbx), %eax
	xorl	$1, %eax
	movzbl	%al, %eax
	addq	%rax, 16(%r12)
.L639:
	leaq	88(%rbx), %rdi
	call	pthread_mutex_destroy
	movq	8(%rbx), %rdi
	call	free
	movq	%rbx, %rdi
	call	free
	movl	%r13d, %eax
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	movq	24(%rsp), %r13
	movq	32(%rsp), %r14
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE81:
	.size	Quadtree_free_helper, .-Quadtree_free_helper
	.globl	Quadtree_free
	.type	Quadtree_free, @function
Quadtree_free:
.LFB82:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$40, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, %r13
	movq	%rsi, %rdi
	movq	$0, (%rsp)
	movq	$0, 8(%rsp)
	movq	$0, 16(%rsp)
	movq	$0, 24(%rsp)
	movq	32(%rsi), %rax
	testq	%rax, %rax
	je	.L643
.L650:
	movq	32(%rdi), %rdi
	movq	32(%rdi), %rax
	testq	%rax, %rax
	jne	.L650
.L643:
	testq	%rdi, %rdi
	jne	.L653
	jmp	.L645
.L649:
	movq	%rbx, %rdi
.L653:
	movq	40(%rdi), %rbx
	movq	24(%rsp), %r12
	movl	$0, %ebp
	testq	%rdi, %rdi
	je	.L646
	movzbl	80(%rdi), %ebp
	xorl	$1, %ebp
	movzbl	%bpl, %ebp
.L646:
	movq	%rsp, %rsi
	call	Quadtree_free_helper
	movzbl	%al, %eax
	imull	%eax, %ebp
	movslq	%ebp, %rbp
	addq	%rbp, %r12
	movq	%r12, 24(%rsp)
	testq	%rbx, %rbx
	jne	.L649
.L645:
	movq	(%rsp), %rax
	movq	%rax, 0(%r13)
	movq	8(%rsp), %rax
	movq	%rax, 8(%r13)
	movq	16(%rsp), %rax
	movq	%rax, 16(%r13)
	movq	24(%rsp), %rax
	movq	%rax, 24(%r13)
	movq	%r13, %rax
	addq	$40, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE82:
	.size	Quadtree_free, .-Quadtree_free
	.local	lock_attr
	.comm	lock_attr,4,4
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1071644672
	.align 8
.LC4:
	.long	0
	.long	1138753536
	.align 8
.LC8:
	.long	2696277389
	.long	1051772663
	.align 8
.LC9:
	.long	2696277389
	.long	-1095710985
	.align 8
.LC10:
	.long	0
	.long	-1074790400
	.align 8
.LC11:
	.long	0
	.long	1072693248
	.align 8
.LC12:
	.long	0
	.long	0
	.align 8
.LC13:
	.long	0
	.long	1070596096
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
