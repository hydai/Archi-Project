;;; Testcase 01 - Write to $zero

        ;; Use $t0 as counter
init:   add     $t0,    $zero,  $zero
        addi    $t1,    $zero,  100
        
loop:   ;; Arithmetic related
        add     $zero,  $zero,  $zero
        sub     $zero,  $zero,  $zero
        and     $zero,  $zero,  $zero
        or      $zero,  $zero,  $zero
        ;; Memory related
        lw      $zero,  24($zero)
        lui     $zero,  0x12E8
        ;; Do 100 times
        addi    $t0,    $t0,    1
        bne     $t0,    $t1,    loop

end:    halt
