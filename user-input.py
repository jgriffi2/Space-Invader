import serial 
import readchar

class UserInputController(object):
  def __init__(self):
    ser = serial.Serial('/dev/cu.usbserial-ADAPFFKjO', 9600)

  def runInputLoop(self):
    while True:
      char = repr(readchar.readkey())
      if char == '\\x1b[A' or char == 'w':
        self.ser.write('U')
      elif char == '\\x1b[C' or char == 'd':
        self.ser.write('R')
      elif char == '\\x1b[D' or char == 'a':
        self.ser.write('L')
      elif char == '\\x1b[B' or char == 's':
        self.ser.write('D')
      else char == 'q':
        self.ser.write('D')
        return

def main():
  user_input = UserInputController()
  user_input.runInputLoop()

if __name__ == "__main__":
    main()

