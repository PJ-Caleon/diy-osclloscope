# scope-fw: a DIY oscilloscope on the STM32F103C6T6 Blue Pill

This is an oscilloscope project that displays 0 to 3.3V waveform and measures its frequency, peak to peak voltage and average. It has 4 modes that can be toggled (Time/div, Voltage/div, Edge if Rising or Falling, L - trigger level)

[![ci](https://github.com/PJ-Caleon/diy-osclloscope/actions/workflows/ci.yml/badge.svg)](https://github.com/PJ-Caleon/diy-osclloscope/actions)

## Hardware
- STM32F103C6T6
- 3 buttons 
- 0.96-inch SSD1306 OLED
- 10k potentiometer
- ST-Link V2 
- CH340 USB-to-TTL 

## Wiring
| Signal | Pin | Wire to |
|---|---|---|
| PWM test output | PA1 | PA2 (jumper) |
| ADC input | PA2 | signal under test (or PA1) |
| UART TX | PA9 | CH340 RXD |
| UART RX | PA10 | CH340 TXD |
| I2C clock (SCL) | PB6 | OLED SCK/SCL |
| I2C data (SDA) | PB7 | OLED SDA |
| MODE button | PB12 | button to GND |
| UP button | PB13 | button to GND |
| DOWN button | PB14 | button to GND |
| Power | 3V3, GND | OLED VCC and GND, CH340 GND |

## Build, test, flash


```
make firmware
```

```
make test-active
```

```
make flash 
```

## How it works
A timer (Tim3) triggers the ADC at a sampling rate set by the time/div settings. The ADC converts the voltage on PA2 into a number from 0 to 4095. The DMA stores each result in a buffer of 256 samples. The scope then finds the trigger point on where the signal crosses the trigger level on the chosen edge and calculates the minimum, maximum, average and frequency. It then draws a 128-sample window starting at the trigger point on the OLED with the readings shown as two lines of text. 

The MODE button chooses what to change:
T (time/div), V (volts/div), L (trigger level), E (trigger edge, rising or falling). Up and down change the chosen settings 

## Measurements

Wire PA1 to PA2, PWM at 25%, time/div at 500us

- Frequency: 1.00 kHz 
- Vpp : 3.30 V 
- Average : 0.83 V

## Calibration
The ADC does not read exactly 0 at 0V or 4095 at 3.3 V so the scope uses a two point calibration. With PA2 wired to GND, the average of 256 samples at 32000 Hz was 17 counts. With PA2 wired to 3V3, it read 4085 counts. The span is 4085 - 17 = 4068. So the Global Calibration(g_cal) is {17, 4068}. Every sample is corrected with (raw - 17) x 4095 / 4068 which maps back to 0 and 4095

## Limits

- The input must stay within 0V to 3.3V and there is no input protection, so a voltage outside this range can damage the board 
- At slow time/div, the sample rate drops below twice the signal frequency so the samples form a false output
