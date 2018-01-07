void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(CS_pin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  String date = __DATE__;
  //  String m = date.substring(0,3);
  //  String y = date.substring(date.length()-4,date.length());
  String fileDate = date.substring(0, 3);
  fileDate = date.substring(date.length() - 2, date.length()) + month2int(fileDate);
  fileName = "log_" + fileDate + ".cvs";

  Serial.println("Checking file: " + fileName + "...");


  if (SD.exists(fileName)) {
    //  if (myFile) {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    Serial.println("File found.");
    myFile = SD.open(fileName, FILE_WRITE);
  } else {
    //myFile = SD.open(fileName, FILE_WRITE);
    //Write cvs header
    myFile = SD.open(fileName, FILE_WRITE);
    Serial.println("Open a new file.");
    myFile.println("TIME,T1,T2,T3,T4,P1,P2,D1,D2,D3,OVER1,OVER2,LEX,H,STATE");

  }
  if (!myFile) {
    Serial.println("Open Failure.");
  }
  myFile.close();


}

void writeFile(String time_message) {
  myFile = SD.open(fileName, FILE_WRITE);

  // if the file opened okay, write to it:
  //  if (SD.exists(fileName)) {

  if (myFile) {
    //    Serial.print("Writing to test.txt...");

    String st = "";
    for (int i = 0; i < 6; i++) {
      //stdata[i] = String(data[i], 2);
      st += String(data[i], 2);
      st += ",";
    }
    for (int i = 6; i < 15; i++) {
      st += String(int(data[i]));
      st += ",";
    }
    st = time_message + ',' + st;
    myFile.println(st);
    myFile.close();

  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening: ");
    Serial.println(fileName);
  }

}

