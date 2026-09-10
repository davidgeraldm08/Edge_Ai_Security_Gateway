# Edge AI Security Gateway

A Raspberry Pi-based Edge AI Security Gateway designed to provide local network monitoring, anomaly detection, and automated response for IoT devices and IP cameras.

The system analyzes network traffic at the edge instead of relying entirely on cloud-based security, allowing suspicious traffic to be detected and responded to locally.

## Project Overview

The Edge AI Security Gateway acts as an intermediary between IoT devices and the network. It monitors network traffic, extracts relevant packet features, and uses a machine learning model to identify potentially anomalous behavior.

When suspicious traffic is detected, the gateway can respond by blocking the associated traffic through network firewall rules.

## Architecture

```text
IoT Devices / IP Cameras
          │
          ▼
   Edge AI Gateway
   ┌─────────────────────┐
   │ Packet Monitoring    │
   │ Feature Extraction  │
   │ AI Anomaly Detection │
   │ Firewall Response   │
   │ Web Dashboard       │
   └─────────────────────┘
          │
          ▼
     Router / Internet
```

## Key Features

* Real-time network traffic monitoring
* Machine learning-based anomaly detection
* Local processing on Raspberry Pi
* Detection of suspicious network behavior
* Automated firewall-based response
* Web-based monitoring dashboard
* Network Address Translation (NAT)
* IP forwarding and gateway functionality
* Designed for constrained IoT environments

## Technologies

### Hardware

* Raspberry Pi 5 8 GB
* USB Ethernet adapter
* USB Wi-Fi adapter

### Software

* Raspberry Pi OS Lite 64-bit
* Python
* Flask
* Scapy
* Scikit-learn
* NumPy
* Pandas
* Joblib
* Linux
* NetworkManager
* iptables

### Machine Learning

The project uses an **Isolation Forest** model for anomaly detection.

The model analyzes network-level features including:

* Source IP
* Destination port
* Protocol type
* Packet length
* Traffic direction

The model is trained using normal network traffic and deployed to the Raspberry Pi for local inference.

## Detected Threat Categories

The project focuses on identifying network behavior associated with:

* Brute-force attacks
* Botnet activity
* Port scanning
* DDoS traffic

## Network Configuration

The gateway was designed with separate interfaces for WAN, LAN, and wireless access.

| Interface | Purpose                      |
| --------- | ---------------------------- |
| `eth0`    | Primary WAN / Internet       |
| `wlan0`   | Wi-Fi WAN fallback           |
| `wlan1`   | Wireless client access point |
| `eth1`    | Wired client LAN             |

Example network segments:

```text
Wireless Clients
192.168.50.0/24
        │
      wlan1
        │
        ▼
┌──────────────────┐
│ Raspberry Pi 5   │
│ Edge AI Gateway  │
└──────────────────┘
        │
      eth1
        │
192.168.60.0/24
        │
     IoT / LAN
```

## Dashboard

The project includes a Flask-based dashboard for monitoring network activity and displaying anomaly detection results.

The dashboard can be used to observe:

* Network traffic
* Detected anomalies
* Source information
* Detection results
* Security response status

## Project Limitations

This project focuses on **network-level security monitoring**.

It does not provide:

* Physical device security
* Firmware vulnerability analysis
* Deep packet payload inspection
* Complete protection against all IoT attacks

## Project Goals

The main objectives of the project are:

1. Provide local AI-assisted network monitoring for IoT environments.
2. Reduce reliance on cloud-based security processing.
3. Detect anomalous network behavior in real time.
4. Provide automated responses to suspicious traffic.
5. Demonstrate the feasibility of Edge AI security on resource-constrained hardware.

## Thesis Project

**Edge AI Security Gateway Implementation using Raspberry Pi for Cloud-Bound IoT Protection**

Developed as an undergraduate thesis project using Raspberry Pi, machine learning, network monitoring, and Linux-based network security technologies.

## Disclaimer

This project was developed for academic and research purposes. It should be tested in an isolated and authorized network environment before being used in a production network.
