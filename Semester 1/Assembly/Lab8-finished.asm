.model small
.stack
.data
	y dw 0
	num dw 3
	len db, ?
	dn dw 10
	newLine db 10, 13, '$'
.code

NL macro
	mov ah, 9
	lea dx, newLine
	int 21h
endm
PrintNum macro var
	local toStack
	local output

	mov ax, 0
	mov dx, 0
	mov bx, 0

	mov len, 0
    mov ax, var
	toStack: 
	inc len

	mov dx, 0
	div dn
	add dx, 30h
	
	cmp ax, 0
	push dx
	jne toStack

	output:	
	mov ah, 2
	pop dx
	int 21h

	dec len
	cmp len, 0
	jne output 
	
	NL
endm

mov ax, @data 
mov ds, ax	 
	mov cx, 17
mark:
	mov ax, cx
	mov dx, 0
	div num
	add y, dx

	cmp cx, 5
	loopne mark

	PrintNum y
mov ah, 4Ch 
int 21h
end 