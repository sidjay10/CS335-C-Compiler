# Function : strlen
# Signature : int strlen(char *str)
# Returns : length of string excluding null byte
.text
.globl strlen
strlen:
	# $a0 - Address of null terminated string.
	# Return $v0 - Number of characters in the string.
	move $v0, $zero             # initialize len to 0
_strlen_loop:
	lb $t0, 0($a0)
	beq $t0, $zero, _strlen_ret
	addi $v0, $v0, 1
	addi $a0, $a0, 1
	j _strlen_loop
_strlen_ret:
	jr $ra


# Function : str_index_of
# Signature : int str_index_of(char *str, char c) 
# Returns : the index of the first instance of a character in string
.text
.globl str_index_of
str_index_of:
	# $a0 - Address of the string.
	# $a1 - Character to find first instance of.
	# Returns: $v0: Index of first occurrence, or -1.
	move $v0, $a0
_sio_loop:
	lbu $t0, 0($v0)
	beq $t0, $a1, _sio_found
	beq $t0, $zero, _sio_dne
	addi $v0, $v0, 1
	j _sio_loop
_sio_found:                     # First occurance found
	sub $v0, $v0, $a0
	jr $ra
_sio_dne:                       # Does not exist
	li $v0, -1
	jr $ra

###########
# Function : substr
# Signature : int substr(char *str, char *search_str)
# Returns : first index of substring in a string.
.text
.globl substr
substr:
	# $a0 - Address of null-terminated string to search within.
	# $a1 - Address of null-terminated substring to search for.
	# Returns $v0 - First index of $a1 in $a0, or -1 if not found.
    sub $sp, $sp, 12
	sw $ra, 8($sp)      # push ($ra)
	sw $a0, 4($sp)      # push ($a0)
	sw $a1, 0($sp)      # push ($s1)
	move $a0, $a1
	jal strlen          # Get the length of the substring. 
	move $t0, $v0       # Store length in $t0
    lw $a1, 0($sp)
	addi $sp, $sp, 4    # pop ($a1)
    lw $a0, 0($sp)
	addi $sp, $sp, 4    # pop ($a0) : $ra still on stack

	move $t1, $a0
	move $t2, $a1
	sub $sp, $sp, 4
    sw $a0, 0($sp)      # push ($a0) : save $a0 to compute index
_ssio_loop:
	# $t0: Length of substring.
	# $t1: Current search location in $a0.
	# $t2: Address of substring.
	# Check if we have reached the end of $a0.
	lbu $t3, 0($t1)
	beq $t3, $zero, _ssio_not_found

	# Call strncmp on the substring and the current index into $a0.
	move $a0, $t1       # Current addr in $a0
	move $a1, $t2       # Address of substring.
	move $a2, $t0       # Num characters to compare.
    sub $sp, $sp, 12
	sw $t0, 8($sp)      # push($t0)
	sw $t1, 4($sp)      # push($t1)
	sw $t2, 0($sp)      # push($t2)
	jal strncmp
    lw $t2, 0($sp)
	addi $sp, $sp, 4    # pop($t2)
    lw $t1, 0($sp)
	addi $sp, $sp, 4    # pop($t1)
    lw $t0, 0($sp)
	addi $sp, $sp, 4    # pop($t0)

	beq $v0, $zero, _ssio_found     # Substr found 
	addi $t1, $t1, 1                # Increment index and continue 
	j _ssio_loop
_ssio_not_found:
    lw $a0, 0($sp)
	addi $sp, $sp, 4    # pop($a0)	
    lw $ra, 0($sp)
	addi $sp, $sp, 4    # pop($ra)	
	li $v0, -1      
	jr $ra
_ssio_found:              
    lw $a0, 0($sp)
	addi $sp, $sp, 4    # pop($a0)
    lw $ra, 0($sp)
	addi $sp, $sp, 4    # pop($ra)
	sub $v0, $t1, $a0	# Current address minus original address
	jr $ra

###########
# Function : strncpy 
# Description : Copies at most 'n' bytes of a string into another buffer.
# Signature : int strncpy(char *dest, char *src, int b)
# Returns : Number of characters written to dest including null character
.text
.globl strncpy
strncpy:
    # $a0 - Address of destination buffer.
	# $a1 - Address of source buffer.
	# $a2 - num bytes to write in dest buffer (including null)
	# Returns: $v0 - Number of characters written out (including null byte).
	move $t0, $a0	            # $t0 points to current destination byte
	move $t1, $a1	            # $t1 points to current source byte
	add $t2, $a0, $a2	        # $t2 is 1 byte past the last write
_strncpy_loop:
	beq $t0, $t2, _strncpy_overflow	# Stop if we overflow
	lbu $t3, 0($t1)             # load
	sb $t3, 0($t0)              # write
	addi $t0, $t0, 1
	addi $t1, $t1, 1
	beq $t3, $zero, _strncpy_return	# Return if null terminator
	j _strncpy_loop
_strncpy_overflow:
	addi $t2, $t2, -1
	sb $zero, 0($t2)
_strncpy_return:
	sub $v0, $t0, $a0
	jr $ra

###########
# Function : strcmp
# Signature : int strcmp(char *first, char *second)
# Return : Compare two strings and return 0 if they are the same, <0 if the first is less than the second, >0 if vice versa. 
.text
.globl strcmp
strcmp:
	# $a0 - Address of first string.
	# $a1 - Address of second string.
	# Returns $v0 - Value of comparison.
	
    lb $t0, 0($a0)
	lb $t1, 0($a1)
	bne $t0, $t1, _strcmp_ne
	bne $t0, $zero, _strcmp_eq_contd  # $t0 == $t1: 
	move $v0, $zero             # $t0 == $t1 == '\0': 
	jr $ra
_strcmp_eq_contd:
	addi $a0, $a0, 1            # $t0 == $t1 != '\0': 
	addi $a1, $a1, 1
	j strcmp
_strcmp_ne:
	sub $v0, $t0, $t1           # $t0 != $t1: 
	jr $ra

###########
# Function : strncmp
# Signature : int strncmp(char *first, char *second, int n)
# Returns : Compare first n bytes of two strings and return 0 if they are the same, <0 if the first is less than the second, >0 if vice versa 
.text
.globl strncpy
strncmp:
	# $a0 - Address of first string.
	# $a1 - Address of second string.
	# $a2 - Number of bytes to compare.
	# Returns $v0 - Value of comparison.
	lb $t0, 0($a0)
	lb $t1, 0($a1)
	beq $a2, $zero, _strncmp_end
	addi $a2, $a2, -1
	bne $t0, $t1, _strncmp_ne
	bne $t0, $zero, _strncmp_eq_continue
_strncmp_end:
	move $v0, $zero
	jr $ra
_strncmp_eq_continue:
	addi $a0, $a0, 1
	addi $a1, $a1, 1
	j strncmp
_strncmp_ne:
	sub $v0, $t0, $t1
	jr $ra

###########
# Function : memcpy
# Signature : void memcpy(char *dest, char *src, int num)
# Description : memcpy: Move num bytes from a source buffer to destination buffer.
.text
.globl memcpy
memcpy:
	# $a0 - Address of destination buffer.
	# $a1 - Address of source buffer.
	# $a2 - Number of bytes to write.
	beq $a2, $zero, _memcpy_return
	lbu $t0, 0($a1)
	sb $t0, 0($a0)
	addi $a0, $a0, 1
	addi $a1, $a1, 1
	addi $a2, $a2, -1
	j memcpy
_memcpy_return:
	jr $ra

###########
# Function : memset
# Signature : void memset(char *buf, char b, int n)
# Description : Fill a chunk of memory with a byte.
.text
.globl memset
memset:
	# $a0 - Pointer to buffer to write.
	# $a1 - Byte to fill.
	# $a2: Amount of bytes to write.
	beq $a2, $zero, _memset_ret
	sb $a1, 0($a0)
	addi $a0, $a0, 1
	addi $a2, $a2, -1
	j memset
_memset_ret:
	jr $ra

###########
# Function : atoi
# Signature : int atoi(char *str)
# Returns : Integer value of string
.text
.globl atoi
atoi:
	# $a0 - Address of null-terminated decimal string.
	# Returns $v0 - Value of string.
	move $v0, $zero
	li $t0, 10
_atoi_loop:
	lbu $t1, 0($a0)		        # load character
	beq $t1, $zero, _atoi_ret   # stop at null terminator
	mult $v0, $t0		        # move up result by one decimal place
	mflo $v0
    addi $t2, $zero, '0'        # subi $t1, $t1, '0'
    sub $t1, $t1, $t2           # get character value
	add $v0, $v0, $t1		    # add character value to result
	addi $a0, $a0, 1		    # increment string pointer
	j _atoi_loop
_atoi_ret:
	jr $ra

###########
# Function : htoi
# Description : Convert hexadecimal string to integer
# Signature : int htoi(char *str)
.text
.globl htoi
htoi:
	# $a0 - Address of null-terminated, positive hex string
	# Returns $v0 - Value of string.
	# XXX: Suports upper&lower case. No error handling, or whitespace. String must be positive
	move $v0, $zero
	li	$t1, 'a'
	li	$t2, 'A'
_htoi_loop:
	lbu $t0, 0($a0)
	addi $a0, $a0, 1
	beq $t0, $zero, _htoi_ret
	sll $v0, $v0, 4
	bge $t0, $t1, _htoi_lc
	bge $t0, $t2, _htoi_UC
    addi $t3, $zero, '0'        # subi $t0, $t0, '0'
    sub $t0, $t0, $t3           # get character value
	add $v0, $v0, $t0
	j _htoi_loop
_htoi_UC:
	addi $t0, $t0, -55
	add $v0, $v0, $t0
	j _htoi_loop
_htoi_lc:
	addi $t0, $t0, -87
	add $v0, $v0, $t0
	j _htoi_loop
_htoi_ret:
	jr	$ra


###########
# Function : str_append
# Signature : void str_append(char *prefix, char *suffix, char *dest)
# Description : Append two strings into a single buffer.
.text
.globl str_append
str_append:
	# $a0 - The address of the prefix string (null-terminated).
	# $a1 - The address of the suffix string (null-terminated).
	# $a2 - The address of the destination buffer
	# XXX NOTE:
	# $a0 may be the same address as $a2.
	# $a0 may be the same address as $a1.
	# $a1 MAY NOT be the same address as $a2.
	# No other overlap between the buffers is allowed.
	sub $sp, $sp, 16    
    sw $ra, 12($sp) # push($ra): Save for later
    sw $a0, 8($sp)  # push($a0): Save for first strlen call.
	sw $a1, 4($sp)  # push($a1)
	sw $a2, 0($sp)  # push($a2)
	jal strlen      # Get length of the prefix string.
	sub $sp, $sp, 4
    sw $v0, 0($sp)  # push($v0)	: Save the length of the prefix string

	# If $a0 == $a2, we only copy suffix
	beq $a0, $a2, _str_append_copy_suffix

	# Copy $a0 into $a2
    lw $a2, 0($sp)
	addi $sp, $sp, 4 # pop($a2)	    
    lw $a0, 0($sp)
	addi $sp, $sp, 4 # pop($a0)	    
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)	    
    lw $a1, 0($sp)
	addi $sp, $sp, 4 # pop($a1)	    
	sub $sp, $sp, 16
    sw $a1, 12($sp)  # push($a1)	    
	sw $t0, 8($sp)   # push($t0)
	sw $a0, 4($sp)   # push($a0)
	sw $a2, 0($sp)   # push($a2)
	addi $a2, $a2, 1	# strncpy needs to include the null byte
	jal strncpy
_str_append_copy_suffix:
	# Increment the destination buffer to point to next open character.
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)	        
    lw $a2, 0($sp)
	addi $sp, $sp, 4 # pop($a2)	        
    lw $a1, 0($sp)
	addi $sp, $sp, 4 # pop($a1)	        
    lw $a0, 0($sp)
	addi $sp, $sp, 4 # pop($a0)	
	add $a2, $a2, $t0
	sub $sp, $sp, 12
    sw $a0, 8($sp)   # push($a0)	
    sw $a1, 4($sp)   # push($a1)	
	sw $a2, 0($sp)   # push($a2)	

	# Get the length of the suffix string
	move $a0, $a1
	jal strlen

	# strncpy from the suffix to the destination.
	move $a2, $v0
    lw $a0, 0($sp)
	addi $sp, $sp, 4 # pop($a0)
    lw $a1, 0($sp)
    addi $sp, $sp, 4 # pop($a1)
	sub $sp, $sp, 8
    sw $a1, 4($sp)   # push($a1)
	sw $a0, 0($sp)   # push($a0)
	addi $a2, $a2, 1	# strncpy needs to include the null byte
	jal strncpy

	# Finally, we can pop everything out and return.
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $ra, 0($sp)
	addi $sp, $sp, 4 # pop($ra)
	jr $ra

###########
# Function : str_nappend
# Signature : void str_nappend(char *prefix, char *suffix, char *dest, int n)
# Description : Append two strings into a single buffer.
.text
.globl str_nappend
str_nappend:
	# $a0: The address of the prefix string (null-terminated).
	# $a1: The address of the suffix string (null-terminated).
	# $a2: The address of the destination buffer.
	# $a3: The maximum number of bytes to write to $a2.
	# XXX NOTE:
	# $a0 may be the same address as $a2.
	# $a0 may be the same address as $a1.
	# $a1 MAY NOT be the same address as $a2.
	# No other overlap between the buffers is allowed.
    sub $sp, $sp, 20
	sw $ra, 16($sp) # push($ra) : Save for later

	sw $a0, 12($sp) # push($a0)	: Save the parameters for first strlen call.
    sw $a1, 8($sp)  # push($a1)
	sw $a2, 4($sp)  # push($a2)
	sw $a3, 0($sp)  # push($a3)
	jal strlen	# Get the length of the prefix string.
	sub $sp, $sp, 4
    sw $v0, 0($sp)  # push($v0)	: Save the length of the prefix string

	# If $a0 == $a2, we skip prefix copy
	beq $a0, $a2, _str_nappend_copy_suffix

	# Copy $a0 into $a2
    lw $a2, 0($sp)
	addi $sp, $sp, 4 # pop($a2)	: saved prefix length
    lw $a3, 0($sp)
	addi $sp, $sp, 4 # pop($a3)	: N bytes in dest buffer
    lw $a0, 0($sp)
	addi $sp, $sp, 4 # pop($a0)	: destination buffer
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)	: Save the suffix string real quick.
    lw $a1, 0($sp)
	addi $sp, $sp, 4 # pop($a1)	: source buffer
	sub $sp, $sp, 20
    sw $a1, 16($sp)  # push($a1)	
	sw $t0, 12($sp)  # push($t0)
	sw $a0, 8($sp)   # push($a0)
	sw $a3, 4($sp)   # push($a3)
	sw $a2, 0($sp)   # push($a2)

	addi $a2, $a2, 1	# strncpy needs to include the nul byte

	# If dest buffer has fewer bytes than source we return.
	bgt $a2, $a3, _str_nappend_early_return
	jal strncpy
_str_nappend_copy_suffix:
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)	
    lw $a3, 0($sp)
	addi $sp, $sp, 4 # pop($a3)	
    lw $a2, 0($sp)
	addi $sp, $sp, 4 # pop($a2)	
    lw $a1, 0($sp)
	addi $sp, $sp, 4 # pop($a1)	
    lw $a0, 0($sp)
	addi $sp, $sp, 4 # pop($a0)	
	add $a2, $a2, $t0       # Increment the destination buffer to point to next open character. 
	
	sub $a3, $a3, $t0       # Decrement the destination buffer bytes remaining. 
	sub $sp, $sp, 16
    sw $a0, 12($sp) # push($a0)	
	sw $a1, 8($sp)  # push($a1)	
	sw $a2, 4($sp)  # push($a2)	
	sw $a3, 0($sp)  # push($a3)	

	# Get the length of the suffix string
	move $a0, $a1
	jal strlen

	# Now, use that length for strncpy from the suffix to the destination.
	move $a2, $v0
    lw $a3, 0($sp)
	addi $sp, $sp, 4 # pop($a3)
    lw $a0, 0($sp)
	addi $sp, $sp, 4 # pop($a0)
    lw $a1, 0($sp)
	addi $sp, $sp, 4 # pop($a1)
	sub $sp, $sp, 12
    sw $a1, 8($sp)   # push($a1)
	sw $a0, 4($sp)   # push($a0)
	sw $a3, 0($sp)   # push($a3)
	addi $a2, $a2, 1	# strncpy needs to include the nul byte

	# If the dest buffer has fewer bytes than source, early return
	bgt $a2, $a3, _str_nappend_early_return2
	jal strncpy

	# Finally, we can pop everything out and return.
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $ra, 0($sp)
	addi $sp, $sp, 4 # pop($ra)
	jr $ra

_str_nappend_early_return:
	# First string bigger than buffer.
	move $a2, $a3
	jal strncpy
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
	j _str_nappend_return
_str_nappend_early_return2:
	# Second string bigger than buffer.
	move $a2, $a3
	jal strncpy
_str_nappend_return:
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $t0, 0($sp)
	addi $sp, $sp, 4 # pop($t0)
    lw $ra, 0($sp)
	addi $sp, $sp, 4 # pop($ra)
	jr $ra

