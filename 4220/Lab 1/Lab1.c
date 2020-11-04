/* ece4220lab1_isr.c
 * ECE4220/7220
 * Author: Jacob Alongi
 */
#ifndef MODULE
#define MODULE
#endif
#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
/* Declare your pointers for mapping the necessary GPIO registers.
   You need to map:
   - Pin Event detect status register(s)
   - Rising Edge detect register(s) (either synchronous or asynchronous should work)
   - Function selection register(s)
   - Pin Pull-up/pull-down configuration registers
*/
int irq_ptr;	// variable needed to identify the handler


//(PG 90) of manual list addresses
//Important: remember that the GPIO base register address is 0x3F200000, 
//not the one shown in the BCM2835 ARM Peripherals manual.
#define GPIO_BASE 0x3F200000 // GPSEL0 is at base address
#define BLOCK_SIZE 4096

//0x 7E20 0004 -> 04/4 = x1
#define GPSEL1_OFFSET 0x01
//0x 7E20 0008 -> 08/4 = x2
#define GPSEL2_OFFSET 0x02
//0x 7E20 001C -> 1C -> 28/4 = x7
#define GPSET0_OFFSET 0x07
//0x 7E20 0028 -> 28 -> 40/4 = 10 -> xA
#define GPCLR0_OFFSET 0x0A
//0x 7E20 0034 -> 34 -> 52/4 = 13 -> xD
#define GPLEV0_OFFSET 0x0D
//0x 7E20 0040 -> 64 -> 64/4 = 16 -> x10
#define GPEDS0_OFFSET 0x10
//0x 7E20 007C -> 124 -> 124/4 = 31 -> x1F
#define GPAREN0_OFFSET 0x1F
//0x 7E20 0094 -> 148 -> 148/4 = 46 -> x25
#define GPPUD_OFFSET 0x25
//0x 7E20 0098 -> 152 -> 152/4 = 38 -> x26
#define GPPUDCLK0_OFFSET 0x26 //
#define LED 3
#define CLR_LED 6

// Helper functions
int init_button(void);
void exit_button(void);

int irq_ptr; // variable needed to idenitify the handler

static irqreturn_t button_isr(int irq, void *dev_id){
    // In general, you want to disable the interrupt while handling it.
    disable_irq_nosync(79); // Disable 79

    // Instructions/Configuration for Red LED and Button reads
	unsigned long *gpsel0       = (unsigned long*)ioremap(GPIO_BASE, BLOCK_SIZE);
	unsigned long *gpset0       = gpsel0 + GPSET0_OFFSET;
	unsigned long *gpclr0       = gpsel0 + GPCLR0_OFFSET;
    unsigned long *gpeds0       = gpsel0 + GPEDS0_OFFSET;
    unsigned long *gplev0       = gpsel0 + GPLEV0_OFFSET;
    
    unsigned long event_detection = ioread32(gpeds0);
    unsigned long mask_BCM_pins_16_to_20 = 0x000F8000;// 0b 0000 0000 0000 1111 1000 0000 0000 0000
    unsigned long pin_used = event_detection & mask_BCM_pins_16_to_20;//16-20 mapped to buttons

    printk("%lx", pin_used);


    //select all LEDs for output mode
    // need to shift 2 bits as LEDs are bit2-5
    iowrite32(0x2490 << 2, gpsel0); 

    switch (pin_used)
    {
        /*
        pin allocation bit2 - bit5 for LEDs:
        b2,3,4,5: red,yellow,green,blue
        */
        case 0X10000://red BCM 16
			((ioread32(gplev0) & 1 << 2) == 0) ? iowrite32(1 << 2, gpset0) : iowrite32(1 << 2, gpclr0);
            break;
        case 0X20000://yellow 17
            ((ioread32(gplev0) & 1 << 3) == 0) ? iowrite32(1 << 3, gpset0) : iowrite32(1 << 3, gpclr0);
            break;
        case 0X40000://green 18
            ((ioread32(gplev0) & 1 << 4) == 0) ? iowrite32(1 << 4, gpset0) : iowrite32(1 << 4, gpclr0);
            break;
        case 0X80000://blue 19
            ((ioread32(gplev0) & 1 << 5) == 0) ? iowrite32(1 << 5, gpset0) : iowrite32(1 << 5, gpclr0);
            break;
        case 0://all 20
            ((ioread32(gplev0) & 1 << 5) == 0) ? iowrite32(0xF << 2, gpset0) : iowrite32(0xF << 2, gpclr0);
        default:
            break;
    }

    iowrite32(0x001F0000, gpeds0);
    enable_irq(79); // Re-enable 79iowrite32(1 << 3, gpclr0);
    printk("Interrupt handled\n");

    //for event detection status registers:
    //The bit is cleared by writing a “1” to the relevant bit. (pg 96 GPIO)
    iowrite32(0xFFFFFFFF, gpeds0);
    return IRQ_HANDLED;
}


int init_button()
{   
    int dummy = 0;
    // Map GPIO registers
    unsigned long *gpsel0       = (unsigned long*)ioremap(GPIO_BASE, BLOCK_SIZE);
    // Then you can offset to the addresses of the other registers
    unsigned long *gpsel1       = gpsel0 + GPSEL1_OFFSET;
    unsigned long *gpsel2       = gpsel0 + GPSEL2_OFFSET;
    unsigned long *gppud        = gpsel0 + GPPUD_OFFSET;
    unsigned long *gppudclk0    = gpsel0 + GPPUDCLK0_OFFSET;
    unsigned long *gparen0      = gpsel0 + GPAREN0_OFFSET;
	

	// configure all ports connected to the push buttons as inputs. (Pg 101) 
	    // 4 for gpsel1 (buttons 1-4 BCM 16-19)
    iowrite32(0x0, gpsel1); // GPSEL pin 16 to input mode

        // 1 for gpsel 2 (button 5 BCM 20)
    iowrite32(0x0, gpsel2); // GPSEL pin 20 to input mode

    /*
    follow steps from page 101:
    The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
    the respective GPIO pins. These registers must be used in conjunction with the GPPUD
    register to effect GPIO Pull-up/down changes. The following sequence of events is
    required:
    */

		// 1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
		// to remove the current Pull-up/down)
    iowrite32(0b01, gppud);
        // 2. Wait 150 cycles – this provides the required set-up time for the control signal
    udelay(100);
        // 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
		// modify – NOTE only the pads which receive a clock will be modified, all others will
		// retain their previous state.
    iowrite32(0x001F0000, gppudclk0);
        // 4. Wait 150 cycles – this provides the required hold time for the control signal
    udelay(100);
        //5. Write to GPPUD to remove the control signal
    iowrite32(0x0, gppud);
        //6. Write to GPPUDCLK0/1 to remove the clock
    iowrite32(0x0, gppudclk0);
    // Enable (Async) Rising Edge detection for all 5 GPIO ports.
    iowrite32(0x001F0000, gparen0);
    // Request the interrupt / attach handler 
    dummy = request_irq(79, button_isr, IRQF_SHARED, "Button_handler", &irq_ptr);

	printk("Init Complete.\n");
	return 0;
}

void exit_button()
{
    unsigned long *gpsel0       = (unsigned long*)ioremap(GPIO_BASE, BLOCK_SIZE);
    unsigned long *gpeds0       = gpsel0 + GPEDS0_OFFSET;
    unsigned long *gpren0      = gpsel0 + GPAREN0_OFFSET;
    printk("Init Clean.\n");
    // Good idea to clear the Event Detect status register here, just in case.
    //The bit is cleared by writing a “1” to the relevant bit. (pg 96 GPIO)
    iowrite32(0xFFFFFFFF, gpeds0);
	// Disable (Async) Rising Edge detection for all 5 GPIO ports.
	iowrite32(0x00000000, gpren0);
	// Remove the interrupt handler; you need to provide the same identifier
	free_irq(79, &irq_ptr);
	printk("Init Complete.\n");
}

module_init(init_button);
module_exit(exit_button);

