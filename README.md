## Readme

Uart input output ascii text.

baud rate : 115200

ͨ�����������õ�Դ��Ĳ��������ص�Դͨ��������״̬������FLASH ��.




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

����


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

����ĳһ·��Դ���أ���ѯĳһ·��״̬

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

���ã���ѯPOWER1�ĵ�ѹ

```
����12V
AT+POWER1=0\r\n
41 54 2b 50 4f 57 45 52 31 3d 30 0d 0a 
����19V
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


7. Get Temperature, оƬ�ڲ����¶�ֵ


```
AT+GETTEMP?\r\n
41 54 2b 47 45 54 54 45 4d 50 3f 0d 0a 

```

8. Get Ref Voltage,оƬ�Ĳο���ѹ, 3.3V


```
AT+GETVREF?\r\n
41 54 2b 47 45 54 56 52 45 46 3f 0d 0a

```

9. Get Current from I1,I2,I3, I4, I5

��������������Ϊ0.1 ohm
I = V/R,

1~5 -> ��·����

0 -> �ܵ���


```
AT+GETCUR=0\r\n
41 54 2b 47 45 54 43 55 52 3d 30 0d 0a
AT+GETCUR=1\r\n
41 54 2b 47 45 54 43 55 52 3d 31 0d 0a
AT+GETCUR=2\r\n
41 54 2b 47 45 54 43 55 52 3d 32 0d 0a
```

### IDE
ʹ����AT32 IDE, eclipse ��Ϊ���ɿ���������

### Changes

```
1.5 - Power 1 will be ON after power up, no persistent state storage;

2.0 - New version, for D-Flipflop to control IN-power and channe 1 power;


```






