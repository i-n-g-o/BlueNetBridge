# BlueNetBridge
A bridge to connect bluetooth devices to the internet for BLE devices featuring a serial communication channel.

Currently it supports Adafruit "Bluefruit LE" or "Bluefruit UART Friend":<br>
https://www.adafruit.com/product/2487<br>
https://www.adafruit.com/product/2479
<br>
<br>
Serial data starting with "http://" or "https://" will get relayed to the internet sending a http request. Depending on the response-configuration of the bridge the response is written back to the BLE device.

It features some special commands to configure the bridge or to requesting information about the bridge in the from of:
[prefix character]:[command]:[value]
<br>
<br>
  
### configuration commands - prefix c:

| command | description |
|---------|-------------|
| c:response | get the response-setting of the bridge |
| c:response:1 | configures the bridge to send http response to the device or not |
<br>

### device commands - prefix d:

| command | description |
|---------|-------------|
| d:info | get OS versions of the bridge |
