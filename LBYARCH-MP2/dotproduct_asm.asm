section .text
bits 64
default rel

global dotproduct_asm

dotproduct_asm:
    ; Input: rcx = n, rdx = A[], r8 = B[]
    ; Output: xmm0 = dot product result
    
    ; Initialize accumulators to 0.0
    xorpd xmm0, xmm0        ; xmm0 = 0.0 (accumulator for element 0)
    xorpd xmm3, xmm3        ; xmm3 = 0.0 (accumulator for element 1)
    
    ; Check if n <= 0
    test rcx, rcx
    jle .done
    
    ; Process pairs of elements using SIMD
    mov rax, rcx            ; rax = n
    shr rax, 1              ; rax = n / 2 (number of pairs)
    
    ; Check if we have pairs to process
    test rax, rax
    jz .handle_remainder
    
.loop_pairs:
    ; Load two doubles from A (A[i] and A[i+1])
    movupd xmm1, [rdx]      
    
    ; Load two doubles from B (B[i] and B[i+1])
    movupd xmm2, [r8]       
    
    ; Multiply element-wise: xmm1 = A[i]*B[i], A[i+1]*B[i+1]
    mulpd xmm1, xmm2        
    
    ; Add first element (lower 64 bits) to xmm0
    addsd xmm0, xmm1        
    
    ; Extract and add second element (upper 64 bits) to xmm3
    ; Shift xmm1 right by 64 bits and add to xmm3
    movhlps xmm1, xmm1      ; Move high double to low position
    addsd xmm3, xmm1        
    
    ; Move to next pair in A (2 * 8 bytes = 16 bytes)
    add rdx, 16             
    
    ; Move to next pair in B
    add r8, 16              
    
    ; Decrement pair counter
    dec rax
    jnz .loop_pairs
    
    ; Combine the two accumulators
    addsd xmm0, xmm3
    
.handle_remainder:
    ; Check if n is odd (if n % 2 == 1, there's a remainder)
    test rcx, 1             
    jz .done
    
    ; Process the last single element
    movsd xmm1, [rdx]       ; Load A[last]
    movsd xmm2, [r8]        ; Load B[last]
    
    ; Multiply single precision: xmm1 = A[last] * B[last]
    mulsd xmm1, xmm2        
    
    ; Add to result
    addsd xmm0, xmm1        
    
.done:
    ret