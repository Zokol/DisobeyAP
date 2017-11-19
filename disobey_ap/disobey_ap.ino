/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <DNSServer.h>

#define MAX_TELNET_CLIENTS 5
#define CHATLOG_SIZE 10
const char *ssid = "Hki-open";
uint8_t mac[WL_MAC_ADDR_LENGTH];
String macID;

WiFiServer telnet_server(23);
WiFiClient telnet_clients[MAX_TELNET_CLIENTS];
ESP8266WebServer server(80);

const byte DNS_PORT = 53;

DNSServer dnsServer;

IPAddress ip(10,0,0,1);
IPAddress gateway(10,0,0,1);
IPAddress subnet(255,255,255,0);

int visitor_count = 0;

String page_Content = "";
//String chatlog[CHATLOG_SIZE];
//int chatlog_pointer = 0;
String chatlog = "";

void handleRoot() {
  visitor_count += 1;
	server.send(200, "text/html", page_Content);
  Serial.println();
  Serial.print("Page load count: ");
  Serial.print(visitor_count);
}


void changeSSID() {
  /*
  Serial.println("ssid-start\r\n");
  Serial.println(server.hostHeader());
  Serial.print("request arguments: ");
  int request_args = server.args();
  Serial.println(request_args);
  Serial.print("request argiment[0]: ");
  String request_argument0_value = server.arg(0);
  String request_argument0_name = server.argName(0);
  Serial.println(request_argument0_value + ": " + request_argument0_name);
  Serial.print("request headers: ");
  int request_headers = server.headers();
  Serial.println(request_headers);
  Serial.print("request header[0]: ");
  String request_header0_value = server.header(0);
  String request_header0_name = server.headerName(0);
  Serial.println(request_header0_name + ": " + request_header0_value);
  WiFiClient client = server.client();
  Serial.println("client initialized\r\n");
  if(client){
    Serial.println("client available\r\n");
    //String req = client.read();
    //Serial.print("client content: ");
    //Serial.println(req);
    client.flush();
  }
  Serial.println("returning\r\n");
  */
  String new_ssid = server.arg("ssid");
  if(new_ssid.length() > 0){
    char new_ssid_char[new_ssid.length()+1];
    new_ssid.toCharArray(new_ssid_char, new_ssid.length()+1);
    server.send(200, "text/html", "SSID will be changed in 10s");
    WiFi.disconnect();
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_OFF);
    delay(500);
    
    Serial.print("Configuring access point...\r\n");
  
    // Read mac-address to memory
    WiFi.softAPmacAddress(mac);
    macID = mac_to_string();
    macID.toUpperCase();
  
    // Setup WiFi Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(new_ssid_char);
  }
  else {
    server.send(200, "text/html", "ERROR -- Incorrect value for SSID");
  }
}


void rx_msg(byte msg){
  //chatlog = msg;
  Serial.print("Client said"); Serial.print(msg);
}

String mac_to_string(){
  String str = "";
  for(int i = 0; i < WL_MAC_ADDR_LENGTH; i++){
    str += String(mac[i], HEX);
  }
  return str;
}


void handle_telnet(){
  uint8_t i;
  
  if(telnet_server.hasClient()){
    for(i = 0; i < MAX_TELNET_CLIENTS; i++){
      //find free/disconnected spot
      if (!telnet_clients[i] || !telnet_clients[i].connected()){
        if(telnet_clients[i]) telnet_clients[i].stop();
        telnet_clients[i] = telnet_server.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    //no free/disconnected spot so reject
    WiFiClient telnet_client = telnet_server.available();
    telnet_client.stop();
    }
  }
  
  //check clients for data
  for(i = 0; i < MAX_TELNET_CLIENTS; i++){
    if (telnet_clients[i] && telnet_clients[i].connected()){
      if(telnet_clients[i].available()){
        //get data from the telnet client and push it to the UART
        //rx_msg(telnet_clients[i].readString());
        while(telnet_clients[i].available()) {
          Serial.write("Telnet client rx\r\n");
          Serial.write(telnet_clients[i].read());
        }
      }
    }
  }

  /*
  for(i = 0; i < MAX_TELNET_CLIENTS; i++){
    if (telnet_clients[i] && telnet_clients[i].connected()){
      telnet_clients[i].write();
      delay(1);
    }
  } 
  */
}

void setup() {
  
  page_Content += "<!DOCTYPE HTML>\r\n<html>\r\n";
  page_Content += "<head>\r\n";
  page_Content += "<title>Disobey</title>\r\n";
  page_Content += "<meta name='disobey' content='Disobey is a event that celebrates hacker culture with competitions, challenges, workshops and its own beer! You are welcome to join us to learn new skills, practice old ones or compete for bragging rights and fame. (there will be cool prizes)!'>\r\n";
  page_Content += "<meta name='date' content='Jan 12-13 2018'>\r\n";
  page_Content += "<meta name='location' content='Kattilahalli, Helsinki'>\r\n";
  page_Content += "<meta name='website' content='disobey.fi'>\r\n";
  page_Content += "<meta name='debug' content='{mac:" + macID + ", vc:" + String(visitor_count) + "}'>\r\n";
  page_Content += "</head>\r\n";
  page_Content += "<body style='background:black;'>\r\n";
  page_Content += "<div id='container' style='width:100%; margin:auto; color: green; font-family: courier; font-size: -webkit-xxx-large; text-align:center;'>\r\n";
  page_Content += "</br><p>You can't spell IDIoT without IoT</p>\r\n";
  page_Content += "<font size='-3'><pre><font color=white>101101110100100</font><font color=#f7f7f7>1</font><font color=#e6e6e6>1</font><font color=#d1d1d1>1</font><font color=#b0b0b0>1</font><font color=#939393>1</font><font color=#7a7a7a>0</font><font color=#656565>0</font><font color=#535353>1</font><font color=#444444>0</font><font color=#393939>1</font><font color=#313131>0</font><font color=#2c2c2c>1</font><font color=#2a2a2a>1</font><font color=#2c2c2c>0</font><font color=#313131>1</font><font color=#393939>1</font><font color=#444444>1</font><font color=#535353>0</font><font color=#656565>1</font><font color=#7a7a7a>0</font><font color=#939393>0</font><font color=#b0b0b0>1</font><font color=#d1d1d1>0</font><font color=#e6e6e6>0</font><font color=#f7f7f7>1</font><font color=white>111100101011011</font><br><font color=white>10100100111</font><font color=#fdfdfd>1</font><font color=#d9d9d9>1</font><font color=#aaaaaa>0</font><font color=#787878>0</font><font color=#535353>1</font><font color=#3a3a3a>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>01</font><font color=#333333>1</font><font color=#474747>0</font><font color=#575757>1</font><font color=#656565>1</font><font color=#6f6f6f>1</font><font color=#797979>0</font><font color=#7e7e7e>1</font><font color=gray>0</font><font color=#7e7e7e>0</font><font color=#797979>1</font><font color=#6f6f6f>0</font><font color=#656565>0</font><font color=#575757>1</font><font color=#474747>1</font><font color=#333333>1</font><font color=#2a2a2a>11</font><font color=#2b2b2b>0</font><font color=#3a3a3a>0</font><font color=#535353>1</font><font color=#787878>0</font><font color=#aaaaaa>1</font><font color=#d9d9d9>0</font><font color=#fdfdfd>1</font><font color=white>10111010010</font><br><font color=white>01111100</font><font color=#f9f9f9>1</font><font color=#d6d6d6>0</font><font color=#8d8d8d>1</font><font color=#4d4d4d>0</font><font color=#333333>1</font><font color=#2a2a2a>1</font><font color=#2f2f2f>0</font><font color=#3b3b3b>1</font><font color=#636363>1</font><font color=#909090>1</font><font color=#b7b7b7>0</font><font color=#d9d9d9>1</font><font color=#eeeeee>0</font><font color=#f4f4f4>0</font><font color=#f8f8f8>1</font><font color=#fcfcfc>0</font><font color=white>0111110</font><font color=#fcfcfc>0</font><font color=#f8f8f8>1</font><font color=#f4f4f4>0</font><font color=#eeeeee>1</font><font color=#d9d9d9>0</font><font color=#b7b7b7>1</font><font color=#909090>1</font><font color=#636363>0</font><font color=#3b3b3b>1</font><font color=#2f2f2f>1</font><font color=#2a2a2a>1</font><font color=#333333>0</font><font color=#4d4d4d>1</font><font color=#8d8d8d>0</font><font color=#d6d6d6>0</font><font color=#f9f9f9>1</font><font color=white>00111110</font><br><font color=white>010101</font><font color=#f1f1f1>1</font><font color=#acacac>0</font><font color=#595959>1</font><font color=#2b2b2b>1</font><font color=#2a2a2a>1</font><font color=#2e2e2e>0</font><font color=#545454>1</font><font color=#919191>0</font><font color=#cbcbcb>0</font><font color=#f6f6f6>1</font><font color=white>001</font><font color=#fbfbfb>1</font><font color=#e7e7e7>1</font><font color=#d6d6d6>1</font><font color=#c3c3c3>1</font><font color=#aeaeae>0</font><font color=#9e9e9e>0</font><font color=#949494>1</font><font color=#8d8d8d>0</font><font color=#8b8b8b>1</font><font color=#8e8e8e>0</font><font color=#969696>1</font><font color=#a2a2a2>1</font><font color=#b2b2b2>0</font><font color=#c8c8c8>1</font><font color=#dadada>1</font><font color=#ececec>1</font><font color=#fdfdfd>0</font><font color=white>100</font><font color=#f6f6f6>1</font><font color=#cbcbcb>0</font><font color=#919191>0</font><font color=#545454>1</font><font color=#2e2e2e>1</font><font color=#2a2a2a>1</font><font color=#2b2b2b>1</font><font color=#595959>1</font><font color=#acacac>0</font><font color=#f1f1f1>0</font><font color=white>101011</font><br><font color=white>0111</font><font color=#fcfcfc>0</font><font color=#ababab>1</font><font color=#4a4a4a>0</font><font color=#2b2b2b>0</font><font color=#2a2a2a>1</font><font color=#323232>0</font><font color=#7d7d7d>0</font><font color=#cfcfcf>1</font><font color=#f7f7f7>1</font><font color=white>111</font><font color=#dddddd>0</font><font color=#a7a7a7>0</font><font color=#767676>1</font><font color=#505050>0</font><font color=#3e3e3e>1</font><font color=#313131>0</font><font color=#2a2a2a>1101110100</font><font color=#2b2b2b>1</font><font color=#343434>0</font><font color=#424242>0</font><font color=#575757>1</font><font color=#818181>1</font><font color=#b4b4b4>1</font><font color=#ebebeb>1</font><font color=white>100</font><font color=#f7f7f7>1</font><font color=#cfcfcf>0</font><font color=#7d7d7d>1</font><font color=#323232>0</font><font color=#2a2a2a>1</font><font color=#2b2b2b>1</font><font color=#4a4a4a>0</font><font color=#ababab>1</font><font color=#fcfcfc>1</font><font color=white>1010</font><br><font color=white>010</font><font color=#d9d9d9>0</font><font color=#585858>1</font><font color=#2b2b2b>1</font><font color=#2a2a2a>1</font><font color=#333333>1</font><font color=gray>1</font><font color=#e8e8e8>0</font><font color=#fefefe>0</font><font color=white>1</font><font color=#fdfdfd>0</font><font color=#dfdfdf>1</font><font color=#959595>0</font><font color=#4c4c4c>1</font><font color=#2c2c2c>1</font><font color=#2a2a2a>01</font><font color=#373737>1</font><font color=#4c4c4c>1</font><font color=#707070>0</font><font color=#929292>1</font><font color=#acacac>0</font><font color=#c1c1c1>0</font><font color=#cecece>1</font><font color=#d6d6d6>0</font><font color=#d9d9d9>0</font><font color=#d5d5d5>1</font><font color=#cbcbcb>1</font><font color=#bcbcbc>1</font><font color=#a6a6a6>1</font><font color=#8a8a8a>1</font><font color=#676767>0</font><font color=#454545>0</font><font color=#333333>1</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#2e2e2e>0</font><font color=#5c5c5c>1</font><font color=#a8a8a8>1</font><font color=#eaeaea>0</font><font color=white>11</font><font color=#fefefe>1</font><font color=#e8e8e8>0</font><font color=gray>1</font><font color=#333333>0</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#585858>0</font><font color=#d9d9d9>0</font><font color=white>111</font><br><font color=white>11</font><font color=#bbbbbb>0</font><font color=#3a3a3a>0</font><font color=#2a2a2a>10</font><font color=#4c4c4c>1</font><font color=#cbcbcb>0</font><font color=white>110</font><font color=#dcdcdc>1</font><font color=#787878>1</font><font color=#353535>1</font><font color=#2a2a2a>01</font><font color=#353535>0</font><font color=#6e6e6e>0</font><font color=#b0b0b0>1</font><font color=#dedede>0</font><font color=#fbfbfb>0</font><font color=white>1111100101011</font><font color=#f6f6f6>0</font><font color=#d5d5d5>1</font><font color=#a1a1a1>1</font><font color=#5e5e5e>1</font><font color=#2e2e2e>0</font><font color=#2a2a2a>10</font><font color=#404040>0</font><font color=#909090>1</font><font color=#ededed>0</font><font color=white>011</font><font color=#cbcbcb>1</font><font color=#4c4c4c>1</font><font color=#2a2a2a>10</font><font color=#3a3a3a>0</font><font color=#bbbbbb>1</font><font color=white>01</font><br><font color=white>0</font><font color=#bfbfbf>1</font><font color=#323232>1</font><font color=#2a2a2a>01</font><font color=#5b5b5b>1</font><font color=#ececec>1</font><font color=white>01</font><font color=#fcfcfc>0</font><font color=#9f9f9f>0</font><font color=#373737>1</font><font color=#2a2a2a>00</font><font color=#343434>1</font><font color=#8c8c8c>1</font><font color=#e4e4e4>1</font><font color=#fcfcfc>1</font><font color=white>1001010110111010010</font><font color=#f9f9f9>0</font><font color=#d5d5d5>1</font><font color=#717171>1</font><font color=#2e2e2e>1</font><font color=#2a2a2a>11</font><font color=#454545>0</font><font color=#c2c2c2>0</font><font color=#fefefe>1</font><font color=white>01</font><font color=#ececec>0</font><font color=#5b5b5b>1</font><font color=#2a2a2a>10</font><font color=#323232>1</font><font color=#bfbfbf>1</font><font color=white>1</font><br><font color=#e7e7e7>0</font><font color=#393939>1</font><font color=#2a2a2a>00</font><font color=#494949>1</font><font color=#efefef>0</font><font color=white>01</font><font color=#fbfbfb>1</font><font color=#7e7e7e>1</font><font color=#2a2a2a>110</font><font color=#585858>0</font><font color=#dddddd>1</font><font color=white>0101101110100100111110010</font><font color=#c1c1c1>1</font><font color=#404040>0</font><font color=#2a2a2a>11</font><font color=#2e2e2e>0</font><font color=#a9a9a9>1</font><font color=white>110</font><font color=#efefef>1</font><font color=#494949>0</font><font color=#2a2a2a>01</font><font color=#393939>0</font><font color=#e7e7e7>0</font><br><font color=#868686>1</font><font color=#2a2a2a>111</font><font color=#bbbbbb>1</font><font color=white>001</font><font color=#a3a3a3>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>01</font><font color=#525252>1</font><font color=#efefef>0</font><font color=white>111010010011111001010110111</font><font color=#d7d7d7>0</font><font color=#373737>1</font><font color=#2a2a2a>00</font><font color=#313131>1</font><font color=#d1d1d1>0</font><font color=white>011</font><font color=#bbbbbb>1</font><font color=#2a2a2a>110</font><font color=#868686>0</font><br><font color=#424242>1</font><font color=#2a2a2a>01</font><font color=#393939>0</font><font color=#fcfcfc>1</font><font color=white>10</font><font color=#fbfbfb>1</font><font color=#3c3c3c>1</font><font color=#2a2a2a>101</font><font color=#cdcdcd>0</font><font color=white>01001111100101011011101001001</font><font color=#999999>1</font><font color=#2a2a2a>111</font><font color=#6c6c6c>0</font><font color=white>010</font><font color=#fcfcfc>1</font><font color=#393939>0</font><font color=#2a2a2a>11</font><font color=#424242>0</font><br><font color=#2c2c2c>1</font><font color=#2a2a2a>11</font><font color=#4f4f4f>0</font><font color=white>100</font><font color=#ebebeb>1</font><font color=#2c2c2c>0</font><font color=#2a2a2a>01</font><font color=#353535>1</font><font color=#fdfdfd>1</font><font color=white>11001010110111010010011111001</font><font color=#d3d3d3>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>01</font><font color=#4b4b4b>1</font><font color=white>0111</font><font color=#4f4f4f>0</font><font color=#2a2a2a>10</font><font color=#2c2c2c>0</font><br><font color=#424242>1</font><font color=#2a2a2a>00</font><font color=#393939>1</font><font color=#fcfcfc>1</font><font color=white>11</font><font color=#fefefe>1</font><font color=#c7c7c7>0</font><font color=#979797>0</font><font color=#9b9b9b>1</font><font color=#d4d4d4>0</font><font color=white>101101110100100111110010101101</font><font color=#fcfcfc>1</font><font color=#c1c1c1>1</font><font color=#959595>0</font><font color=#9d9d9d>1</font><font color=#dadada>0</font><font color=white>010</font><font color=#fcfcfc>0</font><font color=#393939>1</font><font color=#2a2a2a>11</font><font color=#424242>1</font><br><font color=#868686>1</font><font color=#2a2a2a>001</font><font color=#bbbbbb>0</font><font color=white>101101110100100111110010101101110100100111110</font><font color=#bbbbbb>0</font><font color=#2a2a2a>101</font><font color=#868686>0</font><br><font color=#e7e7e7>1</font><font color=#393939>1</font><font color=#2a2a2a>01</font><font color=#494949>1</font><font color=#efefef>1</font><font color=white>0100100111110010101101110100100111110010101</font><font color=#efefef>1</font><font color=#494949>0</font><font color=#2a2a2a>11</font><font color=#393939>1</font><font color=#e7e7e7>0</font><br><font color=white>1</font><font color=#bfbfbf>0</font><font color=#323232>0</font><font color=#2a2a2a>10</font><font color=#5b5b5b>0</font><font color=#ececec>1</font><font color=white>1111001010</font><font color=#fafafa>1</font><font color=#cecece>1</font><font color=#a8a8a8>0</font><font color=#ababab>1</font><font color=#d6d6d6>1</font><font color=#fcfcfc>1</font><font color=white>0100</font><font color=#f5f5f5>1</font><font color=#d8d8d8>0</font><font color=#cecece>01111100</font><font color=#cfcfcf>1</font><font color=#e6e6e6>0</font><font color=#fefefe>1</font><font color=white>01101110</font><font color=#ececec>1</font><font color=#5b5b5b>0</font><font color=#2a2a2a>01</font><font color=#323232>0</font><font color=#bfbfbf>0</font><font color=white>1</font><br><font color=white>11</font><font color=#bbbbbb>1</font><font color=#3a3a3a>1</font><font color=#2a2a2a>00</font><font color=#4c4c4c>1</font><font color=#cbcbcb>0</font><font color=white>101101110</font><font color=#acacac>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>01</font><font color=#2d2d2d>0</font><font color=#c7c7c7>0</font><font color=white>1111</font><font color=#6e6e6e>1</font><font color=#2a2a2a>0010101101</font><font color=#333333>1</font><font color=#cdcdcd>1</font><font color=white>0100100</font><font color=#cbcbcb>1</font><font color=#4c4c4c>1</font><font color=#2a2a2a>11</font><font color=#3a3a3a>1</font><font color=#bbbbbb>0</font><font color=white>01</font><br><font color=white>010</font><font color=#d9d9d9>1</font><font color=#585858>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>1</font><font color=#333333>1</font><font color=gray>1</font><font color=#e8e8e8>0</font><font color=#fefefe>1</font><font color=white>001001</font><font color=#f1f1f1>1</font><font color=#a2a2a2>1</font><font color=#7b7b7b>1</font><font color=#7e7e7e>1</font><font color=#acacac>0</font><font color=#f7f7f7>0</font><font color=white>1010</font><font color=#dedede>1</font><font color=#a2a2a2>1</font><font color=#989898>01110100</font><font color=#9a9a9a>1</font><font color=#b9b9b9>0</font><font color=#f8f8f8>0</font><font color=white>1111</font><font color=#fefefe>1</font><font color=#e8e8e8>0</font><font color=gray>0</font><font color=#333333>1</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#585858>0</font><font color=#d9d9d9>1</font><font color=white>101</font><br><font color=white>1101</font><font color=#fcfcfc>0</font><font color=#ababab>0</font><font color=#4a4a4a>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>0</font><font color=#323232>1</font><font color=#7d7d7d>1</font><font color=#cfcfcf>1</font><font color=#f7f7f7>1</font><font color=white>10010101101110100100111110010</font><font color=#f7f7f7>1</font><font color=#cfcfcf>0</font><font color=#7d7d7d>1</font><font color=#323232>1</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#4a4a4a>1</font><font color=#ababab>1</font><font color=#fcfcfc>0</font><font color=white>1001</font><br><font color=white>001111</font><font color=#f1f1f1>1</font><font color=#acacac>0</font><font color=#595959>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>0</font><font color=#2e2e2e>1</font><font color=#545454>0</font><font color=#919191>1</font><font color=#cbcbcb>1</font><font color=#f6f6f6>0</font><font color=white>11101001001111100101011</font><font color=#f6f6f6>0</font><font color=#cbcbcb>1</font><font color=#919191>1</font><font color=#545454>1</font><font color=#2e2e2e>0</font><font color=#2a2a2a>1</font><font color=#2b2b2b>0</font><font color=#595959>0</font><font color=#acacac>1</font><font color=#f1f1f1>0</font><font color=white>011111</font><br><font color=white>00101011</font><font color=#f9f9f9>0</font><font color=#d6d6d6>1</font><font color=#8d8d8d>1</font><font color=#4d4d4d>1</font><font color=#333333>0</font><font color=#2a2a2a>1</font><font color=#2f2f2f>0</font><font color=#3b3b3b>0</font><font color=#636363>1</font><font color=#909090>0</font><font color=#b7b7b7>0</font><font color=#d9d9d9>1</font><font color=#eeeeee>1</font><font color=#f4f4f4>1</font><font color=#f8f8f8>1</font><font color=#fcfcfc>1</font><font color=white>0010101</font><font color=#fcfcfc>1</font><font color=#f8f8f8>0</font><font color=#f4f4f4>1</font><font color=#eeeeee>1</font><font color=#d9d9d9>1</font><font color=#b7b7b7>0</font><font color=#909090>1</font><font color=#636363>0</font><font color=#3b3b3b>0</font><font color=#2f2f2f>1</font><font color=#2a2a2a>0</font><font color=#333333>0</font><font color=#4d4d4d>1</font><font color=#8d8d8d>1</font><font color=#d6d6d6>1</font><font color=#f9f9f9>1</font><font color=white>10010101</font><br><font color=white>10111010010</font><font color=#fdfdfd>0</font><font color=#d9d9d9>1</font><font color=#aaaaaa>1</font><font color=#787878>1</font><font color=#535353>1</font><font color=#3a3a3a>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>01</font><font color=#333333>0</font><font color=#474747>1</font><font color=#575757>0</font><font color=#656565>1</font><font color=#6f6f6f>1</font><font color=#797979>0</font><font color=#7e7e7e>1</font><font color=gray>1</font><font color=#7e7e7e>1</font><font color=#797979>0</font><font color=#6f6f6f>1</font><font color=#656565>0</font><font color=#575757>0</font><font color=#474747>1</font><font color=#333333>0</font><font color=#2a2a2a>01</font><font color=#2b2b2b>1</font><font color=#3a3a3a>1</font><font color=#535353>1</font><font color=#787878>1</font><font color=#aaaaaa>0</font><font color=#d9d9d9>0</font><font color=#fdfdfd>1</font><font color=white>01011011101</font><br><font color=white>001001111100101</font><font color=#f7f7f7>0</font><font color=#e6e6e6>1</font><font color=#d1d1d1>1</font><font color=#b0b0b0>0</font><font color=#939393>1</font><font color=#7a7a7a>1</font><font color=#656565>1</font><font color=#535353>0</font><font color=#444444>1</font><font color=#393939>0</font><font color=#313131>0</font><font color=#2c2c2c>1</font><font color=#2a2a2a>0</font><font color=#2c2c2c>0</font><font color=#313131>1</font><font color=#393939>1</font><font color=#444444>1</font><font color=#535353>1</font><font color=#656565>1</font><font color=#7a7a7a>0</font><font color=#939393>0</font><font color=#b0b0b0>1</font><font color=#d1d1d1>0</font><font color=#e6e6e6>1</font><font color=#f7f7f7>0</font><font color=white>110111010010011</font><br></pre></font>";
  page_Content += "</div>\r\n";
  page_Content += "</body>\r\n";
  page_Content += "</html>\n";
  
	delay(1000);
	Serial.begin(74880);
	Serial.println();
	
	Serial.print("Cleaning up...\r\n");

  WiFi.disconnect();
  WiFi.softAPdisconnect();
  WiFi.mode(WIFI_OFF);
  delay(500);
  
  Serial.print("Configuring access point...\r\n");

  // Read mac-address to memory
  WiFi.softAPmacAddress(mac);
  macID = mac_to_string();
  macID.toUpperCase();

  // Setup WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
	WiFi.softAP(ssid);

  dnsServer.setTTL(300);
  dnsServer.start(DNS_PORT, "*", ip);
 
	server.onNotFound([](){
	  handleRoot();
	});
	server.on("/", handleRoot);
  server.on("/ssid", changeSSID);
	server.begin();
	Serial.println("HTTP server started\r\n");

  telnet_server.begin();
  telnet_server.setNoDelay(true);
}

void loop() {
  dnsServer.processNextRequest();
	server.handleClient();
}
