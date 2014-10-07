==========================================
              LED Cube 4x4x4
==========================================

MCU:
   ATmega16, 8 MHz, internal RC-oscillator

Connections:

  A3,A4 - debug
  D2 - IR-receiver
  D4,D5,D6,D7 - common anodes for layers (with npn-transistors)
  B3,B2,B1,B0 - cathodes, column 1 (with 470 Ohm resistors)
  B7,B6,B5,B4 - cathodes, column 2
  C4,C5,C6,C7 - cathodes, column 3
  C0,C1,C2,C3 - cathodes, column 4
