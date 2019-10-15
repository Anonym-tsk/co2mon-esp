# CO2 + Temperature sensor based on MT8057 and ESP

--------------------------------------------------------------
## Что умеет
* Получает температуру и CO2, отправляет по mqtt
* При старте создает точку доступа с возможностью настройки WiFi соединения и MQTT сервера
* OTA обновления через Platform.io и логи по Telnet

## Что использовалось
* [Датчик CO2 MT8057](https://masterkit.ru/shop/1266110)
* Wemos D1 Mini

## Подключение MT8057
Внутри есть разъём с четырьмя пинами
```
  -------
 |O # # #|
  -------
  | | | |
  1 2 3 4
  | | | |
 1 pin - empty
 2 pin - Digital Pin (see src/config/hardware.h)
 3 pin - Digital Pin (see src/config/hardware.h)
 4 pin - GND
```

Ссылки:
* https://habr.com/post/390607/
* https://habr.com/company/dadget/blog/394333/
* https://revspace.nl/CO2MeterHacking
* https://github.com/revspace/co2sensor

---

Enjoy my work? Help me out for a couple of :beers: or a :coffee:!

[![coffee](https://www.buymeacoffee.com/assets/img/custom_images/black_img.png)](https://www.buymeacoffee.com/qcDXvboAE)
