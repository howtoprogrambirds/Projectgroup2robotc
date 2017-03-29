#pragma config(Sensor, S3,     BWSensor,       sensorLightActive) //de licht sensor word geconfigureerd.
#pragma config(Motor,  motorB,          RightMotor,    tmotorNXT, PIDControl, driveRight, encoder) //MotorB word ingesteld als de rechter motor.
#pragma config(Motor,  motorC,          LeftMotor,     tmotorNXT, PIDControl, driveLeft, encoder) // MotorC is de linker motor.
#pragma platform(NXT)
//---------------------------------------------------------------------------------------------------

// Dit zijn variabele gedefineerd van de app die op sharepoint staat.
long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;
// Dit word gebruikt om de input van de app met bluetooth naar de robot te sturen.
//--------------------------------------------------------------------------------

//De functie om de zwarte lijn te volgen word hier gemaakt.
void volg_lijn(){

	if (SensorValue(BWSensor) < 45) { //als er een zwarte lijn wordt gedecteerd zet dan het rechter motortje aan.
			motor[motorC] = 0;
			motor[motorB] = 30;
	}
	else {
		motor[motorC] = 30; //als er niet een zwarte lijn word gedecteerd zet dan het linker motortje aan.
		motor[motorB] = 0;
	}

	//reset de motors naar 0 anders blijft de robot oneindig rondjes rijden.
	motor[motorC] = 0;
	motor[motorB] = 0;
}
//-------------------------------------------------------------------------

//De functie om een geluidje af te spelen.
void sound_repeat() {
	// Het geluidje dat wij hebben gekozen (het kortste geluidje).
	playSound(soundBlip);

	return;
}
//---------------------------------------------------------------------------

task main()
{
	// Dit is van de app die we van sharepoint hebben gehaald.
	TFileIOResult nBTCmdRdErrorStatus;
	int nSizeOfMessage;
  	ubyte nRcvBuffer[kMaxSizeOfMessage];

  	string s;
  	//Hierboven staan de variabelen van de app gedefineerd.
//-----------------------------------------------------------------------------
  
  	while (true)
  	{
		//Er wordt hier een string ontvangen van de app en die wordt op de display van de robot weergeven.
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
    			if(s == "UP"){ 					//Als de ontvangen string van de app "UP" is moet de robot naar voren bewegen.
    				motor[LeftMotor] = 50;			//Het linker motortje gaat aan met 50% van het totale vermogen.
    				motor[RightMotor] = 50;			//Het rechter motortje gaat aan met 50% van het totale vermogen.
    			}//--------------------------------------------------------------------------------------------------------------------------
    			if(s == "DOWN"){				//Als de ontvangen string van de app "DOWN" is moet de robot naar achteren rijden.
    				motor[LeftMotor] = -50;			//Het linker motortje gaat met 50% van het vermogen in tegengestelde richting bewegen.
    				motor[RightMotor] = -50;		//Het Rechter motortje gaat met 50% van het vermogen in tegengestelde richting bewegen.
			}//--------------------------------------------------------------------------------------------------------------------------
    			if(s == "LEFT"){				//Als de ontvangen string "LEFT" is moet de robot een kwartslag naar links draaien.
    				motor[LeftMotor] = -23;			//Het linker motertje gaat met 23% van het vermogen in tegengestelde rirchting bewegen.
    				motor[RightMotor] = 23;			//Het rechter motortje gaat aan met 23% van het totale vermogen.
    				wait1Msec(800);				//Het stukje code hierboven moet 800 miliseconden worden uitgevoerd.
    				s = "C";				//Nadat de robot heeft gedraaid word de string variable naar "C" gezet om de robot te stoppen.
    			}//---------------------------------------------------------------------------------------------------------------------------
    			if(s == "RIGHT"){				//Als de ontvangen string "RIGHT" is moet de robot naar rechts draaien.
    				motor[LeftMotor] = 23;
    				motor[RightMotor] = -23;
    				wait1Msec(800);
    				s = "C";				//Het stuk code hierboven is hetzelfde als naar links sturen maar dan in de andere richting.
    			}//----------------------------------------------------------------------------------------------------------------------------
    			if(s == "C" ) {					//Als de ontvangen string "C" is dan moet de robot stoppen. (stilstaan)
		    	  	motor[LeftMotor] = 0;			//Het linker motortje wordt uit gezet.
    				motor[RightMotor] = 0;			//Het rechter motortje wordt uit gezet.
			}//----------------------------------------------------------------------------------------------------------------------------
        		if(s == "A"){					//Als de string "A" is moet de functie volg lijn worden uitgevoerd.
       			 volg_lijn();					//Functie volg lijn wordt aangeroepen.
			}//----------------------------------------------------------------------------------------------------------------------------
		}
		if(motor[motorC] || motor[motorB]){			//Als 1 van de 2 motortjes aanstaat (of allebei)speel dan een geluidje af.
      			sound_repeat();					//De functie sound_repeat om een geluidje af te spelen wordt aangeroepen.
   	   }//------------------------------------------------------------------------------------------------------------------------------
	wait1Msec(100);							//Wacht 100 miliseconden voor de volgende input van de app wordt ontvangen.
	}
  return;
}
