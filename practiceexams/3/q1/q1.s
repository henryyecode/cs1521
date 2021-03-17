# COMP1521 18s1 Exam Q1
# int isIdent(int matrix[N][N], int N)

   .text
   .globl isIdent

# params: matrix=$a0, N=$a1
isIdent:
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
   # if you need to save more than four $s? registers
   # add extra code here to save them on the stack

# ... your code for the body of isIdent(m,N) goes here ...
   li   $s0,    0           # int i = 0 : $s0

iloop:
    
   beq   $s0,   $a1, endiloop        
   li    $s1,   0           # int j = 0 : $s1
  
jloop:

   beq   $s1,   $a1, endjloop   
   li    $t0,   4           # $t0 = sizeof int
   la    $t1,   $a0         # load address of m
   mul   $t2,   $s0,  $a1   # size of row offset 
   add   $t2,   $t2,  $s1   # add column offset to row offset
   mul   $t1,   $t1,  $t2   # mul offset by sizeof int
   lw    $s2,   ($t1)       # m[i][j] : $s2
   bne   $s0,   $s1,  nextcond  
   li    $t0,   1
   beq   $s2,   $t0,  nextcond
   li    $v0,   0
   j    return

nextcond:
   beq   $s0,  $s1, fail
   li    $t0,  0
   beq   $s2,  $t0, fail
   li    $v0,   0
   j    return

fail:
   
   addi  $s1,  $s1, 1
   j    jloop
   
endjloop:

   addi  $s0,  $s0, 1   
   j     iloop      

endiloop:
  li    $v0,   1

return:
     







# epilogue
   # if you saved more than six $s? registers
   # add extra code here to restore them
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

