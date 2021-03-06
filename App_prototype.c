#pragma config(Sensor, S2,     ColorSensor,    sensorColorNxtFULL)//Configure color light sensor.
#pragma config(Sensor, S3,     sensorLeft,     sensorLightActive) //Configure black&white light sensor.
#pragma config(Motor,  motorB,          RightMotor,    tmotorNXT, PIDControl, driveRight, encoder) //Configure motorb as right motor.
#pragma config(Motor,  motorC,          LeftMotor,     tmotorNXT, PIDControl, driveLeft, encoder) // configure motorc as left motor.
#pragma platform(NXT)
//---------------------------------------------------------------------------------------------------

// Define app variables.
long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;
// These are used to send signals over bluetooth to the robot.
//--------------------------------------------------------------------------------

//Function to follow a black line is created here.

// stating that some functions exist and making a constant variable for the speed.
void forward(int speed);
void RGBValue(string sColor);
void searchLine(int speed, string sColor);
const int speed = 10;
bool stop = true;

void RGBValue(string &sColor) {
	  while (true) {
	    switch (SensorValue[ColorSensor])
	    {
	      case BLACKCOLOR:    sColor = "Black";     return;
	      case BLUECOLOR:     sColor = "Blue";      return;
	      case GREENCOLOR:    sColor = "Green";     return;
	      case YELLOWCOLOR:   sColor = "Yellow";    return;
	      case REDCOLOR:      sColor = "Red";       return;
	      case WHITECOLOR:    sColor = "White";     return;
	      default:            sColor = "???";       return;
			}
		}
}

// function to drive the robot forwards, only gets called if he only detects non black light.
void forward (int speed) {
	motor[RightMotor] = speed;
	motor[LeftMotor] = speed;
	return;
}

// function to go left or right depending on which of the 2 sensor detects a black line.
void searchLine(int speed, string sColor) {
	while (true) {
		RGBValue(sColor);
		if 	(SensorValue(sensorLeft) <= 45){
			motor[RightMotor] = 40;
		}
		if (sColor == "Black"){
			motor[LeftMotor] = 40;
		}
		if (SensorValue(sensorLeft) > 45 && (sColor != "Black")) {
			return;
		}
	}
}

//-------------------------------------------------------------------------

//Function to play a sound is created here.
void sound_repeat() {
	// The sound we chose. (This is the shortest sound).
	playSound(soundBlip);

	return;
}
//---------------------------------------------------------------------------

task main()
{
	// Variables from the app
	TFileIOResult nBTCmdRdErrorStatus;
	int nSizeOfMessage;
  	ubyte nRcvBuffer[kMaxSizeOfMessage];

  	string s;
  	//The variables are defined.
//-----------------------------------------------------------------------------

  	while (true)
  	{
		//The robot receives a string that the robot will desplay.
    		nSizeOfMessage = cCmdMessageGetSize(INBOX);
				string sColor;
				if (stop == false) {
						forward(speed);
						RGBValue(sColor);
						if (SensorValue(sensorLeft) <= 45 || sColor == "Black")
							searchLine(speed, sColor);
				}
    		if (nSizeOfMessage > kMaxSizeOfMessage){
      			nSizeOfMessage = kMaxSizeOfMessage;
      		}
    		if (nSizeOfMessage > 0){
    			nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    			nRcvBuffer[nSizeOfMessage] = '\0';
    			s = "";
    			stringFromChars(s, (char *) nRcvBuffer);
    			displayCenteredBigTextLine(4, s);
//---------------------------------------------------------------------------------------------------------------------------------------------------
    			if(s == "UP"){ 					//If received string equals "UP" move robot forward.
    				motor[LeftMotor] = 30;
    				motor[RightMotor] = 30;			//Turn on both motors with 50% power.
    			}//--------------------------------------------------------------------------------------------------------------------------
    			if(s == "DOWN"){				//If received string equals "DOWN" move robot backwards.
    				motor[LeftMotor] = -30;
    				motor[RightMotor] = -30;		//Both motors turn on with 50% power in opposite direction.
					}//--------------------------------------------------------------------------------------------------------------------------
    			if(s == "LEFT"){				//If received string equals "LEFT" turn robot to the left.
    				motor[LeftMotor] = -23;
    				motor[RightMotor] = 23;			//Left motor moves backwards while right motor moves forward at 23% power.
    				wait1Msec(800);				//The robot has to turn for a total of 800 miliseconds.
    				s = "C";				//Reset string to "C" to stop the robot
    			}//---------------------------------------------------------------------------------------------------------------------------
    			if(s == "RIGHT"){				//If the received string equals "RIGHT" turn the robot to the right.
    				motor[LeftMotor] = 23;
    				motor[RightMotor] = -23;
    				wait1Msec(800);
    				s = "C";				//Same code as moving left, but now it moves in opposite direction.
    			}//----------------------------------------------------------------------------------------------------------------------------
    			if(s == "C" ) {					//If received string equals "C" stop the robot. (this is the break)
		    	  motor[LeftMotor] = 0;
    				motor[RightMotor] = 0;			//Both motors are turned off and the robot stops moving.
						stop = true;
    				}//----------------------------------------------------------------------------------------------------------------------------
        		if(s == "A"){					//If the received string equals "A" execute function: volg_lijn.
							stop = false;

					}//----------------------------------------------------------------------------------------------------------------------------
		}
		if(motor[motorC] || motor[motorB]){			//If either motor (or both) is turned on, play a sound.
      			sound_repeat();					//Call function: sound_repeat.
   	}//------------------------------------------------------------------------------------------------------------------------------
	wait1Msec(100);							//Wait 100 miliseconds to receive a new command from app.
	}
  return;
}
