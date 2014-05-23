        ;; Merge Sort
        ;; Author: hydai(z54981220@gmail.com)
        ;; $zero => 0 (fixed)
        ;; $at => count of inversion pair
        ;; $v0 => address of dst arr
        ;; $v1 => size of dst arr
        ;; $a0 => address of lsc
        ;; $a1 => size of lsc
        ;; $a2 => address of rsc
        ;; $a3 => size of rsc
        ;; $t0 => tmp counter for i
        ;; $t1 => tmp counter for lct
        ;; $t2 => tmp counter for rct
        ;; $t3 => tmp counter for dstct
        ;; $gp => 1 (assign)
        ;; $sp => stack pointer
        ;; $ra => return pointer

init:   ;; Initialize
        add     $t0,    $zero,  $zero
        addi    $t0,    $t0,    1
        add     $gp,    $t0,    $zero   ;; Set $gp to 1
        add     $at,    $gp,    $t0
        sub     $at,    $at,    $at     ;; Set $at to 0
        add     $v0,    $at,    $at     ;; Set $v0 as array address
        addi    $v1,    $v0,    32      ;; Set $v1 as array size
        jal     mgs
        j       exit                    ;; Goto exit

mgs:    ;; Merge sort divide part
        beq     $v1,    $gp,    mgsre   ;; Check size of array <= 1 or not
        sra     $a1,    $v1,    1       ;; lscSize = arrSize / 2
        add     $t9,    $a1,    $zero
        sll     $t9,    $t9,    2       ;; $t9 = size of memory
        sub     $a3,    $v1,    $a1     ;; rscSize = arrSize - lscSize
        add     $t8,    $a3,    $zero
        sll     $t8,    $t8,    2       ;; $t8 = size of memory
        add     $a0,    $sp,    $zero   ;; Address of lsc = $sp
        add     $sp,    $sp,    $t9     ;; Alloc mem from stack for lsc
        add     $a2,    $sp,    $zero   ;; Address of rsc = $sp
        add     $sp,    $sp,    $t8     ;; Alloc mem from stack for rsc

        ;; Load data to lsc
        add     $t1,    $zero,  $zero
        add     $t2,    $a0,    $zero
        add     $t3,    $v0,    $zero
ldLsc:  ;; Load lsc from memory
        lw      $t4,    0($t3)
        sw      $t4,    0($t2)
        addi    $t3,    $t3,    4
        addi    $t2,    $t2,    4
        addi    $t1,    $t1,    1
        bne     $a1,    $t1,    ldLsc

        ;; Load data to rsc
        add     $t1,    $zero,  $zero
        add     $t2,    $a2,    $zero
ldRsc:  ;; Load rsc from memory
        lw      $t4,    0($t3)
        sw      $t4,    0($t2)
        addi    $t3,    $t3,    4
        addi    $t2,    $t2,    4
        addi    $t1,    $t1,    1
        bne     $a3,    $t1,    ldRsc
        ;; TODO: recursize call merge sort and merge

mgsre:  ;; Return
        jr      $ra

mg:     ;; Merge conquer part
        ;; TODO: merge

exit:   ;; Exit pointer
        halt
        halt
        halt
        halt
        halt
