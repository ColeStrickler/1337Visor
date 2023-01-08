.const



.code

extern exitcheck : proc

__enter_svm proc
	mov rsp, rcx		; We setting rsp to the Stack Ptr that we passed in for initialization
	
loop_start:
	
	mov rax, [rsp]		; we passed in the guest VMCB Physical Address, so this is moving the 
						; Guest VMCB into rax as required for the VMRUN and VMLOAD instructions
		

	vmload rax			; Load some state from the VMCB
	

	vmrun rax			; enter SVM, we will not leave here until a #VMEXIT occurs


	vmsave rax			; save some state into VMCB




	
	push  rax
    push  rbx
    push  rcx
    push  rdx
    push  rbp
    push  rsi
    push  rdi
    push  r8
    push  r9
    push  r10
    push  r11
    push  r12
    push  r13
    push  r14
    push  r15

    ; Push size of 8 * 15
    ; Before this rsp = ptr_guest_vmcb
    ; rsp + 8*15 => ptr_guest_VMCB
    ; rsp + 8*16 => ptr_host_VMCB
    ; rsp + 8*17 => ptr_Self ===> we want to grab this as well
                

                            ; setup args for exitcheck(Virtual_Processor_Data, Register_Context)
    mov rdx, rsp            ; stack pointer is currently setup in format of Register_Context struct
    mov rcx, [rsp + 88h]    ; 8*17 = 0x88, and since this is a stack ptr, we will dereference it to get the data there

    
    sub rsp, 88h            ; allocate save space for volatile registers

    movaps [rsp + 20h], xmm0
    movaps [rsp + 30h], xmm1
    movaps [rsp + 40h], xmm2
    movaps [rsp + 50h], xmm3
    movaps [rsp + 60h], xmm4
    movaps [rsp + 70h], xmm5


    call exitcheck


    movaps xmm5, [rsp + 70h]
    movaps xmm4, [rsp + 60h]
    movaps xmm3, [rsp + 50h]
    movaps xmm2, [rsp + 40h]
    movaps xmm1, [rsp + 30h]
    movaps xmm0, [rsp + 20h]


    add rsp, 88h


    test al, al               ; set ZF to 1 if al == 0, al holds our returned boolean value from exitcheck()

    pop  r15                  ; restore host register context
    pop  r14
    pop  r13
    pop  r12
    pop  r11
    pop  r10
    pop  r9
    pop  r8
    pop  rdi
    pop  rsi
    pop  rbp
    pop  rdx
    pop  rcx
    pop  rbx
    pop  rax

    jnz exit_virtualization
    jmp loop_start


exit_virtualization:
    ; rcx holds the stack pointer that we orignally passed in to __enter_svm()
    ; we need to restore this
    mov rsp, rcx
    
    ; update the magic value
    mov ecx, 'hypr'

    jmp rbx



__enter_svm endp


END