.model small
.stack
.data
	s db 12 dup ("$");Строка для ввода
	
	newLine db 10, 13, '$'
.code
NL macro ;Макрос перехода на новую строку
	mov ah, 9
	lea dx, newLine
	int 21h
endm

mov ax, @data 
mov ds, ax	 
	mov es, ax ;инициализация сегмента es

	mov ah, 3Fh ;Ввод числа
	lea dx, s
	mov cx, 12
	int 21h
	
	cld ;ZF = 0
		;То есть слева направо
	lea di, s ;Передаем адрес s
			  ;регистру di
	mov al, '*' ;Помещаем "*" в al
	
	repnz scasb ;Пвоторяем пока не найдём символ "*"
	
	;mov ah, 2
	;mov dl, [di-1]
	;int 21h
	
	lea ax, s ;Получаем ссылку на s
	sub di, ax ;Вычитаем адрес s
			   ;из найденного адреса
	mov s[di-1], "#" ;Замена "*" на "#"

	mov ah, 9 ;Вывод
	lea dx, s
	int 21h

	;NL
	
	;mov ah, 2
	;mov dx, di
	;add dx, 30h
	;int 21h
	
mov ah, 4Ch 
int 21h
end 