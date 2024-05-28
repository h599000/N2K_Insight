# N2K Insight

N2K Insight is a solution developed to enable remote monitoring of marine vessel systems using the NMEA 2000 network. The primary goal is to provide accessibility to real-time and historical data from vessel systems, overcoming the limitations of data being available only locally.

## Table of Contents

- [Project Overview](#project-overview)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Running the Project](#running-the-project)
- [License](#license)

## Project Overview

N2K Insight aims to enhance the remote monitoring capabilities of marine vessels by integrating with the existing NMEA 2000 infrastructure. This project involves the design, implementation, and testing of a system capable of extracting, managing, and displaying NMEA 2000 data.
The project is intended to work in paralell with propriatory hardware.

## Getting Started

### Prerequisites

To get started with N2K Insight, you will need the following:

- Microcontroller with CAN bus interface (e.g., ESP32 with CAN interface)
- NMEA 2000 network setup
- [PlatformIO](https://platformio.org/) IDE
- [InfluxDB](https://www.influxdata.com/) for data storage
- [Grafana](https://grafana.com/) for data visualization

### Installation

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/yourusername/N2K_Insight.git
    cd N2K_Insight
    ```

2. **Install Dependencies**:
    Ensure you have PlatformIO installed and then install the project dependencies:
    ```bash
    pio lib install
    ```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
