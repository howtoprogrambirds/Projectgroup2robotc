task main()
{
 string varA = "hello";
  int varB = 100;

  // print varA on line 1 (lines start at 0, so this is the 2nd line on the screen)
  nxtDisplayTextLine(1, "varA: %s", varA);

  // print varB on line 2
  nxtDisplayTextLine(2, "varB: %d", varB);

  // print both varA and varB on line 3
  nxtDisplayTextLine(3, "Both: %s, %d", varA, varB);

  // wait until the cows come home
  while(true);
}
