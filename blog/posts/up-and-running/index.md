---
title: Getting up and running
date: 2022-03-18
---
We spent the day configuring **Node-RED**, **MQTT**, and deployment to MS Azure.
![Node MCU IoT](./node_mcu_graphic.webp)

## Setup & Configuration
First thing we did was to spin up a VM on Azure, on which we installed NodeJS, along with node-RED as a global `npm` package. We wanted to create a MQTT broker that our smart devices can talk to, so we used [maqiatto](https://maqiatto.com/)
as a service and hooked it up in node-RED using the MQTT-out node.

In that way, node-RED is acting as a MQTT client hosted on Azure, and then we can add as many devices / topics with MQTT clients as needed - e.g. `ESP32` chips such as the Node MCU.
![](./MQTT.png)
On the diagram 4 topics are shown. We will only be needing 2, but it still gets the point across - we use Node-RED to create flows, which control how we interact with the MQTT devices.

## Homework
We decided some group members would prepare some work before next friday.
**Andreas & Kristoffer**
* Rain mechanism
* Driving a motor
* Brainstorming casing

Alex & Jonathan
* Rain sensor
* Write blog
* Brainstorming casing

By planning ahead, we are able to spend time in the entire group more efficiently, as we can then focus on getting Node-RED and the IoT part of the project up and running.