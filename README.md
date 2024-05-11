# atNowWeatherService

This project aims to create a website that allows users to check the temperature, pressure, and humidity in real-time using small ESP32 sensors. The environmental data can be accessed either through a web application or via an API. Additionally, the project will generate analysis reports and charts based on the collected data.

## Technology Stack

- **Backend**: Java
- **Database**: MongoDB
- **API**: OpenAPI 3.0
- **Frontend**: JavaScript, HTML, CSS
- **Embedded**: ESP32 with AHT20 and BMP280 sensors

## Features

- Real-time monitoring of temperature, pressure, and humidity 🌡️🌬️💧
- Web-based interface for accessing environmental data 🌐
- RESTful API for programmatic access to environmental data 📡
- Generation of analysis reports and charts based on collected data 📊

## Embedded

The project utilizes ESP32 microcontroller with built-in Wi-Fi and the following sensors:
- AHT20: Temperature and Humidity Sensor
  - Temperature Measurement Range: -40°C to 85°C
  - Temperature Measurement Accuracy: ±0.3°C
  - Temperature Measurement Resolution: 0.01°C
  - Humidity Measurement Range: 0% to 100% RH
  - Humidity Measurement Accuracy: ±2%
  - Humidity Measurement Resolution: 0.024%
- BMP280: Temperature and Pressure Sensor
  - Temperature Measurement Range: -40°C to 85°C (±1°C error), 0°C to 65°C (±0.5°C error)
  - Temperature Measurement Resolution: 0.01°C
  - Pressure Measurement Range: 300hPa to 1300hPa
  - Pressure Measurement Accuracy: ±0.12hPa @25°C

## Contributing 🚀

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and submit a pull request with your changes. Additional contributions are appreciated after the project is launched and basic functionalities are in place.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).
