# 8digitVFDcontroller
A vfd controller and a sample clock I wrote months ago, using a cheap display with integrated controller module.
The display is a C51 STM32 VFD, a cheap (15$) vfd from aliexpress. It has an integrated controller which we are going to send commands to with an arduino.
Therefore, the code is a .ino file. I include two: The one provided by the seller, which apparently isn't available unless you ask them, and a sample code for a clock I wrote.

Here's the original file with maybe a bit modified commets for readability (original_futabavfd.ino), the translated comments one (translated_futabavfd.ino) and finally the (documented_futabavfd.ino) one I tried my best to document in terms of roughly what each function does or how to use them.
Turns out it's pretty easy to control as the only functions you need to understand are the ones on void loop(), and you would just copy the void setup() stuff and variables declared at the beginning.
IMPORTANT: THERE's A CHARACTER TO HEX TABLE SOMEWHERE IN THE PDF BUT HERE ARE SOME USEFUL HEX VALUES FOR COMMON CHARACTERS: 
[0xA = ":"] [0x9F = " "] [0xE9 = "233" Basically full brightness]
ALSO, WHEN USING WRITEONECHAR() OR WRITESTR() THE LEFT ARGUMENT IS POSITION (FROM 0 TO 7 LEFT TO RIGHT) AND RIGH ARGUMENT IS CHARACTER OR STRING