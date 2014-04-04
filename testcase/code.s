        ;; Use $t0 as counter
init:   add     $t0,    $zero,  $zero
        addi    $t1,    $zero,  10
        
loop:   ;; Arithmetic related
        add     $zero,  $zero,  $zero
        sub     $zero,  $zero,  $zero
        and     $zero,  $zero,  $zero
        or      $zero,  $zero,  $zero
        ;; Memory related
        lw      $zero,  24($zero)
        lui     $zero,  0x12E8
        ;; Do 10 times
        addi    $t0,    $t0,    1
        bne     $t0,    $t1,    loop

        ;; Load 0x7fffffff
        lw      $s0,    0($zero)
        ;; Add 0x7fffffff to 1
        addi    $s1,    $zero,  1
        add     $s1,    $s1,    $s0     ;Overflow
        ;; Push result
        addi    $sp,    $sp,    -4
        sw      $s0,    0($sp)
        add     $1,     $s0,    $zero

        ;; Add 1 to 0x7fffffff (Immediate)
        addi    $s2,    $s0,    1       ;Overflow
        ;; Push result
        addi    $sp,    $sp,    -4
        sw      $s2,    0($sp)
        add     $2,     $s2,    $zero

        ;; Access memory at 0xffffffff + 13 = 12
        lw      $s0,    16($zero)
        lw      $s3,    13($s0)
        ;; Push result
        addi    $sp,    $sp,    -4
        sw      $s3,    0($sp)
        add     $3,     $s3,    $zero
        ;; Push expected result
        lw      $t0,    8($zero)
        addi    $sp,    $sp,    -4
        sw      $t0,    0($sp)
        add     $4,     $t0,    $zero

        ;; Test corner case of 0x80000000
        lw      $s0,    8($zero)
        ;; Add 1 to 0x80000000
        addi    $s1,    $s0,    1       ;Overflow
        ;; Push result
        addi    $sp,    $sp,    4
        sw      $s1,    0($sp)
        add     $5,     $s1,    $zero
        ;; Push expected result
        addi    $t1,    $zero,  0x80000001
        addi    $sp,    $sp,    4
        sw      $t1,    0($sp)
        add     $6,     $t1,    $zero

        ;; Address overflow to end test
        addi    $t2,    $zero,  1
        lb      $zero,  1023($t2)     ;Overflow
