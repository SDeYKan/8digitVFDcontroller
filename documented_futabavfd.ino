//IMPORTANT: THERE's A CHARACTER TO HEX TABLE SOMEWHERE IN THE PDF BUT HERE ARE SOME USEFUL HEX VALUES FOR COMMON CHARACTERS:
//[0xA = ":"] [0x9F = " "] [0xE9 = "233" Basically full brightness]
//ALSO, WHEN USING WRITEONECHAR() OR WRITESTR() THE LEFT ARGUMENT IS POSITION (FROM 0 TO 7 LEFT TO RIGHT) AND RIGH ARGUMENT IS CHARACTER OR STRING

//arduino uno r3 Pin connection (引脚接法)
uint8_t din = 11;
// DA 
uint8_t clk = 13;
// CK
uint8_t cs = 10;
// CS
uint8_t Reset = 1;
// RS
uint8_t en = 0;
// EN


//This apparently is the main function, every other function calls it in a different way depending of what you want to display and is used both for display text and to send commands
void write_6302(unsigned char w_data)
{
  unsigned char i;
  for (i = 0; i < 8; i++) {
    digitalWrite(clk, LOW);
    if ((w_data & 0x01) == 0x01) {
      digitalWrite(din, HIGH); 
    }
    else {
    digitalWrite(din, LOW);
    w_data >>= 1;
    digitalWrite(clk, HIGH);
    } 
  }
}


//This function just calls the write function but includes cs LOW then HIGH behaviour (vfd only reads commands while cs is LOW apparently)
//You can use it to control brightness as in the loop() function does
void VFD_cmd(unsigned char command) { 
  digitalWrite(cs, LOW);
  write_6302(command);
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}


//This apparently just changes brightness to max so that it shows the text on the display (after somehow (auto?) hiding it to run a command)
//Its just a VFD_cmd function with the "command" parameter fixed to max brightness
void S1201_show(void) { 
  digitalWrite(cs, LOW);
  //Start transfer (开始传输) 
  write_6302(0xe8);
  //Shift register start position (地址寄存器起始位置)
  digitalWrite(cs, HIGH);
  //Stop transfer (停止传输)
}


//This initializes the vfd, according to documentation (the pdf) it sends some specific commands or parameters to the vfd
void VFD_init() { 
  //SET HOW MANY digital numbers
  digitalWrite(cs, LOW);
  write_6302(0xe0);
  delayMicroseconds(5);
  write_6302(0x07); //8 digital 
  digitalWrite(cs, HIGH);
  delayMicroseconds(5); 
  digitalWrite(cs, LOW);
  write_6302(0xe4); //set brightness
  delayMicroseconds(5);
  write_6302(0xff); //leave 255 max 
  digitalWrite(cs, HIGH);
  delayMicroseconds(5); 
} 

//Print a specified character at a specified position
/****************************** 在指定位置打印一个字符(用户自定义,所有CG-ROM中的) x:0~11;chr:要显示的字符编码 *******************************/ 
void S1201_WriteOneChar(unsigned char x, unsigned char chr) {
  digitalWrite(cs, LOW); //Start transfer (开始传输) 
  write_6302(0x20 + x); //Shift register start position (地址寄存器起始位置) It first sends the position where to write
  write_6302(chr + 0x30); //Then it sends the character we want to write
  digitalWrite(cs, HIGH); //Stop transfer (停止传输)
  S1201_show(); //This function shows whats written (aka bright to max) so that (provided that the same command is used to introduce comands and to write on screen) the screen isn't mixed with stray characters from the commands
}


//Prints a string of english characters only starting at the specified position
/****************************** 在指定位置打印字符串 (仅适用于英文,标点,数字) x:0~11;str:要显示的字符串 *******************************/ 
void S1201_WriteStr(unsigned char x, const char *str) { 
  digitalWrite(cs, LOW);  //Start transfer (开始传输)
  write_6302(0x20 + x);  //Shift register start position (地址寄存器起始位置) As in writeonechar(), it first indicates where to start writing
  while (*str) { 
    write_6302(*str); //ascii and corresponding character table conversion (与对应字符表转换) (this while sends the characters one by one to vfd)
    str++; 
  } 
  digitalWrite(cs, HIGH); //Stop transfer (停止传输) 
  S1201_show(); //This function shows whats written (aka bright to max) so that (provided that the same command is used to introduce comands and to write on screen) the screen isn't mixed with stray characters from the commands
} 


void setup() { 
  pinMode(en, OUTPUT); 
  pinMode(clk, OUTPUT);
  pinMode(din, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(Reset, OUTPUT);
  digitalWrite(en, HIGH);
  digitalWrite(Reset, LOW);
  delayMicroseconds(5);
  digitalWrite(Reset, HIGH);
  VFD_init(); //Init function necessary for it to initialize, sends a bunch of commands in the right order for the controller to know what to do
} 

//These three functions here are the only ones you actually need to know to controll the display
void loop() {
  VFD_cmd(0xE9); // Full brightness(233) (全亮) runs a command, in this case a change brightness command (look at function above to see how it runs a command)
  delay(1000);
  S1201_WriteOneChar(0, 0); 
  S1201_WriteOneChar(1, 1);
  S1201_WriteOneChar(2, 2);
  S1201_WriteOneChar(3, 3);
  S1201_WriteOneChar(4, 4);
  S1201_WriteOneChar(5, 5);
  S1201_WriteOneChar(6, 6);
  S1201_WriteOneChar(7, 7);
  delay(1000);
  S1201_WriteStr(0, "ABCDEFGH");
  delay(1000); 
}