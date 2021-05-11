	.text
	.globl print_int
print_int:
	li $v0, 1
	syscall
	jr $ra
	
	.text
	.globl print_char
print_char:
	li $v0, 11
	syscall
	jr $ra

	.text
	.globl scan_int 
scan_int:
	li $v0, 5
	syscall
	jr $ra
	
	.text
	.globl exit 
exit:
	li $v0, 10
	syscall
	jr $ra
