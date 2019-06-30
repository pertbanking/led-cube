
#define LAYERS_OFF() PORTL = 0;
#define SET_LAYER(layer) PORTL = 1 << layer;
#define SET_TARGET_REG(reg) PORTA = reg & 0b00000111;
#define SET_REG_VALS(vals) PORTC = vals & 0b11111111;

const char* MAGIC_BYTES = "JANDY";  // the magic bytes at the beginning of a cube frame
                                    // yes, this is an inside joke.

unsigned char buffer0[64]; // the data for the current frame of the cube
                                    // 8x8x8: an 8x8 square of 8-bit chars
unsigned char buffer1[64]; // we use two buffers: one for displaying, and 
                                    // one for filling with values from serial.
unsigned char* cube = buffer0; // we set this pointer to whatever is the current cube
unsigned char* store = buffer1;// set this one to the next cube
bool bufferswitch = false; // false => cube is at buffer0, true => buffer1

void setup() {
  // pins 22-24 (PA0-PA2) are for the decoder inputs
  // pins 37-30 (PC0-PC7) are the data bits for each row, and 
  // pins 49-42 (PL0-PL7) are the layer selector bits
  for (int i = 22; i <= 49; ++i) {
    pinMode(i, OUTPUT);
    if (i == 24)
      i = 29;
    if (i == 37)
      i = 41;
  }
  
  LAYERS_OFF();
  
  for (int i = 0; i < 64; ++i) {
    cube[i] = 0x00;
  }
  
  Serial.begin(115200);
}

bool receiving = false;
int magic = 0;  // number of magic bytes traversed.
int cube_ref = 0;  // the reference for the input cube byte stream

void loop() {
  if (Serial.available() > 0) {
    char bittyboi = Serial.read();
    if (!receiving) {
      if (bittyboi == MAGIC_BYTES[magic])  // next magic byte received!
        ++magic;
      else
        magic = 0;
      if (magic == 5) {  // all bytes received
        receiving = true;
        magic = 0;
      }
    } else {
      // set 'store' to the values we receive
      store[cube_ref] = bittyboi;
      ++cube_ref;
      if (cube_ref > 63) {
        // switch the references
        if (bufferswitch) {
          store = buffer1;
          cube = buffer0;
          bufferswitch = false;
        } else {
          store = buffer0;
          cube = buffer1;
          bufferswitch = true;
        }
        cube_ref = 0;
        receiving = false;
      }
    }
  }
  
//    __asm("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
//    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
//    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
  
  for (int i = 0; i < 8; ++i) {
    SET_REG_VALS(cube[8*i]);
    SET_TARGET_REG(1);
    SET_REG_VALS(cube[8*i+1]);
    SET_TARGET_REG(2);
    SET_REG_VALS(cube[8*i+2]);
    SET_TARGET_REG(3);
    SET_REG_VALS(cube[8*i+3]);
    SET_TARGET_REG(4);
    SET_REG_VALS(cube[8*i+4]);
    SET_TARGET_REG(5);
    SET_REG_VALS(cube[8*i+5]);
    SET_TARGET_REG(6);
    SET_REG_VALS(cube[8*i+6]);
    SET_TARGET_REG(7);
    SET_REG_VALS(cube[8*i+7]);
    SET_TARGET_REG(0);
    
    
    SET_LAYER(i);  // turn the layer on and let the lights flash for a second
    
    // the number of nop's here determines the brightness of the cube
    __asm("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
    
    LAYERS_OFF();
    
  }
  
}

// JANDY?99?00009009000090090000?99?000000000000000000000000000000000000
// JANDY0000000000000000000000000000000000000000000000000000000000000000
