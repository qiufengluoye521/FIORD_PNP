#ifndef _DEBUG_PRINT_H
#define _DEBUG_PRINT_H

#include "stdint.h"
#include "stm32g0xx_hal.h"

#define  MAX_NUMBER_BYTES  256
#define KERN_ERR        "<0>"   /* error conditions                 */
#define KERN_WARNING    "<1>"   /* warning conditions               */
#define KERN_NOTICE     "<2>"   /* normal but significant condition */
#define KERN_INFO       "<3>"   /* informational                    */
#define KERN_DEBUG      "<4>"   /* debug-level messages             */
extern int console_printk[];

#define console_loglevel (console_printk[0])

/**********************************************************************
 * 函数名称： DEBUG_PRINT
 * 功能描述： 自定义串口格式化函数，实现printf打印功能
 * 输入参数： 可变参数
 * 输出参数： 无
 * 返 回 值： 返回0，表示函数正常返回
 * 修改日期   版本号    修改人       修改内容
 * -----------------------------------------------
 * 2021/05/06   V1.0    李红波       创建
 ***********************************************************************/
int printm(const char *fmt, ...);

////uartError_t BSP_DEBUG_Init(uint8_t uartId, const uartPara_t *debugUartPara);

#endif /* _DEBUG_PRINT_H */

