#pragma config(Sensor, S3,     BWSensor,       sensorLightActive) //Configure light sensor.
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
void volg_lijn(){

	if (SensorValue(BWSensor) < 45) { //If the lightsensor detects black right motor turns on
		motor[motorC] = 0;
		motor[motorB] = 30;
	}
	else {
		motor[motorC] = 30; //If the light sensor doesn't detect black left motor turns on.
		motor[motorB] = 0;
	}

	//Turn motors off.
	motor[motorC] = 0;
	motor[motorB] = 0;
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
    				motor[LeftMotor] = 50;
    				motor[RightMotor] = 50;			//Turn on both motors with 50% power.
    			}//--------------------------------------------------------------------------------------------------------------------------
    			if(s == "DOWN"){				//If received string equals "DOWN" move robot backwards.
    				motor[LeftMotor] = -50;
    				motor[RightMotor] = -50;		//Both motors turn on with 50% power in opposite direction.
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
					}//----------------------------------------------------------------------------------------------------------------------------
        		if(s == "A"){					//If the received string equals "A" execute function: volg_lijn.
       			 volg_lijn();					//Call function: volg_lijn.
					}//----------------------------------------------------------------------------------------------------------------------------
		}
		if(motor[motorC] || motor[motorB]){			//If either motor (or both) is turned on, play a sound.
      			sound_repeat();					//Call function: sound_repeat.
   	}//------------------------------------------------------------------------------------------------------------------------------
	wait1Msec(100);							//Wait 100 miliseconds to receive a new command from app.
	}
  return;
}
