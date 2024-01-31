String networkName = "network name";                   //Ağımızın adını buraya yazıyoruz.    
String networkPw = "network password";             //Ağımızın şifresini buraya yazıyoruz.

int motorPin1 = 3, motorPin2 = 4, motorPin3 = 5, motorPin4 = 6; //Step motor pinlerini tanımlıyoruz.

void setup(){ 
  pinMode(motorPin1, OUTPUT);                                   //Motor pinleri OUTPUT olarak ayarlanır.
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  Serial.begin(115200);                                         //Seri haberleşmeyi başlatıyoruz.
  Serial.println("AT");                                         //AT komutu ile modül kontrolünü yapıyoruz.
  
  while(!Serial.find("OK")){                                    //Modül hazır olana kadar bekliyoruz.
    Serial.println("AT");
  }

  delay(1000);  
  Serial.println("AT+RST");   
  
  delay(1000);
  while(!Serial.find("ready"))                                  //Resetleme işleminin bitmesini bekliyoruz.
  
  delay(1000);
  Serial.println("AT+CWMODE=1");                                //Modülümüzü client olarak ayarlıyoruz.
  
  while(!Serial.find("OK"));                                    //Ayarlamanın tamamlanmasını bekliyoruz.
  Serial.println("AT+CWJAP=\""+networkName+"\",\""+networkPw+"\"");   //Wifi'a bağlanıyoruz.
  
  while(!Serial.find("OK"));                                    //Bağlanana kadar bekliyoruz.
  Serial.print("AT+CIFSR\r\n");                                 //IP adresini ve MAC adresini okuyoruz.
  Serial.print(espRead(1000));                                 //IP ve MAC adresini ekrana yazıyoruz.
  
  serialClean(2000);                                          //Seri haberleşmede kullanmadığımız verforward temizliyoruz.
  Serial.print("AT+CIPMUX=1\r\n");                              //Çoklu bağlantı ayarını yapıyoruz.
  
  serialClean(2000);
  Serial.print("AT+CIPSERVER=1,80\r\n");                        //Server oluşturuyoruz ve 80. porttan dinlemeye başlıyoruz.
  
  serialClean(2000);
}

void loop(){
  if(Serial.available()){                                       //Seri haberleşme verisi geldiğinde içindeki komutlar çalışır.
    if(Serial.find("+IPD,")){                                   //Bağlantı isteği geldiğinde içindeki komutlar çalışır.
      delay(200);
      int connectionId = Serial.read() - 48;                    //Bağlantı numarasını okuyoruz.
      String command = espRead(1000);                            //Bağlantı komutlarını alıyoruz.    
      if(command.indexOf("step=forward") != -1){                    //İleri komutu geldiğinde içerisindeki komutlar çalışır.
        for(int adim = 0; adim < 5; adim++){                    //Step motora 5 kere forward hareketi yaptırılır.
          stepForward(50); 
        }
      }
      else if(command.indexOf("step=back") != -1){                //back komutu geldiğinde içerisindeki komutlar çalışır.
        for(int adim = 0; adim < 5; adim++){                    //Step motora 5 kere back hareketi yaptırılır.
          stepback(50); 
        }
      }
      String webPage = "<h1>Step Motor Kontrol</h1><br>";                           
      webPage+="<br><a href=\"?step=forward\"><button><h1>forward</h1></button></a>";
      webPage+="<br><br><a href=\"?step=back\"><button><h1>back</h1></button></a>";
      
      command = "AT+CIPSEND=";                                    //Sayfa verisinin uzunluğu komut değişkenine eşitlenir.

      command += connectionId;
      command += ",";
      command +=webPage.length();
      command +="\r\n";

      Serial.print(command);                                      //Komut gönderilir.
      delay(1000);

      Serial.print(webPage);                                      //Sayfa verisi gönderilir.
      delay(1000);

      command = "AT+CIPCLOSE=";                                   //Bağlantı sonlandırma komutu ayarlanır.    
      command+=connectionId;
      command+="\r\n";

      Serial.print(command);                                      //Bağlantı sonlandırma komutu gönderilir.
    }
  }
}

String espRead(long int timeout){                           //ESP'den gelen dönütleri okur.
  long int beginning = millis();  
  String receive;
  while(millis() - beginning < timeout){                   
    if(Serial.available()>0){
        char c = Serial.read(); 
        receive += c;
    } 
  }
  receive.replace("AT+","");
  return receive;
}

void serialClean(long int timeout){                         //Seri haberleşmede kullanmadığımız byteları temizler.
  long int beginning = millis();
  while(millis() - beginning < timeout){
    if(Serial.available()>0){
        Serial.read(); 
    } 
  }
}

void stepForward(int sleep){                               //Step motora forward hareketi yaptırır.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(sleep);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(sleep);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(sleep);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(sleep);
}
void stepback(int sleep){                                //Step motora forward hareketi yaptırır.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(sleep);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW); 
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(sleep);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(sleep);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(sleep);
}
