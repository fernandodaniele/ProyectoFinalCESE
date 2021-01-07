void setup (){
 Serial1.begin(115200);
Serial.begin(115200);  
}

void loop (){
 
 if(Serial1.available()>0){
   Serial.print ( "Dato : ");
   Serial1.print ( "Dato : ");
   Serial.println(Serial1.read());
 } 
  
}
