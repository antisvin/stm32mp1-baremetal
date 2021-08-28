#include <malloc.h>
#include <stdio.h>

#if 0
#include "heap.h"
// define these before defining the malloc/free macros
void *pvPortMalloc(size_t xWantedSize)
{
	return malloc(xWantedSize);
}
void vPortFree(void *pv)
{
	free(pv);
}
#endif

#include "audio_stream.hh"
#include "basicmaths.h"
#include "drivers/stm32xx.h"
#include "message.h"
#include "shared/drivers/leds.hh"
#include "shared/drivers/uart.hh"
#include "shared/stm32disco_conf.hh"

#include "owl_patch.hh"
#include "ProgramVector.h"


using namespace STM32MP1Disco;

ProgramVector programVector;

void main()
{
	Uart<UART4_BASE> uart;
	// UI
	uart.write("\r\n\r\nStarting Audio Processor\r\n");
	uart.write("Press User1 button to select a synth\r\n");

	// BlueLED blue_led;
	User1Button button1;
	User2Button button2;

	//uart.write("Using Synth: ");
	//uart.write(synths.name[current_synth]);
	//uart.write("\r\n");

	AudioStream audio;	

	/*
	HeapRegion_t regions[2];
	extern char _sheap, __HEAP_SIZE;
	regions[0].location = (uint8_t*)_sheap;
	regions[0].size = (uint32_t)__HEAP_SIZE;
	regions[0].location = NULL;
	regions[0].size = 0;
	*/

	programVector.buttonChangedCallback = onButtonChanged;


	//size_t before = xPortGetFreeHeapSize();
	//setup(pv);
	OwlPatch<AudioStreamConf> owl;
	//pv->heap_bytes_used = before - xPortGetFreeHeapSize();

    audio.start(owl.process_func);
//		    [&owl](AudioInBuffer &in_buffer, AudioOutBuffer &out_buffer) {
//			    owl.process(in_buffer, out_buffer); 
//		    }
//        );
	// audio.start(synths.process_func[current_synth]);

	while (true) {
		button1.update();
		button2.update();

		// Select synth
		if (button1.is_just_pressed()) {
		}

		if (button2.is_just_pressed()) {
			// we can do something here... send a signal to a synth, or adjust a parameter?
		}
	}
}

extern "C" void aux_core_main() {}
