 #include <Adafruit_NeoPixel.h>
#define PIN 2     // input pin Neopixel is attached to
#define NUMPIXELS      100// number of neopixels in Ring

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay

int bLS = 0; // Left Button
int bMS = 0; // Middle Button
int bRS = 0; // Right Button

int cooldown = 0; // Checks if the player has shot in the last button press

int tick = 0; // Keeps track of game stage

int blueD = 0;

int sp[90]; // Screenspace for Alien and bullets
int playerPos = 0; // Position of player on bottom row

// Starts input channels and allows for serial logging
void setup() {
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pixels.begin(); // Initializes the NeoPixel library.
  Serial.begin(9600);

  for (int i = 0; i < 90; i++) {
    sp[i] = 0;
  }
}

// Fish?
void loop() {
  gameTick();
  Serial.print(bMS);
  //setColor();

  delay(delayval); // Delay for a period of time (in milliseconds).

  pixels.show();
}


// Moves game state forward
void gameTick() {
  checkButtons(); // Checks for player inputs
  alienMovement(); // Moves every alien forward
  if (tick == 1) spawnAlien(); // Spawns an alien every 2 frames
  setColor(); // Refreshes screen

  if (tick == 2)
  {
    tick = 0; // Resets game tick (Only two)
  }
  tick++; // Next tick

  // Processes player input
  if (bMS == 1 && cooldown == 0) {
    shoot();
  } else if (bRS == 1) {
    if (playerPos != 0) {
      playerPos--;
    }
  } else if (bLS == 1) {
    if (playerPos != 9) {
      playerPos++;
    }
  }

  if (sp[89] == 3) {
    miami();
  }

  if (cooldown > 0) cooldown--;
  //updateBullets();
}


// Bang bang
void shoot() {
  cooldown = 4;
  for (int i = 0; i < 9; i++) {
    if (i % 2 == 0) {
      sp[i * 10 + (9 - playerPos)] = 5; // Even
    } else {
      sp[i * 10 - (9 - playerPos) + 9] = 5; // Odd
    }
  }
  //sp[80 + (9 - playerPos)] = 5; // Even
}

void updateBullets() {
  for (int i = 0; i < 9; i++) {
    if (i % 2 == 0) {
      sp[i * 10 + (9 - playerPos)] = 5; // Even
    } else {
      sp[i * 10 - (9 - playerPos) + 9] = 5; // Odd
    }
  }
}

// Spawns an alien
void spawnAlien() {
  sp[0] = 3;
}

// Moves each alien forward per tick
void alienMovement() {
  for (int i = 90; i > -1; i--)
  {
    if (sp[i] != 0 && sp[i] < 4)
    {
      if (sp[i + 1] == 5) {
        sp[i] = 0;
      }

      if (i < 90) {
        sp[i + 1] = sp[i];
        sp[i] = 0;
      } else {
        //End State
      }
    }
  }
}

// Refreshes screen with appropriate colors
void setColor(){
  for (int i = 0; i < 90; i++)
  {
    if (sp[i] != 0 && sp[i] < 4) {
      pixels.setPixelColor(i, 255, 0, 0);
    }
    else if (sp[i] == 0) {
      pixels.setPixelColor(i, 0, 0, 0);
    }
    else if (sp[i] == 5) {
      pixels.setPixelColor(i, 0, 255, 0);
      sp[i] = 0;
    }
  }

  if (cooldown != 0) blueD = cooldown * 20;
  else blueD = 0;

  // Shows player position with blue LED
  for (int i = 0; i < 10; i++) {
    if (i == playerPos) pixels.setPixelColor(i + 90, blueD, blueD, 255);
    else pixels.setPixelColor(i + 90, 0, 0, 0);
  }
}

// Player Inputs
void checkButtons() {
  bLS = digitalRead(7);
  bMS = digitalRead(6);
  bRS = digitalRead(5);
}

void miami() {
  int r;
  int g;
  int b;
  int count;
  while (true) {
    r += random(-1, 1);
    g += random(-1, 1);
    b += random(-1, 1);

    if (count < 100) {
      pixels.setPixelColor(count, r, g, b);
      count++;
    } else {
      count = 0;
    }
    pixels.show();
  }
}
