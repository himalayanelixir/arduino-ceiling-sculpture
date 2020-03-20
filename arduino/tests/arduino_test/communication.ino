//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// This file contains functions that help the arduino communitate with the
// raspberry pi

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// handles the receiving of data over the serial port
void RecvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && new_data == false)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        received_chars[ndx] = rc;
        ndx++;
        if (ndx >= num_chars)
        {
          ndx = num_chars - 1;
        }
      }
      else
      {
        received_chars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        new_data = true;
        Serial.print("-----");
        Serial.print(received_chars);
        Serial.println("----");
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
}

// sends message back to raspberry pi saying the command has been executed
void Finished()
{
  timeout_counter = 0;
    // TODO - not sure if we need this 
    for (int i = 0; i < NUMBER_MOTORS; i++)
    {
      my_servo[i].write(90);
      motor_commands[i][0] = 0;
      motor_commands[i][1] = 0;
      motor_commands[i][2] = 0;
      motor_commands[i][3] = IGNORE_INPUT_TIME;
    }

  Serial.println("Finished Current Job!");
  Serial.println(">");
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void PopulateArray()
{

  // temp string used to store the char array
  // easier to do opperations on string than chars
  String received_string = "";
  // give the string the value of the char array
  received_string = received_chars;

  // now lets populate the motor command array with values from the received
  // string
  for (int i = 0; i < NUMBER_MOTORS; i++)
  {
    // we break everything in to pairs of values
    int search1 = (i * 2);
    int search2 = ((i * 2) + 1);

    String value1 = getValue(received_string, ',', search1);
    String value2 = getValue(received_string, ',', search2);

    if (value1 == "Up")
    {
      motor_commands[i][0] = 1;
    }
    else if (value1 == "Down")
    {
      motor_commands[i][0] = 2;
    }
    else if (value1 == "None")
    {
      motor_commands[i][0] = 0;
    }
    else if (value1 == "Reset")
    {
      motor_commands[i][0] = 3;
    }
    else
    {
      // Sends Error Message
    }

    motor_commands[i][1] = value2.toInt();
  }

  // print array
  //for (int i = 0; i < NUMBER_MOTORS; i++)
  //{
  //  Serial.print("Motor Number: ");
  //  Serial.print(i);
  //  Serial.print(" - Direction: ");
  //  Serial.print(motor_commands[i][0]);
  //  Serial.print(" - Rotations: ");
  //  Serial.println(motor_commands[i][1]);
  //}
}

// helps get a particular value from the incoming data string
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// sends error message back to raspberry pi
void Invalid()
{
  Serial.print("Arduino: ");
  Serial.println("Invalid Input!");
  Serial.println(">");
}