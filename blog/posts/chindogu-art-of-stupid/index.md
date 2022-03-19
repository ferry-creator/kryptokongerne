---
title: Chindōgu - The Art of Stupid
date: 2022-03-11
---
#wow!
Most people dislike **cold rainy weather**. Are we loosing touch with nature by sheltering?
![Umbrella in rain](./umbrella.webp)

*Gone are the days of staring out the window into the pouring rain, dreaming of simpler times away from the digital duties of modern life.*
## Introducing: The Rain Simulator!
We want to create an `IoT` solution that allows everyday people like you to experience rain indoors while still getting the 🏔 **True Outdoor Experience**.

Using our exciting new rain technology 💦 ***Real-Splash*** ™ 💦 , we'll ensure you stay fresh and hydrated throughout your day in the open office treadmill.

## Project Overview
The idea is to simulate rain indoors whenever it's raining outdoors. This will not only notify users indoors to stay inside, it will also provie them a more authentic and natural experience.
* Rain sensor module placed outdoors
* Indoor rain experience (powered by ***Real-Splash*** ™)
* MQTT between rain sensor and indoor rain experience
* Node-RED ties everything together

The rain sensor will be connected to a Node MCU, and so will the indoor rain experience.  
Therefore 2x Node MCU's is required. The `ESP32` modules (Node MCU) will connect to Node-RED via MQTT - in this way allowing for automatically starting the indoor rain simulation whenever it's raining outside for reals.