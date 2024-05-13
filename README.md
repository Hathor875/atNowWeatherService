# 🌦️ atNowWeatherService 🌡️

This project aims to create a website that allows users to check the temperature, pressure, and humidity in real-time using small ESP2866 sensors. The environmental data can be accessed either through a web application or via an API. Additionally, the project will generate analysis reports and charts based on the collected data.

## Technology Stack

- **Backend**: Java
- **Database**: MongoDB
- **API**: OpenAPI 3.0
- **Frontend**: JavaScript, HTML, CSS
- **Embedded**: ESP2688 with AHT20 and BMP280 sensors programmed in C++ with arduino framework
- **Charts and analysis**: Python

## Features

- Real-time monitoring of temperature, pressure, and humidity 🌡️🌬️💧
- Web-based interface for accessing environmental data 🌐
- RESTful API for programmatic access to environmental data 📡
- Generation of analysis reports and charts based on collected data 📊

## Embedded

The project utilizes ESP2688 microcontroller with built-in Wi-Fi and the following sensors:
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

## In Progress
- **HTTPS SERVER ON ESP**:
Now the device starts in AP mode with the given name and password set in the program.
At the address 192.168.1.1 we have a service that allows you to log in to the selected Wi-Fi network.
to do:
  - rewriting to https
  -saving the password, network name and configuration to eeprom
  -handling the case of wrong WiFi password etc
  -restore after connection loss
  -signaling connection status

Moreover, the LED flashing has been modified to use the built-in timer, but this still needs to be improved.
Also need to add the reset function to factory settings using the button

## To Do

- **Start Project**: Set up project structure and environment.
- **Roadmap**: Define project milestones and timeline.
- **First Working Prototype**: Develop embedded system with temperature measurement functionality only.
- **First Backend Version**: Implement backend to receive and display temperature in a simple HTML page, and store it in the database.
- **API**: Create a RESTful API to access environmental data.

## Contributing 🚀

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and submit a pull request with your changes. Additional contributions are appreciated after the project is launched and basic functionalities are in place.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).


