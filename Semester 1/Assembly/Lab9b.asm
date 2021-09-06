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
	mov ah, 3Fh ;Ввод строки
	lea dx, s
	mov cx, 12
	int 21h

	mov bx, 0
	cbw 
b:
	cmp bx, ax;сравниваем bx с длиной
	je ex ;Если дошли до конца, выход
	inc bx ;Увеличиваем счётчик

	cmp s[bx], "*" ;Сравниваем с "*"
	jne b ;Прыгаем в начало цикла пока не найдём

ex:
	mov s[bx], "#" ;Заменяем "*" на "#"

	mov ah, 9;Вывод результата
	lea dx, s
	int 21h
mov ah, 4Ch 
int 21h
end 