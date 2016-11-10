int player[3];
int maxEnemies = 16;
int enemies[maxEnemies][3];
int waitPeriod = 100;
int iteration = 0;
int alive = 1;
byte LEDon[4][4][4];
bool positive;

const byte POSITIVE_PINS[8] = {3, 5, 9, 7, 2, 4, 8, 6};
const byte NEGATIVE_PINS[8] = {A3, A5, A1, 12, A2, A4, 13, 11};

void setup() {
  // Make all of the N-wire and P-wire pins outputs
  for (byte i = 0; i < 8; i++) {
    pinMode(POSITIVE_PINS[i], OUTPUT);
    digitalWrite(POSITIVE_PINS[i], HIGH);
    pinMode(NEGATIVE_PINS[i], OUTPUT);
    digitalWrite(NEGATIVE_PINS[i], HIGH);
  }

  Serial.begin(115200);
  Serial.setTimeout(100);
}

/* Function: getValue
   ------------------
   Returns the value in a 4x4x4 values array, each dimension representing an
   axis of the LED cube, that corresponds to the given P-wire and N-wire
   number.

   This function is called by display(), in order to find whether an LED for a
   particular P-wire and N-wire should be switched on.
*/
inline byte getValue(byte values[4][4][4], byte pNum, byte nNum)
{
  byte x;
  byte y = pNum % 4;
  byte z = nNum % 4;

  if (nNum <= 3) {
    if (pNum <= 3) x = 3;
    else x = 0;
  } else {
    if (pNum <= 3) x = 2;
    else x = 1;
  }

  return values[x][y][z];
}

/* Function: display
   -----------------
   Runs through one multiplexing cycle of the LEDs, controlling which LEDs are
   on.

   During this function, LEDs that should be on will be turned on momentarily,
   one row at a time. When this function returns, all the LEDs will be off
   again, so it needs to be called continuously for LEDs to be on.
*/
void display(byte values[4][4][4])
{
  for (byte pNum = 0; pNum < 8; pNum++) { // iterate through P-wires

    // Set up all the N-wires first
    for (byte nNum = 0; nNum < 8; nNum++) { // iterate through N-wires
      byte value = getValue(values, pNum, nNum); // look up the value
      if (value > 0) digitalWrite(NEGATIVE_PINS[nNum], LOW);
      else digitalWrite(NEGATIVE_PINS[nNum], HIGH);
    }

    digitalWrite(POSITIVE_PINS[pNum], LOW);

    delayMicroseconds(1000);

    digitalWrite(POSITIVE_PINS[pNum], HIGH);

  }
}

void loop() {
  // if player is alive
  if (alive) {

    resetLEDs();

    // move player
    if (Serial.available()) {
      char control = Serial.read();
      if (control == 'U') {
        if (player[1] != 3) player[1] = player[1] + 1;

      } else if (control == 'D') {
        if (player[1] != 0) player[1] = player[1] - 1;

      } else if (control == 'L') {
        if (player[0] != 0) player[1] = player[1] - 1;

      } else if (control == 'R') {
        if (player[0] != 3) player[1] = player[1] + 1;

      } else if (control == 'S') {
        // check if enemy has same xy coords (destroy if so)
        destroyEnemies();
      } else if (control == 'Q') {
        resetLEDs();
        alive = 0;
      }
    }
    
    // move enemies
    if (iteration == waitPeriod) {
      if (checkDirection()) moveEnemiesDown();
      else moveEnemiesSide();
      iteration = 0;
      waitPeriod--;
    }
    iteration++;  
  } else {
    death_blink();
    reset_board();
  }
}

static void destroyEnemies() {
  for (int[3] enemy : enemies) {
    if (player[0] == enemy[0] && player[1] == enemy[1]) {
      LEDon[enemy[0]][enemy[1]][enemy[2]] = 0;
    }
  }
}

static void resetLEDs() {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      for (int z = 0; z < 4; z++) {
        LEDon[x][y][z] = 0;
      }
    }
  }
}

static void checkDirection() {
  for (int[3] enemy : enemies) {
    if (positive) {
      if (enemy[0] == 3) positive = !positive;
    } else {
      if (enemy[0] == 0) positive = !positive;
    }
  }
}

static void moveEnemiesDown() {
  for (int[3] enemy : enemies) {
    enemy[2] = enemy[2] - 1;
    if (enemy[2] == 0) {
      alive--;
      reset_board();
    }
  }
}

static void moveEnemiesSide() {
  for (int[3] enemy: enemies) {
    if (positive && enemy[0] != 3) enemy[0] = enemy[0] + 1;
    else if (!positive && enemy[0] != 0) enemy[0] = enemy[0] - 1;
  }

static void death_blink() {
  for (byte i = 0; i < 5; i++) {
    display(allOn);
    delay(1000);
  }
}

static void reset_board() {
  alive = 1;
  

}

