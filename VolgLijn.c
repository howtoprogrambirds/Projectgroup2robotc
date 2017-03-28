#pragma config(Sensor, S3,     BWSensor,       sensorLightActive)
#pragma config(Motor,  motorB,          motorRight,    tmotorNXT, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorC,          motorLeft,     tmotorNXT, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void sound_repeat() {
		while(motor[motorC] || motor[motorB]) {
		playSound(soundBeepBeep);
		wait1Msec(1000);
	}
}

task main()
{
	//ClearTimer(T1);
	//sound_repeat();
	while (1) {
		if (SensorValue(BWSensor) < 45) {
			motor[motorC] = 0;
			motor[motorB] = 30;
		}
		else {
			motor[motorC] = 30;
			motor[motorB] = 0;
		}

		sound_repeat();

	}
	motor[motorC] = 0;
	motor[motorB] = 0;
}
