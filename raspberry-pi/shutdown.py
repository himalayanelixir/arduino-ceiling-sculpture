#!/home/pi/code/pi_env/bin/python3
# Copyright 2020 Harlen Bains
# linted using pylint
# formatted using black
"""This program shutsdown the Raspberry Pi safely. It uses the shutdown function
  from button.py and blinks the button leds 5 times before shutting down.
"""

import os
import RPi.GPIO as GPIO  # pylint: disable=import-error
from button import shutdown


def main():
    """Shutsdown the Raspberry Pi
    """
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    led_pin = 23
    GPIO.setup(led_pin, GPIO.OUT)
    os.system("sudo systemctl stop button.service")
    shutdown(led_pin)


if __name__ == "__main__":
    main()