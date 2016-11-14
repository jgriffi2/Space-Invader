# pip install serial
# pip install readchar

import serial 
import readchar

class UserInputController(object):
  def __init__(self):
    self.ser = serial.Serial('/dev/cu.usbserial-ADAPFFKjO', 115200)

  def runInputLoop(self):
    while True:
      char = repr(readchar.readkey())
      if char == "'\\x1b[A'" or char == "'w'":
        self.ser.write(b"U")
      elif char == "'\\x1b[C'" or char == "'d'":
        self.ser.write(b"R")
      elif char == "'\\x1b[D'" or char == "'a'":
        self.ser.write(b"L")
      elif char == "'\\x1b[B'" or char == "'s'":
        self.ser.write(b"D")
      elif char == "' '":
        self.ser.write(b"S")
      elif char == "'q'":
        self.ser.write(b"Q")
        return
      else:
        continue

def main():
  user_input = UserInputController()
  user_input.runInputLoop()
  return

if __name__ == "__main__":
    main()

