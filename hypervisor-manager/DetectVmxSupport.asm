
PUBLIC MainAsm
.code _text

MainAsm PROC PUBLIC
push rax

; do stuffs here

pop rax
ret
MainAsm ENDP

                 
DetectVmxSupport PROC PUBLIC
    SUB RSP, 32
    XOR RAX, RAX
    XOR RCX, RCX
    MOV RAX, 80000001h
    CPUID
    BT ECX, 3h          ; CPUID Spec for AMD-V support is ECX[2]
    JC Ret1
    XOR RAX, RAX
    MOV RAX, 0
    ADD RSP, 32
    ret


Ret1:
    XOR RAX, RAX
    MOV RAX, 1
    ADD RSP, 32
    ret
DetectVmxSupport ENDP 


END 
