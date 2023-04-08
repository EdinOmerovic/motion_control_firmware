# Motion control firmware for C2000 LaunchPad

Welcome to the firmware repository for C2000 series LaunchPAD running motion control algorithm discussed during ETF's Motion Control Course 22/23.

This repository contains the firmware code for the C2000 series  LaunchPAD, a popular development platform for motion control applications. The firmware is specifically designed to run motion control algorithms on the LaunchPAD utilizing the experimental setup specially designed for the Couse.

The setup consists of:

- RENISHAW RGH24O15D30B - incremental encoder
- FAULHABER LM2070-080-11-C - linear DC motor
- FAULHABER MCLM3006 - motor controller 



/TODO: dodaj sliku i šemu setupa



The firmware is open-source, meaning that you can modify and extend it to suit your specific needs. It includes a range of features, such as real-time control, advanced motion control algorithms, and support for multiple motor types. Additionally, it includes a comprehensive set of examples and tutorials to help you get started quickly and easily.

We hope that you find this firmware repository helpful in your motion control development efforts. If you have any questions or comments, please don't hesitate to reach out to us via the issue tracker.



The used trajectory tracking algorithm is described below and implemented in main.c 

![](./docs/images/algorithm_scheme.png)





### Firmware development

To start developing first install the [Texas instruments Code Compose Studio](https://www.ti.com/tool/download/CCSTUDIO/12.2.0) and install [TI C2000 Software development kit.](https://www.ti.com/tool/C2000WARE)  



After that, just import this project and start playing with it. 



<u>TODO:</u> 

- [ ] Implementirati dovođene referentne trajektorije korištenjem analognog napona `getTrajector()`
- [ ] Implementirati računanje vrijednosti drugig izvoda za `getTrajector()`
