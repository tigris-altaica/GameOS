#include "screen.h"
#include "types.h"
#include "io.h"
#include "tetris.h"

#define IDT_TYPE_INTR (0x0E)
// Селектор секции кода, установленный загрузчиком ОС
#define GDT_CS (0x8)
#define PIC1_PORT (0x20)

void keyb_handler();
void keyb_process_keys();
void on_key(int scan_code);

struct idt_entry
{     
    unsigned short base_lo;    // Младшие биты адреса обработчика     
    unsigned short segm_sel;   // Селектор сегмента кода     
    unsigned char always0;     // Этот байт всегда 0     
    unsigned char flags;       // Флаги тип. Флаги: P, DPL, Типы - это константы - IDT_TYPE...     
    unsigned short base_hi;    // Старшие биты адреса обработчика 
} __attribute__((packed)); // Выравнивание запрещено 
 
// Структура, адрес которой передается как аргумент команды lidt 
struct idt_ptr 
{ 
    unsigned short limit;     
    unsigned int base; 
} __attribute__((packed)); // Выравнивание запрещено 
 
struct idt_entry g_idt[256]; // Реальная таблица IDT 
struct idt_ptr g_idtp;       // Описатель таблицы для команды lidt
typedef void (*intr_handler)();  
// Пустой обработчик прерываний. Другие обработчики могут быть реализованы по этому шаблону 
void default_intr_handler() 
{  
    asm("pusha");  
    // ... (реализация обработки)  
    asm("popa; leave; iret"); 
} 
 
void intr_reg_handler(int num, unsigned short segm_sel, unsigned short flags, intr_handler hndlr) 
{  
    unsigned int hndlr_addr = (unsigned int) hndlr; 
    g_idt[num].base_lo = (unsigned short) (hndlr_addr & 0xFFFF);  
    g_idt[num].segm_sel = segm_sel;  
    g_idt[num].always0 = 0;  
    g_idt[num].flags = flags;  
    g_idt[num].base_hi = (unsigned short) (hndlr_addr >> 16); 
} 
 
// Функция инициализации системы прерываний: заполнение массива с адресами обработчиков 
void intr_init() 
{     
    int i;     
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]); 
 
    for(i = 0; i < idt_count; i++)         
        intr_reg_handler(i, GDT_CS, 0x80 | IDT_TYPE_INTR, default_intr_handler); // segm_sel=0x8, P=1, DPL=0, Type=Intr 
} 
 
void intr_start() 
{  
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]); 
 
    g_idtp.base = (unsigned int) (&g_idt[0]);  
    g_idtp.limit = (sizeof (struct idt_entry) * idt_count) - 1; 
 
    asm("lidt %0" : : "m" (g_idtp) ); 
} 
 
void intr_enable() {  
    asm("sti"); 
} 
 
void intr_disable() {  
    asm("cli"); 
} 

void keyb_init() 
{ 
    // Регистрация обработчика прерывания  
    intr_reg_handler(0x09, GDT_CS, 0x80 | IDT_TYPE_INTR, keyb_handler); // segm_sel=0x8, P=1, DPL=0, Type=Intr  
    // Разрешение только прерываний клавиатуры от контроллера 8259  
    out8(PIC1_PORT + 1, 0xFF ^ 0x02); // 0xFF - все прерывания, 0x02 - бит IRQ1 (клавиатура).  
    // Разрешены будут только прерывания, чьи биты установлены в 0 
} 

void keyb_handler() 
{  
    asm("pusha"); 
 // Обработка поступивших данных  
    keyb_process_keys();  
 // Отправка контроллеру 8259 нотификации о том, что прерывание обработано   
    out8(PIC1_PORT, 0x20);   
    asm("popa; leave; iret"); 
} 
 
void keyb_process_keys() 
{
    // Проверка что буфер PS/2 клавиатуры не пуст (младший бит присутствует)
    u8 data;
    in8(0x64, &data);
    if (data & 0x01)  
    {   
        unsigned char scan_code;   
        in8(0x60, &data);
        scan_code = data;// Считывание символа с PS/2 клавиатуры 
        if (scan_code < 128)  
            on_key(scan_code);  
    } 
}

unsigned char * memcpy (unsigned char *dst_, const unsigned char *src_, size_t n) 
{
  unsigned char *dst = dst_;
  const unsigned char *src = src_;
  while (n--)
    *dst++ = *src++;
  return dst_;
}

unsigned int rand()
{
    static unsigned int x0 = 100;
    unsigned int a = 84589, c = 45989, m = 217728;

    unsigned int x = (a * x0 + c) % m;
    x0 = x;

    return x; 
}

void on_key(int scan_code)
{
    switch (scan_code) {
    case 75: // LeftArrow
        MoveLeft();
        break;
    case 77: // RightArrow
        MoveRight();
        break;
    case 80: // DownArrow
        MoveDown();
        break;
    case 57: // Space
        MoveDownToBottom();
        break;
    case 72: // UpArrow
        RotateFigure();
        break;
    case 19: // R
        Start();
        break;
    case 1: // ESC
        Exit();
        break;
    default:
    }

}

int main(void)
{   
    intr_init();
    intr_start();
    intr_enable();
    keyb_init();

    cursor_moveto(MAX_ROW, MAX_COL);
    
    PlayTetris();
}

