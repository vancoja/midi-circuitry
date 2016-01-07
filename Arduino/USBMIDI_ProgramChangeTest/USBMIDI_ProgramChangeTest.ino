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

USB UsbHost;
USBH_MIDI UsbMidi(&UsbHost);

void setup() {
  // stop if no usb host shield found
  if (UsbHost.Init() == -1) {
    while (1);
  }
  Serial.begin(9600);
  Serial.println("USB Host Connected");
}

void loop() {
  uint8_t data[2];
  UsbHost.Task();
  if (UsbHost.getUsbTaskState() == USB_STATE_RUNNING) {
    Serial.println("USB Host Running");
    for (byte i = 0; i < 10; ++i) {
      data[0] = midi::ProgramChange;
      data[1] = i;
      Serial.print("Sending ProgramChange ");
      Serial.println(i);
      UsbMidi.SendData(data, 0);
      delay(2000);
    }
  }
}
