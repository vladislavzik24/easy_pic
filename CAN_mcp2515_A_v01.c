// #############################################################################
//
// Название     : CAN_mcp2515_A_v01.с, основной файл библиотеки
// Версия       : v01_2012-06-012
//
// Назначение   : Библиотека для работы с CAN через контроллер CAN-SPI (mcp2515 или аналог.)
// Платформа    : Отладочная плата EasyPIC 6, модули CAN-SPI
// Зависимости  : Функции Delay_us(), Delay_ms() среды microC PRO for PIC
//                Библиотека типов CAN_typedef_v01.h
//                Файл настроек библиотеки CAN_mcp2515_A_v01.h
//
// Автор        : Змушко Е.С.
// Рефакторинг	: А.В. Цурко (Ratgor) [не проводился]
//
// #############################################################################

//софтовый SPI интерфейс (пример из википедии)
unsigned char spi_putc_A(unsigned char byte)
{	
    unsigned char bit_;				//счетчик переданных/принятых бит
	
	//цикл повторяется 8 раз. Ровно столько нужно что бы передать байт и принять входящий
    for (bit_ = 0; bit_ < 8; bit_++) 
    {
        //передавать начинам со старшего
		//если MSB == 1, устанавливаем MOSI В 1
        if (byte & 0x80) 
            CAN_SET_MOSI_A();
		//иначе в 0
        else
            CAN_CLR_MOSI_A();
        //старший бит передали, на его место сдвигаем перед ним стоящий    
        byte <<= 1;

        /* ждём половину тактового периода перед тем как сгенерировать фронт */
        Delay_us(5);
		//установить SCL В 1
        CAN_SET_SCL_A();

		//считываем уровень на линии MISO (входящие данные) 
		//и записываем в первый бит (данные к этому моменту уже сдвинуты в лево и первый 
		// бит == 0
        byte |= CAN_TEST_MISO_A();

        /* ждём половину тактового периода перед тем как сгенерировать спад */
        Delay_us(5);
		//устанавливаем SCL в 0
        CAN_CLR_SCL_A();

		//повторяется еще пока bit_ != 0 (всего 8 раз)
    }
	//MOSI устанавливаем в 0
    CAN_CLR_MOSI_A();
    
	//возвращаем сформированный байт из входных бит
    return byte;
}

//запись в CAN контроллер байта данных dat по адрессу adress
void mcp2515_write_register_A(unsigned char adress, unsigned char dat)
{
		//выбрать чип
        CAN_CLR_CS_A();

        spi_putc_A(SPI_WRITE);	//отправить команду CAN контроллеру на запись данных(описаны в хидере и даташите)
        spi_putc_A(adress);		//адрес куда писать
        spi_putc_A(dat);		//данный для записи

		//отпустить чип
        CAN_SET_CS_A();
}

// -------------------------------------------------------------------------
//чтение регистра с адресом adress из контроллера CAN
unsigned char mcp2515_read_register_A(unsigned char adress)
{
        unsigned char dat;

        CAN_CLR_CS_A();

        spi_putc_A(SPI_READ);	//команда на чтение регистра
        spi_putc_A(adress);		//адрес читаемого регистра

        dat = spi_putc_A(0xff);	//читаем регитср

        CAN_SET_CS_A();			

        return dat;
}

// -------------------------------------------------------------------------
//команда на модификацию регистра управления в CAN контроллере
//отправляется команда, потом адрес, потом маска(в маске биты установленные в 1, могут быть изменены. 
//Например 00100101 - могут быть изменены только биты 0,2,5. На что они будут изменяться(0 или 1), 
//указано в dat )
//в dat находятся значения, на которые будут изменены биты "выбранные" маской
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
//чтение статус регистра CAN контроллера
unsigned char mcp2515_read_status_A(unsigned char type)
{
        unsigned char dat;

        CAN_CLR_CS_A();

        spi_putc_A(type);
        dat = spi_putc_A(0xff);

        CAN_SET_CS_A();

        return dat;
}

//инициализация CAN контроллера для передачи по витой паре
char mcp2515_init_A(void)
{       
         //настроить входы/выходы
        SPI_TRIS_A &= ~((1<<CAN_CS)|(1<<CAN_SCL)|(1<<CAN_MOSI)|(1<<CAN_RST));
		SPI_TRIS_A |= (1<<CAN_MISO)|(1<<CAN_INT);
		CAN_SET_RST_A();
        CAN_CLR_CS_A();
	    CAN_CLR_SCL_A();
        CAN_CLR_MOSI_A();
        CAN_CLR_MISO_A();
		CAN_CLR_INT_A();
		
       
        // софтовый ресет CAN контроллера, отправкой команды
        CAN_CLR_CS_A();
        spi_putc_A(SPI_RESET);
        CAN_SET_CS_A();

        // ожидание для реинициализации
        Delay_us(10);

        //загрузить в конфигурационные регистры данные
        CAN_CLR_CS_A();
        //команды на запись
        spi_putc_A(SPI_WRITE);
        //адрес регитсра ( его название- CNF3 адрес -0x28)
        spi_putc_A(CNF3);

        //следуюший байт за адресом это данные которые будут в него записаны
        spi_putc_A((1<<PHSEG21));                // Bitrate 125 kbps at 16 MHz
        //следующий байт пишется в регистр с адресом на 1 больще 0x29 (CNF2)
        spi_putc_A((1<<BTLMODE)|(1<<PHSEG11));
        //еще на 1 больше адрес (В CNF1) 0x30
        spi_putc_A((1<<BRP2)|(1<<BRP1)|(1<<BRP0));

        //следующий 0x31 (CANINTE) включить прерывания
        spi_putc_A((1<<RX1IE)|(1<<RX0IE));
        CAN_SET_CS_A();

        //читаем CNF1 и сверяем с тем что записал
		//если совпадает, то идем дальше, если нет, то вернуть 0
        if (mcp2515_read_register_A(CNF1) != ((1<<BRP2)|(1<<BRP1)|(1<<BRP0))) {
                return 0;
        }
		//инициализация регистрво по одному

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
//передача сообщения. Структура tCAN содержит идентификационное поле, поле удаленног озапроса
//длину данных, данные.
unsigned char mcp2515_send_message_A(tCAN *message)
{
        unsigned char status;
        unsigned char length;
        unsigned char i;
        unsigned char address;
		
		//чтение регистра STATUS для определения свободного буферу
        status = mcp2515_read_status_A(SPI_READ_STATUS);

        /* Statusbyte:
         *
         * Bit        Function
         *  2        TXB0CNTRL.TXREQ
         *  4        TXB1CNTRL.TXREQ
         *  6        TXB2CNTRL.TXREQ
         */

		//если бит 2 из статуса = 0, то свободен первый буфер с адресом 0x00
        if (!(status&(1<<2))) {
                address = 0x00;
        }
		//если бит 4 из статуса = 0, то свободен второй буфер с адресом 0x02
        else if (!(status&(1<<4))) {
                address = 0x02;
        }
		//если бит 6 из статуса = 0, то свободен третий буфер с адресом 0x04
        else if (!(status&(1<<6))) {
                address = 0x04;
        }
		//иначе все занято
        else {
                // all buffer used => could not send message
                return 0;
        }
		
		//команда на запись буфер(старшая часть байта) и адрес буфера(младшая часть)
        CAN_CLR_CS_A();
        spi_putc_A(SPI_WRITE_TX | address);
		//
		//передаем ID в формате стандартного кадра
        spi_putc_A(message->id >> 3);
        spi_putc_A(message->id << 5);
		//должны быть 0 для совместимости с расширенным кадром 
        spi_putc_A(0);
        spi_putc_A(0);

		
        length = message->header.length & 0x0f;

		//отправка длины данных и удаленного запроса
		//0 - передача данных; 1 - запрос данных
        if (message->header.rtr) {
                // a rtr-frame has a length, but contains no data
                spi_putc_A((1<<RTR) | length);
        }
		//если rtr == 0, то передаем данные
		//отправлем длину и сами данные
        else {
                // set message length
                spi_putc_A(length);

                // флем данные в буфер
                for (i=0;i<length;i++) {
                        spi_putc_A(message->dat[i]);
                }
        }
		//отпустить чип.
       CAN_SET_CS_A();

        Delay_us(1);

        // отправить команду контроллеру на отправку сообщения из буфера
        CAN_CLR_CS_A();
		//вычисляем из адреса номер буфера и 
        address = (address == 0) ? 1 : address;
		//отправляем номер буфера вместе с командой на отправку
        spi_putc_A(SPI_RTS | address);
        CAN_SET_CS_A();

        return address;
}

//получить принятое сообщение из буфера CAN контроллера
unsigned char mcp2515_get_message_A(tCAN *message)
{
        // read status
        unsigned char status;
        unsigned char addr;
        unsigned char length;
        unsigned char i;

		//чтение статуса что бы узнат ьв каком буфере сохранены данные
        status = mcp2515_read_status_A(SPI_RX_STATUS);

		//входязих регистра всего два
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
		//оправить адрес буфера с данными 
        CAN_CLR_CS_A();
        spi_putc_A(addr);

        // начало его чтения
        message->id  = (unsigned int) spi_putc_A(0xff) << 3;
        message->id |=                spi_putc_A(0xff) >> 5;

        //чтение расширенного идентификатора
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