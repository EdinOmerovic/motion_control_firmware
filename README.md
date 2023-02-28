# Firmware za kontroler robotskog sistema

Ovaj repozitoriji sadrži firmware koji se vrti na **LaunchPad LAUNCHXL-F28379D** sistemu.

Sistem ima zadatak da 

- Čita izlaze sa enkodera
- Vrijednosti enkodera proslijeđuje na dSpace
- Vodi računa o sigurnosti aktuatora korištenjem endstop-ova

U budućnosti bi bilo pametno da se cijeli algoritam upravljanja vrti na razvojnoj pločici i da se prestane koristiti SMRDLJIVI code composer. 



### Prerequisites

1. Instaliranje SMRDLJIVOG [code composera](https://www.ti.com/tool/CCSTUDIO)
2. Instaliranje [C2000Ware SDK](https://www.ti.com/tool/C2000WARE)



Prije korištenja provjerite da li ste povezali biblioteke iz C2000 sa projektom:

![](.\docs\images\libraries.png)





Podesite predprocesorske direktive:

![](.\docs\images\predefined_symbol.png)