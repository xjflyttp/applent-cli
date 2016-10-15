# applent-cli
Applent AT824 AT825 AT826 cli

## Raspbian Jessie Lite
```
1. install https://github.com/signal11/hidapi
2. git clone https://github.com/xjflyttp/applent-cli
3. make
```

## Usege
```
//query system info
hid "idn?" "" 
// output "AT826,REV A1.0,<Serial Number>,Applent Instruments Inc."

//query measure value
hid "FETC?" ""
// output "+7.929158e-15,+0.000000e+00"

etc....
```
