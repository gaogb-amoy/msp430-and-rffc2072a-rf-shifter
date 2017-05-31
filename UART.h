
#define CMD_RESET 0x01
#define CMD_IBTY  0x02
#define CMD_OBTY  0x03

void UART_tx_rgst_data(char addr, int rdata);

void UART_init();
void UART_send_string(char * str);
void UART_send_addr_data(char addr, int rdata);