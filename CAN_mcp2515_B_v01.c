// #############################################################################
//
// Название     : CAN_mcp2515_B_v01.с, основной файл библиотеки
// Версия       : v01_2012-06-012
//
// Назначение   : Библиотека для работы с CAN через контроллер CAN-SPI (mcp2515 или аналог.)
// Платформа    : Отладочная плата EasyPIC 6, модули CAN-SPI
// Зависимости  : Функции Delay_us(), Delay_ms() среды microC PRO for PIC
//                Библиотека типов CAN_typedef_v01.h
//                Файл настроек библиотеки CAN_mcp2515_B_v01.h
//
// Автор        : Змушко Е.С.
// Рефакторинг	: А.В. Цурко (Ratgor) [не проводился]
//
// #############################################################################

//все точно такое же как и для контроллера A. отличие в хидере в порте подключения
// и в названиях функций.
unsigned char spi_putc_B(unsigned char byte)
{
    unsigned char bit_;

    for (bit_ = 0; bit_ < 8; bit_++) 
    {
        /* записать MOSI по спаду предыдущего тактового импульса */
        if (byte & 0x80)
            CAN_SET_MOSI_B();
        else
            CAN_CLR_MOSI_B();
            
        byte <<= 1;

        /* ждём половину тактового периода перед тем как сгенерировать фронт */
        Delay_us(5);

        CAN_SET_SCL_B();
        byte |= CAN_TEST_MISO_B();
        /* ждём половину тактового периода перед тем как сгенерировать спад */
        Delay_us(5);

        CAN_CLR_SCL_B();
    }
    CAN_CLR_MOSI_B();
    
    return byte;
}

void mcp2515_write_register_B(unsigned char adress, unsigned char dat)
{
        CAN_CLR_CS_B();

        spi_putc_B(SPI_WRITE);
        spi_putc_B(adress);
        spi_putc_B(dat);

        CAN_SET_CS_B();
}

// -------------------------------------------------------------------------
unsigned char mcp2515_read_register_B(unsigned char adress)
{
        unsigned char dat;

        CAN_CLR_CS_B();

        spi_putc_B(SPI_READ);
        spi_putc_B(adress);

        dat = spi_putc_B(0xff);

        CAN_SET_CS_B();

        return dat;
}

// -------------------------------------------------------------------------
void mcp2515_bit_modify_B(unsigned char adress, unsigned char mask, unsigned char dat)
{
        CAN_CLR_CS_B();

        spi_putc_B(SPI_BIT_MODIFY);
        spi_putc_B(adress);
        spi_putc_B(mask);
        spi_putc_B(dat);

        CAN_SET_CS_B();
}

// ----------------------------------------------------------------------------
unsigned char mcp2515_read_status_B(unsigned char type)
{
        unsigned char dat;

        CAN_CLR_CS_B();

        spi_putc_B(type);
        dat = spi_putc_B(0xff);

        CAN_SET_CS_B();

        return dat;
}

char mcp2515_init_B(void)
{       
         //настроить на выход
        SPI_TRIS_B &= ~((1<<CAN_CS)|(1<<CAN_SCL)|(1<<CAN_MOSI)|(1<<CAN_RST));
                SPI_TRIS_B |= (1<<CAN_MISO)|(1<<CAN_INT);
                CAN_SET_RST_B();
        CAN_CLR_CS_B();
            CAN_CLR_SCL_B();
        CAN_CLR_MOSI_B();
        CAN_CLR_MISO_B();
                CAN_CLR_INT_B();
                
       
/*        //включить SPI как мастер
        SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(1<<SPR0);
        SPSR = 0;     */

        // ресет выводом и отправка команды 0хС0 на ресет b'11000000'
        CAN_CLR_CS_B();
        spi_putc_B(SPI_RESET);
        CAN_SET_CS_B();

        // wait a little bit until the MCP2515 has restarted
        Delay_us(10);

        // load CNF1..3 Register
        CAN_CLR_CS_B();
        //команда на запись 0х02
        spi_putc_B(SPI_WRITE);
        //#define CNF3                0x28
        spi_putc_B(CNF3);

        //пишем в CNF3
        spi_putc_B((1<<PHSEG21));                // Bitrate 125 kbps at 16 MHz
        //в CNF2
        spi_putc_B((1<<BTLMODE)|(1<<PHSEG11));
        //В CNF1
        spi_putc_B((1<<BRP2)|(1<<BRP1)|(1<<BRP0));

        //CANINTE включить прерывания
        spi_putc_B((1<<RX1IE)|(1<<RX0IE));
        CAN_SET_CS_B();

        //читаем CNF1 и сверяем с тем что записал
        if (mcp2515_read_register_B(CNF1) != ((1<<BRP2)|(1<<BRP1)|(1<<BRP0))) {
                return 0;
        }

        // deaktivate the RXnBF Pins (High Impedance State)
        mcp2515_write_register_B(BFPCTRL, 0);

        // set TXnRTS as inputs
        mcp2515_write_register_B(TXRTSCTRL, 0);

        // turn off filters => receive any message
        mcp2515_write_register_B(RXB0CTRL, (1<<RXM1)|(1<<RXM0));
        mcp2515_write_register_B(RXB1CTRL, (1<<RXM1)|(1<<RXM0));

        // reset device to normal mode
        mcp2515_write_register_B(CANCTRL, 0);

        return 1;
}

// ----------------------------------------------------------------------------
unsigned char mcp2515_send_message_B(tCAN *message)
{
        unsigned char status;
        unsigned char length;
        unsigned char i;
        unsigned char address;

        status = mcp2515_read_status_B(SPI_READ_STATUS);

        /* Statusbyte:
         *
         * Bit        Function
         *  2        TXB0CNTRL.TXREQ
         *  4        TXB1CNTRL.TXREQ
         *  6        TXB2CNTRL.TXREQ
         */


        if (!(status&(1<<2))) {
                address = 0x00;
        }
        else if (!(status&(1<<4))) {
                address = 0x02;
        }
        else if (!(status&(1<<6))) {
                address = 0x04;
        }
        else {
                // all buffer used => could not send message
                return 0;
        }

        CAN_CLR_CS_B();
        spi_putc_B(SPI_WRITE_TX | address);

        spi_putc_B(message->id >> 3);
        spi_putc_B(message->id << 5);

        spi_putc_B(0);
        spi_putc_B(0);


        length = message->header.length & 0x0f;

        if (message->header.rtr) {
                // a rtr-frame has a length, but contains no data
                spi_putc_B((1<<RTR) | length);
        }
        else {
                // set message length
                spi_putc_B(length);

                // data
                for (i=0;i<length;i++) {
                        spi_putc_B(message->dat[i]);
                }
        }
       CAN_SET_CS_B();

        Delay_us(1);

        // send message
        CAN_CLR_CS_B();
        address = (address == 0) ? 1 : address;
        spi_putc_B(SPI_RTS | address);
        CAN_SET_CS_B();

        return address;
}

unsigned char mcp2515_get_message_B(tCAN *message)
{
        // read status
        unsigned char status;
        unsigned char addr;
        unsigned char length;
        unsigned char i;

        status = mcp2515_read_status_B(SPI_RX_STATUS);

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

        CAN_CLR_CS_B();
        spi_putc_B(addr);

        // read id
        message->id  = (unsigned int) spi_putc_B(0xff) << 3;
        message->id |=                spi_putc_B(0xff) >> 5;

        //чтение расширенного идентификатора
        spi_putc_B(0xff);
        spi_putc_B(0xff);

        // read DLC
        length = spi_putc_B(0xff) & 0x0f;

        message->header.length = length;
        message->header.rtr = (status&(1<<3)) ? 1 : 0;

        // read data
        for (i=0;i<length;i++) {
                message->dat[i] = spi_putc_B(0xff);
        }
        CAN_SET_CS_B();

        // clear interrupt flag
        if (status&(1<<6)) {
                mcp2515_bit_modify_B(CANINTF, (1<<RX0IF), 0);
        }
        else {
                mcp2515_bit_modify_B(CANINTF, (1<<RX1IF), 0);
        }

        return (status & 0x07) + 1;
}


// ----------------------------------------------------------------------------
// check if there are any new messages waiting

unsigned char mcp2515_check_message_B(void) 
{
        return (!CAN_TEST_INT_B());
}