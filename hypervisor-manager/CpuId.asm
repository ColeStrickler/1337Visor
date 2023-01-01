
;; Created by Xeno Kovah in 2021 for OpenSecurityTraining2 Architecture 2001 class
;; This Source Code Form is subject to the terms of 
;; the Mozilla Public License, v. 2.0
.code 
call_cpuid PROC PUBLIC eax_in_val:DWORD, ecx_in_val:DWORD, eax_out_ptr:QWORD, ebx_out_ptr:QWORD, ecx_out_ptr:QWORD, edx_out_ptr:QWORD
   ; Since masm doen't seem to handle the x86-64 
   ; calling convention with variables properly when trying to use parameters
   ; (or I can't find the right syntax),
   ; I just need to grab stuff from the registers myself I guess...
   mov r10, rcx                 ; eax_in_val
   mov r11, rdx                 ; ecx_in_val
   mov r12, r8                  ; eax_out_ptr
   mov r13, r9                  ; ebx_out_ptr
   mov r14, qword ptr [rsp+30h] ; ecx_out_ptr
   mov r15, qword ptr [rsp+38h] ; edx_out_ptr

   mov rax, r10
   mov rcx, r11
   cpuid

   mov dword ptr [r12], eax
   mov dword ptr [r13], ebx
   mov dword ptr [r14], ecx
   mov dword ptr [r15], edx
   ret
call_cpuid ENDP 
end
