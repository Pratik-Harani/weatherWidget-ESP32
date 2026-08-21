# ESP32 TTGO Weather Widget

A weather display for the LILYGO TTGO T-Display (ESP32). It connects to WiFi, pulls current conditions from the OpenWeatherMap API, and shows the temperature and a short description on the built-in screen, refreshing automatically every 30 minutes.

```
         ──────────────────────
        │        18°C          │
        │     light rain       │
         ──────────────────────
```

## Hardware

- **LILYGO TTGO T-Display** (ESP32 with integrated 1.14" 240x135 TFT display)
- USB-C or Micro-USB cable for flashing and power
- A WiFi network with internet access

## Libraries

Install these via the Arduino IDE Library Manager (or PlatformIO):

- [`TFT_eSPI`](https://github.com/Bodmer/TFT_eSPI) - display driver (must be configured for the TTGO T-Display; see library docs for `User_Setup.h`)
- [`ArduinoJson`](https://arduinojson.org/) - JSON parsing (v6.x)
- `WiFi` - bundled with the ESP32 board package
- ESP32 board support installed via Board Manager ([instructions](https://github.com/espressif/arduino-esp32))

## Setup

1. **Get an OpenWeatherMap API key** — sign up at [openweathermap.org](https://openweathermap.org/api) and grab a free API key for the Current Weather Data endpoint.
2. **Find your coordinates** — get the latitude and longitude for the location you want to display weather for.
3. **Create `secrets.h`** in the project root (see below).
4. **Flash the project** to your TTGO board using the Arduino IDE or PlatformIO.
5. **Power it on** — once connected to WiFi, it will fetch and display the weather, updating every 30 minutes.

## Adding your `secrets.h`

This file is **not included** in the repo. Create a file named `secrets.h` in the same folder as the main `.ino` file with the following contents:

```cpp
#ifndef SECRETS_H
#define SECRETS_H

#define SECRET_WEATHER_API_KEY "your_openweathermap_api_key"
#define SECRET_LATITUDE        "your_latitude"
#define SECRET_LONGITUDE       "your_longitude"

// WiFi credentials (used by connectToWifi.h)
#define SECRET_WIFI_SSID       "your_wifi_name"
#define SECRET_WIFI_PASSWORD   "your_wifi_password"

#endif
```

## Configuration

You can tweak a few constants at the top of `weatherWidget.ino` to change them to your preferences.

| Constant | Description | Default |
|---|---|---|
| `WEATHER_UPDATE_FREQUENCY_MINS` | How often to refresh the weather | `30` |
| `BG_COLOR` / `FG_COLOR` | Background/text colors | white / black |
| `defaultTextSize` | Starting font size (auto-shrinks to fit longer descriptions) | `4` |

## How it works

On each update cycle, the device opens a raw HTTP connection to `api.openweathermap.org`, requests current weather for the configured coordinates, ignores the HTTP headers, and parses the JSON body with `ArduinoJson` to get temperature, conditions, and description. If the WiFi connection drops, the screen shows a "WiFi Connection Failed" message instead.

## Contributing

Contributions are welcome! If you'd like to add anything, feel free to fork the repo and submit a pull request.

## License

This project is licensed under the terms of the MIT license.
