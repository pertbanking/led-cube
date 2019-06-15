
#define LAYERS_OFF() PORTB = 0;
#define SET_LAYER(layer) PORTB = 1 << layer;
#define SET_TARGET_REG(reg) PORTC = reg & 0b00000111;
#define SET_REG_VALS(vals) PORTD = vals << 2;

const char* MAGIC_BYTES = "JANDY";  // the magic bytes at the beginning of a cube frame
                                    // yes, this is an inside joke.

char** cube;  // the data for the current frame of the cube
              // 8x8x8: an 8x8 square of 8-bit chars

void setup() {
  // pins A0-A2 (PC0-PC2) are for the decoder inputs
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  // pins 2-9 (PD2-PD7 and PB0-PB1) are the data bits for each row
  for (int i = 2; i <= 11; ++i) {
    pinMode(i, OUTPUT);
  }
  
  cube = new char*[8];
  for (int i = 0; i < 8; ++i) {
    cube[i] = new char[8];
    for (int j = 0; j < 8; ++j) {
      cube[i][j] = 0x00;
    }
  }
  
  Serial.begin(115200);
}

bool receiving = false;
int magic = 0;  // number of magic bytes traversed.
int cube_ref = 0;  // the reference for the input cube byte stream

void loop() {
  if (Serial.available() > 0) {
//    Serial.write("Receiving!\n");
    char bittyboi = Serial.read();
    if (!receiving) {
      if (bittyboi == MAGIC_BYTES[magic])  // next magic byte received!
        ++magic;
      else
        magic = 0;
      if (magic == 5) {  // all bytes received
//      Serial.write("Magic received!\n");
        receiving = true;
        magic = 0;
      }
    } else {
      // set 'cube' to the values we receive
      int layer = cube_ref / 8;
      cube[layer][cube_ref % 8] = bittyboi;
      ++cube_ref;
      if (cube_ref > 63) {
//        for (int i = 0; i < 7; ++i) {
//          for (int j = 0; j < 7; ++j) {
//            Serial.println(cube[i][j]);
//          }
//        }
        cube_ref = 0;
        receiving = false;
      }
    }
  }
  
  for (int i = 0; i < 8; ++i) {
    LAYERS_OFF();
    SET_REG_VALS(cube[i][0]);
    SET_TARGET_REG(1);
    SET_REG_VALS(cube[i][1]);
    SET_TARGET_REG(2);
    SET_REG_VALS(cube[i][2]);
    SET_TARGET_REG(3);
    SET_REG_VALS(cube[i][3]);
    SET_TARGET_REG(4);
    SET_REG_VALS(cube[i][4]);
    SET_TARGET_REG(5);
    SET_REG_VALS(cube[i][5]);
    SET_TARGET_REG(6);
    SET_REG_VALS(cube[i][6]);
    SET_TARGET_REG(7);
    SET_REG_VALS(cube[i][7]);
    SET_TARGET_REG(0);
    SET_LAYER(i);  // turn the layer on and let the lights flash for a second
    
    // the number of nop's here determines the brightness of the cube
    __asm("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
    
  }
  
}

// JANDY?99?00009009000090090000?99?000000000000000000000000000000000000
