## Readme

Uart input output ascii text.

baud rate : 115200

通过串口来设置电源板的参数，开关电源通道。设置状态保存在FLASH 中.




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

3. Set Echo Mode

不用


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

设置某一路电源开关，查询某一路的状态

```
AT+OFFPOWER=1\r\n
AT+OFFPOWER=2\r\n
41 54 2b 4f 46 46 50 4f 57 45 52 3d 31 0d 0a
41 54 2b 4f 46 46 50 4f 57 45 52 3d 32 0d 0a
41 54 2b 4f 46 46 50 4f 57 45 52 3d 33 0d 0a
41 54 2b 4f 46 46 50 4f 57 45 52 3d 34 0d 0a
41 54 2b 4f 46 46 50 4f 57 45 52 3d 35 0d 0a
AT+ONPOWER=1\r\n
41 54 2b 4f 4e 50 4f 57 45 52 3d 31 0d 0a
41 54 2b 4f 4e 50 4f 57 45 52 3d 32 0d 0a
41 54 2b 4f 4e 50 4f 57 45 52 3d 33 0d 0a
41 54 2b 4f 4e 50 4f 57 45 52 3d 34 0d 0a
41 54 2b 4f 4e 50 4f 57 45 52 3d 35 0d 0a
AT+GETPOWER=1\r\n
41 54 2b 47 45 54 50 4f 57 45 52 3d 31 0d 0a
41 54 2b 47 45 54 50 4f 57 45 52 3d 32 0d 0a
41 54 2b 47 45 54 50 4f 57 45 52 3d 33 0d 0a
41 54 2b 47 45 54 50 4f 57 45 52 3d 34 0d 0a
41 54 2b 47 45 54 50 4f 57 45 52 3d 35 0d 0a
```

5.  Set POWER1 voltage, 

设置，查询POWER1的电压

```
设置12V
AT+POWER1=0\r\n
41 54 2b 50 4f 57 45 52 31 3d 30 0d 0a 
设置19V
AT+POWER1=1\r\n
41 54 2b 50 4f 57 45 52 31 3d 31 0d 0a 
AT+POWER1?\r\n
41 54 2b 50 4f 57 45 52 31 3f 0d 0a

```

6. Get Voltage,

```
AT+GETVIN?\r\n
41 54 2b 47 45 54 56 49 4e 3f 0d 0a

AT+GETV12?\r\n
41 54 2b 47 45 54 56 31 32 3f 0d 0a

AT+GETVOUT?\r\n
41 54 2b 47 45 54 56 4f 55 54 3f 0d 0a


```


7. Get Temperature, 芯片内部的温度值


```
AT+GETTEMP?\r\n
41 54 2b 47 45 54 54 45 4d 50 3f 0d 0a 

```

8. Get Ref Voltage,芯片的参考电压, 3.3V


```
AT+GETVREF?\r\n
41 54 2b 47 45 54 56 52 45 46 3f 0d 0a

```

9. Get Current from I1,I2,I3, I4, I5

测量电流，电阻为0.1 ohm
I = V/R,

1~5 -> 分路电流

0 -> 总电流


```
AT+GETCUR=0\r\n
41 54 2b 47 45 54 43 55 52 3d 30 0d 0a
AT+GETCUR=1\r\n
41 54 2b 47 45 54 43 55 52 3d 31 0d 0a
AT+GETCUR=2\r\n
41 54 2b 47 45 54 43 55 52 3d 32 0d 0a
```

### IDE
使用了AT32 IDE, eclipse 作为集成开发环境。

### Changes

```
1.5 - Power 1 will be ON after power up, no persistent state storage;

2.0 - New version, for D-Flipflop to control IN-power and channe 1 power;


```






