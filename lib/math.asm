# Function : power
# Signature : int pow(int b, int e)
# Returns : b^e

.text
.globl pow
pow:
    # $a0 - base
    # $a1 - exponent
    addi $sp, $sp, -8
    sw $ra, 4($sp)
    sw $a1, 8($sp)					   
    
    bnez $a1, rec
    addi $sp, $sp, 8        # pop the stack
    li $v0, 1               # return 1
    jr $ra
rec:
    addi $a1, $a1 -1	    # e = e - 1
    jal pow
    mul $v0, $v0, $a0       # b * (b^e)
    lw $ra, 4($sp)
    lw $a1, 8($sp)
    addi $sp, $sp, 8
    jr $ra

# Function : fast exponentiation algorithm
# Signature : int fpow(int b, int e)
# Returns : b^e

.text
.globl fpow
fpow:
    # $a0 - base
    # $a1 - exponent
    addi $sp, $sp, -8       # free some space of the stack
    sw $a1, 4($sp)          # this argument changes, we need to keep it
    sw $ra, 8($sp)

    bnez $a1, L1            # base case of recursion b^0 = 1
    addi $sp, $sp, 8        # pop the stack
    li $v0, 1
    jr $ra                  # return
L1:
    addi $sp, $sp, -4       # we jump to a procedure
    sw $ra, 12($sp)         # to know if our number
    jal iseven	            # is even
    lw $ra, 12($sp)
    addi $sp, $sp, 4
    beqz $t0, L2            # if number is even
    div $a1, $a1, 2         # b^e = (b^(e/2))^2
    jal fpow
    mul $v0, $v0, $v0        # square the value
    lw $ra, 8($sp)          # pop the stack
    lw $a1, 4($sp)          # and return
    addi $sp, $sp, 8
    jr $ra
L2:		                    # if number is not even
    addi $a1, $a1, -1       # b^e = b * b^(e - 1)
    jal fpow
    mul $v0, $v0, $a0       # the multiplication
    lw $ra, 8($sp)          # pop the stack
    lw $a1, 4($sp)          # and return
    addi $sp, $sp, 8
    jr $ra

# Function : iseven
# Signature : int iseven(int n)
# Returns : 0 if odd 1 if even
.text
.globl iseven
iseven:			
    rem $t0, $a1, 2         # psedoinstruction for the rem
    beqz $t0, true						
    li $t0, 0               
    jr $ra		            # return 0 for false
true:			            
    li $t0, 1	            # return 1 for true
    jr $ra

# Function : greatest common divisor
# Signature : int gcd(int a, int b)
# Returns : gcd of a,b
.text
.globl gcd
gcd:
    # $a0 - First number
    # $a1 - Second number
    move $t0, $a0
    move $t1, $a1
gcdloop:
    beq $t1, $0, gcddone
    div $t0, $t1
    move $t0, $t1
    mfhi $t1
    j gcdloop
gcddone:
    move $v0, $t0
    jr $ra

# Function : maximum
# Signaure : int max(int a, int b)
# Returns : max of two numbers
.text
.globl max
max:
    # $a0 - Contains first number A
    # $a1 - Contains second number B
    bge $a0, $a1, maxA
    move $v0, $a1
    j maxdone
maxA:
    move $v0, $a0
maxdone:
    jr $ra

# Function : minimum
# Signaure : int min(int a, int b)
# Returns : min of two numbers
.text
.globl min
min:
    # $a0 - Contains first number A
    # $a1 - Contains second number B
    bge $a0, $a1, minB
    move $v0, $a0
    j mindone
minB:
    move $v0, $a1
mindone:
    jr $ra
