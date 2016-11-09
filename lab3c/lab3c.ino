player
numEnemies
enemies
velocity
alive

// vector of int[3] for enemies, int[3] holds x, y, z positions
// int[3] for player, holds x, y, z positions
// int for velocity
// int for numEnemies (might not need this if we have vector
// bool for alive

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
 * ------------------
 * Returns the value in a 4x4x4 values array, each dimension representing an
 * axis of the LED cube, that corresponds to the given P-wire and N-wire
 * number.
 *
 * This function is called by display(), in order to find whether an LED for a
 * particular P-wire and N-wire should be switched on.
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
 * -----------------
 * Runs through one multiplexing cycle of the LEDs, controlling which LEDs are
 * on.
 *
 * During this function, LEDs that should be on will be turned on momentarily,
 * one row at a time. When this function returns, all the LEDs will be off
 * again, so it needs to be called continuously for LEDs to be on.
 */
void display(byte values[4][4][4])
{
  for (byte pNum = 0; pNum < 8; pNum++) { // iterate through P-wires

    // Set up all the N-wires first
    for (byte nNum = 0; nNum < 8; nNum++) { // iterate through N-wires
      byte value = getValue(values, pNum, nNum); // look up the value
      if(value > 0) digitalWrite(NEGATIVE_PINS[nNum], LOW);
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
    // listen for keys for movement
    // listen for keys for shooting
    // destory enemies if hit
    // move player if moved
    // move enemies
    // check if enemies are at base
    // set alive accordingly
    
  } else {
    // prompt user to begin
  }
}
