## Readme

Uart input output ascii text.

baud rate : 115200


### Commands
Start with "AT", end with "\r\n"

AT -> Head

TAG -> VCCOUT

VALUE -> 0

\r\n -> Tail


1. Ping

```
AT\r\n
OK\r\n
41 54 0D 0A
```

2. Set VCC_OUT

```
AT+VCCOUT?\r\n
41 54 2b 56 43 43 4f 55 54 3f 0D 0A
// 12V
0\r\n 
// 19V
1\r\n 
AT+VCCOUT=0\r\n
41 54 2b 56 43 43 4f 55 54 3d 30 0d 0a
OK\r\n
AT+VCCOUT=1\r\n
41 54 2b 56 43 43 4f 55 54 3d 31 0d 0a
OK\r\n
```

3. Set Echo Mode

```
AT+ECHO?\r\n
41 54 2b 45 43 48 4f 3f 0d 0a
0\r\n
1\r\n
AT+ECHO=0\r\n
41 54 2b 45 43 48 4f 3d 30 0d 0a 
OK\r\n
AT+ECHO=1\r\n
41 54 2b 45 43 48 4f 3d 31 0d 0a 
OK\r\n

```

4. Set POWER1~5 , ON, OFF

```
AT+OFFPOWER=1\r\n
41 54 2b 4f 46 46 50 4f 57 45 52 3d 31 0d 0a
AT+ONPOWER=1\r\n
41 54 2b 4f 4e 50 4f 57 45 52 3d 31 0d 0a
41 54 2b 4f 4e 50 4f 57 45 52 3d 32 0d 0a
41 54 2b 4f 4e 50 4f 57 45 52 3d 33 0d 0a
AT+GETPOWER=1\r\n
41 54 2b 47 45 54 50 4f 57 45 52 3d 31 0d 0a
41 54 2b 47 45 54 50 4f 57 45 52 3d 32 0d 0a
41 54 2b 47 45 54 50 4f 57 45 52 3d 33 0d 0a
```

5.  Set POWER1 voltage, 

```
AT+POWER1=0\r\n

AT+POWER1=1\r\n

AT+POWER1?\r\n


```



