// Uses USB Host Shield
// https://www.circuitsathome.com/usb-host-shield-hardware-manual
// Requires USB MIDI class driver for Arduino USB Host Shield 2.0 Library
// https://github.com/YuuichiAkagawa/USBH_MIDI

#include <MIDI.h>
#include <usbh_midi.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

MIDI_CREATE_DEFAULT_INSTANCE();
USB UsbHost;
USBH_MIDI UsbMidi(&UsbHost);

void setup() {  
	// stop if no usb host shield found
	if (UsbHost.Init() == -1) {
		while(1); 
	}
	// initialize MIDI
	MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
	uint8_t data[2];
	UsbHost.Task();
	if (UsbHost.getUsbTaskState() == USB_STATE_RUNNING) {
		if (MIDI.read()) {
			data[0] = MIDI.getType();
			if (data[0] == midi::ProgramChange) {
				data[1] = MIDI.getData1();
				UsbMidi.SendData(data, 0);
			}
		}
	}
}
