/* 
 * gpioport.c
 *
 *  Created on: Mar 21, 2013
 *      Author: Tele
 */

#ifdef __KERNEL__

#else // __KERNEL__

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#endif // __KERNEL__

#include "CHIP_mmio.h"

static void* gpBase = 0;
volatile uint32_t* gpPB_data = 0;
volatile uint32_t* gpPE_data = 0;
volatile uint32_t* gpPG_data = 0;


#ifdef __KERNEL__

int ioport_init(void)
{
	gpBase = SUNXI_PIO_BASE;
	gpPB_data = (uint32_t*)(gpBase + SUNXI_PORT_B_BASE + SUNXI_GPIO_DATAPORT_OFFSET);
	gpPE_data = (uint32_t*)(gpBase + SUNXI_PORT_E_BASE + SUNXI_GPIO_DATAPORT_OFFSET);
	gpPG_data = (uint32_t*)(gpBase + SUNXI_PORT_G_BASE + SUNXI_GPIO_DATAPORT_OFFSET);
}

///////////////////////////
int sunxi_gpio_set_cfgpin(uint32_t pin, uint32_t val)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_CFG_INDEX(pin);
	uint32_t offset = GPIO_CFG_OFFSET(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	cfg = readl(&pio->cfg[0] + index);
	cfg &= ~(0xf << offset);
	cfg |= val << offset;

	writel(cfg, &pio->cfg[0] + index);

	return 0;
}
// Example: sunxi_gpio_set_cfgpin(SUNXI_GPG(9), SUNXI_GPIO_OUTPUT);

///////////////////////////
int sunxi_gpio_get_cfgpin(uint32_t pin)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_CFG_INDEX(pin);
	uint32_t offset = GPIO_CFG_OFFSET(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	cfg = readl(&pio->cfg[0] + index);
	cfg >>= offset;

	return (cfg & 0xf);
}

///////////////////////////
inline void setPE04(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000010; writel(gpPE_data, data); }
inline void setPE05(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000020; writel(gpPE_data, data); }
inline void setPE06(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000040; writel(gpPE_data, data); }
inline void setPE07(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000080; writel(gpPE_data, data); }
inline void setPE08(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000100; writel(gpPE_data, data); }
inline void setPE09(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000200; writel(gpPE_data, data); }
inline void setPE10(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000400; writel(gpPE_data, data); }
inline void setPE11(void) { UINT32_t data = readl(gpPE_data); data |= 0x00000800; writel(gpPE_data, data); }

inline void setPB03(void) { UINT32_t data = readl(gpPB_data); data |= 0x00000008; writel(gpPB_data, data); }
inline void setPB04(void) { UINT32_t data = readl(gpPB_data); data |= 0x00000010; writel(gpPB_data, data); }
inline void setPB10(void) { UINT32_t data = readl(gpPB_data); data |= 0x00000400; writel(gpPB_data, data); }
inline void setPG09(void) { UINT32_t data = readl(gpPG_data); data |= 0x00000200; writel(gpPG_data, data); }

inline void clrPE04(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000010; writel(gpPE_data, data); }
inline void clrPE05(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000020; writel(gpPE_data, data); }
inline void clrPE06(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000040; writel(gpPE_data, data); }
inline void clrPE07(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000080; writel(gpPE_data, data); }
inline void clrPE08(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000100; writel(gpPE_data, data); }
inline void clrPE09(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000200; writel(gpPE_data, data); }
inline void clrPE10(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000400; writel(gpPE_data, data); }
inline void clrPE11(void) { UINT32_t data = readl(gpPE_data); data &= ~0x00000800; writel(gpPE_data, data); }

inline void clrPB03(void) { UINT32_t data = readl(gpPB_data); data &= ~0x00000008; writel(gpPB_data, data); }
inline void clrPB04(void) { UINT32_t data = readl(gpPB_data); data &= ~0x00000010; writel(gpPB_data, data); }
inline void clrPB10(void) { UINT32_t data = readl(gpPB_data); data &= ~0x00000400; writel(gpPB_data, data); }
inline void clrPG09(void) { UINT32_t data = readl(gpPG_data); data &= ~0x00000200; writel(gpPG_data, data); }

inline uint32_t read_portE(void){	return readl(gpPE_data);}
inline void write_portE(uint32_t data){writel(gpPE_data, data);}


#else // __KERNEL__

///////////////////////////
int ioport_init(void)
{
	int memfd;
	off_t addr_start, addr_offset;
	uint32_t pagesize, pagemask;
	void* mapped_base;

	memfd = open("/dev/mem", O_RDWR|O_SYNC);
	if (memfd < 0)
	{
//		_log(LOG_ERROR, "ioport_init: Unable to open /dev/mem\n");
		return memfd;
	}

	pagesize = sysconf(_SC_PAGESIZE);
	pagemask = (~(pagesize-1));

	addr_start  = SUNXI_PIO_BASE & pagemask;
	addr_offset = SUNXI_PIO_BASE & ~pagemask;

	mapped_base = mmap(0, pagesize*2, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, addr_start);
	if (mapped_base == MAP_FAILED )
	{
//		_log(LOG_ERROR, "ioport_init: Unable to mmap file\n");
		close(memfd);
		return (-1);
	}

	gpBase = mapped_base + addr_offset;

	gpPB_data = (uint32_t*)(gpBase + SUNXI_PORT_B_BASE + SUNXI_GPIO_DATAPORT_OFFSET);
	gpPE_data = (uint32_t*)(gpBase + SUNXI_PORT_E_BASE + SUNXI_GPIO_DATAPORT_OFFSET);
	gpPG_data = (uint32_t*)(gpBase + SUNXI_PORT_G_BASE + SUNXI_GPIO_DATAPORT_OFFSET);

	close(memfd);

	return 0;
}

///////////////////////////
void ioport_cleanup(void)
{
	uint32_t addr_start;
	uint32_t pagesize, pagemask;
	void* mapped_base;

	if(gpBase)
	{
		pagesize = sysconf(_SC_PAGESIZE);
		pagemask = (~(pagesize-1));

		addr_start = SUNXI_PIO_BASE & pagemask;
		mapped_base = gpBase - addr_start;
		munmap(mapped_base, pagesize*2);
	}
}

///////////////////////////
int read_port(uint32_t port_offset)
{
	return *(volatile uint32_t*)(gpBase + port_offset);
}

///////////////////////////
void write_port(uint32_t port_offset, uint32_t port_data)
{
	*(volatile uint32_t*)(gpBase + port_offset) = port_data;
}

///////////////////////////
int sunxi_gpio_set_cfgpin(uint32_t pin, uint32_t val)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_CFG_INDEX(pin);
	uint32_t offset = GPIO_CFG_OFFSET(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	cfg = *(&pio->cfg[0] + index);
	cfg &= ~(0xf << offset);
	cfg |= val << offset;

	*(&pio->cfg[0] + index) = cfg;
	return 0;
}
// Example: sunxi_gpio_set_cfgpin(SUNXI_GPG(9), SUNXI_GPIO_OUTPUT);

///////////////////////////
int sunxi_gpio_get_cfgpin(uint32_t pin)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_CFG_INDEX(pin);
	uint32_t offset = GPIO_CFG_OFFSET(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	cfg = *(&pio->cfg[0] + index);
	cfg >>= offset;
	return (cfg & 0xf);
}

///////////////////////////
int sunxi_gpio_output_set(uint32_t pin)
{
	uint32_t dat;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t num = GPIO_NUM(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	dat = *(&pio->dat);
	dat |= 1 << num;
	*(&pio->dat) = dat;
	return 0;
}


///////////////////////////
int sunxi_gpio_output_clr(uint32_t pin)
{
	uint32_t dat;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t num = GPIO_NUM(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	dat = *(&pio->dat);
	dat &= ~(1 << num);

	*(&pio->dat) = dat;
	return 0;
}

int sunxi_gpio_input(uint32_t pin)
{
	uint32_t dat;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t num = GPIO_NUM(pin);

	struct sunxi_gpio* pio = &((struct sunxi_gpio_reg*)gpBase)->gpio_bank[bank];

	dat = *(&pio->dat);
	dat >>= num;
	return (dat & 0x1);
}

///////////////////////////
//
inline void setPE04(void) {*gpPE_data |= 0x00000010;}
inline void setPE05(void) {*gpPE_data |= 0x00000020;}
inline void setPE06(void) {*gpPE_data |= 0x00000040;}
inline void setPE07(void) {*gpPE_data |= 0x00000080;}
inline void setPE08(void) {*gpPE_data |= 0x00000100;}
inline void setPE09(void) {*gpPE_data |= 0x00000200;}
inline void setPE10(void) {*gpPE_data |= 0x00000400;}
inline void setPE11(void) {*gpPE_data |= 0x00000800;}

inline void setPB03(void) {*gpPB_data |= 0x00000008;}
inline void setPB04(void) {*gpPB_data |= 0x00000010;}
inline void setPB10(void) {*gpPB_data |= 0x00000400;}
inline void setPG09(void) {*gpPG_data |= 0x00000200;}

inline void clrPE04(void) {*gpPE_data &= ~0x00000010;}
inline void clrPE05(void) {*gpPE_data &= ~0x00000020;}
inline void clrPE06(void) {*gpPE_data &= ~0x00000040;}
inline void clrPE07(void) {*gpPE_data &= ~0x00000080;}
inline void clrPE08(void) {*gpPE_data &= ~0x00000100;}
inline void clrPE09(void) {*gpPE_data &= ~0x00000200;}
inline void clrPE10(void) {*gpPE_data &= ~0x00000400;}
inline void clrPE11(void) {*gpPE_data &= ~0x00000800;}

inline void clrPB03(void) {*gpPB_data &= ~0x00000008;}
inline void clrPB04(void) {*gpPB_data &= ~0x00000010;}
inline void clrPB10(void) {*gpPB_data &= ~0x00000400;}
inline void clrPG09(void) {*gpPG_data &= ~0x00000200;}

#endif // __KERNEL__
