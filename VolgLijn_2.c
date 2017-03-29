#pragma config(Sensor, S3,     BWSensor,       sensorLightActive)
#pragma config(Motor,  motorB,          motorRight,    tmotorNXT, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorC,          motorLeft,     tmotorNXT, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void sound_repeat() {
		while(motor[motorC] || motor[motorB]) {   // as long as motorC or motorB are on
		playSound(soundBeepBeep);  		  // play sound "soundBeepBeep"
		wait1Msec(1000);                          // wait one second
	}
}

task main()
{
	//ClearTimer(T1);
	//sound_repeat();
	while (1) {
		if (SensorValue(BWSensor) < 45) {   	// if the value that the sensor picks up is below 45
			motor[motorC] = 0; 		// set motor C speed to: 0 and motor B speed to: 30
			motor[motorB] = 30;
		}
		else {
			motor[motorC] = 30;  		// set motor C speed to: 30 and motor B speed to: 0
			motor[motorB] = 0;
		}
		sound_repeat(); 			// call upon function sound_repeat
	}
	motor[motorC] = 0; 				// turn motor C and B off
	motor[motorB] = 0;
}
