;;; Testcase 02 - Overflow
;;; The result should be pairs of same value on stack (0x240)

        ;; Load 0x7fffffff
        lw      $s0,    0($zero)
        ;; Add 0x7fffffff to 1
        addi    $s1,    $zero,  1
        add     $s1,    $s1,    $s0     ;Overflow
        ;; Push result
        addi    $sp,    $sp,    -4
        sw      $s0,    0($sp)

        ;; Add 1 to 0x7fffffff (Immediate)
        addi    $s2,    $s0,    1       ;Overflow
        ;; Push result
        addi    $sp,    $sp,    -4
        sw      $s2,    0($sp)

        ;; Access memory at 0xffffffff + 13 = 12
        lw      $s0,    16($zero)
        lw      $s3,    13($s0)
        ;; Push result
        addi    $sp,    $sp,    -4
        sw      $s3,    0($sp)
        ;; Push expected result
        lw      $t0,    8($zero)
        addi    $sp,    $sp,    -4
        sw      $t0,    0($sp)

        ;; Test corner case of 0x80000000
        lw      $s0,    8($zero)
        ;; Add 1 to 0x80000000
        addi    $s1,    $s0,    1       ;Overflow
        ;; Push result
        addi    $sp,    $sp,    4
        sw      $s1,    0($sp)
        ;; Push expected result
        addi    $t1,    $zero,  0x80000001
        addi    $sp,    $sp,    4
        sw      $t1,    0($sp)

end:    halt
