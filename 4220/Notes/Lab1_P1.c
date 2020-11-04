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
   
   Important: remember that the GPIO base register address is 0x3F200000, not the
   one shown in the BCM2835 ARM Peripherals manual.
*/

int irq_ptr;	// variable needed to identify the handler

#define BLOCK_SIZE 4096
#define LED 3
#define CLR_LED 6

unsigned long GPIO_BASE    = 0x3F200000;
unsigned long *GPSET0       = 0x3F200007; // = 7 want to divide original value of 28 by 4 = 7
unsigned long *GPCLR0       = 0x3F20000A; // = 10 want to divide original value of 40 by 4 = 10
unsigned long *GPEDS0       = 0x3F200010; 
unsigned long *GPLEV0       = 0x3F20000D;
unsigned long *GPSEL1       = 0x3F200001;
unsigned long *GPSEL2       = 0x3F200002;
unsigned long *GPPUD        = 0x3F200025;
unsigned long *GPPUDCLK0    = 0x3F200026;
unsigned long *GPAREN0      = 0x3F20001F;

// Helper functions
int init_button(void);
void exit_button(void);

// Interrupt handler function. Tha name "button_isr" can be different.
// You may use printk statements for debugging purposes. You may want to remove
// them in your final code.
static irqreturn_t button_isr(int irq, void *irq_ptr){
    
    
    disable_irq_nosync(79); // In general, you want to disable the interrupt 79 while handling it.

    unsigned long *gpsel0 = (unsigned long*)ioremap(GPIO_BASE, BLOCK_SIZE);
    prink("%x\n", gpsel0);
    // This same handler will be called regardless of what button was pushed,
	// assuming that they were properly configured.
	// How can you determine which button was the one actually pushed?
		
	// DO STUFF (whatever you need to do, based on the button that was pushed)

	// IMPORTANT: Clear the Event Detect status register before leaving.
    // Instructions/Configuration for Red LED and Button reads


    //read in the pin used from Event Detection Status Register (pg 97)
    unsigned long pin_used_raw = ioread32(GPEDS0);
	/*The rising edge detect enable registers define the pins for which a rising edge
	transition sets a bit in the event detect status registint a = l is sampled using the
	system clock and then it is looking for a “011” pattern on the sampled signal. This
	has the effect of suppressing glitches.
	*/
 


    unsigned long pin_mask = 0x000F8000;
    unsigned long pin_used = pin_used_raw & pin_mask;

    printk("%lx", pin_used);

    // GPSEL pins 5, 4, 3, and 2 to output mode (LEDS)
    iowrite32(0b001001001001000000, gpsel0); 

    switch (pin_used)
    {
        case 0X10000:
			((ioread32(GPLEV0) & 0b100) == 0b0) ? iowrite32(0b100, GPSET0) : iowrite32(0b100, GPCLR0);
            printk("Button 1 (BCM 16) Used");
            break;
        case 0X20000:
            ((ioread32(GPLEV0) & 0b1000) == 0b0) ? iowrite32(0b1000, GPSET0) : iowrite32(0b1000, GPCLR0);
            printk("Button 2 (BCM 17) Used");
            break;
        case 0X40000:
            ((ioread32(GPLEV0) & 0b10000) == 0b0) ? iowrite32(0b10000, GPSET0) : iowrite32(0b10000, GPCLR0);
            printk("Button 3 (BCM 18) Used");
            break;
        case 0X80000:
            ((ioread32(GPLEV0) & 0b100000) == 0b0) ? iowrite32(0b100000, GPSET0) : iowrite32(0b100000, GPCLR0);
            printk("Button 4 (BCM 19) Used");
            break;
        case 0:
            ((ioread32(GPLEV0) & 0b100000) == 0b0) ? iowrite32(0b111100, GPSET0) : iowrite32(0b111100, GPCLR0);
            printk("Button 5 (BCM 20) Used");
            break;
        default:
            break;
    }

    iowrite32(0x001F0000, GPEDS0);
    enable_irq(79); // Re-enable 79iowrite32(1 << 3, GPCLR0); // GPCLR pin 3 to LOW

    printk("Interrupt handled\n");
    
    return IRQ_HANDLED;
}

// Want to toggle led based on button, need to configure this button with interupt
int init_button()
{   
    unsigned long *gpsel0 = (unsigned long*)ioremap(GPIO_BASE, BLOCK_SIZE);
    // Local function variables
    int dummy = 0;
    // Map GPIO registers
	// Remember to map the base address (beginning of a memory page)
	// Then you can offset to the addresses of the other registers

	// Don't forget to configure all ports connected to the push buttons as inputs.
	
	// You need to configure the pull-downs for all those ports. There is
	// a specific sequence that needs to be followed to configure those pull-downs.
	// The sequence is described on page 101 of the BCM2837-ARM-Peripherals manual.
	// You can use  udelay(100);  for those 150 cycles mentioned in the manual.
	// It's not exactly 150 cycles, but it gives the necessary delay.
	// WiringPi makes it a lot simpler in user space, but it's good to know
	// how to do this "by hand".
		
	// Enable (Async) Rising Edge detection for all 5 GPIO ports.
			
	// Request the interrupt / attach handler (line 79, doesn't match the manual...)
	// The third argument string can be different (you give the name)

    // Instructions/Configuration for Buttons
    // Can select pins 10-19 with this


	printk("Begin INIT Instructions.\n");
	
    // 4 for GPSEL1 (buttons 1-4 BCM 16-19)
    iowrite32(0x0, GPSEL1); // GPSEL pin 16 to input mode

    // 1 for gpsel 2 (button 5 BCM 20)
    iowrite32(0x0, GPSEL2); // GPSEL pin 20 to input mode

    // GPPUD Configuration to make all reads pulldown

    // You need to configure the pull-downs for all those ports. There is
	// a specific sequence that needs to be followed to configure those pull-downs.
	// The sequence is described on page 101 of the BCM2837-ARM-Peripherals manual (pg 101):
		// The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
		// the respective GPIO pins. These registers must be used in conjunction with the GPPUD
		// register to effect GPIO Pull-up/down changes. The following sequence of events is
		// required:

		// 1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
		// to remove the current Pull-up/down)
    iowrite32(0b01, GPPUD);
        // 2. Wait 150 cycles – this provides the required set-up time for the control signal
    udelay(100);
        // 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
		// modify – NOTE only the pads which receive a clock will be modified, all others will
		// retain their previous state.
    iowrite32(0x001F0000, GPPUDCLK0);
    	// 4. Wait 150 cycles – this provides the required hold time for the control signal
    udelay(100);
    	//5. Write to GPPUD to remove the control signal
    iowrite32(0x0, GPPUD);
    	//6. Write to GPPUDCLK0/1 to remove the clock
    iowrite32(0x0, GPPUDCLK0);
    
    iowrite32(0x001F0000, GPAREN0);

    dummy = request_irq(79, button_isr, IRQF_SHARED, "Button_handler", &irq_ptr);

	printk("Finish INIT Instructions.\n");
	return 0;
}






void exit_button()
{
    // Good idea to clear the Event Detect status register here, just in case.
		
	// Disable (Async) Rising Edge detection for all 5 GPIO ports.
	
	// Remove the interrupt handler; you need to provide the same identifier
	printk("Begin CLEANUP Instructions.\n");
	free_irq(79, &irq_ptr);
	printk("Finish CLEANUP Instructions.\n");
}

module_init(init_button);
module_exit(exit_button);

