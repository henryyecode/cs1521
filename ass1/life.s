# board1.s ... Game of Life on a 10x10 grid

	.data

N:	.word 10  # gives board dimensions

board:
	.byte 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 1, 0, 1, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 1, 1, 1, 0, 0, 0
	.byte 0, 0, 0, 1, 0, 0, 1, 0, 0, 0
	.byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0

newBoard: .space 100
# COMP1521 19t2 ... Game of Life on a NxN grid
#
# Written by <<YOU>>, June 2019

## Requires (from `boardX.s'):
# - N (word): board dimensions
# - board (byte[][]): initial board state
# - newBoard (byte[][]): next board state
    .data
msg1:   .asciiz "# Iterations: "
msg2:   .asciiz "=== After iteration "
msg3:   .asciiz " ===\n"
eol:    .asciiz "\n"
dot:    .asciiz "."   
hash:   .asciiz "#"
## Provides:
	.globl	main
	.globl	decideCell
	.globl	neighbours
	.globl	copyBackAndShow


########################################################################
# .TEXT <main>
	.text
main:
# Frame:	...
# Uses:		...
# Clobbers:	...

# Locals:	...

# Structure:
#	main
#	-> [prologue]
#	-> ...
#	-> [epilogue]

# Code:
    # Your main program code goes here.  Good luck!
	# Set up stack frame
	addi	$sp, $sp, -4
	sw	$fp, ($sp)	# push $fp
	la	$fp, ($sp)
	addi	$sp, $sp, -4
	sw	$ra, ($sp)	# push $ra
	addi	$sp, $sp, -4
	sw	$s0, ($sp)	# push $s0
	addi	$sp, $sp, -4
	sw	$s1, ($sp)	# push $s1
	addi	$sp, $sp, -4
	sw	$s2, ($sp)	# push $s2
	addi	$sp, $sp, -4
	sw	$s3, ($sp)	# push $s3
	
	# Main Code
	li  $s0, 0          # int maxiters = 0 ==> $s0
	
	la  $a0, msg1
	li	$v0, 4
	syscall			    # printf("# Iterations: ")
	
	li	$v0, 5
	syscall			    # scanf("%d", into $v0)
	move    $s0, $v0    # $s0 holds maxiters
	
    li  $s1, 1          # $s1 holds n
    
main_loopiter:
    bgt $s1, $s0, main_endloopiter              # (n > maxiters), jump to endloopiter

    li  $s2, 0                  # $s2 holds i 

main_looprow:
    lw  $t1, N                  # $t1 holds N
    beq $s2, $t1, main_endlooprow    # !(i < N), jump to endlooprow
    
    li  $s3, 0                  # $s3 holds j    

main_loopcol:
    lw  $t1, N
    beq $t1, $s3, main_endloopcol    # !(j < N), jump to endloopcol
    move  $a0, $s2
    move  $a1, $s3
    jal neighbours              # neighbours(i, j)
    move $t0, $v0               # $t0 holds nn
    
    lw   $t1, N
    mul  $t2, $t1, $s2               # offset of above rows (num of rows * size)
    add  $t2, $t2, $s3               # offset of current row         
    lb   $a0, board($t2)             # load integer at board[i][j]
    move $a1, $t0
    jal  decideCell
    sb   $v0, newBoard($t2)
    
    addi $s3, $s3, 1
    j main_loopcol
    
main_endloopcol:
    addi $s2, $s2, 1
    j main_looprow

main_endlooprow:
    # printf(stuff)
    la  $a0, msg2
	li	$v0, 4
	syscall			    # printf("=== After iteration ")
	
	move $a0, $s1
	li  $v0, 1
	syscall             # printf(n)
	
	la  $a0, msg3
	li	$v0, 4
	syscall			    # printf(" ===\n")
	
    
    jal copyBackAndShow
    
    addi $s1, $s1, 1
    j main_loopiter

main_endloopiter:
    
    
    # Clean up stack frame
    lw	$s3, ($sp)	# pop $s3
	addi	$sp, $sp, 4
	lw	$s2, ($sp)	# pop $s2
	addi	$sp, $sp, 4
	lw	$s1, ($sp)	# pop $s1
	addi	$sp, $sp, 4
	lw	$s0, ($sp)	# pop $s0
	addi	$sp, $sp, 4
	lw	$ra, ($sp)	# pop $ra
	addi	$sp, $sp, 4
	lw	$fp, ($sp)	# pop $fp
	addi	$sp, $sp, 4
    
    li	$v0, 0
	jr	$ra		# return 0


# Put your other functions here

### decideCell() function
	.text
decideCell:
# Frame:	...
# Uses:		...
# Clobbers:	...

# Locals:	...

# Structure:
#	decideCell
#	-> [prologue]
#	-> ...
#	-> [epilogue]

# Code:
    # Set up stack frame
	addi	$sp, $sp, -4
	sw	$fp, ($sp)	# push $fp
	la	$fp, ($sp)
	addi	$sp, $sp, -4
	sw	$ra, ($sp)	# push $ra
	addi	$sp, $sp, -4
	sw	$s0, ($sp)	# push $s0
	addi	$sp, $sp, -4
	sw	$s1, ($sp)	# push $s1
	addi	$sp, $sp, -4
	sw	$s2, ($sp)	# push $s2
	addi	$sp, $sp, -4
	sw	$s3, ($sp)	# push $s3
	
	# Code for decideCell
	li  $v0, 0              # char ret, initialised at 0 
	li  $t0, 1
	beq  $a0, $t0, old1     # if (old == 1) jump to old1
    li  $t0, 3
    beq  $a1, $t0, ret1     # if (nn == 3) jump to ret1
    j end                   # else, jump to end

old1:
    li  $t0, 2              
	blt  $a1, $t0, end      # if (nn < 2) jump to end
	beq  $a1, $t0, ret1     # if (nn == 2) jump to ret1
	li  $t0, 3              
	beq  $a1, $t0, ret1     # if (nn == 3) jump to ret1
    j end
    
ret1:
    li  $v0, 1              # set return value to 1

end:
    # Clean up stack frame
    lw	$s3, ($sp)	# pop $s3
	addi	$sp, $sp, 4
	lw	$s2, ($sp)	# pop $s2
	addi	$sp, $sp, 4
	lw	$s1, ($sp)	# pop $s1
	addi	$sp, $sp, 4
	lw	$s0, ($sp)	# pop $s0
	addi	$sp, $sp, 4
	lw	$ra, ($sp)	# pop $ra
	addi	$sp, $sp, 4
	lw	$fp, ($sp)	# pop $fp
	addi	$sp, $sp, 4
	jr	$ra

### neighbours() function
	.text
neighbours:
# Frame:	...
# Uses:		...
# Clobbers:	...

# Locals:	...

# Structure:
#	neighbours
#	-> [prologue]
#	-> ...
#	-> [epilogue]

# Code:
    # Set up stack frame
	addi	$sp, $sp, -4
	sw	$fp, ($sp)	# push $fp
	la	$fp, ($sp)
	addi	$sp, $sp, -4
	sw	$ra, ($sp)	# push $ra
	addi	$sp, $sp, -4
	sw	$s0, ($sp)	# push $s0
	addi	$sp, $sp, -4
	sw	$s1, ($sp)	# push $s1
	addi	$sp, $sp, -4
	sw	$s2, ($sp)	# push $s2
	addi	$sp, $sp, -4
	sw	$s3, ($sp)	# push $s3
	
	# Code for neighbour
	li  $v0, 0          # int nn = 0
	li  $t0, -1         # int x = -1  row number

neigh_looprow:
    li   $t2, 1
    bgt  $t0, $t2, neigh_endlooprow            # if (x > 1), jump to endlooprow
    
    li   $t1, -1                               # int y =-1 col number

neigh_loopcol:                                
    bgt  $t1, $t2, neigh_endloopcol            # if (y > 1), jump to endloopcol
    add  $t3, $a0, $t0                   # $t3 = i + x
    bltz $t3, addy                        # if (i + x) < 0, jump to y++
    lw   $s1, N                          # load N into $s1
    addi $s1, $s1, -1                    # $s1 = N - 1
    bgt  $t3, $s1, addy                   # (i + x) > (N - 1), jump to y++
    add  $t4, $a1, $t1                   # $t4 = j + y
    bltz $t4, addy                        # if (j + y) < 0, jump to y++
    bgt  $t4, $s1, addy                   # (j + y) > (N - 1), jump to y++        
    bnez $t0, skip                       # (x != 0), skip and condition
    beqz $t1, addy                        # (x == 0 && y == 0), jump to y++

skip:
    lw   $s1, N                      # size of array
    mul  $s2, $t3, $s1               # offset of above rows (num of rows * size)
    add  $s3, $t4, $s2               # offset of current row         
    lb   $s0, board($s3)             # load integer at board[i + x][j + y]
    li   $s1, 1
    bne  $s0, $s1, addy
    addi $v0, $v0, 1  
    
addy:
    addi $t1, $t1, 1
    j neigh_loopcol    
    
neigh_endloopcol:
    addi $t0, $t0, 1
    j neigh_looprow  

neigh_endlooprow:
  
	# Clean up stack frame
    lw	$s3, ($sp)	# pop $s3
	addi	$sp, $sp, 4
	lw	$s2, ($sp)	# pop $s2
	addi	$sp, $sp, 4
	lw	$s1, ($sp)	# pop $s1
	addi	$sp, $sp, 4
	lw	$s0, ($sp)	# pop $s0
	addi	$sp, $sp, 4
	lw	$ra, ($sp)	# pop $ra
	addi	$sp, $sp, 4
	lw	$fp, ($sp)	# pop $fp
	addi	$sp, $sp, 4
	jr	$ra


### copyBackAndShow() function
	.text
copyBackAndShow:
# Frame:	...
# Uses:		...
# Clobbers:	...

# Locals:	...

# Structure:
#	copyBackAndShow
#	-> [prologue]
#	-> ...
#	-> [epilogue]

# Code:
    # Set up stack frame
	addi	$sp, $sp, -4
	sw	$fp, ($sp)	# push $fp
	la	$fp, ($sp)
	addi	$sp, $sp, -4
	sw	$ra, ($sp)	# push $ra
	addi	$sp, $sp, -4
	sw	$s0, ($sp)	# push $s0
	addi	$sp, $sp, -4
	sw	$s1, ($sp)	# push $s1
	addi	$sp, $sp, -4
	sw	$s2, ($sp)	# push $s2
	addi	$sp, $sp, -4
	sw	$s3, ($sp)	# push $s3
	
	# Code for copyBackAndShow
	li   $t0, 0                          # int i = 0 ==> $t0
looprow:
    lw   $t2, N                          # N = size of array ==> $t2
    bge  $t0, $t2, endlooprow            # if !(i > N), jump to endlooprow
    
    li   $t1, 0                          # int j = 0 ==> $t1       
loopcol:                                 
    bge  $t1, $t2, endloopcol            # if !(j > N), jump to endloopcol
                       
    mul  $s1, $t0, $t2               # offset of above rows (num of rows * size)
    add  $s1, $s1, $t1               # offset of current row          
    lb   $s0, newBoard($s1)          # load integer at newBoard[i][j]
    sb   $s0, board($s1)             # save loaded integer into board[i][j]
    bnez $s0, next                   # board[i][j] != 0, jump to next   
   
    
    la	$a0, dot
	li	$v0, 4
	syscall                             #putchar ('.')
	addi $t1, $t1, 1
	j loopcol
    
next:
    la	$a0, hash
	li	$v0, 4
	syscall                             #putchar ('#')     
    addi $t1, $t1, 1
    j loopcol
    
endloopcol:
    addi $t0, $t0, 1
    la	$a0, eol
	li	$v0, 4
	syscall                             #putchar ('\n')  
    j looprow  

endlooprow:
	# Clean up stack frame
    lw	$s3, ($sp)	# pop $s3
	addi	$sp, $sp, 4
	lw	$s2, ($sp)	# pop $s2
	addi	$sp, $sp, 4
	lw	$s1, ($sp)	# pop $s1
	addi	$sp, $sp, 4
	lw	$s0, ($sp)	# pop $s0
	addi	$sp, $sp, 4
	lw	$ra, ($sp)	# pop $ra
	addi	$sp, $sp, 4
	lw	$fp, ($sp)	# pop $fp
	addi	$sp, $sp, 4
	jr	$ra










