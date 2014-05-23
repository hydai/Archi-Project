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
        ;; Set $gp to 1
        add     $gp,    $t0,    $zero
        ;; Set $at to 0
        add     $at,    $gp,    $t0
        sub     $at,    $at,    $at
        ;; Set $v0 as array address 
        add     $v0,    $at,    $at
        ;; Set $v1 as array size
        addi    $v1,    $v0,    32
        jal     mgs
        ;; Exit
        j       exit

mgs:    ;; Merge sort divide part
        ;; TODO

mgsre:  ;; Return
        jr      $ra

mg:     ;; Merge conquer part
        ;; TODO

exit:   ;; Exit pointer
        halt
        halt
        halt
        halt
        halt
