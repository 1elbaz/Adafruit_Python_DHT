/* Thanks to Tom Cubie, Olimex, and Tele for this port.
 * Original file name: gpioport.h
 * https://www.olimex.com/forum/index.php?topic=436.0
 *
 *  Created on: Mar 21, 2013
 *      Author: Tele
 */

#ifndef GPIOPORT_H_
#define GPIOPORT_H_


#define SUNXI_PIO_BASE		(0x01c20800)	// Programmable Input Output
#define SUNXI_GPIO_DATAPORT_OFFSET  (0x10)
#define SUNXI_GPIO_INPUT        (0)
#define SUNXI_GPIO_OUTPUT       (1)

//----------------------------------//
//       PORT BASE DEFINITIONS      //
//----------------------------------//

#define SUNXI_PORT_A_BASE      (0*0x24)
#define SUNXI_PORT_B_BASE      (1*0x24)
#define SUNXI_PORT_C_BASE      (2*0x24)
#define SUNXI_PORT_D_BASE      (3*0x24)
#define SUNXI_PORT_E_BASE      (4*0x24)
#define SUNXI_PORT_F_BASE      (5*0x24)
#define SUNXI_PORT_G_BASE      (6*0x24)
#define SUNXI_PORT_H_BASE      (7*0x24)
#define SUNXI_PORT_I_BASE      (8*0x24)

//----------------------------------//
//         PIO DEFINITIONS          //
//----------------------------------//

#define SUNXI_PIO_00           (0x00000001L <<  0)
#define SUNXI_PIO_01           (0x00000001L <<  1)
#define SUNXI_PIO_02           (0x00000001L <<  2)
#define SUNXI_PIO_03           (0x00000001L <<  3)
#define SUNXI_PIO_04           (0x00000001L <<  4)
#define SUNXI_PIO_05           (0x00000001L <<  5)
#define SUNXI_PIO_06           (0x00000001L <<  6)
#define SUNXI_PIO_07           (0x00000001L <<  7)
#define SUNXI_PIO_08           (0x00000001L <<  8)
#define SUNXI_PIO_09           (0x00000001L <<  9)
#define SUNXI_PIO_10           (0x00000001L <<  10)
#define SUNXI_PIO_11           (0x00000001L <<  11)
#define SUNXI_PIO_12           (0x00000001L <<  12)
#define SUNXI_PIO_13           (0x00000001L <<  13)
#define SUNXI_PIO_14           (0x00000001L <<  14)
#define SUNXI_PIO_15           (0x00000001L <<  15)

#define GPIO_BANK(pin)			((pin) >> 5)
#define GPIO_NUM(pin)			((pin) & 0x1F)
#define GPIO_CFG_INDEX(pin)		(((pin) & 0x1F) >> 3)
#define GPIO_CFG_OFFSET(pin)	((((pin) & 0x1F) & 0x7) << 2)


/* SUNXI GPIO number definitions */
#define SUNXI_GPE04   (0x0000084)
#define SUNXI_GPE05   (0x0000085)
#define SUNXI_GPE06   (0x0000086)
#define SUNXI_GPE07   (0x0000087)
#define SUNXI_GPE08   (0x0000088)
#define SUNXI_GPE09   (0x0000089)
#define SUNXI_GPE10   (0x000008A)
#define SUNXI_GPE11   (0x000008B)

#define SUNXI_GPB03   (0x0000023)
#define SUNXI_GPB04   (0x0000024)
#define SUNXI_GPB10   (0x000002A)
#define SUNXI_GPG09   (0x00000C9)

#define GPIO_BANK_GPB03 (0x00000001)
#define GPIO_BANK_GPE04 (0x00000004)
#define GPIO_BANK_GPG09 (0x00000006)

struct sunxi_gpio
{
	uint32_t cfg[4];
	uint32_t dat;
	uint32_t drv[2];
	uint32_t pull[2];
};

// gpio interrupt control
struct sunxi_gpio_int
{
	uint32_t cfg[4];
	uint32_t ctl;
	uint32_t sta;
	uint32_t deb;    // interrupt debounce
};

struct sunxi_gpio_reg
{
	struct sunxi_gpio gpio_bank[9];
	uint8_t res[0xbc];
	struct sunxi_gpio_int gpio_int;
};

int ioport_init(void);
void ioport_cleanup(void);
int sunxi_gpio_get_cfgpin(uint32_t pin);
int sunxi_gpio_set_cfgpin(uint32_t pin, uint32_t val);
int sunxi_gpio_output_set(uint32_t pin);
int sunxi_gpio_output_clr(uint32_t pin);

extern volatile uint32_t* gpPB_data;
extern volatile uint32_t* gpPE_data;
extern volatile uint32_t* gpPG_data;

void setPE04(void);
void setPE05(void);
void setPE06(void);
void setPE07(void);
void setPE08(void);
void setPE09(void);
void setPE10(void);
void setPE11(void);

void clrPE04(void);
void clrPE05(void);
void clrPE06(void);
void clrPE07(void);
void clrPE08(void);
void clrPE09(void);
void clrPE10(void);
void clrPE11(void);

void setPB03(void);
void setPB04(void);
void setPB10(void);
void setPG09(void);

void clrPB03(void);
void clrPB04(void);
void clrPB10(void);
void clrPG09(void);

#endif /* GPIOPORT_H_ */

