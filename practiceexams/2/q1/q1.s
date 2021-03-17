# COMP1521 17s2 Final Exam
# void colSum(m, N, a)

   .text
   .globl colSum

# params: m=$a0, N=$a1, a=$a2
colSum:
# prologue
   addi $sp, $sp, -4
   sw   $fp, ($sp)
   la   $fp, ($sp)
   addi $sp, $sp, -4
   sw   $ra, ($sp)
   addi $sp, $sp, -4
   sw   $s0, ($sp)
   addi $sp, $sp, -4
   sw   $s1, ($sp)
   addi $sp, $sp, -4
   sw   $s2, ($sp)
   addi $sp, $sp, -4
   sw   $s3, ($sp)
   addi $sp, $sp, -4
   sw   $s4, ($sp)
   addi $sp, $sp, -4
   sw   $s5, ($sp)
   # if you need to save more than six $s? registers
   # add extra code here to save them on the stack

# suggestion for local variables (based on C code):
# m=#s0, N=$s1, a=$s2, row=$s3, col=$s4, sum=$s5

   # add code for your colSum function here
colSum:

    move $s0,   $a0   # m = $s0
    move $s1,   $a1,  # N = $s1
    move $s2,   $a3,  # a = $s2
    li   $s4,   0     # col = 0 = $s4

colloop:

    beq  $s4,   $s1,    endcolloop      # !(col < N)     
    li   $s5,   0                       # sum = $s5 = 0
    li   $s3,   0                       # row = $s3 = 0

rowloop:
    
    beq  $s3,   $s1,    endrowloop      # !(row < N)
    li   $t0,   4                       # size of int
    move $t1,   $s0                     # load address of m
    mul  $t2,   $s1,    $s4             # offset of previous cols
    add  $t2,   $t2,    $s3             # offset of prev cols plus offset of curr row
    mul  $t2,   $t2,    4               # mult offset by size of int
    lw   $t3,   ($t2)                   # load word at m[row][col]
    add  $s5,   $s5,    $t3             # sum += m[row][col]
    addi $s3,   $s3,    1               # row++
    j rowloop                           # loop
    
endrowloop:
    
    li   $t0,   4                       # size of int
    move $t1,   $s2                     # load address of a
    add  $t1,   $t1,    $s4             # add col offset 
    sw   $s5,   ($t1)                   # save sum into sum[col]
    addi $s4,   $s4,    1               # col++
    j    colloop                        # loop
    
endcolloop:


# epilogue
   # if you saved more than six $s? registers
   # add extra code here to restore them
   lw   $s5, ($sp)
   addi $sp, $sp, 4
   lw   $s4, ($sp)
   addi $sp, $sp, 4
   lw   $s3, ($sp)
   addi $sp, $sp, 4
   lw   $s2, ($sp)
   addi $sp, $sp, 4
   lw   $s1, ($sp)
   addi $sp, $sp, 4
   lw   $s0, ($sp)
   addi $sp, $sp, 4
   lw   $ra, ($sp)
   addi $sp, $sp, 4
   lw   $fp, ($sp)
   addi $sp, $sp, 4
   j    $ra

