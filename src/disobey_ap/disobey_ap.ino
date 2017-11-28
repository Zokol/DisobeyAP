/*
  Disobey WiFi Access Point
  Author: Heikki Juva
*/

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const char *ssid = "Hki-open";
uint8_t mac[WL_MAC_ADDR_LENGTH];
String macID;

ESP8266WebServer server(80);

const byte DNS_PORT = 53;

DNSServer dnsServer;

IPAddress ip(10,0,0,1);
IPAddress gateway(10,0,0,1);
IPAddress subnet(255,255,255,0);

int visitor_count = 0;

void handleRoot() {
  
  String html = "";
  visitor_count += 1;
  
  html += "<!DOCTYPE HTML>\r\n<html>\r\n";
  html += "<head>\r\n";
  html += "<title>Disobey</title>\r\n";
  html += "<meta name='disobey' content='Disobey is a event that celebrates hacker culture with competitions, challenges, workshops and its own beer! You are welcome to join us to learn new skills, practice old ones or compete for bragging rights and fame. (there will be cool prizes)!'>\r\n";
  html += "<meta name='date' content='Jan 12-13 2018'>\r\n";
  html += "<meta name='location' content='Kattilahalli, Helsinki'>\r\n";
  html += "<meta name='website' content='disobey.fi'>\r\n";
  
  html += "<meta name='debug' content='{mac:";
  html += macID;
  html += ", vc:";
  html += String(visitor_count);
  html += "}'>\r\n";

  html += "</head>\r\n";
  html += "<body style='background:black;'>\r\n";
  html += "<div id='container' style='width:100%; margin:auto; color: green; font-family: courier; font-size: -webkit-xxx-large; text-align:center;'>\r\n";
  html += "</br><p>You can't spell IDIoT without IoT</p>\r\n";
  html += "<font size='-3'><pre><font color=white>101101110100100</font><font color=#f7f7f7>1</font><font color=#e6e6e6>1</font><font color=#d1d1d1>1</font><font color=#b0b0b0>1</font><font color=#939393>1</font><font color=#7a7a7a>0</font><font color=#656565>0</font><font color=#535353>1</font><font color=#444444>0</font><font color=#393939>1</font><font color=#313131>0</font><font color=#2c2c2c>1</font><font color=#2a2a2a>1</font><font color=#2c2c2c>0</font><font color=#313131>1</font><font color=#393939>1</font><font color=#444444>1</font><font color=#535353>0</font><font color=#656565>1</font><font color=#7a7a7a>0</font><font color=#939393>0</font><font color=#b0b0b0>1</font><font color=#d1d1d1>0</font><font color=#e6e6e6>0</font><font color=#f7f7f7>1</font><font color=white>111100101011011</font><br><font color=white>10100100111</font><font color=#fdfdfd>1</font><font color=#d9d9d9>1</font><font color=#aaaaaa>0</font><font color=#787878>0</font><font color=#535353>1</font><font color=#3a3a3a>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>01</font><font color=#333333>1</font><font color=#474747>0</font><font color=#575757>1</font><font color=#656565>1</font><font color=#6f6f6f>1</font><font color=#797979>0</font><font color=#7e7e7e>1</font><font color=gray>0</font><font color=#7e7e7e>0</font><font color=#797979>1</font><font color=#6f6f6f>0</font><font color=#656565>0</font><font color=#575757>1</font><font color=#474747>1</font><font color=#333333>1</font><font color=#2a2a2a>11</font><font color=#2b2b2b>0</font><font color=#3a3a3a>0</font><font color=#535353>1</font><font color=#787878>0</font><font color=#aaaaaa>1</font><font color=#d9d9d9>0</font><font color=#fdfdfd>1</font><font color=white>10111010010</font><br><font color=white>01111100</font><font color=#f9f9f9>1</font><font color=#d6d6d6>0</font><font color=#8d8d8d>1</font><font color=#4d4d4d>0</font><font color=#333333>1</font><font color=#2a2a2a>1</font><font color=#2f2f2f>0</font><font color=#3b3b3b>1</font><font color=#636363>1</font><font color=#909090>1</font><font color=#b7b7b7>0</font><font color=#d9d9d9>1</font><font color=#eeeeee>0</font><font color=#f4f4f4>0</font><font color=#f8f8f8>1</font><font color=#fcfcfc>0</font><font color=white>0111110</font><font color=#fcfcfc>0</font><font color=#f8f8f8>1</font><font color=#f4f4f4>0</font><font color=#eeeeee>1</font><font color=#d9d9d9>0</font><font color=#b7b7b7>1</font><font color=#909090>1</font><font color=#636363>0</font><font color=#3b3b3b>1</font><font color=#2f2f2f>1</font><font color=#2a2a2a>1</font><font color=#333333>0</font><font color=#4d4d4d>1</font><font color=#8d8d8d>0</font><font color=#d6d6d6>0</font><font color=#f9f9f9>1</font><font color=white>00111110</font><br><font color=white>010101</font><font color=#f1f1f1>1</font><font color=#acacac>0</font><font color=#595959>1</font><font color=#2b2b2b>1</font><font color=#2a2a2a>1</font><font color=#2e2e2e>0</font><font color=#545454>1</font><font color=#919191>0</font><font color=#cbcbcb>0</font><font color=#f6f6f6>1</font><font color=white>001</font><font color=#fbfbfb>1</font><font color=#e7e7e7>1</font><font color=#d6d6d6>1</font><font color=#c3c3c3>1</font><font color=#aeaeae>0</font><font color=#9e9e9e>0</font><font color=#949494>1</font><font color=#8d8d8d>0</font><font color=#8b8b8b>1</font><font color=#8e8e8e>0</font><font color=#969696>1</font><font color=#a2a2a2>1</font><font color=#b2b2b2>0</font><font color=#c8c8c8>1</font><font color=#dadada>1</font><font color=#ececec>1</font><font color=#fdfdfd>0</font><font color=white>100</font><font color=#f6f6f6>1</font><font color=#cbcbcb>0</font><font color=#919191>0</font><font color=#545454>1</font><font color=#2e2e2e>1</font><font color=#2a2a2a>1</font><font color=#2b2b2b>1</font><font color=#595959>1</font><font color=#acacac>0</font><font color=#f1f1f1>0</font><font color=white>101011</font><br><font color=white>0111</font><font color=#fcfcfc>0</font><font color=#ababab>1</font><font color=#4a4a4a>0</font><font color=#2b2b2b>0</font><font color=#2a2a2a>1</font><font color=#323232>0</font><font color=#7d7d7d>0</font><font color=#cfcfcf>1</font><font color=#f7f7f7>1</font><font color=white>111</font><font color=#dddddd>0</font><font color=#a7a7a7>0</font><font color=#767676>1</font><font color=#505050>0</font><font color=#3e3e3e>1</font><font color=#313131>0</font><font color=#2a2a2a>1101110100</font><font color=#2b2b2b>1</font><font color=#343434>0</font><font color=#424242>0</font><font color=#575757>1</font><font color=#818181>1</font><font color=#b4b4b4>1</font><font color=#ebebeb>1</font><font color=white>100</font><font color=#f7f7f7>1</font><font color=#cfcfcf>0</font><font color=#7d7d7d>1</font><font color=#323232>0</font><font color=#2a2a2a>1</font><font color=#2b2b2b>1</font><font color=#4a4a4a>0</font><font color=#ababab>1</font><font color=#fcfcfc>1</font><font color=white>1010</font><br><font color=white>010</font><font color=#d9d9d9>0</font><font color=#585858>1</font><font color=#2b2b2b>1</font><font color=#2a2a2a>1</font><font color=#333333>1</font><font color=gray>1</font><font color=#e8e8e8>0</font><font color=#fefefe>0</font><font color=white>1</font><font color=#fdfdfd>0</font><font color=#dfdfdf>1</font><font color=#959595>0</font><font color=#4c4c4c>1</font><font color=#2c2c2c>1</font><font color=#2a2a2a>01</font><font color=#373737>1</font><font color=#4c4c4c>1</font><font color=#707070>0</font><font color=#929292>1</font><font color=#acacac>0</font><font color=#c1c1c1>0</font><font color=#cecece>1</font><font color=#d6d6d6>0</font><font color=#d9d9d9>0</font><font color=#d5d5d5>1</font><font color=#cbcbcb>1</font><font color=#bcbcbc>1</font><font color=#a6a6a6>1</font><font color=#8a8a8a>1</font><font color=#676767>0</font><font color=#454545>0</font><font color=#333333>1</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#2e2e2e>0</font><font color=#5c5c5c>1</font><font color=#a8a8a8>1</font><font color=#eaeaea>0</font><font color=white>11</font><font color=#fefefe>1</font><font color=#e8e8e8>0</font><font color=gray>1</font><font color=#333333>0</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#585858>0</font><font color=#d9d9d9>0</font><font color=white>111</font><br><font color=white>11</font><font color=#bbbbbb>0</font><font color=#3a3a3a>0</font><font color=#2a2a2a>10</font><font color=#4c4c4c>1</font><font color=#cbcbcb>0</font><font color=white>110</font><font color=#dcdcdc>1</font><font color=#787878>1</font><font color=#353535>1</font><font color=#2a2a2a>01</font><font color=#353535>0</font><font color=#6e6e6e>0</font><font color=#b0b0b0>1</font><font color=#dedede>0</font><font color=#fbfbfb>0</font><font color=white>1111100101011</font><font color=#f6f6f6>0</font><font color=#d5d5d5>1</font><font color=#a1a1a1>1</font><font color=#5e5e5e>1</font><font color=#2e2e2e>0</font><font color=#2a2a2a>10</font><font color=#404040>0</font><font color=#909090>1</font><font color=#ededed>0</font><font color=white>011</font><font color=#cbcbcb>1</font><font color=#4c4c4c>1</font><font color=#2a2a2a>10</font><font color=#3a3a3a>0</font><font color=#bbbbbb>1</font><font color=white>01</font><br><font color=white>0</font><font color=#bfbfbf>1</font><font color=#323232>1</font><font color=#2a2a2a>01</font><font color=#5b5b5b>1</font><font color=#ececec>1</font><font color=white>01</font><font color=#fcfcfc>0</font><font color=#9f9f9f>0</font><font color=#373737>1</font><font color=#2a2a2a>00</font><font color=#343434>1</font><font color=#8c8c8c>1</font><font color=#e4e4e4>1</font><font color=#fcfcfc>1</font><font color=white>1001010110111010010</font><font color=#f9f9f9>0</font><font color=#d5d5d5>1</font><font color=#717171>1</font><font color=#2e2e2e>1</font><font color=#2a2a2a>11</font><font color=#454545>0</font><font color=#c2c2c2>0</font><font color=#fefefe>1</font><font color=white>01</font><font color=#ececec>0</font><font color=#5b5b5b>1</font><font color=#2a2a2a>10</font><font color=#323232>1</font><font color=#bfbfbf>1</font><font color=white>1</font><br><font color=#e7e7e7>0</font><font color=#393939>1</font><font color=#2a2a2a>00</font><font color=#494949>1</font><font color=#efefef>0</font><font color=white>01</font><font color=#fbfbfb>1</font><font color=#7e7e7e>1</font><font color=#2a2a2a>110</font><font color=#585858>0</font><font color=#dddddd>1</font><font color=white>0101101110100100111110010</font><font color=#c1c1c1>1</font><font color=#404040>0</font><font color=#2a2a2a>11</font><font color=#2e2e2e>0</font><font color=#a9a9a9>1</font><font color=white>110</font><font color=#efefef>1</font><font color=#494949>0</font><font color=#2a2a2a>01</font><font color=#393939>0</font><font color=#e7e7e7>0</font><br><font color=#868686>1</font><font color=#2a2a2a>111</font><font color=#bbbbbb>1</font><font color=white>001</font><font color=#a3a3a3>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>01</font><font color=#525252>1</font><font color=#efefef>0</font><font color=white>111010010011111001010110111</font><font color=#d7d7d7>0</font><font color=#373737>1</font><font color=#2a2a2a>00</font><font color=#313131>1</font><font color=#d1d1d1>0</font><font color=white>011</font><font color=#bbbbbb>1</font><font color=#2a2a2a>110</font><font color=#868686>0</font><br><font color=#424242>1</font><font color=#2a2a2a>01</font><font color=#393939>0</font><font color=#fcfcfc>1</font><font color=white>10</font><font color=#fbfbfb>1</font><font color=#3c3c3c>1</font><font color=#2a2a2a>101</font><font color=#cdcdcd>0</font><font color=white>01001111100101011011101001001</font><font color=#999999>1</font><font color=#2a2a2a>111</font><font color=#6c6c6c>0</font><font color=white>010</font><font color=#fcfcfc>1</font><font color=#393939>0</font><font color=#2a2a2a>11</font><font color=#424242>0</font><br><font color=#2c2c2c>1</font><font color=#2a2a2a>11</font><font color=#4f4f4f>0</font><font color=white>100</font><font color=#ebebeb>1</font><font color=#2c2c2c>0</font><font color=#2a2a2a>01</font><font color=#353535>1</font><font color=#fdfdfd>1</font><font color=white>11001010110111010010011111001</font><font color=#d3d3d3>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>01</font><font color=#4b4b4b>1</font><font color=white>0111</font><font color=#4f4f4f>0</font><font color=#2a2a2a>10</font><font color=#2c2c2c>0</font><br><font color=#424242>1</font><font color=#2a2a2a>00</font><font color=#393939>1</font><font color=#fcfcfc>1</font><font color=white>11</font><font color=#fefefe>1</font><font color=#c7c7c7>0</font><font color=#979797>0</font><font color=#9b9b9b>1</font><font color=#d4d4d4>0</font><font color=white>101101110100100111110010101101</font><font color=#fcfcfc>1</font><font color=#c1c1c1>1</font><font color=#959595>0</font><font color=#9d9d9d>1</font><font color=#dadada>0</font><font color=white>010</font><font color=#fcfcfc>0</font><font color=#393939>1</font><font color=#2a2a2a>11</font><font color=#424242>1</font><br><font color=#868686>1</font><font color=#2a2a2a>001</font><font color=#bbbbbb>0</font><font color=white>101101110100100111110010101101110100100111110</font><font color=#bbbbbb>0</font><font color=#2a2a2a>101</font><font color=#868686>0</font><br><font color=#e7e7e7>1</font><font color=#393939>1</font><font color=#2a2a2a>01</font><font color=#494949>1</font><font color=#efefef>1</font><font color=white>0100100111110010101101110100100111110010101</font><font color=#efefef>1</font><font color=#494949>0</font><font color=#2a2a2a>11</font><font color=#393939>1</font><font color=#e7e7e7>0</font><br><font color=white>1</font><font color=#bfbfbf>0</font><font color=#323232>0</font><font color=#2a2a2a>10</font><font color=#5b5b5b>0</font><font color=#ececec>1</font><font color=white>1111001010</font><font color=#fafafa>1</font><font color=#cecece>1</font><font color=#a8a8a8>0</font><font color=#ababab>1</font><font color=#d6d6d6>1</font><font color=#fcfcfc>1</font><font color=white>0100</font><font color=#f5f5f5>1</font><font color=#d8d8d8>0</font><font color=#cecece>01111100</font><font color=#cfcfcf>1</font><font color=#e6e6e6>0</font><font color=#fefefe>1</font><font color=white>01101110</font><font color=#ececec>1</font><font color=#5b5b5b>0</font><font color=#2a2a2a>01</font><font color=#323232>0</font><font color=#bfbfbf>0</font><font color=white>1</font><br><font color=white>11</font><font color=#bbbbbb>1</font><font color=#3a3a3a>1</font><font color=#2a2a2a>00</font><font color=#4c4c4c>1</font><font color=#cbcbcb>0</font><font color=white>101101110</font><font color=#acacac>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>01</font><font color=#2d2d2d>0</font><font color=#c7c7c7>0</font><font color=white>1111</font><font color=#6e6e6e>1</font><font color=#2a2a2a>0010101101</font><font color=#333333>1</font><font color=#cdcdcd>1</font><font color=white>0100100</font><font color=#cbcbcb>1</font><font color=#4c4c4c>1</font><font color=#2a2a2a>11</font><font color=#3a3a3a>1</font><font color=#bbbbbb>0</font><font color=white>01</font><br><font color=white>010</font><font color=#d9d9d9>1</font><font color=#585858>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>1</font><font color=#333333>1</font><font color=gray>1</font><font color=#e8e8e8>0</font><font color=#fefefe>1</font><font color=white>001001</font><font color=#f1f1f1>1</font><font color=#a2a2a2>1</font><font color=#7b7b7b>1</font><font color=#7e7e7e>1</font><font color=#acacac>0</font><font color=#f7f7f7>0</font><font color=white>1010</font><font color=#dedede>1</font><font color=#a2a2a2>1</font><font color=#989898>01110100</font><font color=#9a9a9a>1</font><font color=#b9b9b9>0</font><font color=#f8f8f8>0</font><font color=white>1111</font><font color=#fefefe>1</font><font color=#e8e8e8>0</font><font color=gray>0</font><font color=#333333>1</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#585858>0</font><font color=#d9d9d9>1</font><font color=white>101</font><br><font color=white>1101</font><font color=#fcfcfc>0</font><font color=#ababab>0</font><font color=#4a4a4a>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>0</font><font color=#323232>1</font><font color=#7d7d7d>1</font><font color=#cfcfcf>1</font><font color=#f7f7f7>1</font><font color=white>10010101101110100100111110010</font><font color=#f7f7f7>1</font><font color=#cfcfcf>0</font><font color=#7d7d7d>1</font><font color=#323232>1</font><font color=#2a2a2a>0</font><font color=#2b2b2b>1</font><font color=#4a4a4a>1</font><font color=#ababab>1</font><font color=#fcfcfc>0</font><font color=white>1001</font><br><font color=white>001111</font><font color=#f1f1f1>1</font><font color=#acacac>0</font><font color=#595959>0</font><font color=#2b2b2b>1</font><font color=#2a2a2a>0</font><font color=#2e2e2e>1</font><font color=#545454>0</font><font color=#919191>1</font><font color=#cbcbcb>1</font><font color=#f6f6f6>0</font><font color=white>11101001001111100101011</font><font color=#f6f6f6>0</font><font color=#cbcbcb>1</font><font color=#919191>1</font><font color=#545454>1</font><font color=#2e2e2e>0</font><font color=#2a2a2a>1</font><font color=#2b2b2b>0</font><font color=#595959>0</font><font color=#acacac>1</font><font color=#f1f1f1>0</font><font color=white>011111</font><br><font color=white>00101011</font><font color=#f9f9f9>0</font><font color=#d6d6d6>1</font><font color=#8d8d8d>1</font><font color=#4d4d4d>1</font><font color=#333333>0</font><font color=#2a2a2a>1</font><font color=#2f2f2f>0</font><font color=#3b3b3b>0</font><font color=#636363>1</font><font color=#909090>0</font><font color=#b7b7b7>0</font><font color=#d9d9d9>1</font><font color=#eeeeee>1</font><font color=#f4f4f4>1</font><font color=#f8f8f8>1</font><font color=#fcfcfc>1</font><font color=white>0010101</font><font color=#fcfcfc>1</font><font color=#f8f8f8>0</font><font color=#f4f4f4>1</font><font color=#eeeeee>1</font><font color=#d9d9d9>1</font><font color=#b7b7b7>0</font><font color=#909090>1</font><font color=#636363>0</font><font color=#3b3b3b>0</font><font color=#2f2f2f>1</font><font color=#2a2a2a>0</font><font color=#333333>0</font><font color=#4d4d4d>1</font><font color=#8d8d8d>1</font><font color=#d6d6d6>1</font><font color=#f9f9f9>1</font><font color=white>10010101</font><br><font color=white>10111010010</font><font color=#fdfdfd>0</font><font color=#d9d9d9>1</font><font color=#aaaaaa>1</font><font color=#787878>1</font><font color=#535353>1</font><font color=#3a3a3a>1</font><font color=#2b2b2b>0</font><font color=#2a2a2a>01</font><font color=#333333>0</font><font color=#474747>1</font><font color=#575757>0</font><font color=#656565>1</font><font color=#6f6f6f>1</font><font color=#797979>0</font><font color=#7e7e7e>1</font><font color=gray>1</font><font color=#7e7e7e>1</font><font color=#797979>0</font><font color=#6f6f6f>1</font><font color=#656565>0</font><font color=#575757>0</font><font color=#474747>1</font><font color=#333333>0</font><font color=#2a2a2a>01</font><font color=#2b2b2b>1</font><font color=#3a3a3a>1</font><font color=#535353>1</font><font color=#787878>1</font><font color=#aaaaaa>0</font><font color=#d9d9d9>0</font><font color=#fdfdfd>1</font><font color=white>01011011101</font><br><font color=white>001001111100101</font><font color=#f7f7f7>0</font><font color=#e6e6e6>1</font><font color=#d1d1d1>1</font><font color=#b0b0b0>0</font><font color=#939393>1</font><font color=#7a7a7a>1</font><font color=#656565>1</font><font color=#535353>0</font><font color=#444444>1</font><font color=#393939>0</font><font color=#313131>0</font><font color=#2c2c2c>1</font><font color=#2a2a2a>0</font><font color=#2c2c2c>0</font><font color=#313131>1</font><font color=#393939>1</font><font color=#444444>1</font><font color=#535353>1</font><font color=#656565>1</font><font color=#7a7a7a>0</font><font color=#939393>0</font><font color=#b0b0b0>1</font><font color=#d1d1d1>0</font><font color=#e6e6e6>1</font><font color=#f7f7f7>0</font><font color=white>110111010010011</font><br></pre></font>";
  html += "</div>\r\n";
  html += "</body>\r\n";
  html += "</html>\n";
  
  Serial.println();
  Serial.print("Page size: ");
  Serial.print(html.length());
	server.send(200, "text/html", html);
  Serial.println();
  Serial.print("Page load count: ");
  Serial.println(visitor_count);
  Serial.println(macID);
}

void changeSSID() {
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

String mac_to_string(){
  String str = "";
  for(int i = 0; i < WL_MAC_ADDR_LENGTH; i++){
    str += String(mac[i], HEX);
  }
  return str;
}

void setup() {
  
	delay(1000);
	Serial.begin(74880);
	Serial.println();
  
	Serial.println("Cleaning up...\r\n");

  WiFi.disconnect();
  WiFi.softAPdisconnect();
  WiFi.mode(WIFI_OFF);
  delay(500);
  
  Serial.println("Configuring access point...");

  // Read mac-address to memory
  WiFi.softAPmacAddress(mac);
  macID = mac_to_string();
  macID.toUpperCase();

  // Setup WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
	WiFi.softAP(ssid);
  
  Serial.println("Access point UP");

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", ip);
  
  Serial.println("DNS server UP");
  
	server.on("/", handleRoot); // Add root path
  server.on("/generate_204", handleRoot); // Android captive portal
  server.on("/ssid", changeSSID); // Add SSID modification page path
  server.onNotFound(handleRoot);  // Add default path for 404-page (in this case, we want redirect to root)
	server.begin();
	Serial.println("HTTP server UP");
}

void loop() {
  dnsServer.processNextRequest();
	server.handleClient();
}
