bits 64
extern malloc, puts, printf, fflush, abort, free
global main


section   .data
empty_str: db 0x0
int_format: db "%ld ", 0x0
data: dq 4, 8, 15, 16, 23, 42
data_length: equ ($-data) / 8


section .text

;;; print_int proc
print_int:
    push rbp
      
    push rax
    push rdi
    push rsi

    mov rsi, rdi
    mov rdi, int_format
    xor rax, rax
    call printf

    xor rdi, rdi
    call fflush

    pop rsi
    pop rdi
    pop rax
    pop rbp
    ret

;;; p proc
p:
    mov rax, rdi
    and rax, 1
    ret
    
;;; add_element proc
add_element:
    push rbp
    push rbx
    push r14
        
    mov rbp, rsp
    sub rsp, 16

    mov r14, rdi
    mov rbx, rsi

    mov rdi, 16
    call malloc
    test rax, rax
    jz abort

    mov [rax], r14
    mov [rax + 8], rbx

    mov rsp, rbp
    pop r14
    pop rbx
    pop rbp
    ret    
    
;;; m proc
m:
    push rbp
    push rax
    push rbx
    push r12
    
print_loop:
    test rdi, rdi
    jz outm
    mov rbx, rdi
    
    mov rdi, [rdi]
    call print_int

    mov rdi, [rbx + 8]
    jmp print_loop

outm:
    pop r12
    pop rbx
    pop rax        
    pop rbp
    ret

;;; f proc
f:
    mov rax, rsi

    push rbx
    push r12
    push r13
    
add_loop:
    
    test rdi, rdi
    jz outf
        
    mov rbx, rdi
    mov r12, rsi
    
    mov rdi, [rdi]
    call p
    test rax, rax
    jz z
    
    mov rdi, [rbx]
    mov rsi, r12
    call add_element
    mov rsi, rax
    jmp ff
    
z:
    mov rsi, r12
    
ff:
    mov rdi, [rbx + 8]
    jmp add_loop;
    
outf:
    pop r13
    pop r12
    pop rbx
    ret

;;; mem_free
mem_free:
    push rbp
    mov rbp, rsp
       
    push rax
    push rdi
    push rbx
    push rsi
    
.next_iter:
    test rdi, rdi   
    jz .outmf1
    mov rbx, [rdi + 8]
    call free
    mov rdi, rbx
    jmp .next_iter        
.outmf1:
    pop rsi
    pop rbx
    pop rdi
    pop rax
    pop rbp
    ret              
        

main:
    mov rbp, rsp; for correct debugging
    push rbx

    xor rax, rax
    mov rbx, data_length
adding_loop:
    mov rdi, [data - 8 + rbx * 8]
    mov rsi, rax
    call add_element
    dec rbx
    jnz adding_loop
        
    mov r14, rax
    mov rbx, rax
    mov rdi, rax
    call m
    
    
    mov rdi, empty_str
    call puts

    xor rsi, rsi
    mov rdi, rbx
    call f

    mov rdi, rsi
    mov rbx, rsi
    call m


    mov rdi, empty_str
    call puts
                                              
    mov rdi, r14
    call mem_free
    
    mov rdi, rbx
    call mem_free

    pop rbx

    xor rax, rax
    ret