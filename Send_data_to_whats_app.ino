#include <WiFi.h>         //Including wifi.h library it will take care of all wifi related task
#include <HTTPClient.h>   //Including HTTPClient.h library to use all api

const char* ssid = "Infinix HOT 30";             //Add your WiFi ssid
const char* password =  "shansaya";    //Add your WiFi password

String apiKey = "5046106";              //Add your Token number that bot has sent you on WhatsApp messenger
String phone_number = "8801878904575"; //Add your WhatsApp app registered phone number (same number that bot send you in url)

String url;                            //url String will be used to store the final generated URL
String message = "";
String message1 = "";

#include <TinyGPSPlus.h>

TinyGPSPlus gps;


void setup() 
{
  pinMode(A0,INPUT);
  
  Serial.begin(115200);
  Serial2.begin(9600);
  WiFi.begin(ssid, password);              // Try to connect with the given SSID and PSS
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to the WiFi network"); // Print wifi connect message
  delay(3000);
  // use message_to_whatsapp function to send your own message
  message_to_whatsapp("We are successfully connected");  // you send your own message just change "hello from TechTOnions" to your message.
}
void loop()
{
  //loop will do nothing for this example
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
  int val = digitalRead(A0);
  if(val==1){
    Serial.println(val);
    //message_to_whatsapp();
    message_to_whatsapp(message1+message);
  }
  
}

void  message_to_whatsapp(String message)       // user define function to send meassage to WhatsApp app
{
  //adding all number, your api key, your message into one complete url
  url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);

  postData(); // calling postData to run the above-generated url once so that you will receive a message.
}

void postData()     //userDefine function used to call api(POST data)
{
  int httpCode;     // variable used to get the responce http code after calling api
  HTTPClient http;  // Declare object of class HTTPClient
  http.begin(url);  // begin the HTTPClient object with generated url
  httpCode = http.POST(url); // Finaly Post the URL with this function and it will store the http code
  if (httpCode == 200)      // Check if the responce http code is 200
  {
    Serial.println("Sent ok."); // print message sent ok message
  }
  else                      // if response HTTP code is not 200 it means there is some error.
  {
    Serial.println("Error."); // print error message.
  }
  http.end();          // After calling API end the HTTP client object.
}

String urlencode(String str)  // Function used for encoding the url
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}

void displayInfo()

{
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    Serial.print("Lat: ");
    double latitude = gps.location.lat();
    String latitudeStr = String(latitude, 2);
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    double longitude = gps.location.lng();
    String longitudeStr = String(longitude, 2);
    //String long = longitude;
    Serial.print(gps.location.lng(), 6);
    Serial.println();
    message1 = "It's an Emergency Situation\n";
    message = "Lng: "+ longitudeStr + " Lat: "+latitudeStr;
    Serial.println(message);
  } 
  else
  {
    Serial.print(F("INVALID"));
  }
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
