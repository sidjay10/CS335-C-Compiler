	.text
	.globl print_int
print_int:
	li $v0, 1
	syscall
	jr $ra
	
	.text
	.globl scan_int 
scan_int:
	li $v0, 5
	syscall
	jr $ra

	.text
	.globl print_char
print_char:
	li $v0, 11
	syscall
	jr $ra

	.text
	.globl scan_char
scan_char:
	li $v0, 12
	syscall
	jr $ra
	
	.text
	.globl print_string
print_string:
	li $v0, 4
	syscall
	jr $ra
	
	.text
	.globl scan_string
scan_string:
	li $v0, 8
	syscall
	jr $ra
