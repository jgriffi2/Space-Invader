int player[3];
int maxEnemies = 16;
int enemies[16][3];
int waitPeriod = 200;
int addMultiplier = 2;
int iteration = 0;
int alive = 1;
byte LEDon[4][4][4];
bool positive;
int addEnemyIteration = 0;
int x = 2;
int y = 1;
int z = 0;
int death_blink_wait = 25;

const byte POSITIVE_PINS[8] = {3, 5, 9, 7, 2, 4, 8, 6};
const byte NEGATIVE_PINS[8] = {A3, A5, A1, 12, A2, A4, 13, 11};

static void movePlayer(char control) {
  LEDon[player[z]][player[y]][player[2]] = 0;
  if (control == 'U') {
    if (player[y] < 3) player[y] = player[y] + 1;

  } else if (control == 'D') {
    if (player[y] > 0) player[y] = player[y] - 1;

  } else if (control == 'L') {
    if (player[2] > 0) player[2] = player[2] - 1;

  } else if (control == 'R') {
    if (player[2] < 3) player[2] = player[2] + 1;
  }
}

static void destroyEnemies() {
  for (int i = 0; i < maxEnemies; i++) {
    int enemy[3];
    memcpy(enemy, enemies[i], sizeof(int[3]));
    if (enemy[x] != -1) {
      if (player[x] == enemy[x] && player[y] == enemy[y]) {
        LEDon[enemy[z]][enemy[y]][enemy[x]] = 0;
        int temp[3] = {0, 0, -1};
        memcpy(enemies[i], temp, sizeof(int[3]));
      }
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

static bool checkDirection() {
  for (int i = 0; i < maxEnemies; i++) {
    int enemy[3];
    memcpy(enemy, enemies[i], sizeof(int[3]));
    if (enemy[x] != -1) {
      if (positive) {
        if (enemy[x] == 3) {
          positive = !positive;
          return true;
        }
      } else {
        if (enemy[x] == 0) {
          positive = !positive;
          return true;
        }
      }
    }
  }
  return false;
}

static void reset_board() {
  int newPlayer[3] = {0, 0, 0};
  memcpy(player, newPlayer, sizeof(int[3]));
  for (int enemy = 0; enemy < maxEnemies; enemy++) {
    int temp[3] = {0, 0, -1};
    memcpy(enemies[enemy], temp, sizeof(int[3]));
  }
  waitPeriod = 100;
  iteration = 0;
  resetLEDs();
}

static void moveEnemiesDown() {
  for (int i = 0; i < maxEnemies; i++) {
    int enemy[3];
    memcpy(enemy, enemies[i], sizeof(int[3]));
    if (enemy[x] != -1) {
      enemy[z] = enemy[z] - 1;
      memcpy(enemies[i], enemy, sizeof(int[3]));
      if (enemy[z] == 0) {
        alive--;
        reset_board();
      }
    }
  }
}

static void moveEnemiesSide() {
  for (int i = 0; i < maxEnemies; i++) {
    int enemy[3];
    memcpy(enemy, enemies[i], sizeof(int[3]));
    if (enemy[x] != -1) {
      if (positive && enemy[x] != 3) enemy[x] = enemy[x] + 1;
      else if (!positive && enemy[x] != 0) enemy[x] = enemy[x] - 1;
      memcpy(enemies[i], enemy, sizeof(int[3]));
    }
  }
}

static void death_blink() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        LEDon[i][j][k] = 1;
      }
    }
  }
  int death_blink_period = 0;
  while (death_blink_period < death_blink_wait) {
    display(LEDon);
    delay(1000);
    death_blink_period = death_blink_period + 1;
  }
}

static void addEnemy() {
  for (int i = 0; i < maxEnemies; i++) {
    int enemy[3];
    memcpy(enemy, enemies[i], sizeof(int[3]));
    if (enemy[x] == -1) {
      int temp[3] = { 3, (int)random(0, 4), (int)random(0, 4) };
      memcpy(enemies[i], temp, sizeof(int[3]));
      break;
    }
  }
}

void setup() {
  // Make all of the N-wire and P-wire pins outputs
  for (byte i = 0; i < 8; i++) {
    pinMode(POSITIVE_PINS[i], OUTPUT);
    digitalWrite(POSITIVE_PINS[i], HIGH);
    pinMode(NEGATIVE_PINS[i], OUTPUT);
    digitalWrite(NEGATIVE_PINS[i], HIGH);
  }
  reset_board();

  Serial.begin(115200);
  Serial.setTimeout(100);
}

inline byte getValue(byte values[4][4][4], byte pNum, byte nNum)
{
  byte xp;
  byte yp = pNum % 4;
  byte zp = nNum % 4;

  if (nNum <= 3) {
    if (pNum <= 3) xp = 3;
    else xp = 0;
  } else {
    if (pNum <= 3) xp = 2;
    else xp = 1;
  }

  return values[xp][yp][zp];
}

void display(byte values[4][4][4]) {
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

void turnLEDsOn() {
  LEDon[player[z]][player[y]][player[x]] = 1;
  for (int i = 0; i < maxEnemies; i++) {
    int enemy[3];
    memcpy(enemy, enemies[i], sizeof(int[3]));
    if (enemy[x] != -1) {
      LEDon[enemy[z]][enemy[y]][enemy[x]] = 1;
    }
  }
}

void loop() {
  // if player is alive
  if (alive) {

    resetLEDs();

    // move player
    if (Serial.available()) {
      char control = Serial.read();
      if (control == 'U' || control == 'D' || control == 'L' || control == 'R') {
        movePlayer(control);
      } else if (control == 'S') {
        destroyEnemies();
      } else if (control == 'Q') {
        resetLEDs();
        alive = 0;
      }
    }

    // move enemies
    if (iteration >= waitPeriod) {
      if (checkDirection()) moveEnemiesDown();
      else moveEnemiesSide();
      iteration = 0;
      if (waitPeriod > 0) waitPeriod--;
    }
    if (addEnemyIteration >= addMultiplier * waitPeriod) {
      addEnemyIteration = 0;
      addEnemy();
    }
    iteration++;
    addEnemyIteration++;
    turnLEDsOn();
    display(LEDon);
  } else {
    death_blink();
    reset_board();
  }
}

