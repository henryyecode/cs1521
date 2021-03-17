# $a0 = int *src : array of ints
# $a1 = int n
# $a2 = int *dest : array of ints

main:

        li      $s0,    0                       # int i = 0 
        li      $s1,    0                       # int j = 0

loop:

        bge     $s0,    $a1,    endloop:
        li      $t0,    4                       # size of int
        mul     $t0,    $t0,    $s0             # size * offset(index)
        add     $t0,    $a0,    $t0             # add index to address
        lw      $s2,    ($t0)                   # load address
        li      $t1,    2
        rem     $s3,    $s2,    $t1             # $s2 =  src[i] % 2   
        bnez    $s3,    skip
        li      $t0,    4
        mul     $t0,    $t0,    $s1
        add     $t0,    $t0,    $a2
        sw      $s2,    ($t0)
        addi    $s1,    $s1,    1

skip:   
        addi    $s0,    $s0,    1   
        j       loop                 
        
endloop:
        
        move    $v0,    $s1


