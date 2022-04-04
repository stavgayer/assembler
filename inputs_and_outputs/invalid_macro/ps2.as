; file ps.as
    .entry LIST
    .extern W
MAIN: add r3, LIST
LOOP: prn #48
macro m2
inc r6
mov r3, W
endm
macro m3
inc r1
mov r1, W
endm
lea STR, r6
m2
macro m2
inc r7
mov r7, W
endm
m2
sub r1, r4
bne END
cmp val1, #-6
bne END[r15]
dec K
    .entry MAIN
sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, -9
    .data -100
    .entry K
K: .data 31
    .extern val1