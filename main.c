#pragma config(Sensor, S1,     SonarSensor,    sensorSONAR)
#pragma config(Sensor, S2,     RightSensor,    sensorColorNxtRED)
#pragma config(Sensor, S3,     LeftSensor,     sensorLightActive)
#pragma config(Motor,  motorA,          FireMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorNXT, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorC,          LeftMotor,     tmotorNXT, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(NXT)

// Define app variables to send signals over bluetooth to the robot.
const int kMaxSizeOfMessage = 30;
const int INBOX = 5;

// global variable definitions
const int default_speed = 24;
int speed = 0;
bool stop = true;

// function declarations
void forward(int speed);
void searchLine(int speed, float sumL, float sumR);
void slowDownStop();
void stopObstacle();

// Slow down function so the robot does not stop abruptly
void slowDownStop() {
	for(; speed > 0; speed--) {
		motor[LeftMotor] = speed;
		motor[RightMotor] = speed;
		wait1Msec(50);
	}
	stopAllMotors();
	stop = true;
	return;
}

// Slows down and stops in a smooth manner when obstacle is detected
void stopObstacle() {
	if (speed > 0 && stop == false) {
		if(SensorValue(SonarSensor) < 24)
		{
			 slowDownStop();
		}
	}
	return;
}

// function to drive the robot forwards, only gets called if both sensors detect white.
void forward (int speed) {
	motor[RightMotor] = speed;
	motor[LeftMotor] = speed;
	stop = false;
	return;
}

// function to go left or right depending on which of the 2 sensor detects a black line.
void searchLine(int speed, float sumL, float sumR) {
	while (true) {
		stopObstacle();  // stop slowly for obstacle if there is an object within range

		if(sumL < 0.5) {
			motor[RightMotor] = speed + (48*sumL);
			motor[LeftMotor] = 0;
		} else if (sumR < 0.5){
			motor[RightMotor] = 0;
			motor[LeftMotor] = speed + (48*sumR);
		}
		return;
	}
}

task main() {
	// Define variables for the mobile app
	TFileIOResult nBTCmdRdErrorStatus;
	int nSizeOfMessage;
	ubyte nRcvBuffer[kMaxSizeOfMessage];
	string s;

	while (true) {
		int L = SensorValue(LeftSensor);
		int R = SensorValue(RightSensor);

		float upL = L-15;
		float downL = 72-15;
		float sumL = (upL/downL);

		float upR = R-10;
		float downR = 53-10;
		float sumR = (upR/downR);

		displayCenteredTextLine(0, "Sonar: %d", SensorValue[SonarSensor]);  	// display Sonar Distance of Obstacle
		displayCenteredTextLine(1, "Color: %d", SensorValue[RightSensor]);  	// display RGB Value
		displayCenteredTextLine(2, "Light: %d", SensorValue[LeftSensor]);  	// display Light Value
		displayCenteredTextLine(3, "sumL %.2f", sumL);
		displayCenteredTextLine(4, "sumR %.2f", sumR);

		//wait1Msec(100);  // Wait 100 milliseconds to help display correctly
		stopObstacle();  // Stop slowly for obstacle

		// The robot receives a string that the robot will display.
		nSizeOfMessage = cCmdMessageGetSize(INBOX);

		if (stop == false) {
			forward(speed);

			if (sumL < 0.5 || sumR < 0.5) {
				searchLine(speed, sumL, sumR);
			}

			if (sumL < 0.5 && sumR < 0.5){  // if both sensors are black stop slowly (intersection)
				slowDownStop();  // stop slowly
			}
		}

		if (nSizeOfMessage > kMaxSizeOfMessage){
			nSizeOfMessage = kMaxSizeOfMessage;
		}

		if (nSizeOfMessage > 0){
			nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
			nRcvBuffer[nSizeOfMessage] = '\0';
			s = "";
			stringFromChars(s, (char *) nRcvBuffer);
			displayCenteredBigTextLine(6, s);
			if(s == "UP"){ 											// If received string equals "UP" move robot forward.
				speed = default_speed;
				motor[LeftMotor] = 30;
				motor[RightMotor] = 30;						// Turn on both motors with 50% power.
			}//---------------------------------------------------------------------------------------------------------------------------
			if(s == "DOWN"){										// If received string equals "DOWN" move robot backwards.
				motor[LeftMotor] = -30;
				motor[RightMotor] = -30;					// Both motors turn on with 50% power in opposite direction.
			}//-----------------------------------------------------------------------------------------------------------------------
			if(s == "LEFT"){										// If received string equals "LEFT" turn robot to the left.
				motor[LeftMotor] = -15;
				motor[RightMotor] = 15;						// Left motor moves backwards while right motor moves forward at 23% power.
				wait1Msec(400);										// The robot has to turn for a total of 800 miliseconds.
				s = "C";													// Reset string to "C" to stop the robot
			}//---------------------------------------------------------------------------------------------------------------------------
			if(s == "RIGHT"){										// If the received string equals "RIGHT" turn the robot to the right.
				motor[LeftMotor] = 15;
				motor[RightMotor] = -15;
				wait1Msec(400);
				s = "C";													// Same code as moving left, but now it moves in opposite direction.
			}//---------------------------------------------------------------------------------------------------------------------------
			if(s == "C" ) {											// If received string equals "C" stop the robot. (this is the break)
				slowDownStop();					// Both motors are turned off to stop robot from moving.
			}//---------------------------------------------------------------------------------------------------------------------------
			if(s == "A"){												// If the received string equals "A" execute function: volg_lijn.
				speed = default_speed;
				stop = false;
			}//---------------------------------------------------------------------------------------------------------------------------
			if(s == "FIRE"){												// If the received string equals "FIRE" turn on arm motor.
				motor[FireMotor] = 60;
				wait1Msec(200);
				motor[FireMotor] = 0;

			}//---------------------------------------------------------------------------------------------------------------------------
			if(stop == true) {
				s = "C";
			}
		}
		if(motor[motorC] || motor[motorB]){		// If either motor (or both) is turned on, play a sound.
			playSound(soundBlip);           		// Play a sound
		}
		wait1Msec(100);												// Wait 100 miliseconds to receive a new command from app.
	}
	return;
}
