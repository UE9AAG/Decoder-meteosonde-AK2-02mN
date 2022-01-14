# Decoder-meteosonde-AK2-02mH
An example of decoding telemetry of a Russian-made weather balloon AK2-02mN. I would be very happy if someone can help integrate this decoder into a DL9RDZ or Mysondigo project.
For decoding AK2-02mN weather balloon signals using the TTGO LORA32 T3 V1.6.1 module. The TTGO module was improved: a second LORA module was installed in parallel with the LORA module. All contacts of the second module are connected to the contacts of the same name of the first module, except for the NSS signal and the high-frequency input for connecting the antenna. The NSS signal is connected to pin 18. The quartz resonator was replaced on the second module: instead of the standard one at 32 MHz, a resonator at 30.72 MHz was installed. This is a prerequisite for receiving signals of this type of weather balloons. 
