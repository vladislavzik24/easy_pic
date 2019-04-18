// #############################################################################
//
// ��������     : CLCD_Screen_Settings_v05, ���� ��������.
// ����������   : ���������� ��� ������ � ����������� 64�128 LCD (WDG0151 ��� ���������)
// ���������    : ���������� ����� EasyPIC 6
// �����������  : ������� Delay_us(), Delay_ms() ����� microC PRO for PIC
//                ���������� mikroE FLASH � mikroE C_Math ����� microC PRO for PIC
//                �������� ���� ���������� CLGD_Screen_v04.h
//
// �����        : ����� �.�. (Ratgor), ��������� �.�.
// �����������        : �.�. ����� (Ratgor) [�� ����������]
//
// #############################################################################
//
// ��������:
//
//          ������ ������������ ����� _����������_,
//          ����������� �������� �����, ������ � ����� �� ����������� ��-�������
//
//          ���������� ������:
//          1) ��������� ����� �� �����������
//          2) ��������� ����� (8 �������� �� ��������� �����) �� �����������
//          3) ��������� ������� (��������, ���������, �����, ����� ����������)
//          4) ��������� ������ ��������
//          5) ��������� ������ �����
//          6) ������������� ������
//          7) ������������� ����������
//          8) ���� ������
//          9) ���� �����������
//          10) ������ �����
//          11) ����������� �����
//          12) �������������� ����� ������
//          13) �������������� ����� � ������ (�������������� �������)
//          14) �������� � ������ (������� ��� ����������� ���������� ��������)
//
// ����������:
//
//          1) �������� ���������� (��� ��������� ������)
//             � ������������� (AVR -> PIC) ������� �� ������� �������
//          2) ������� ��������� ����� �� ������������� ����� ������������,
//             ������ ��������� ������� �� C_Math ����������
//          3) ������ 0.3 ��������� �� 0.4 ����� ����������� ������� ���������
//             �����. ������ 0.4 -> 0.3� ��������� ��������� ����� ��������
//          4) ������ 0.4 ��������� �� 0.5 ����� ���������� ������� ������
//             ������. ������ 0.5 -> 0.4� ��������� ��������� ����� ��������
//
// #############################################################################
//
// -----------------------------------------------------------------------------
// ������� ��������� �������:
// -----------------------------------------------------------------------------
// void GLCD_Draw_Pixel (unsigned char X_pos, unsigned char Y_pos, unsigned char color)
// ��������� �������:
// ���������� �� x (����� �������: 0-127)
// ���������� �� y (������ ����: 0-63)
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ����� (�� ������� �� y: 8 ����� �� 8 ��������):
// -----------------------------------------------------------------------------
// void GLCD_Draw_Byte (unsigned char X_pos, unsigned char Y_pos, unsigned char data_byte, unsigned char color)
// ��������� �������:
// ���������� �� x (����� �������: 0-127)
// ���������� �� y, ����� ������ (������ ����: 0-7)
// ����� �������, ������ ���� ����������� ��� ���������� '1', �������� ��� ���������� '0'
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ������� (8 ����� �� 18 ��������):
// -----------------------------------------------------------------------------
// void GLCD_Draw_Char (unsigned char X_pos, unsigned char Y_pos, unsigned char data_char, unsigned char color)
// ��������� �������:
// ���������� �� x (����� �������: 0-127)
// ���������� �� y, ����� ������ (������ ����: 0-7)
// �������� ������ 5x7 � ������ 7x8, ��������������: A-z, �-�, 0-9, .,!?:;" � �.�.
// ���� �������: 0-������, 1-�����
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ������ (8 ����� �� 18 ��������):
// -----------------------------------------------------------------------------
// void GLCD_Draw_String (unsigned char X_pos, unsigned char Y_pos, char *data_string, unsigned char color)
// ��������� �������:
// ���������� �� x (����� �������: 0-127)
// ���������� �� y, ����� ������ (������ ����: 0-7)
// �������� ������ ��������, ��������� �� ������ ������� ���������� ������� ������� �� 255 �������� (������� - ��. ����)
// ���� �������: 0-������, 1-�����
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� �����:
// -----------------------------------------------------------------------------
// void GLCD_Draw_Line(unsigned char X0_pos, unsigned char Y0_pos, unsigned char X1_pos,unsigned char Y1_pos, unsigned char coolor)                         
// ��������� �������:
// ���������� ������ ����� �� x (����� �������: 0-127)
// ���������� ������ ����� �� y (������ ����: 0-63)
// ���������� ������ ����� �� x (����� �������: 0-127)
// ���������� ������ ����� �� y (������ ����: 0-63)
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ������� ��������������:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Rectangle(unsigned char X0_pos, unsigned char Y0_pos, unsigned char X1_pos, unsigned char Y1_pos, unsigned char coolor)
// ��������� �������:
// ���������� �������� ������ ���� �� x (����� �������: 0-127)
// ���������� �������� ������ ���� �� y (������ ����: 0-63)
// ���������� ������� ������� ���� �� x (����� �������: 0-127)
// ���������� ������� ������� ���� �� y (������ ����: 0-63)
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ������������ ��������������:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Rectangle_Fill(unsigned char X0_pos, unsigned char Y0_pos, unsigned char X1_pos, unsigned char Y1_pos, unsigned char coolor)
// ��������� �������:
// ���������� �������� ������ ���� �� x (����� �������: 0-127)
// ���������� �������� ������ ���� �� y (������ ����: 0-63)
// ���������� ������� ������� ���� �� x (����� �������: 0-127)
// ���������� ������� ������� ���� �� y (������ ����: 0-63)
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ������ ����������:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Circle(unsigned char X0_pos, unsigned char Y0_pos, unsigned char Radius, unsigned char coolor)
// ��������� �������:
// ���������� ������ ���������� �� x (����� �������: 0-127)
// ���������� ������ ���������� �� y (������ ����: 0-63)
// ������ ���������� (�� ������ ��������� ���������� ������ �� x)
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ��������� ����������� ����������:
// -----------------------------------------------------------------------------
// void Glcd_Draw_Circle_Fill(unsigned char X0_pos, unsigned char Y0_pos, unsigned char Radius, unsigned char coolor)
// ��������� �������:
// ���������� ������ ���������� �� x (����� �������: 0-127)
// ���������� ������ ���������� �� y (������ ����: 0-63)
// ������ ���������� (�� ������ ��������� ���������� ������ �� x)
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ������� ������ (��������� ������� ������):
// -----------------------------------------------------------------------------
// void GLCD_Clear()
// ��������� �������:
// ��� ����������.
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ������� ������ (��������� ������ ������):
// -----------------------------------------------------------------------------
// void GLCD_Fill()
// ��������� �������:
// ��� ����������.
// ���� �������: 0-������, 1-�����, 2-�������������
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� ������ ������:
// -----------------------------------------------------------------------------
// void GLCD_Move_Screen (signed char step)
// ��������� �������:
// step: ���, �� ������� ����� ������� ����� ������, � �������� (-128..127)
// ������� ���������� ������.
//
// -----------------------------------------------------------------------------
// ������� �������������� ����� � ������:
// -----------------------------------------------------------------------------
// unsigned char numPosCode (unsigned char number, unsigned char position, unsigned char radix)
// ��������� �������:
// ����� ��� �������������� (�����������: 0-255)
// ������� ������������� ������ � ����� (0-max, ������� �� ������� ���������)
// ������� ��������� (��������: 10-����������, 16-�����������������)
// ������� ���������� ASCII ��� ����� � ����� �� ����������� �������.
//
// -----------------------------------------------------------------------------
// ������� �������� � ����������� ���������:
// -----------------------------------------------------------------------------
// void wait(int tact)
// ��������� �������:
// ���������� ������ �������� (0-65535)
// ������� ���������� ������.
//
// #############################################################################



/*##############################################################*/
/* begin of TUNEABLE part of the library                        */
/*##############################################################*/

#define GLCD_PORT_D7    PORTD    // ���� ��� ��������� ������ ���� ������
#define GLCD_PORT_D6    PORTD
#define GLCD_PORT_D5    PORTD
#define GLCD_PORT_D4    PORTD
#define GLCD_PORT_D3    PORTD
#define GLCD_PORT_D2    PORTD
#define GLCD_PORT_D1    PORTD
#define GLCD_PORT_D0    PORTD
#define GLCD_PORT_CS1   PORTB    // ���� ��� ������ ������ ���� ����������
#define GLCD_PORT_CS2   PORTB
#define GLCD_PORT_RS    PORTB
#define GLCD_PORT_RW    PORTB
#define GLCD_PORT_E     PORTB
#define GLCD_PORT_RST   PORTB

#define GLCD_DDR_D7     TRISD    // ��������� ����������� ����� ������ ���� ������
#define GLCD_DDR_D6     TRISD
#define GLCD_DDR_D5     TRISD
#define GLCD_DDR_D4     TRISD
#define GLCD_DDR_D3     TRISD
#define GLCD_DDR_D2     TRISD
#define GLCD_DDR_D1     TRISD
#define GLCD_DDR_D0     TRISD
#define GLCD_DDR_CS1    TRISB    // ��������� ����������� ����� ������ ���� ����������
#define GLCD_DDR_CS2    TRISB
#define GLCD_DDR_RS     TRISB
#define GLCD_DDR_RW     TRISB
#define GLCD_DDR_E      TRISB
#define GLCD_DDR_RST    TRISB

#define GLCD_PIN_D7     PORTD    // ���� ��� ��������� ������ ���� ������
#define GLCD_PIN_D6     PORTD
#define GLCD_PIN_D5     PORTD
#define GLCD_PIN_D4     PORTD
#define GLCD_PIN_D3     PORTD
#define GLCD_PIN_D2     PORTD
#define GLCD_PIN_D1     PORTD
#define GLCD_PIN_D0     PORTD
#define GLCD_PIN_CS1    PORTB    // ���� ��� ������ ������ ���� ����������
#define GLCD_PIN_CS2    PORTB
#define GLCD_PIN_RS     PORTB
#define GLCD_PIN_RW     PORTB
#define GLCD_PIN_E      PORTB
#define GLCD_PIN_RST    PORTB

#define GLCD_BIT_D7     7        // ������� ������ ���� ������ � ��������������� �����
#define GLCD_BIT_D6     6
#define GLCD_BIT_D5     5
#define GLCD_BIT_D4     4
#define GLCD_BIT_D3     3
#define GLCD_BIT_D2     2
#define GLCD_BIT_D1     1
#define GLCD_BIT_D0     0
#define GLCD_BIT_CS1    0        // ������� ������ ���� ���������� � ��������������� �����
#define GLCD_BIT_CS2    1
#define GLCD_BIT_RS     2
#define GLCD_BIT_RW     3
#define GLCD_BIT_E      4
#define GLCD_BIT_RST    5

/* GLCD_DELAY; is macros for                */
/* set default delays in tacts              */
/* in functions GLCD_Draw_...               */
#define GLCD_DELAY 1

/*##############################################################*/
/* end of TUNEABLE part of the library                          */
/*##############################################################*/