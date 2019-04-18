// #############################################################################
//
// ��������     : CAN_mcp2515_A_v01.�, �������� ���� ����������
// ������       : v01_2012-06-012
//
// ����������   : ���������� ��� ������ � CAN ����� ���������� CAN-SPI (mcp2515 ��� ������.)
// ���������    : ���������� ����� EasyPIC 6, ������ CAN-SPI
// �����������  : ������� Delay_us(), Delay_ms() ����� microC PRO for PIC
//                ���������� ����� CAN_typedef_v01.h
//                ���� �������� ���������� CAN_mcp2515_A_v01.h
//
// �����        : ������ �.�.
// �����������	: �.�. ����� (Ratgor) [�� ����������]
//
// #############################################################################

//�������� SPI ��������� (������ �� ���������)
unsigned char spi_putc_A(unsigned char byte)
{	
    unsigned char bit_;				//������� ����������/�������� ���
	
	//���� ����������� 8 ���. ����� ������� ����� ��� �� �������� ���� � ������� ��������
    for (bit_ = 0; bit_ < 8; bit_++) 
    {
        //���������� ������� �� ��������
		//���� MSB == 1, ������������� MOSI � 1
        if (byte & 0x80) 
            CAN_SET_MOSI_A();
		//����� � 0
        else
            CAN_CLR_MOSI_A();
        //������� ��� ��������, �� ��� ����� �������� ����� ��� �������    
        byte <<= 1;

        /* ��� �������� ��������� ������� ����� ��� ��� ������������� ����� */
        Delay_us(5);
		//���������� SCL � 1
        CAN_SET_SCL_A();

		//��������� ������� �� ����� MISO (�������� ������) 
		//� ���������� � ������ ��� (������ � ����� ������� ��� �������� � ���� � ������ 
		// ��� == 0
        byte |= CAN_TEST_MISO_A();

        /* ��� �������� ��������� ������� ����� ��� ��� ������������� ���� */
        Delay_us(5);
		//������������� SCL � 0
        CAN_CLR_SCL_A();

		//����������� ��� ���� bit_ != 0 (����� 8 ���)
    }
	//MOSI ������������� � 0
    CAN_CLR_MOSI_A();
    
	//���������� �������������� ���� �� ������� ���
    return byte;
}

//������ � CAN ���������� ����� ������ dat �� ������� adress
void mcp2515_write_register_A(unsigned char adress, unsigned char dat)
{
		//������� ���
        CAN_CLR_CS_A();

        spi_putc_A(SPI_WRITE);	//��������� ������� CAN ����������� �� ������ ������(������� � ������ � ��������)
        spi_putc_A(adress);		//����� ���� ������
        spi_putc_A(dat);		//������ ��� ������

		//��������� ���
        CAN_SET_CS_A();
}

// -------------------------------------------------------------------------
//������ �������� � ������� adress �� ����������� CAN
unsigned char mcp2515_read_register_A(unsigned char adress)
{
        unsigned char dat;

        CAN_CLR_CS_A();

        spi_putc_A(SPI_READ);	//������� �� ������ ��������
        spi_putc_A(adress);		//����� ��������� ��������

        dat = spi_putc_A(0xff);	//������ �������

        CAN_SET_CS_A();			

        return dat;
}

// -------------------------------------------------------------------------
//������� �� ����������� �������� ���������� � CAN �����������
//������������ �������, ����� �����, ����� �����(� ����� ���� ������������� � 1, ����� ���� ��������. 
//�������� 00100101 - ����� ���� �������� ������ ���� 0,2,5. �� ��� ��� ����� ����������(0 ��� 1), 
//������� � dat )
//� dat ��������� ��������, �� ������� ����� �������� ���� "���������" ������
void mcp2515_bit_modify_A(unsigned char adress, unsigned char mask, unsigned char dat)
{
        CAN_CLR_CS_A();

        spi_putc_A(SPI_BIT_MODIFY);
        spi_putc_A(adress);
        spi_putc_A(mask);
        spi_putc_A(dat);

        CAN_SET_CS_A();
}

// ----------------------------------------------------------------------------
//������ ������ �������� CAN �����������
unsigned char mcp2515_read_status_A(unsigned char type)
{
        unsigned char dat;

        CAN_CLR_CS_A();

        spi_putc_A(type);
        dat = spi_putc_A(0xff);

        CAN_SET_CS_A();

        return dat;
}

//������������� CAN ����������� ��� �������� �� ����� ����
char mcp2515_init_A(void)
{       
         //��������� �����/������
        SPI_TRIS_A &= ~((1<<CAN_CS)|(1<<CAN_SCL)|(1<<CAN_MOSI)|(1<<CAN_RST));
		SPI_TRIS_A |= (1<<CAN_MISO)|(1<<CAN_INT);
		CAN_SET_RST_A();
        CAN_CLR_CS_A();
	    CAN_CLR_SCL_A();
        CAN_CLR_MOSI_A();
        CAN_CLR_MISO_A();
		CAN_CLR_INT_A();
		
       
        // �������� ����� CAN �����������, ��������� �������
        CAN_CLR_CS_A();
        spi_putc_A(SPI_RESET);
        CAN_SET_CS_A();

        // �������� ��� ���������������
        Delay_us(10);

        //��������� � ���������������� �������� ������
        CAN_CLR_CS_A();
        //������� �� ������
        spi_putc_A(SPI_WRITE);
        //����� �������� ( ��� ��������- CNF3 ����� -0x28)
        spi_putc_A(CNF3);

        //��������� ���� �� ������� ��� ������ ������� ����� � ���� ��������
        spi_putc_A((1<<PHSEG21));                // Bitrate 125 kbps at 16 MHz
        //��������� ���� ������� � ������� � ������� �� 1 ������ 0x29 (CNF2)
        spi_putc_A((1<<BTLMODE)|(1<<PHSEG11));
        //��� �� 1 ������ ����� (� CNF1) 0x30
        spi_putc_A((1<<BRP2)|(1<<BRP1)|(1<<BRP0));

        //��������� 0x31 (CANINTE) �������� ����������
        spi_putc_A((1<<RX1IE)|(1<<RX0IE));
        CAN_SET_CS_A();

        //������ CNF1 � ������� � ��� ��� �������
		//���� ���������, �� ���� ������, ���� ���, �� ������� 0
        if (mcp2515_read_register_A(CNF1) != ((1<<BRP2)|(1<<BRP1)|(1<<BRP0))) {
                return 0;
        }
		//������������� ��������� �� ������

        // deaktivate the RXnBF Pins (High Impedance State)
        mcp2515_write_register_A(BFPCTRL, 0);

        // set TXnRTS as inputs
        mcp2515_write_register_A(TXRTSCTRL, 0);

        // turn off filters => receive any message
        mcp2515_write_register_A(RXB0CTRL, (1<<RXM1)|(1<<RXM0));
        mcp2515_write_register_A(RXB1CTRL, (1<<RXM1)|(1<<RXM0));

        // reset device to normal mode
        mcp2515_write_register_A(CANCTRL, 0);

        return 1;
}

// ----------------------------------------------------------------------------
//�������� ���������. ��������� tCAN �������� ����������������� ����, ���� ��������� ��������
//����� ������, ������.
unsigned char mcp2515_send_message_A(tCAN *message)
{
        unsigned char status;
        unsigned char length;
        unsigned char i;
        unsigned char address;
		
		//������ �������� STATUS ��� ����������� ���������� ������
        status = mcp2515_read_status_A(SPI_READ_STATUS);

        /* Statusbyte:
         *
         * Bit        Function
         *  2        TXB0CNTRL.TXREQ
         *  4        TXB1CNTRL.TXREQ
         *  6        TXB2CNTRL.TXREQ
         */

		//���� ��� 2 �� ������� = 0, �� �������� ������ ����� � ������� 0x00
        if (!(status&(1<<2))) {
                address = 0x00;
        }
		//���� ��� 4 �� ������� = 0, �� �������� ������ ����� � ������� 0x02
        else if (!(status&(1<<4))) {
                address = 0x02;
        }
		//���� ��� 6 �� ������� = 0, �� �������� ������ ����� � ������� 0x04
        else if (!(status&(1<<6))) {
                address = 0x04;
        }
		//����� ��� ������
        else {
                // all buffer used => could not send message
                return 0;
        }
		
		//������� �� ������ �����(������� ����� �����) � ����� ������(������� �����)
        CAN_CLR_CS_A();
        spi_putc_A(SPI_WRITE_TX | address);
		//
		//�������� ID � ������� ������������ �����
        spi_putc_A(message->id >> 3);
        spi_putc_A(message->id << 5);
		//������ ���� 0 ��� ������������� � ����������� ������ 
        spi_putc_A(0);
        spi_putc_A(0);

		
        length = message->header.length & 0x0f;

		//�������� ����� ������ � ���������� �������
		//0 - �������� ������; 1 - ������ ������
        if (message->header.rtr) {
                // a rtr-frame has a length, but contains no data
                spi_putc_A((1<<RTR) | length);
        }
		//���� rtr == 0, �� �������� ������
		//��������� ����� � ���� ������
        else {
                // set message length
                spi_putc_A(length);

                // ���� ������ � �����
                for (i=0;i<length;i++) {
                        spi_putc_A(message->dat[i]);
                }
        }
		//��������� ���.
       CAN_SET_CS_A();

        Delay_us(1);

        // ��������� ������� ����������� �� �������� ��������� �� ������
        CAN_CLR_CS_A();
		//��������� �� ������ ����� ������ � 
        address = (address == 0) ? 1 : address;
		//���������� ����� ������ ������ � �������� �� ��������
        spi_putc_A(SPI_RTS | address);
        CAN_SET_CS_A();

        return address;
}

//�������� �������� ��������� �� ������ CAN �����������
unsigned char mcp2515_get_message_A(tCAN *message)
{
        // read status
        unsigned char status;
        unsigned char addr;
        unsigned char length;
        unsigned char i;

		//������ ������� ��� �� ����� �� ����� ������ ��������� ������
        status = mcp2515_read_status_A(SPI_RX_STATUS);

		//�������� �������� ����� ���
        if (status&(1<<6)) 
                // message in buffer 0
                addr = SPI_READ_RX;
        else if (status&(1<<7))
                // message in buffer 1
                addr = SPI_READ_RX | 0x04;

        else {
                // Error: no message available
                return 0;
        }
		//�������� ����� ������ � ������� 
        CAN_CLR_CS_A();
        spi_putc_A(addr);

        // ������ ��� ������
        message->id  = (unsigned int) spi_putc_A(0xff) << 3;
        message->id |=                spi_putc_A(0xff) >> 5;

        //������ ������������ ��������������
        spi_putc_A(0xff);
        spi_putc_A(0xff);

        // read DLC
        length = spi_putc_A(0xff) & 0x0f;

        message->header.length = length;
        message->header.rtr = (status&(1<<3)) ? 1 : 0;

        // read data
        for (i=0;i<length;i++) {
                message->dat[i] = spi_putc_A(0xff);
        }
        CAN_SET_CS_A();

        // clear interrupt flag
        if (status&(1<<6)) {
                mcp2515_bit_modify_A(CANINTF, (1<<RX0IF), 0);
        }
        else {
                mcp2515_bit_modify_A(CANINTF, (1<<RX1IF), 0);
        }

        return (status & 0x07) + 1;
}


// ----------------------------------------------------------------------------
// check if there are any new messages waiting

unsigned char mcp2515_check_message_A(void) 
{
        return (!CAN_TEST_INT_A());
}