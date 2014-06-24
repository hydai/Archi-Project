        ;; Use $t0 as counter
init:   add     $t0,    $zero,  $zero
        addi    $t1,    $zero,  10

loop:   ;; Memory related
        lw      $t2,    24($zero)
        lui     $t2,    0x12E8
        lw      $t5,    0($zero)
        lh      $t7,    2($zero)
        lui     $t3,    28
        lbu     $t3,    32($zero)
        lh      $t4,    10($zero)

        ;; Do 10 times
        addi    $t0,    $t0,    1
        bne     $t0,    $t1,    loop

        ;; Load word from 0 to 23 and push onto stack
        add     $t0,    $zero,  $zero
        addi    $t1,    $zero,  24
loop2:  lw      $t2,    0($t0)
        addi    $sp,    $sp,    -4
        sw      $t2,    0($sp)
        ;; Increment loop counter
        addi    $t0,    $t0,    4
        bne     $t0,    $t1,    loop2

        ;; Load half word from 0 to 23 and push onto stack
        add     $t0,    $zero,  $zero
        addi    $t1,    $zero,  24
loop3:  lh      $t2,    0($t0)
        addi    $sp,    $sp,    -2
        sh      $t2,    0($sp)
        ;; Increment loop counter
        addi    $t0,    $t0,    2
        bne     $t0,    $t1,    loop3

        ;; Load byte from 0 to 23 and push onto stack
        add     $t0,    $zero,  $zero
        addi    $t1,    $zero,  24
loop4:  lb      $t2,    0($t0)
        addi    $sp,    $sp,    -1
        sb      $t2,    0($sp)
        ;; Increment loop counter
        addi    $t0,    $t0,    1
        bne     $t0,    $t1,    loop4

        halt
        halt
        halt
        halt
        halt
