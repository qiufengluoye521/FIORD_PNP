#include "Debug_Print.h"
#include <stdarg.h>
#include "stdio.h"

//==================================================================================================
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
/* printk's without a loglevel use this.. */
#define DEFAULT_MESSAGE_LOGLEVEL 5 /* KERN_WARNING */

/* We show everything that is MORE important than this.. */
#define MINIMUM_CONSOLE_LOGLEVEL 0 /* Minimum loglevel we let people use */
#define DEFAULT_CONSOLE_LOGLEVEL 4 /* anything MORE serious than KERN_DEBUG */

//==================================================================================================

extern UART_HandleTypeDef huart1;

////static uint8_t debugPort = 0;
unsigned char hex_tab[]={'0','1','2','3','4','5','6','7',\
                        '8','9','a','b','c','d','e','f'};
int console_printk[4] = {
    DEFAULT_MESSAGE_LOGLEVEL,   /* default_message_loglevel */
    DEFAULT_MESSAGE_LOGLEVEL,   /* default_message_loglevel */
    MINIMUM_CONSOLE_LOGLEVEL,   /* minimum_console_loglevel */
    DEFAULT_CONSOLE_LOGLEVEL,   /* default_console_loglevel */
};


#ifdef __GNUC__

int _io_PutChar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#else
#define PUTCHAR_PROTOTYPE           int fputc(int ch, FILE *f)
#endif

/*
uartError_t BSP_DEBUG_Init(uint8_t uartId, const uartPara_t *debugUartPara)
{
    uartError_t err = UART_NO_ERR;

    if (uartId >= MAX_UART_PORT || NULL == debugUartPara) {
        return UART_ERR_INPUT_PARA;
    }
    // 初始化时默认先打印warning以上(不含)的信息，需要修改为CAN总线发送，Flash读取方式
    console_printk[0] = DEFAULT_MESSAGE_LOGLEVEL;

    debugPort = uartId;
    err = BSP_UART_Init(debugPort, debugUartPara);

    return err;
}
*/

static int outc(int c) 
{
    _io_PutChar(c);
    return 0;
}

//asmlinkage int vprintk(const char *fmt, va_list args)
int vprintk(const char *fmt, va_list args)
{
    int printed_len;
    char *p;
    static char printk_buf[MAX_NUMBER_BYTES];
    static int log_level_unknown = 1;
    static int msg_level = -1;
    int loglev_char;

    printed_len = vsprintf(printk_buf, fmt, args);
    for (p = printk_buf; *p; p++)
    {
        if (log_level_unknown)
        {
            /* log_level_unknown signals the start of a new line */
            if (p[0] == '<' && p[1] >='0' &&
               p[1] <= '7' && p[2] == '>') {
                loglev_char = p[1];
                p += 3;
                printed_len -= 3;
            } else {
                loglev_char = DEFAULT_MESSAGE_LOGLEVEL
                    + '0';
            }
            log_level_unknown = 0;
            if (!*p)
                break;
        }
        msg_level = loglev_char - '0';
        if(msg_level < console_loglevel)
        {
            outc(*p);
        }
        if (*p == '\n')
        {
            log_level_unknown = 1;
        }
        printed_len++;
    }
    return printed_len;
}

int printm(const char *fmt, ...)
{
    va_list ap;
    int r;

    va_start(ap, fmt);
    r = vprintk(fmt, ap);
    va_end(ap);
    return r;
}
