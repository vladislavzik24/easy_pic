// #############################################################################
//
// Название     : CLCD_Screen_v05, основной файл библиотеки.
// Назначение   : Библиотека для работы с графическим 64х128 LCD (WDG0151 или аналогами)
// Платформа    : Отладочная плата EasyPIC 6
// Зависимости  : Функции Delay_us(), Delay_ms() среды microC PRO for PIC
//                Библиотеки mikroE FLASH и mikroE C_Math среды microC PRO for PIC
//                Файл настроек CLGD_Screen_Settings_v04.h
//
// Автор        : Цурко А.В. (Ratgor), Ращинский П.Н.
// Рефакторинг        : А.В. Цурко (Ratgor) [не проводился]
//
// #############################################################################
//
// Описание:
//
//          Модуль представляет собой _библиотеку_,
//          позволяющую рисовать точки, фигуры и текст на графическом жк-дисплее
//
//          Содержание модуля:
//          1) Рисование точки по координатам
//          2) Рисование байта (8 соседних по вертикали точек) по координатам
//          3) Рисование символа (латиница, кириллица, цифры, знаки препинания)
//          4) Рисование строки символов
//          5) рисование прямой линии
//          6) Прямоугольник пустой
//          7) Прямоугольник заполненый
//          8) Круг пустой
//          9) Круг заполненный
//          10) Пустой экран
//          11) Заполненный экран
//          12) Горизонтальный сдвиг экрана
//          13) Преобразование числа в символ (дополнительная функция)
//          14) Задержка в тактах (функция для организации внутренних задержек)
//
// Примечания:
//
//          1) Является упрощенной (без сенсорной панели)
//             и портированной (AVR -> PIC) версией из другого проекта
//          2) Функции рисования фигур не тестировались после портирования,
//             просто подобраны аналоги из C_Math библиотеки
//          3) Версия 0.3 обновлена до 0.4 после оптимизации функции рисования
//             линии. Версия 0.4 -> 0.3с содержала урезанный набор символов
//          4) Версия 0.4 обновлена до 0.5 после добавления функции сдвига
//             экрана. Версия 0.5 -> 0.4с содержала урезанный набор символов
//
// #############################################################################
//
// -----------------------------------------------------------------------------
// Функция рисования пикселя:
// -----------------------------------------------------------------------------
// void GLCD_Draw_Pixel (unsigned char X_pos, unsigned char Y_pos, unsigned char color)
// Параметры функции:
// Координата по x (слева направо: 0-127)
// Координата по y (сверху вниз: 0-63)
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования байта (на дисплее по y: 8 строк по 8 пикселей):
// -----------------------------------------------------------------------------
// void GLCD_Draw_Byte (unsigned char X_pos, unsigned char Y_pos, unsigned char data_byte, unsigned char color)
// Параметры функции:
// Координата по x (слева направо: 0-127)
// Координата по y, номер строки (сверху вниз: 0-7)
// Маска заливки, прямой цвет применяется для логических '1', обратный для логических '0'
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования символа (8 строк по 18 символов):
// -----------------------------------------------------------------------------
// void GLCD_Draw_Char (unsigned char X_pos, unsigned char Y_pos, unsigned char data_char, unsigned char color)
// Параметры функции:
// Координата по x (слева направо: 0-127)
// Координата по y, номер строки (сверху вниз: 0-7)
// Рисуемый символ 5x7 в ячейке 7x8, поддерживаются: A-z, А-я, 0-9, .,!?:;" и т.д.
// Цвет пикселя: 0-черный, 1-белый
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования строки (8 строк по 18 символов):
// -----------------------------------------------------------------------------
// void GLCD_Draw_String (unsigned char X_pos, unsigned char Y_pos, char *data_string, unsigned char color)
// Параметры функции:
// Координата по x (слева направо: 0-127)
// Координата по y, номер строки (сверху вниз: 0-7)
// Рисуемая строка символов, указатель на первый элемент тесктового массива длинной до 255 символов (символы - см. выше)
// Цвет пикселя: 0-черный, 1-белый
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования линии:
// -----------------------------------------------------------------------------
// void GLCD_Draw_Line(unsigned char X0_pos, unsigned char Y0_pos, unsigned char X1_pos,unsigned char Y1_pos, unsigned char coolor)                         
// Параметры функции:
// Координата первой точки по x (слева направо: 0-127)
// Координата первой точки по y (сверху вниз: 0-63)
// Координата второй точки по x (слева направо: 0-127)
// Координата второй точки по y (сверху вниз: 0-63)
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования пустого прямоугольника:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Rectangle(unsigned char X0_pos, unsigned char Y0_pos, unsigned char X1_pos, unsigned char Y1_pos, unsigned char coolor)
// Параметры функции:
// Координата верхнего левого угла по x (слева направо: 0-127)
// Координата верхнего левого угла по y (сверху вниз: 0-63)
// Координата нижнего правого угла по x (слева направо: 0-127)
// Координата нижнего правого угла по y (сверху вниз: 0-63)
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования заполненного прямоугольника:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Rectangle_Fill(unsigned char X0_pos, unsigned char Y0_pos, unsigned char X1_pos, unsigned char Y1_pos, unsigned char coolor)
// Параметры функции:
// Координата верхнего левого угла по x (слева направо: 0-127)
// Координата верхнего левого угла по y (сверху вниз: 0-63)
// Координата нижнего правого угла по x (слева направо: 0-127)
// Координата нижнего правого угла по y (сверху вниз: 0-63)
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования пустой окружности:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Circle(unsigned char X0_pos, unsigned char Y0_pos, unsigned char Radius, unsigned char coolor)
// Параметры функции:
// Координата центра окружности по x (слева направо: 0-127)
// Координата центра окружности по y (сверху вниз: 0-63)
// Кадиус окружности (не должен привышать координату центра по x)
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция рисования заполненной окружности:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Circle_Fill(unsigned char X0_pos, unsigned char Y0_pos, unsigned char Radius, unsigned char coolor)
// Параметры функции:
// Координата центра окружности по x (слева направо: 0-127)
// Координата центра окружности по y (сверху вниз: 0-63)
// Кадиус окружности (не должен привышать координату центра по x)
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция очистки экрана (рисование черного экрана):
// -----------------------------------------------------------------------------
// void GLCD_Clear()
// Параметры функции:
// Нет параметров.
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция очистки экрана (рисование белого экрана):
// -----------------------------------------------------------------------------
// void GLCD_Fill()
// Параметры функции:
// Нет параметров.
// Цвет пикселя: 0-черный, 1-белый, 2-инвертировать
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция сдвига экрана:
// -----------------------------------------------------------------------------
// void GLCD_Move_Screen (signed char step)
// Параметры функции:
// step: шаг, на который будет сдвинут экран вправо, в пикселях (-128..127)
// Функция возвращает ничего.
//
// -----------------------------------------------------------------------------
// Функция преобразования числа в символ:
// -----------------------------------------------------------------------------
// unsigned char numPosCode (unsigned char number, unsigned char position, unsigned char radix)
// Параметры функции:
// Число для преобразования (беззнаковое: 0-255)
// Позиция преобразуемой цифоры в числе (0-max, зависит от системы счисления)
// Система счисления (например: 10-десятичная, 16-шестнадцатеричная)
// Функция возвращает ASCII код цифры в числе на определённой позиции.
//
// -----------------------------------------------------------------------------
// Функция задержки в выполняемой программе:
// -----------------------------------------------------------------------------
// void wait(int tact)
// Параметры функции:
// Количиство тактов задержки (0-65535)
// Функция возвращает ничего.
//
// #############################################################################



/*##############################################################*/
/* begin of TUNEABLE part of the library                        */
/*##############################################################*/

/*
#define GLCD_PORT_D7    PORTD
#define GLCD_PORT_D6    PORTD
#define GLCD_PORT_D5    PORTD
#define GLCD_PORT_D4    PORTD
#define GLCD_PORT_D3    PORTD
#define GLCD_PORT_D2    PORTD
#define GLCD_PORT_D1    PORTD
#define GLCD_PORT_D0    PORTD
#define GLCD_PORT_CS1   PORTB
#define GLCD_PORT_CS2   PORTB
#define GLCD_PORT_RS    PORTB
#define GLCD_PORT_RW    PORTB
#define GLCD_PORT_E     PORTB
#define GLCD_PORT_RST   PORTB

#define GLCD_DDR_D7     TRISD
#define GLCD_DDR_D6     TRISD
#define GLCD_DDR_D5     TRISD
#define GLCD_DDR_D4     TRISD
#define GLCD_DDR_D3     TRISD
#define GLCD_DDR_D2     TRISD
#define GLCD_DDR_D1     TRISD
#define GLCD_DDR_D0     TRISD
#define GLCD_DDR_CS1    TRISB
#define GLCD_DDR_CS2    TRISB
#define GLCD_DDR_RS     TRISB
#define GLCD_DDR_RW     TRISB
#define GLCD_DDR_E      TRISB
#define GLCD_DDR_RST    TRISB

#define GLCD_PIN_D7     PORTD
#define GLCD_PIN_D6     PORTD
#define GLCD_PIN_D5     PORTD
#define GLCD_PIN_D4     PORTD
#define GLCD_PIN_D3     PORTD
#define GLCD_PIN_D2     PORTD
#define GLCD_PIN_D1     PORTD
#define GLCD_PIN_D0     PORTD
#define GLCD_PIN_CS1    PORTB
#define GLCD_PIN_CS2    PORTB
#define GLCD_PIN_RS     PORTB
#define GLCD_PIN_RW     PORTB
#define GLCD_PIN_E      PORTB
#define GLCD_PIN_RST    PORTB

#define GLCD_BIT_D7     7
#define GLCD_BIT_D6     6
#define GLCD_BIT_D5     5
#define GLCD_BIT_D4     4
#define GLCD_BIT_D3     3
#define GLCD_BIT_D2     2
#define GLCD_BIT_D1     1
#define GLCD_BIT_D0     0
#define GLCD_BIT_CS1    0
#define GLCD_BIT_CS2    1
#define GLCD_BIT_RS     2
#define GLCD_BIT_RW     3
#define GLCD_BIT_E      4
#define GLCD_BIT_RST    5
*/

/* GLCD_DELAY; is macros for                */
/* set default delays in tacts              */
/* in functions GLCD_Draw_...               */
/* #define GLCD_DELAY 1                     */

/*##############################################################*/
/* end of TUNEABLE part of the library                          */
/*##############################################################*/

const char FONT_5x7_WIN1251 [256][5] =
{ // 0b00000000,0b00000000,... = left->right,down=>up 
  // thanks for:
/* http: //www.hwsw.no/snippets/5x7_LCD_font.php http: //electronix.ru/forum/lofiversion/index.php/t44983.html*/
                
    {0x00, 0x00, 0x00, 0x00, 0x00}, //0x00 .. 0x20-1
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
     
    {0x00, 0x00, 0x00, 0x00, 0x00}, // SPACE 0x20
     
    {0x00, 0x00 ,0x4F ,0x00 ,0x00}, // !
    {0x00, 0x07 ,0x00 ,0x07 ,0x00}, // "
    {0x14, 0x7F ,0x14 ,0x7F ,0x14}, // #
    {0x24, 0x2A ,0x7F ,0x2A ,0x12}, // $
    {0x23, 0x13 ,0x08 ,0x64 ,0x62}, // %
    {0x36, 0x49 ,0x55 ,0x22 ,0x50}, // &
    {0x00, 0x05 ,0x03 ,0x00 ,0x00}, // '
    {0x00, 0x1C ,0x22 ,0x41 ,0x00}, // (
    {0x00, 0x41 ,0x22 ,0x1C ,0x00}, // )
    {0x14, 0x08 ,0x3E ,0x08 ,0x14}, // *
    {0x08, 0x08 ,0x3E ,0x08 ,0x08}, // +
    {0x00, 0x50 ,0x30 ,0x00 ,0x00}, // ,
    {0x08, 0x08 ,0x08 ,0x08 ,0x08}, // -
    {0x00, 0x60 ,0x60 ,0x00 ,0x00}, // .
    {0x20, 0x10 ,0x08 ,0x04 ,0x02}, // /
     
    {0x3E ,0x51 ,0x49 ,0x45 ,0x3E}, // 0
    {0x00 ,0x42 ,0x7F ,0x40 ,0x00}, // 1
    {0x42 ,0x61 ,0x51 ,0x49 ,0x46}, // 2
    {0x21 ,0x41 ,0x45 ,0x4B ,0x31}, // 3
    {0x18 ,0x14 ,0x12 ,0x7F ,0x10}, // 4
    {0x27 ,0x45 ,0x45 ,0x45 ,0x39}, // 5
    {0x3C ,0x4A ,0x49 ,0x49 ,0x30}, // 6
    {0x01 ,0x71 ,0x09 ,0x05 ,0x03}, // 7
    {0x36 ,0x49 ,0x49 ,0x49 ,0x36}, // 8
    {0x06 ,0x49 ,0x49 ,0x29 ,0x1E}, // 9
     
    {0x36 ,0x36 ,0x00 ,0x00 ,0x00}, // :
    {0x56 ,0x36 ,0x00 ,0x00 ,0x00}, // ;
    {0x08 ,0x14 ,0x22 ,0x41 ,0x00}, // <
    {0x14 ,0x14 ,0x14 ,0x14 ,0x14}, // =
    {0x00 ,0x41 ,0x22 ,0x14 ,0x08}, // >
    {0x02 ,0x01 ,0x51 ,0x09 ,0x06}, // ?
    {0x30 ,0x49 ,0x79 ,0x41 ,0x3E}, // @ 0x40
     
    {0x7E ,0x11 ,0x11 ,0x11 ,0x7E}, // A
    {0x7F ,0x49 ,0x49 ,0x49 ,0x36}, // B
    {0x3E ,0x41 ,0x41 ,0x41 ,0x22}, // C
    {0x7F ,0x41 ,0x41 ,0x22 ,0x1C}, // D
    {0x7F ,0x49 ,0x49 ,0x49 ,0x41}, // E
    {0x7F ,0x09 ,0x09 ,0x09 ,0x01}, // F
    {0x3E ,0x41 ,0x49 ,0x49 ,0x7A}, // G
    {0x7F ,0x08 ,0x08 ,0x08 ,0x7F}, // H
    {0x00 ,0x41 ,0x7F ,0x41 ,0x00}, // I
    {0x20 ,0x40 ,0x41 ,0x3F ,0x01}, // J
    {0x7F ,0x08 ,0x14 ,0x22 ,0x41}, // K
    {0x7F ,0x40 ,0x40 ,0x40 ,0x40}, // L
    {0x7F ,0x02 ,0x0C ,0x02 ,0x7F}, // M
    {0x7F ,0x04 ,0x08 ,0x10 ,0x7F}, // N
    {0x3E ,0x41 ,0x41 ,0x41 ,0x3E}, // O
    {0x7F ,0x09 ,0x09 ,0x09 ,0x06}, // P
    {0x3E ,0x41 ,0x51 ,0x21 ,0x5E}, // Q
    {0x7F ,0x09 ,0x19 ,0x29 ,0x46}, // R
    {0x46 ,0x49 ,0x49 ,0x49 ,0x31}, // S
    {0x01 ,0x01 ,0x7F ,0x01 ,0x01}, // T
    {0x3F ,0x40 ,0x40 ,0x40 ,0x3F}, // U
    {0x1F ,0x20 ,0x40 ,0x20 ,0x1F}, // V
    {0x3F ,0x40 ,0x30 ,0x40 ,0x3F}, // W
    {0x63 ,0x14 ,0x08 ,0x14 ,0x63}, // X
    {0x07 ,0x08 ,0x70 ,0x08 ,0x07}, // Y
    {0x61 ,0x51 ,0x49 ,0x45 ,0x43}, // Z
     
    {0x00 ,0x7F ,0x41 ,0x41 ,0x00}, // [
    {0x02 ,0x04 ,0x08 ,0x10 ,0x20}, // "\" backslash
    {0x00 ,0x41 ,0x41 ,0x7F ,0x00}, // ]
    {0x04 ,0x02 ,0x01 ,0x02 ,0x04}, // ^
    {0x40 ,0x40 ,0x40 ,0x40 ,0x40}, // _
    {0x00 ,0x01 ,0x02 ,0x04 ,0x00}, // ` 0x60
     
    {0x20 ,0x54 ,0x54 ,0x54 ,0x78}, // a
    {0x7F ,0x50 ,0x48 ,0x48 ,0x30}, // b
    {0x38 ,0x44 ,0x44 ,0x44 ,0x20}, // c
    {0x38 ,0x44 ,0x44 ,0x48 ,0x7F}, // d
    {0x38 ,0x54 ,0x54 ,0x54 ,0x18}, // e
    {0x08 ,0x7E ,0x09 ,0x01 ,0x02}, // f
    {0x0C ,0x52 ,0x52 ,0x52 ,0x3E}, // g
    {0x7F ,0x08 ,0x04 ,0x04 ,0x78}, // h
    {0x00 ,0x44 ,0x7D ,0x40 ,0x00}, // i
    {0x20 ,0x40 ,0x44 ,0x3D ,0x00}, // j
    {0x7F ,0x10 ,0x28 ,0x44 ,0x00}, // k
    {0x00 ,0x41 ,0x7F ,0x40 ,0x00}, // l
    {0x78 ,0x04 ,0x78 ,0x04 ,0x78}, // m
    {0x7C ,0x08 ,0x04 ,0x04 ,0x78}, // n
    {0x38 ,0x44 ,0x44 ,0x44 ,0x38}, // o
    {0x7C ,0x14 ,0x14 ,0x14 ,0x08}, // p
    {0x08 ,0x14 ,0x14 ,0x18 ,0x7C}, // q
    {0x7C ,0x08 ,0x04 ,0x04 ,0x08}, // r
    {0x48 ,0x54 ,0x54 ,0x54 ,0x20}, // s
    {0x04 ,0x3F ,0x44 ,0x40 ,0x20}, // t
    {0x3C ,0x40 ,0x40 ,0x20 ,0x7C}, // u
    {0x1C ,0x20 ,0x40 ,0x20 ,0x1C}, // v
    {0x3C ,0x40 ,0x30 ,0x40 ,0x3C}, // w
    {0x44 ,0x28 ,0x10 ,0x28 ,0x44}, // x
    {0x0C ,0x50 ,0x50 ,0x50 ,0x3C}, // y
    {0x44 ,0x64 ,0x54 ,0x4C ,0x44}, // z
     
    {0x00, 0x08, 0x36, 0x41, 0x00}, // {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // }
    {0x0C, 0x02, 0x0C, 0x10, 0x0C}, // ~
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 0x80-1
                
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 0x80 .. 0xA0-1
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
     
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 0xA0 .. 0xC0-1
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x7E, 0x4B, 0x4A, 0x4B, 0x42}, // Ё
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x38, 0x55, 0x54, 0x55, 0x18}, // ё
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // А  0xC0
    {0x7F, 0x49, 0x49, 0x49, 0x30}, // Б
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // В
    {0x7F, 0x01, 0x01, 0x01, 0x01}, // Г
    {0x60, 0x3E, 0x21, 0x3F, 0x60}, // Д
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // Е
    {0x67, 0x18, 0x7F, 0x18, 0x67}, // Ж
    {0x22, 0x41, 0x49, 0x49, 0x36}, // З
    {0x7F, 0x10, 0x08, 0x04, 0x7F}, // И
    {0x7F, 0x10, 0x09, 0x04, 0x7F}, // Й
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // К
    {0x40, 0x3E, 0x01, 0x01, 0x7E}, // Л
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, // М
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // Н
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // О
    {0x7F, 0x01, 0x01, 0x01, 0x7F}, // П
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // Р
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // С
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // Т
    {0x27, 0x48, 0x48, 0x48, 0x3F}, // У
    {0x1E, 0x21, 0x7F, 0x21, 0x1E}, // Ф
    {0x63, 0x14, 0x08, 0x14, 0x63}, // Х
    {0x3F, 0x20, 0x20, 0x3F, 0x40}, // Ц
    {0x1F, 0x10, 0x10, 0x10, 0x7F}, // Ч
    {0x7F, 0x40, 0x7F, 0x40, 0x7F}, // Ш
    {0x3F, 0x20, 0x3F, 0x20, 0x7F}, // Щ
    {0x01, 0x7F, 0x44, 0x44, 0x38}, // Ъ
    {0x7F, 0x44, 0x7C, 0x00, 0x7F}, // Ы
    {0x7F, 0x44, 0x44, 0x44, 0x38}, // Ь
    {0x22, 0x41, 0x49, 0x49, 0x3E}, // Э
    {0x7F, 0x08, 0x7F, 0x41, 0x7F}, // Ю
    {0x46, 0x29, 0x19, 0x09, 0x7F}, // Я

    {0x20, 0x54, 0x54, 0x54, 0x78}, // а 0xE0
    {0x3C, 0x4A, 0x4A, 0x4B, 0x30}, // б
    {0x7C, 0x54, 0x54, 0x58, 0x20}, // в
    {0x7C, 0x04, 0x04, 0x04, 0x04}, // г
    {0x60, 0x38, 0x24, 0x3C, 0x60}, // д
    {0x38, 0x54, 0x54, 0x54, 0x18}, // е
    {0x74, 0x08, 0x7C, 0x08, 0x74}, // ж
    {0x28, 0x44, 0x54, 0x54, 0x28}, // з
    {0x7C, 0x20, 0x10, 0x08, 0x7C}, // и
    {0x7C, 0x21, 0x12, 0x09, 0x7C}, // й
    {0x7C, 0x10, 0x10, 0x28, 0x44}, // к
    {0x40, 0x78, 0x04, 0x04, 0x7C}, // л
    {0x7C, 0x08, 0x10, 0x08, 0x7C}, // м
    {0x7C, 0x10, 0x10, 0x10, 0x7C}, // н
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x04, 0x04, 0x04, 0x7C}, // п

    {0x7C, 0x14, 0x14, 0x14, 0x08}, // р
    {0x38, 0x44, 0x44, 0x44, 0x28}, // с
    {0x04, 0x04, 0x7C, 0x04, 0x04}, // т
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // у
    {0x18, 0x24, 0x7C, 0x24, 0x18}, // ф
    {0x44, 0x28, 0x10, 0x28, 0x44}, // х
    {0x3C, 0x20, 0x20, 0x3C, 0x40}, // ц
    {0x1C, 0x20, 0x20, 0x20, 0x7C}, // ч
    {0x7C, 0x40, 0x7C, 0x40, 0x7C}, // ш
    {0x3C, 0x20, 0x3C, 0x20, 0x7C}, // щ
    {0x04, 0x7C, 0x48, 0x48, 0x30}, // ъ
    {0x7C, 0x48, 0x48, 0x30, 0x7C}, // ы
    {0x7C, 0x48, 0x48, 0x48, 0x30}, // ь
    {0x28, 0x44, 0x54, 0x54, 0x38}, // э
    {0x7C, 0x38, 0x44, 0x44, 0x38}, // ю
    {0x48, 0x34, 0x14, 0x14, 0x7C}  // я

};

#define GLCD_SET_CS1_1  GLCD_PORT_CS1 |=  (1 << GLCD_BIT_CS1)
#define GLCD_SET_CS1_0  GLCD_PORT_CS1 &= ~(1 << GLCD_BIT_CS1)
#define GLCD_SET_CS2_1  GLCD_PORT_CS2 |=  (1 << GLCD_BIT_CS2)
#define GLCD_SET_CS2_0  GLCD_PORT_CS2 &= ~(1 << GLCD_BIT_CS2)
#define GLCD_SET_RS_1   GLCD_PORT_RS  |=  (1 << GLCD_BIT_RS)
#define GLCD_SET_RS_0   GLCD_PORT_RS  &= ~(1 << GLCD_BIT_RS)
#define GLCD_SET_RW_1   GLCD_PORT_RW  |=  (1 << GLCD_BIT_RW)
#define GLCD_SET_RW_0   GLCD_PORT_RW  &= ~(1 << GLCD_BIT_RW)
#define GLCD_SET_E_1    GLCD_PORT_E   |=  (1 << GLCD_BIT_E)
#define GLCD_SET_E_0    GLCD_PORT_E   &= ~(1 << GLCD_BIT_E)
#define GLCD_SET_RST_1  GLCD_PORT_RST |=  (1 << GLCD_BIT_RST)
#define GLCD_SET_RST_0  GLCD_PORT_RST &= ~(1 << GLCD_BIT_RST)

#define GLCD_DDR_CTRL do                                                        \
        {                                                                           \
                GLCD_DDR_CS1 = ~((~GLCD_DDR_CS1 & ~(1<<GLCD_BIT_CS1)) | (1<<GLCD_BIT_CS1)); \
                GLCD_DDR_CS2 = ~((~GLCD_DDR_CS2 & ~(1<<GLCD_BIT_CS2)) | (1<<GLCD_BIT_CS2)); \
                GLCD_DDR_RS  = ~((~GLCD_DDR_RS  & ~(1<<GLCD_BIT_RS))  | (1<<GLCD_BIT_RS));  \
                GLCD_DDR_RW  = ~((~GLCD_DDR_RW  & ~(1<<GLCD_BIT_RW))  | (1<<GLCD_BIT_RW));  \
                GLCD_DDR_E   = ~((~GLCD_DDR_E   & ~(1<<GLCD_BIT_E))   | (1<<GLCD_BIT_E));   \
                GLCD_DDR_RST = ~((~GLCD_DDR_RST & ~(1<<GLCD_BIT_RST)) | (1<<GLCD_BIT_RST)); \
        } while (0)
#define GLCD_DDR_DATA_OUT do                                                \
        {                                                                       \
                GLCD_DDR_D7 = ~((~GLCD_DDR_D7 & ~(1<<GLCD_BIT_D7)) | (1<<GLCD_BIT_D7)); \
                GLCD_DDR_D6 = ~((~GLCD_DDR_D6 & ~(1<<GLCD_BIT_D6)) | (1<<GLCD_BIT_D6)); \
                GLCD_DDR_D5 = ~((~GLCD_DDR_D5 & ~(1<<GLCD_BIT_D5)) | (1<<GLCD_BIT_D5)); \
                GLCD_DDR_D4 = ~((~GLCD_DDR_D4 & ~(1<<GLCD_BIT_D4)) | (1<<GLCD_BIT_D4)); \
                GLCD_DDR_D3 = ~((~GLCD_DDR_D3 & ~(1<<GLCD_BIT_D3)) | (1<<GLCD_BIT_D3)); \
                GLCD_DDR_D2 = ~((~GLCD_DDR_D2 & ~(1<<GLCD_BIT_D2)) | (1<<GLCD_BIT_D2)); \
                GLCD_DDR_D1 = ~((~GLCD_DDR_D1 & ~(1<<GLCD_BIT_D1)) | (1<<GLCD_BIT_D1)); \
                GLCD_DDR_D0 = ~((~GLCD_DDR_D0 & ~(1<<GLCD_BIT_D0)) | (1<<GLCD_BIT_D0)); \
        } while (0)

#define GLCD_DDR_DATA_IN do                             \
        {                                                   \
                GLCD_DDR_D7 = ~(~GLCD_DDR_D7 & ~(1<<GLCD_BIT_D7));\
                GLCD_DDR_D6 = ~(~GLCD_DDR_D6 & ~(1<<GLCD_BIT_D6));\
                GLCD_DDR_D5 = ~(~GLCD_DDR_D5 & ~(1<<GLCD_BIT_D5));\
                GLCD_DDR_D4 = ~(~GLCD_DDR_D4 & ~(1<<GLCD_BIT_D4));\
                GLCD_DDR_D3 = ~(~GLCD_DDR_D3 & ~(1<<GLCD_BIT_D3));\
                GLCD_DDR_D2 = ~(~GLCD_DDR_D2 & ~(1<<GLCD_BIT_D2));\
                GLCD_DDR_D1 = ~(~GLCD_DDR_D1 & ~(1<<GLCD_BIT_D1));\
                GLCD_DDR_D0 = ~(~GLCD_DDR_D0 & ~(1<<GLCD_BIT_D0));\
        } while (0)
        
        

void wait(int tact)
{
        while (tact--)
        {
                asm{nop;};
        }
                
}

/*##############################################################*/
/* macros and functions below is currently used in library      */
/* for basic operations (GLCD_MCU macros start, read, write)    */
/* (GLCD functions draw pixel, byte, char, string)              */
/*##############################################################*/


/* GLCD_Cmd_Start; is macros for            */
/* reset, power on, set coordinate          */
/* in functions GLCD_Draw_...               */
/* 1. reset GLCD MCUs                       */
/* 2. wait(1);//(no care?)                  */
/* 3. command display on                    */
/* 4. set horizontal coordinate 0-63        */
/* 5. set vertical line coordinate 0-7      */
/* 6. set start(vert) line coordinate 0-63  */
#define GLCD_Cmd_Start do                   \
{                                           \
    GLCD_DDR_CTRL;                          \
    GLCD_SET_RST_0;                         \
    wait(GLCD_DELAY);                       \
    GLCD_SET_RST_1;                         \
    GLCD_DDR_DATA_OUT;                      \
    GLCD_PORT_D7 = 0x3F;                    \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
    GLCD_SET_CS2_1;                         \
    GLCD_PORT_D7 = (0x40 + (X_pos%0x40));   \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
    GLCD_SET_CS2_1;                         \
    GLCD_PORT_D7 = (0xB8 + (Y_pos/8));      \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
    GLCD_SET_CS2_1;                         \
    GLCD_PORT_D7 = (0xC0 + 0);              \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
    GLCD_SET_CS2_1;                         \
} while (0)


/* GLCD_Cmd_Read_L;(..._R) is macros for    */
/* read data bite from left(right) half     */
/* of display in functions GLCD_Draw_...    */
/* 1. Set data input port direction         */
/* 2. DataInput bit = 1                     */
/* 3. ReadWrite bit = 1                     */
/* 4. Select left(right) GLCD MCU, inverse  */
/* 5. Operation clock rising edge           */
/* 6. Read data                             */
/* 7. DataInput bit = 0                     */
/* 8. ReadWrite bit = 0                     */
/* 9. Deselect left(right) GLCD MCU, inverse*/
/* 10. Operation clock falling edge         */

#define GLCD_Cmd_Read_L do                  \
{                                           \
    GLCD_DDR_DATA_IN;                       \
    GLCD_PORT_D7 = 0x00;                    \
    GLCD_SET_RS_1;                          \
    GLCD_SET_RW_1;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_E_1;                           \
    buffer = GLCD_PIN_D7;                   \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
} while (0)

#define GLCD_Cmd_Read_R do                  \
{                                           \
    GLCD_DDR_DATA_IN;                       \
    GLCD_PORT_D7 = 0x00;                    \
    GLCD_SET_RS_1;                          \
    GLCD_SET_RW_1;                          \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
    buffer = GLCD_PIN_D7;                   \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS2_1;                         \
} while (0)


/* GLCD_Cmd_Write_L;(..._R) is macros for   */
/* write data bite to left(right) half      */
/* of display in functions GLCD_Draw_...    */
/* 1. Set data output port direction        */
/* 2. Send data                             */
/* 3. DataInput bit = 1                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
/* 8. Wait(GLCD_DELAY);// (may be no care)  */
/* 9. Operation clock falling edge          */
/* 10. DataInput bit = 0                    */
/* 11. ReadWrite bit = 0                    */
/* 12. Deselect left(right) GLCD MCU,inverse*/
/* 13. Wait(GLCD_DELAY);// (may be no care) */

#define GLCD_Cmd_Write_L do                 \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    GLCD_PORT_D7 = buffer;                  \
    GLCD_SET_RS_1;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_E_1;                           \
    wait(GLCD_DELAY);                       \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
    wait(GLCD_DELAY);                       \
} while (0)

#define GLCD_Cmd_Write_R do                 \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    GLCD_PORT_D7 = buffer;                  \
    GLCD_SET_RS_1;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
    wait(GLCD_DELAY);                       \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS2_1;                         \
    wait(GLCD_DELAY);                       \
} while (0)

/* GLCD_Draw_Pixel is 3-argument function   */
/* for draw one pixel with coordinates x,y  */
/* arg1: horizontal coordinate 0<=x<128     */
/* arg2: vertical coordinate 0<=y<64        */
/* arg3: color: 0-black, 1-white, 2-inverse,*/
/* arg3: color: other-nothing               */

void GLCD_Draw_Pixel (unsigned char X_pos, unsigned char Y_pos, unsigned char color)
{
    unsigned char buffer = 0;
        if((X_pos<128)&&(Y_pos<64))
        {
        // choose left/right GLCD MCU
            if(X_pos<64) // left GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_L;
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_L;
            }
            else // right GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_R;
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_R;
            }    
        // select drawing color function
            switch(color)
            {
            case 0: // pixel off
            buffer = (buffer & ~(1<<Y_pos%8));
                    break;
            case 1: // pixel on
            buffer = (buffer & ~(1<<Y_pos%8)) | (1<<Y_pos%8);
                    break;
            case 2: // pixel invert
                    if(buffer & (1<<Y_pos%8))
                buffer = (buffer & ~(1<<Y_pos%8));
            else
                buffer = (buffer & ~(1<<Y_pos%8)) | (1<<Y_pos%8);
                    break;
            default :
                    break;
            }
        // choose left/right GLCD MCU
            if(X_pos<64) // left GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Write_L;
            }
            else // right GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Write_R;
            }
        }        
}

/* GLCD_Draw_Byte is 4-argument function    */
/* for draw 8 pixels with coordinates x,y   */
/* arg1: horizontal coordinate 0<=x<128     */
/* arg2: vertical coordinate string 0<=y<7  */
/* arg3: byte: bit color mask, MSB~bigger_Y */
/* arg4: color: 0- '1' is black,            */
/* arg4: color: 1- '1' is white,            */
/* arg4: color: 2- '1' is inverse,          */
/* arg4: color: other-nothing               */

void GLCD_Draw_Byte (unsigned char X_pos, unsigned char Y_pos, unsigned char data_byte, unsigned char color)
{
    unsigned char buffer = 0;
        if((X_pos<128)&&(Y_pos<8))
        {
            Y_pos *= 8;

        // choose left/right GLCD MCU
            if(X_pos<64) // left GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_L;
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_L;
            }
            else // right GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_R;
                    GLCD_Cmd_Start;
            GLCD_Cmd_Read_R;
            }
            switch(color)
            {
            case 0: // pixel off
            buffer = ~data_byte;
                    break;
            case 1: // pixel on
            buffer = data_byte;
                    break;
            case 2: // pixel invert
                    buffer = ~buffer;
            default :
                    break;
            }
            if(X_pos<64) // left GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Write_L;
            }
            else // right GLCD MCU
            {
                    GLCD_Cmd_Start;
            GLCD_Cmd_Write_R;
            }
        }        
}

/* GLCD_Draw_Char is 4-argument function    */
/* for draw win1251_5x7 character           */
/* with coordinates x,y                     */
/* arg1: horizontal coordinate 0<=x<128     */
/* arg2: vertical coordinate string 0<=y<7  */
/* arg3: char: 5x7 character, 7x8 cell      */
/* arg4: color: 0- '1' is black,            */
/* arg4: color: 1- '1' is white,            */
/* arg4: color: other-nothing               */

void GLCD_Draw_Char (unsigned char X_pos, unsigned char Y_pos, unsigned char data_char, unsigned char color)
{
    unsigned char buffer = 0;
        unsigned char counter_i = 0;
        if((X_pos<128)&&(Y_pos<8))
        {
            Y_pos *= 8;

        GLCD_Cmd_Start;

        for(counter_i=0;counter_i<7;counter_i++)
            {
                    if((X_pos+counter_i)==64)
            {
                            buffer = X_pos;
                            X_pos = X_pos+counter_i;
                            GLCD_Cmd_Start;
                            X_pos = buffer;
                    }
            if(X_pos+counter_i>127)
                        break;
                // select data byte from font character table to buffer, char cell size 7x8
            if ((counter_i==0)||(counter_i==6))
                        buffer = 0x00;
            else
                      buffer = FLASH_Read(&FONT_5x7_WIN1251[(int)data_char][counter_i-1]);

            // invert if color==0
                    if(!color)        
                    buffer = ~buffer;

            // choose left/right GLCD MCU                        
                if(X_pos+counter_i<64)
                { // left GLCD MCU
                GLCD_Cmd_Write_L;
                }
                else
                { // right GLCD MCU
                GLCD_Cmd_Write_R;
                }
            }
        }                

}

/* GLCD_Draw_Char is 4-argument function    */
/* for draw sting of win1251_5x7 characters */
/* from coordinates x,y                     */
/* arg1: horizontal coordinate 0<=x<128     */
/* arg2: vertical coordinate string 0<=y<7  */
/* arg3: string if chars (array=pointer)    */
/* arg4: color: 0- '1' is black,            */
/* arg4: color: 1- '1' is white,            */
/* arg4: color: other-nothing               */

void GLCD_Draw_String (unsigned char X_pos, unsigned char Y_pos, char *data_string, unsigned char color)
{
    char buffer_char = 0;
        unsigned char counter_j = 0;
        if((X_pos<128)&&(Y_pos<8))
    {
        GLCD_Cmd_Start;
                
        while(counter_j<255)
            {

                    buffer_char = data_string[counter_j];

                    if (buffer_char)
                            counter_j++;
                    else
                            break;

            GLCD_Draw_Char(X_pos,Y_pos,buffer_char,color);
                    X_pos += 7;

            }
        }    
        //GLCD_Cmd_Status;
        //wait(GLCD_DELAY);// (may be no care)
        //DDRB = 0xFF;
        //PORTB = 0x00;
        //PORTB = PINA;
    //GLCD_Cmd_End;
}

/*##############################################################*/
/* End of basic macros and functions of the library             */
/*##############################################################*/

///////////////////////////////Заполнение экрана/////////////////////////////////////////////////
void GLCD_Fill()                                                                                                                                                           
{    
     unsigned char y = 0;
     unsigned char x = 0;
        for (y = 0; y <= 127; y++)
                for(x = 0; x <= 7; x++)
                        GLCD_Draw_Byte(y,x,0xFF,1);                                                                                                                   
}                                                                                                                                                                                           
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Очищение экрана///////////////////////////////////////////////////
void GLCD_Clear()                                                                                                                                                           
{                                                                                                                                                                                           
     unsigned char y = 0;
     unsigned char x = 0;
       for (y = 0; y <= 127; y++)
                for(x = 0; x <= 7; x++)
                        GLCD_Draw_Byte(y,x,0,1);                                                                                                                   
}                                                                                                                                                                                           
/////////////////////////////////////////////////////////////////////////////////////////////////


/* GLCD_Move_Screen is 1-argument function  */
/* for slide screen left or right           */
/* arg1: slide step -128<=x<127             */
/* '+1' is slide 1 pixel to right           */

void GLCD_Move_Screen (signed char step)
{
    unsigned char buffer = 0;
    unsigned char buffer_back = 0;
    unsigned char X_pos = 0;
    unsigned char X_back = 0;
    unsigned char Y_pos = 0;

        for(Y_pos=0;Y_pos<8;Y_pos++){
            for(X_pos=128;X_pos>0;X_pos--){
                
                Y_pos *= 8;
                X_back = X_pos--;

                // Read byte
                // choose left/right GLCD MCU
                if(X_pos<64) // left GLCD MCU
                {
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Read_L;
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Read_L;
                }
                else // right GLCD MCU
                {
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Read_R;
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Read_R;
                }
                

                buffer_back = buffer;
                buffer = 0x00;
                // Clear old place
                if((X_pos>=0)
                && (X_pos<128))
                if(X_pos<64) // left GLCD MCU
                {
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Write_L;
                }
                else // right GLCD MCU
                {
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Write_R;
                }

                X_pos += step;
                buffer = buffer_back;
                // Write new place
                if((X_pos>=0)
                && (X_pos<128))
                if(X_pos<64) // left GLCD MCU
                {
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Write_L;
                }
                else // right GLCD MCU
                {
                    GLCD_Cmd_Start;
                    GLCD_Cmd_Write_R;
                }
                
                X_pos = X_back;
                Y_pos /= 8;
            }
        }
}

///////////////////////////////Рисование линии///////////////////////////////////////////////////
//X0_pos - начальная кордината по X (от 0 до 127)                                                                                           
//Y0_pos - начальная кордината по Y (от 0 до 63)                                                                                           
//X1_pos - конечная кордината по X (от 0 до 127)                                                                                           
//Y1_pos - конечная кордината по Y (от 0 до 63)
//color  - цвет линии:0 - пустой, 1 - закрашенный, 2 - инвертированый
/////////////////////////////////////////////////////////////////////////////////////////////////               /*                                                                                    
/*
void GLCD_Draw_Line(unsigned char X0_pos,unsigned char Y0_pos,                                                                   
                                        unsigned char X1_pos,unsigned char Y1_pos,unsigned char coolor)                           
{
        signed char dx, dy;
 
      signed char x_inter = 0;
     signed char y_inter = 0;

        
        dx = X1_pos - X0_pos;
        dy = Y1_pos - Y0_pos;


        
if (fabs(dx) >= 4)
{       
       if (dx > 0)
                if(dy > 0)
                        for(x_inter = 0; x_inter <= dx; x_inter++)
                                GLCD_Draw_Pixel((x_inter + X0_pos),((x_inter*dy)/dx + Y0_pos),coolor);
                else
                        if(dy == 0)
                                for(x_inter = X0_pos; x_inter <= X1_pos; x_inter++)
                                        GLCD_Draw_Pixel(x_inter,Y0_pos,coolor);        
                        else
                                for(x_inter = 0; x_inter <= dx; x_inter++)
                                        GLCD_Draw_Pixel((x_inter + X0_pos), ((x_inter*dy)/dx + Y0_pos),coolor);                        
        else
                if (dx == 0)
                {
                        if (dy > 0)
                                for(y_inter = Y0_pos; y_inter <= Y1_pos; y_inter++)
                                        GLCD_Draw_Pixel(X0_pos, y_inter, coolor);        
                        else
                                if (dy == 0)
                                        GLCD_Draw_Pixel(X0_pos, Y0_pos, coolor);
                                else
                                        for(y_inter = Y1_pos; y_inter <= Y0_pos; y_inter++)
                                                GLCD_Draw_Pixel(X0_pos, y_inter, coolor);
                }
                else
                        if (dy > 0)
                                for(x_inter = (-dx); x_inter >= 0; x_inter--)
                                        GLCD_Draw_Pixel(x_inter + X1_pos,((x_inter*dy)/dx + Y1_pos),coolor);                        
                        else
                                if(dy == 0)
                                        for( x_inter = X1_pos; x_inter <= X0_pos; x_inter++)
                                                GLCD_Draw_Pixel(x_inter,Y0_pos,coolor);
                                else
                                        for(x_inter = (-dx); x_inter >= 0; x_inter--)
                                                GLCD_Draw_Pixel(x_inter+ X1_pos,((x_inter*dy)/dx)+Y1_pos,coolor);                
}
else
{
        if (dx > 0)
                if(dy > 0)
                        for(y_inter = 0; y_inter <= dy; y_inter++)
                                GLCD_Draw_Pixel(((y_inter*dx)/dy + X0_pos),y_inter + Y0_pos,coolor);
                else
                        if(dy == 0)
                                for(x_inter = X0_pos; x_inter <= X1_pos; x_inter++)
                                        GLCD_Draw_Pixel(x_inter,Y0_pos,coolor);        
                        else
                                for(y_inter = (-dy); y_inter >= 0; y_inter--)
                                        GLCD_Draw_Pixel(((y_inter*dx)/dy + X0_pos), y_inter + Y1_pos,coolor);                        
        else
                if (dx == 0)
                {
                        if (dy > 0)
                                for(y_inter = Y0_pos; y_inter <= Y1_pos; y_inter++)
                                        GLCD_Draw_Pixel(X0_pos, y_inter, coolor);        
                        else
                                if (dy == 0)
                                        GLCD_Draw_Pixel(X0_pos, Y0_pos, coolor);
                                else
                                        for(y_inter = Y1_pos; y_inter <= Y0_pos; y_inter++)
                                                GLCD_Draw_Pixel(X0_pos, y_inter, coolor);
                }
                else
                        if (dy > 0)
                                for(y_inter = 0; y_inter <= dy; y_inter++)
                                        GLCD_Draw_Pixel(((y_inter*dx)/dy + X0_pos),y_inter + Y0_pos,coolor);                        
                        else
                                if(dy == 0)
                                        for(x_inter = X1_pos; x_inter <= X0_pos; x_inter++)
                                                GLCD_Draw_Pixel(x_inter,Y0_pos,coolor);
                                else
                                        for(y_inter = (-dy); y_inter >= 0; y_inter--)
                                                GLCD_Draw_Pixel(((y_inter*dx)/dy + X1_pos),y_inter + Y1_pos,coolor);                
        
}
}
*/
//##############################################################################
//Name:         GLCD_Draw_Line function for GLCD_Screen library
//Author:       Ratgor
//Revision:     v0.3_2012-07-20
//Destination:  optimized, should be update in GLCD_Screen_v03 -> _v04

void GLCD_Draw_Line(
    unsigned char x0,
    unsigned char y0,
    unsigned char x,
    unsigned char y,
    unsigned char color)
{
    unsigned char i,j;

    if((x-x0)<0)i=(x0-x);else i=(x-x0);
    if((y-y0)<0)j=(y0-y);else j=(y-y0);
    if(i>=j) { // if(abs(x-x0)>=abs(y-y0)) {
        if (x>x0)
            for (i=x0; i<=x; i++)
                GLCD_Draw_Pixel(i,y0+(i-x0)*(y-y0)/(x-x0),color);
        else// if (x<x0) & if (x=x0) GLCD_Draw_Pixel(x0,y0,color);
            for (i=x; i<=x0; i++)
                GLCD_Draw_Pixel(i,y+(i-x)*(y0-y)/(x0-x),color);
    } else {
        if (y>y0)
            for (i=y0; i<=y; i++)
                GLCD_Draw_Pixel(x0+(i-y0)*(x-x0)/(y-y0),i,color);
        else // if (y<y0) & never (y=y0)
            for (i=y; i<=y0; i++)
                GLCD_Draw_Pixel(x+(i-y)*(x0-x)/(y0-y),i,color);
    }
}
//##############################################################################

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Рисование прямоугольника//////////////////////////////////////////
//X0_pos - начальная верхняя левая координата по X (от 0 до 127)
//Y0_pos - начальная верхняя левая координата по Y (от 0 до 63)
//X1_pos - конечная нихняя правая координата по X (от 0 до 127)
//Y1_pos - конечная нихняя правая координата по Y (от 0 до 63)
//color  - цвет рамки:0 - пустой, 1 - закрашенный, 2 - инвертированый
void Glcd_Draw_Rectangle(unsigned char X0_pos,unsigned char Y0_pos,
                                                 unsigned char X1_pos,unsigned char Y1_pos,unsigned char coolor)
{
        GLCD_Draw_Line(X0_pos,Y0_pos,X1_pos,Y0_pos,coolor);
        GLCD_Draw_Line(X0_pos,Y0_pos,X0_pos,Y1_pos,coolor);
        GLCD_Draw_Line(X0_pos,Y1_pos,X1_pos,Y1_pos,coolor);
        GLCD_Draw_Line(X1_pos,Y0_pos,X1_pos,Y1_pos,coolor);
                
}
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Рисование закрашеного прямоугольника//////////////////////////////
//X0_pos - начальная верхняя левая координата по X (от 0 до 127)
//Y0_pos - начальная верхняя левая координата по Y (от 0 до 63)
//X1_pos - конечная нихняя правая координата по X (от 0 до 127)
//Y1_pos - конечная нихняя правая координата по Y (от 0 до 63)
//color  - цвет закрашивания:0 - пустой, 1 - закрашенный, 2 - инвертированый
void Glcd_Draw_Rectangle_Fill(unsigned char X0_pos,unsigned char Y0_pos,
                                                      unsigned char X1_pos,unsigned char Y1_pos,unsigned char coolor)
{
        unsigned y_inter = 0;
        for (y_inter = Y0_pos; y_inter <= Y1_pos; y_inter++)
                GLCD_Draw_Line(X0_pos,y_inter,X1_pos,y_inter,coolor);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Рисование окружности//////////////////////////////////////////////
//X0_pos - кордината центра окружности по X (от 0 до 127)
//Y0_pos - кордината центра окружности по Y (от 0 до 63)
//Radius - радиус окружности (не должен привышать X0_pos)
//color  - цвет рамки:0 - пустой, 1 - закрашенный, 2 - инвертированый
void Glcd_Draw_Circle(unsigned char X0_pos,unsigned char Y0_pos,
                                          unsigned char Radius,unsigned char coolor)
{
        unsigned char x_inter = 0;
        unsigned char y_inter = 0;
        for (x_inter = (X0_pos - Radius); x_inter <= (X0_pos + Radius); x_inter++)
                for(y_inter = (Y0_pos - Radius); y_inter <= (Y0_pos + Radius); y_inter++)
                        if ((floor(sqrt(pow((x_inter - X0_pos),2) + pow((y_inter - Y0_pos),2)))) == Radius)
                                GLCD_Draw_Pixel(x_inter,y_inter,coolor);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Рисование закрашеной окружности///////////////////////////////////
//X0_pos - кордината центра окружности по X (от 0 до 127)
//Y0_pos - кордината центра окружности по Y (от 0 до 63)
//Radius - радиус окружности (не должен привышать X0_pos)
//color  - цвет закрашивания:0 - пустой, 1 - закрашенный, 2 - инвертированый
void Glcd_Draw_Circle_Fill(unsigned char X0_pos,unsigned char Y0_pos,
                                                   unsigned char Radius,unsigned char coolor)
{                                                                        
        unsigned char x_inter = 0;
        unsigned char y_inter = 0;
        for (x_inter = (X0_pos - Radius); x_inter <= (X0_pos + Radius); x_inter++)
                for(y_inter = (Y0_pos - Radius); y_inter <= (Y0_pos + Radius); y_inter++)
                        if ((floor(sqrt(pow((x_inter - X0_pos),2) + pow((y_inter - Y0_pos),2)))) <= Radius)
                                GLCD_Draw_Pixel(x_inter,y_inter,coolor);
}
/////////////////////////////////////////////////////////////////////////////////////////////////

//##############################################################################
// Function for ASCII dec_code for digit at position in number of radix.
// 2011-10-28_Ratgor.
// Example 1: numPosCode(123,0,10)=51 (ASCII: 3)
// Example 2: numPosCode(176,1,16)=66 (ASCII: B)
// Optimisation number & buffer type:
// char -> int: RAM+0, ROM+49; long: RAM+5, ROM+442.
unsigned int numPosCode
    (unsigned int number,
     unsigned char position,
     unsigned char radix)
{
  unsigned int result;
  unsigned int buffer;

  buffer = number;

  for (result=0;result<position;result++)
    {buffer = buffer/radix;}

  buffer = buffer%radix;

  if (buffer<10)
    {result=buffer+48;}
  else
    {result=buffer+55;}

  return result;
}// end function numPosCode
//##############################################################################

/*##############################################################*/
/* Old defines below not currently used in library              */
/* They can be used as simple GLCD commands examples            */
/*##############################################################*/

/* GLCD_Cmd_End; is macros for              */
/* Clock tact end and unset control pins:   */
/* 1. Operation clock falling edge          */
/* 2. DataInput bit = 0                     */
/* 3. ReadWrite bit = 0                     */
/* 4. Unselect left GLCD MCU, inverse       */
/* 5. Unselect right GLCD MCU, inverse      */
#define GLCD_Cmd_End do                     \
{                                           \
    GLCD_SET_E_0;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_1;                         \
    GLCD_SET_CS2_1;                         \
} while (0)

/* GLCD_Cmd_reset; is macros for            */
/* Reset GLCD MCUs before use:              */
/* Без ресета команды проходят,             */
/* но не срабатывают                        */
/* 1. Set reset output port direction       */
/* 2. Send inverse reset bit to port        */
/* 3. Wait for some time (may be no care)   */
/* 4. Unset inverse reset bit to port       */
#define GLCD_Cmd_Reset do                   \
{                                           \
    GLCD_DDR_CTRL;                          \
    GLCD_SET_RST_0;                         \
    wait(1);                                \
    GLCD_SET_RST_1;                         \
} while (0)

/* GLCD_Cmd_Display_On; is macros for       */
/* turn on GLCD display:                    */
/* 1. Set data output port direction        */
/* 2. Send display on command (0x3F)        */
/* 3. DataInput bit = 0                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Display_On do              \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    PORTA = 0x3F;                           \
    GLCD_DDR_CTRL;                          \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Display_Off; is macros for      */
/* turn off GLCD display:                   */
/* 1. Set data output port direction        */
/* 2. Send display on command (0x3F)        */
/* 3. DataInput bit = 0                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Display_Off do             \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    PORTA = 0x3E;                           \
    GLCD_DDR_CTRL;                          \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Status; is macros for           */
/* read status of GLCD display:             */
/* 1. Set data input port direction         */
/* 2. Send data input port R_pull-up off    */
/* 3. DataInput bit = 0                     */
/* 4. ReadWrite bit = 1                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Status do                  \
{                                           \
    GLCD_DDR_DATA_IN;                       \
    PORTA = 0x00;                           \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_1;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Set_Address; is macros for      */
/* set Y display coordinate (0-63):         */
/* 1. Set data output port direction        */
/* 2. Send address+command data             */
/* 3. DataInput bit = 0                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Set_Address do             \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    PORTA = 0x40 + 0;                       \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Set_Page; is macros for         */
/* set X display line (0-7):                */
/* 1. Set data output port direction        */
/* 2. Send page+command data                */
/* 3. DataInput bit = 0                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Set_Page do                \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    PORTA = 0xB8 + 0;                       \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Set_Start; is macros for        */
/* set Z display start line (0-63):         */
/* 1. Set data output port direction        */
/* 2. Send start line+command data          */
/* 3. DataInput bit = 0                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Set_Start do               \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    PORTA = 0xC0 + 0;                       \
    GLCD_SET_RS_0;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Set_Data; is macros for         */
/* set data byte for display at Y:8X+Z      */
/* 1. Set data output port direction        */
/* 2. Send data (0x00)                      */
/* 3. DataInput bit = 1                     */
/* 4. ReadWrite bit = 0                     */
/* 5. Select left GLCD MCU, inverse         */
/* 6. Select right GLCD MCU, inverse        */
/* 7. Operation clock rising edge           */
#define GLCD_Cmd_Set_Data do                \
{                                           \
    GLCD_DDR_DATA_OUT;                      \
    PORTA = 0x00;                           \
    GLCD_SET_RS_1;                          \
    GLCD_SET_RW_0;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/* GLCD_Cmd_Read_Data; is macros for        */
/* read data byte of display at Y:8X+Z      */
/* 1. Set data input port direction         */
/* 2. DataInput bit = 1                     */
/* 3. ReadWrite bit = 1                     */
/* 4. Select left GLCD MCU, inverse         */
/* 5. Select right GLCD MCU, inverse        */
/* 6. Operation clock rising edge           */
#define GLCD_Cmd_Read_Data do               \
{                                           \
    GLCD_DDR_DATA_IN;                       \
    GLCD_SET_RS_1;                          \
    GLCD_SET_RW_1;                          \
    GLCD_SET_CS1_0;                         \
    GLCD_SET_CS2_0;                         \
    GLCD_SET_E_1;                           \
} while (0)

/*##############################################################*/
/* End of Old defines (not currently used in library)           */
/*##############################################################*/