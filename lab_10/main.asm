bits 64
global main

%define GTK_WIN_POS_CENTER 1
%define GTK_WIN_WIDTH 700
%define GTK_WIN_HEIGHT 450

extern printf
extern atoi
extern sscanf
extern sprintf
extern find_root

extern exit
extern gtk_init
extern gtk_main
extern g_object_set
extern gtk_main_quit
extern gtk_window_new
extern gtk_widget_show
extern g_signal_connect
extern gtk_window_set_title
extern g_signal_connect_data
extern gtk_window_set_position
extern gtk_settings_get_default
extern gtk_widget_set_size_request
extern gtk_window_set_default_size
extern gtk_entry_new
extern gtk_button_new_with_label
extern gtk_label_new
extern gtk_label_set_text
extern gtk_widget_set_halign
extern gtk_entry_set_text
extern gtk_widget_show_all
extern gtk_container_add
extern gtk_box_new
extern gtk_orientable_set_orientation
extern gtk_box_set_spacing
extern gtk_entry_get_text
extern g_ascii_strtod
extern g_strdup_printf
extern g_free

section .bss
window: resq 1
label_start: resq 1 
label_end: resq 1
label_iterations: resq 1
entry_start: resq 1
entry_end: resq 1
entry_iterations: resq 1
label_result: resq 1
output_result: resq 1
button: resq 1
box: resq 1

section .data
str_result: times 100 db 0
result: dq 0
str_ptr: dq 0

section .rodata
signal:
.clicked: db "clicked", 0
.destroy: db "destroy", 0

title: db "Root search by the chord method.", 0
label_start_text: db "Enter start of the segment:", 0
label_end_text: db "Enter end of the segment:", 0
label_iterations_text: db "Enter number of iterations:", 0
label_result_text: db "Result:", 0
button_text: db "Find root", 0
double_format: db "%lf", 0
int_format: db "%d", 0

section .text
_destroy_window:
    jmp gtk_main_quit
main:
    push rbp
    mov rbp, rsp
    xor rdi, rdi
    xor rsi, rsi
    call gtk_init
    xor rdi, rdi
    call gtk_window_new
    mov qword [ rel window ], rax
    mov rdi, qword [ rel window ]
    mov rsi, title
    call gtk_window_set_title
    mov rdi, qword [ rel window ]
    mov rsi, GTK_WIN_WIDTH
    mov rdx, GTK_WIN_HEIGHT
    call gtk_window_set_default_size
    mov rdi, qword [ rel window ]
    mov rsi, GTK_WIN_POS_CENTER
    call gtk_window_set_position

    ; Создание контейнера
    call gtk_box_new
    mov qword [rel box], rax

    mov rdi, qword [rel box]
    mov rsi, 1
    call gtk_orientable_set_orientation

    mov rdi, qword [rel window]
    mov rsi, qword [rel box]
    call gtk_container_add

    ; Создание лэйблов и полей ввода
    call gtk_entry_new
    mov qword [rel entry_start], rax

    call gtk_entry_new
    mov qword [rel entry_end], rax

    call gtk_entry_new
    mov qword [rel entry_iterations], rax
   
    call gtk_label_new
    mov qword [rel label_start], rax

    call gtk_label_new
    mov qword [rel label_end], rax

    call gtk_label_new
    mov qword [rel label_iterations], rax

    call gtk_label_new
    mov qword [rel label_result], rax

    call gtk_label_new
    mov qword [rel output_result], rax

    ; Добавление текста в лэйблы
    mov rdi, qword [rel label_start]
    mov rsi, label_start_text
    call gtk_label_set_text
    mov rdi, qword [rel label_start]
    mov rsi, 1
    call gtk_widget_set_halign
    
    mov rdi, qword [rel label_end]
    mov rsi, label_end_text
    call gtk_label_set_text
    mov rdi, qword [rel label_end]
    mov rsi, 1
    call gtk_widget_set_halign

    mov rdi, qword [rel label_iterations]
    mov rsi, label_iterations_text
    call gtk_label_set_text
    mov rdi, qword [rel label_iterations]
    mov rsi, 1
    call gtk_widget_set_halign

    mov rdi, qword [rel label_result]
    mov rsi, label_result_text
    call gtk_label_set_text
    mov rdi, qword [rel label_result]
    mov rsi, 1
    call gtk_widget_set_halign

    mov rdi, qword [rel output_result]
    mov rsi, str_result
    call gtk_label_set_text
    mov rdi, qword [rel output_result]
    mov rsi, 1
    call gtk_widget_set_halign

    ; Создание кнопки
    mov rdi, button_text
    call gtk_button_new_with_label
    mov qword [rel button], rax

    ; Добавление виджетов в контейнер
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel label_start ]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel entry_start ]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel label_end ]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel entry_end ]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel label_iterations ]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel entry_iterations]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel label_result]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel output_result]
    call gtk_container_add
    mov rdi, qword [ rel box ]
    mov rsi, qword [ rel button]
    call gtk_container_add

    ; Изменение расстояния между виджетами
    mov rdi, qword [rel box]
    mov rsi, 10
    call gtk_box_set_spacing

    ; Установка обработчика нажатия кнопки
    mov rdi, qword [rel button]
    mov rsi, signal.clicked
    mov rdx, button_clicked_handler
    xor rcx, rcx
    xor r8d, r8d
    xor r9d, r9d
    call g_signal_connect_data

    mov rdi, qword [ rel window ]
    mov rsi, signal.destroy
    mov rdx, gtk_main_quit
    xor rcx, rcx
    xor r8d, r8d
    xor r9d, r9d
    call g_signal_connect_data
    mov rdi, qword [ rel window ]
    call gtk_widget_show_all
    call gtk_main
    ret

button_clicked_handler:
    ; Обработчик события нажатия кнопки
    ; Считываем данные из полей ввода
    mov rdi, [entry_start]
    call gtk_entry_get_text
    mov rdi, rax
    xor rsi, rsi
    call g_ascii_strtod
    movsd xmm0, [rsp]

    mov rdi, [entry_end]
    call gtk_entry_get_text
    mov rdi, rax
    xor rsi, rsi
    call g_ascii_strtod
    movsd xmm1, [rsp]

    mov rdi, [entry_iterations]
    call gtk_entry_get_text
    call convert_to_int
    mov rdi, rax

    mov rsi, [result]

    call find_root

    ; mov rdi, qword [double_format]
    ; movsd xmm0, [result]
    ; mov rax, 1
    ; call g_strdup_printf
    ; mov [str_ptr], rax
    call convert_to_str

    mov rdi, qword [rel output_result]
    mov rsi, [str_result]
    call gtk_label_set_text
    ; mov rdi, [str_ptr]
    ; call g_free
    ret


; convert_to_double:
;     sub rsp, 8 
;     mov rdi, rax
;     mov rsi, double_format
;     lea rdx, [result]
;     mov rax, 0 
;     call sscanf
;     add rsp, 8 
;     ret

convert_to_int:
    sub rsp, 8
    mov rdi, rax
    call atoi
    add rsp, 8
    ret

convert_to_str: 
    sub rsp, 8
    lea rdi, [str_result]
    lea rsi, [double_format]
    movsd xmm0, qword [result]
    mov rax, 1
    call sprintf
    add rsp, 8
    ret
    
fuck:
    mov rdi, qword [rel output_result]
    mov rsi, title
    call gtk_label_set_text