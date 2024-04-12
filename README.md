<img src=Reference-Photos/OSU-Engineering-Placard.png width='1000'>

# High Altitude Balloon (HAB) Senior Capstone Project

<img src=Reference-Photos/Curvature-Photo-Title-Page.jpg width='600'>

## Contents
1. [Project Importance](#user-content-project-importance)
2. [Every Parts List](#user-content-every-parts-list)
3. [Payload Construction](#user-content-payload-construction)
4. [Payload Arduino Code](#user-content-payload-arduino-code)
5. [Tracking Receiver Construction](#user-content-tracking-receiver-construction)
6. [Tracking Receiver Arduino and MATLAB Code](#user-content-tracking-receiver-arduino-and-matlab-code)
7. [Launch Protocol](#user-content-launch-protocol)
8. [Launch Experience and Data Results](#user-content-launch-experience-and-data-results)
9. [Flight Test Data Comparison to Simulation Code](#user-content-flight-test-data-comparison-to-simulation-code)
10. [Photo Results](#user-content-photo-results)
11. [Suggestions for Improvements](#user-content-suggestions-for-improvements)
12. [Final Thoughts and Reflection](#user-content-final-thoughts-and-reflection)
13. [Useful Online Tools](#user-content-useful-online-tools)
14. [Resources and Inspirations](#user-content-resources-and-inspirations)
15. [Acknowledgements](#user-content-acknowledgements)


## Project Importance
For my senior year of college at The Ohio State University, we were assigned a capstone group tasked with completing a year-long project. There were a variety of topics we could possibly be assigned; from creating a workbench meant for Starlab to conducting research for both the Navy and Airforce to creating a cost-effective high-altitude balloon (HAB) using off-the-shelf components. Can you guess which one I was assigned?

For more context on the reason behind this project, the first aerospace engineering course (AERO 2200: Introduction to Aerospace Engineering) that each new, bright-eyed sophmore in the major takes will conduct a high-altitude balloon lab at one point. This lab involves a launch of a high-altitude balloon by the professor and other academic faculty in order to collect atmospheric data such as ambient temperature and pressure in order to be analyzed by the students. The purpose for this lab is to teach the students the standard atmosphere (specifically, how air pressure, temperature, and density changes with altitude).

<img src=Reference-Photos/Standard-Atmosphere-Anderson-Textbook-Photo.jpg width='300'>

#### (Figure from Introduction to Flight Ninth edition by J.D. Anderson, Jr. and M.L. Bowden)

In prior years, including my sophmore year, there were a multitude of issues with the commerical weather balloons that can be bought off the internet (i.e., QRP Light Tracker and Tracksoar). The issues ranging from the balloons themself pre-maturely bursting before it could reach the target altitude or the payload electronics being lost mid-flight. This was a concern to the Mechanical and Aerospace Engineering (MAE) Department since each year they would have to spend upwards of $1000+ for a new weather balloon for each year. This is where our project's importance came in since the goal was to create a new weather balloon payload using off-the-shelf components that was cost-effective (sub $600 for the payload) that could reliably reach a target altitude of 100,000 ft and be recovered for future use. If it couldn't be recovered, the low cost for each payload was important so the department wouldn't need to spend as much money for each launch.

## Every Parts List

### Electronics Parts List

|Hardware|Description|Price|Amount|Total Cost|
| ------ | ------------ | ------ | ------ | ------ |
| [Arduino MKR WAN 1310](https://store-usa.arduino.cc/products/arduino-mkr-wan-1310?selectedStore=us) | The Microcontroller that ran all the payload sensors. Also has built-in Long Range (LoRa) Wide-Area-Network (WAN) module. | $46.00 | 2 | $92.00 |
| [Arduino GPS Shield](https://store-usa.arduino.cc/products/arduino-mkr-gps-shield) | Arduino's own GPS board that can be easily attached to Arduino MKR WAN 1310 board. | $40.10 | 1 | $40.10 |
| [DS18B20 Waterproof Temp. Sensors](https://www.amazon.com/dp/B08V93CTM2?ref=nb_sb_ss_w_as-reorder_k0_1_11&amp=&crid=RYT5ZSFO72L3&amp=&sprefix=temperature) | Waterproof temperature sensors capable of recording down -55℃. Preferably want two: one for atmospheric temp. and the other for payload internal temp. | $13.48 | 1 | $13.48 |
| [Adafruit MPRLS Ported Pressure Sensor](https://www.adafruit.com/product/3965) | Pressure sensor capable of reading down to 0 psi. Ported end allows for silicone tube to be connected and allow pressure sensor to remain inside the payload | $29.95 | 1 | $29.95 |
| [Adafruit PCF8523 Real Time Clock (RTC) Board](https://www.adafruit.com/product/3295) | Keeps date and time. This helps timestamp each data set for easier analysis of data later. You would think a $40+ Arduino board could do this on its own but NOPE! | $6.95 | 1 | $6.95 |
| [3.7v 3700mAh Lithium Polymer Battery](https://www.amazon.com/EEMB-3700mAh-Rechargeable-Connector-Certified/dp/B08215B4KK/ref=sr_1_1_sspa?crid=20V5R89AHSTOM&dib=eyJ2IjoiMSJ9.i3QtSUlWALXHzx4WvPawio5nE3DuZNtlaAzTwZlcaedrIUl8bG37vHRSd6G3BLEs26bsBIXT4LJDbQ4jQ6CRaojLUG1G20UeN4h5BQ5V05ysqw-9mahhmJQQuSkPjDO3kfU_MP4qToX0M8A39r4oYGaqnXrou9bfDniVCoNubOBgNw5_MxaD768Uw6UPDw08XnxpJy2OTMHVfw2eBrtA0ZjpE9l6qzujPwhaE1FOJJEs8uACaxw_vJP6NCmHxURSqHVn3GhzVnkAips7Z8Z9kC_njDkPhLlPFrVpVIYe3zQ.hE46HQr8jkF_Ve8R7ZKTrtOtbZc0rJD0E5v75pV0LIo&dib_tag=se&keywords=Lithium+Polymer+Battery+3.7V+3700mAh&qid=1712352030&s=electronics&sprefix=lithium+polymer+battery+3.7v+3700mah%2Celectronics%2C88&sr=1-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) | Power supply for the entire payload. This may be overkill and can easily be substituted for different capacity battery | $16.99 | 1 | $16.99 |
| [ESP32-CAM Board w/ OV2640 2MP Camera Module](https://www.amazon.com/ESP32-CAM-MB-Aideepen-ESP32-CAM-Bluetooth-Arduino/dp/B0948ZFTQZ/ref=sr_1_3?crid=3ADRQM5WPSH8R&dib=eyJ2IjoiMSJ9.yXp5XSeHR9RK1KEXydQRGuumc9tZ05jxGizfXw2RCQbXrzP7lcyZYc5Edv3_whL8sLvkt60-2xGQqRxAagipV0N6IicNq8Xhd59KW6XKImENwwjO0j2YwUd5ncSJTCpph_vZvf7Ktsh6XZ3x7wWEOcCl48EKT-KELVQ7ShwMyYzrFMn6LhLp0ayXPBwYF8FiBeFLXPGqlWK3La2iUqyn6MMtlU489RWllqWLDrIPFU3PyjL0tPKuwfrp9l__o6GVMarAz9NexIJQYEnlgTGMkMUvrIwBGm1BuDjw1wSiR24.DHfEokd55lnvwPZ-HFIrQvNgxb9w8R2WOF_YL0DQJZQ&dib_tag=se&keywords=esp32%2Bcam&qid=1712352196&s=electronics&sprefix=esp32%2Bcam%2Celectronics%2C87&sr=1-3&th=1) | On-board camera meant to take photoes every 30 seconds and store to an on-board micro-SD card. This can be omitted if pictures aren't desired | $19.99 | 1 | $19.99 |
| [5mm Micro Mini Slide Switch (3 pin/2 position)](https://www.amazon.com/Tnuocke-Vertical-Position-Switches-SS12D10-G5/dp/B099N61GTZ/ref=sr_1_3?dib=eyJ2IjoiMSJ9.hieZvJzpyYUhOdaLU-8tOT8Kaw_T0nSlOyxK3hcC0mS2Cykj4Rx5IoWaG03SCkoKn65fq9aFybT5xGqJjMQ-vWv7LSWhju4pnk9mkpIvkdypkeyfDc5SneBolNvahavdg-LhXjkzvoeNaZXcibgameXqCVynbVs23_FpblE4CxOigKCLRc1JDpVvxyomBUXVaJYLGhIDnhoPSeZjDT7v-gcGGaBH2RmA09w25CdxbG0.HjdL8o9ZiQe0eeN3mq-FZafa3D4daJy0lFJA6LOqFFU&dib_tag=se&keywords=Small+electronic+switch&qid=1712352474&sr=8-3) | Turns on/off the payload. | $6.95 | 1 | $6.95 |
| [Micro-SD Card Breakout Board](https://www.amazon.com/Integrated-Circuit-Interface-Raspberry-Breakout/dp/B08C4WY2WR/ref=sr_1_6?crid=1H5U9X5N7662K&dib=eyJ2IjoiMSJ9.2B-9-yv1-XRRkhnqVxnZoIfIJ-n6KlnbYGr9Z-5vCgXVCEjit3EZprdWluk6iAhJbpa1IaLDMGEohNEkAFO0Ccy8Pew0EaPfMPWAwpOM_6zrKy6V2TW2QXevg7oaWBgIt0YpUTUHoMjseS3kQ8wByTVFta83iZUV7vM3wK3ZHnQZY5_8MONnHxNlOOJtF90cgeqtnk5alF7cQA-oyZj3p_nmBFFPly-wAcsR3K9Yjfo.X-1GGUWiUlABI9zNxozsAN2nad611q0X-KYHwt29-Z8&dib_tag=se&keywords=Micro+sd+card+breakout+board&qid=1712352774&sprefix=micro+sd+card+breakout+board%2Caps%2C93&sr=8-6) | For storing each packet of data on-board the payload iteself. This way if the radio packets end up corrupted or garbage this shouldn't be compromised. | $5.99 | 1 | $5.99 |
| [Wire](https://www.amazon.com/TUOFENG-Hookup-Wires-6-Different-Colored/dp/B07TX6BX47/ref=sr_1_5?crid=NNRZ3R0UF2AV&dib=eyJ2IjoiMSJ9.0is0xfeGIMj9TEgcksIBU7bGdorlTXp3CnIU7qXzmx2dzsyDk5-KXR7JvmXlGoxVo1-p3-CN4jLwv4Q6WifFNXuOrQSh4m3oRF5_TrrPMlc9JCKN9CxpW9_rEPN7m7dwT_viu9ogbmfW5e6gx9oe-QM14YnQPp0oUnCF_05S7mYPApGYscha5KGazwYaku479gs5WjsOMTd-qg9L6fSTKgNFE1OpgR1hwea-UJXJ7NdqkfzJbsQtL-XWP1g-Xjmu70rXQw5K3yo8YZiS5ndDL8Ckhf3yE0bvEQX-f2eCjDU.D8oWhY3Usn-Y6cDyZqv1NnRN4Q50iJuPO1Z4LlxiDcg&dib_tag=se&keywords=electronic%2Bwire&qid=1712353019&sprefix=electronic%2Bwire%2Caps%2C92&sr=8-5&th=1) | For connecting everything together. I'm weird and I really like color coordinating all my wires so I don't blow anything up. Obviously can be omitted from parts if you already have some.  | $14.99 | 1 | $14.99 |
| [Solder](https://www.amazon.com/Rosin-Solder-Electrical-Soldering-JWOWO/dp/B0C7KZF11F/ref=sr_1_7?crid=36X64DBZHL5KC&dib=eyJ2IjoiMSJ9.MbBrySFnx_k0V7AZHV1pQE62uzTa27JHEwMwVz-APvkHmmLqOOaTfkgiHnUilMSK-_5oKAWs5NAiGg-h8BYfMF0gTlyAySK3RLkThuMPfuGtJ82RVq71xyI4DVZ4LnBG32OKAQU_0qumvB6jdr9jFD2kdqXbBUOoszQqTBM6iS-NSFSnIMm8sn_TIQZXfnHHQjuuCqGF9S_TzuVZ8lC4tGYguRTZlpntlwnfAQrPfefrLB4Uy3sDtsUdEMVEacvR3OuHpoSlVMpMioOsx0YIBogus4bC1OlohDv0OcjS5z4.tSYIBs8r1n9v2yp-_3TMlIXahJP6XZi8GSmN4-Hg2sM&dib_tag=se&keywords=electronic%2Bsolder&qid=1712353156&sprefix=electronic%2Bsolder%2Caps%2C92&sr=8-7&th=1) | For connecting everything together. I highly, HIGHLY recommend soldering everything together. The jetstreams up above can get pretty nasty and really put your payload through the wringer. | $7.99 | 1 | $7.99 |
| [SUSB Type-C Charging Module](https://www.amazon.com/Charging-Lithium-Battery-Charger-Protection/dp/B08X6G26Q8/ref=sr_1_3_pp?crid=108Z5U9FF23Y4&dib=eyJ2IjoiMSJ9.fAUmfmscQ6Pn4nR4irzgjaW-BmEPFZRilyTIfolN2ZKrHE1xFFHvcrgYH1FbPZ4ZwN33Ro_jvjlRG20cZn2P_7A3nOISCR8vbYlPK5yVDIb3AyMYoCkwArL0v9ERxR7vd2OjYBHcHwozYBuBt8PHClauOuc6YMY6f8tyCEdicWlGL-8Y09E8hIhj3-GnLkQ2l-Sh_JN5Ujm2gjcRprm6EMHH6ftQauurUeYVSRlCkjQ.iTSouIKTAqJjP72HNWBgcTTMaegfSwn3LCHg_xKcd8M&dib_tag=se&keywords=USB%2Bc%2Bbattery%2Bcharging%2Bboard&qid=1712365314&sprefix=usb%2Bc%2Bbattery%2Bcharging%2Bboar%2Caps%2C92&sr=8-3&th=1) | I noticed some issues with connecting the battery & switch directly to the battery connecter on the Arduino. So I attached one of these to the batteries to make sure it would be fully charged before the flight. | $7.99 | 1 | $7.99 |
| [0.96 Inch OLED Display](https://www.amazon.com/GeeekPi-SSD1306-Display-Arduino-Raspberry/dp/B0833PF7ML/ref=sr_1_4?crid=W3TQ27L7971Z&dib=eyJ2IjoiMSJ9.pqayzekglP-CFKqGl475BarNv4qta3sNXqiJ-XgFOXo_6MguOTd2p7c_5yymtUZFO_00Ifd-UBQBI1zu1_jVm8SKWBWDAqVHmIgy2aKZKsjwLCC0AZOr4-s0bj58WyTYUScT9p6tMWcHqeSI6iesuaohbZaZDUp3rep16x5bp-HTkqt49Fm8KmH_h7nXHYlq1DCipw4_cNA3h7TPZSYtZ1V45kUpzs6zwFHCzEaW0R4.fEpvt7CiJtHAQUVrNv7jMt9g_bO33qfQN5oLGgeAMlg&dib_tag=se&keywords=arduino+mini+oled+display&qid=1712369543&sprefix=arduino+mini+oled+display%2Caps%2C93&sr=8-4) | Used on the tracking receiver to display the HAB's current latitude, longitude, and altitude. | $14.99 | 1 | $14.99 |
| [1.2" x 1.2" Perfboard](https://www.amazon.com/gp/product/B0CBPRBRTT/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) | Used these to create "hubs" for the various wires. Like connecting all power, ground, SCL, and SDA pins together | $7.09 | 1 | $7.09 |

### Launch Equipment List

|Equipment|Description|Price|Amount|Total Cost|
| ------ | ------------ | ------ | ------ | ------ |
| [Helium](https://www.ballooniacs.com/medium-110-cu-ft-helium-tank-rental/) | Obviously need helium for our balloon. Tank rentals can vary, but if you are in the Columbus, OH area I highly recommend BALLOONIACS. They are very nice and were able to get us a tank of helium on very short notice. Just make sure you double check how much helium you will need before launch! | ~$300.00 | 1 | ~$300.00 |
| [Balloon(s)](https://www.scientificsales.com/8244-Weather-Balloon-1200-Grams-Natural-p/8244.htm) | Obviously need a balloon so our project lifts of the ground. Just double check the size balloon you need to reach your target altitude. We went with a 1200 gram balloon in order to reach 100,000 ft | $110.00 | 1 | $110.00 |
| [Pressure Regulator](https://www.amazon.com/Harris-3000606-25GX-Regulator-500-580/dp/B005V0RAYI/ref=sr_1_2?crid=1WMKSOFAIOLM7&dib=eyJ2IjoiMSJ9.g5ClGlclQesMPfjxYGbqaYpNrYic_xLnGNqCXls0OfP0UusrJHA5ri58M3NcdQY8bnYWUKq-v5N51fqfM5WPPQQthYTr00TB5O_UPBnfVwfbqZttlkZwAvdbE9LHVmqOngNy4toz2AiYas4lRryTuEbG7mQUYeinYIKttFnmeVhtmOAwY6KYAXAsCozlKUe6vw9HAbG4IW3s3zi6h6fMC4qseWjw6DQnHlNsiMfqX-H0O8_HUU4hadGMMjgt7Waikhk7-leHg3L150lOGXrSYUWN3L0rXKQdX8DtDkUJB-w.G5_aNA7szjHYuLN207V6bJwu009ZIQRhzUIxc0aP9ws&dib_tag=se&keywords=Harris+25-100+Compressed+Gas+Pressure+Regulator&qid=1712353964&sprefix=harris+25-100+compressed+gas+pressure+regulator%2Caps%2C90&sr=8-2) | A pressure regulator helps fill the balloon safely. This is similar to the one we used, I just recommend getting one with a flared end in order to screw directly into the helium bottle. Obviously this can be replace with a similar device/method that is cheaper or more accessible to you! I was just fortunate enough to borrow one from our engineering department | $141.07 | 1 | $141.07 |
| [Quick Disconnect Hose](https://www.amazon.com/Hromee-Compressor-Fittings-Industrial-Coupler/dp/B08V17V6WJ/ref=sr_1_8?crid=BMX1VC4WT23Z&dib=eyJ2IjoiMSJ9.NgZBocZ_1LEJEtMgpTqpvsc3ocLB-SoYeiwW7bv7csS1j1v0vJbWUoak9v0-knLA7ZXfbiFETH94pleUP7hnrMjqsjbCy2HB21Mb3x-0cvPu6orkxUqL8jHwjTmYGAo0HDAonMcUYolvQwpyKaA1PaB7mnGsDdbpTnWQLAL4BY-ScWF0WeOcPC3oX97GrHyLLHzU2AGy5ITuLRqWUIdaxwT67HdSgz255NAk70J8MxE5MBa4j80F5FfZjapiz4pcfyflje-EC5-VvQbB9ExrSKPmF9ah3qmMEh3a7mm0t6I.P-d4v0BuHc05wo2yFXIuIZcz6ss8Qt2CdoJhKjgdLoY&dib_tag=se&keywords=air+compressor+hose&qid=1712354156&sprefix=air+compressor+hose%2Caps%2C88&sr=8-8) | Air compressor hose for connecting the helium bottle to our ball-valve nozzle that is used to fill the balloon (pictured below). Doesn't have to be this exact same one, I just suggest something similar to this one. | $19.99 | 1 | $19.99 |
| [Fish Scale](https://www.amazon.com/dp/B07JWGHC48?ref=ppx_yo2ov_dt_b_product_details&th=1) | You can use this to determine how much helium is in your balloon. This gives the buoyancy force and then you can figure out the volume of helium inside the balloon if you assume it's spherical. I suggest making sure the fish scale can read the intended buoyancy force. | $19.99 | 1 | $19.99 |
| [Teflon Tape](https://www.menards.com/main/plumbing/plumbing-installation-repair/pipe-sealants-caulk-putty/blue-monster-teflon-pipe-thread-sealant-tape/70886/p-1444440440571-c-8531.htm?exp=false) | For sealing the threads of the pressure regulator when screwing it into the helium bottle | $4.31 | 1 | $4.31 |
| [Large Zip Ties](https://www.walmart.com/ip/Hyper-Tough-14-5-inch-75lb-Yellow-Cable-Ties-50-Count/595201774?from=/search) | For sealing the balloon and attaching the payload/parachute to the balloon. I recommend wrapping the part of the zip tie that will rub up against the balloon with a bit of electrical tape in order to prevent it from cutting the latex | $7.48 | 1 | $7.48 |
| [Wrenches](https://www.menards.com/main/tools/hand-tools/pliers-plier-sets/masterforce-reg-groove-joint-pliers/84-550/p-1444425505984-c-9156.htm) | For screwing the pressure regulator into the helium bottle. Just make sure whichever ones you use will fit the nut on the pressure regulator you're using | $9.78 | 1 | $19.78 |
| [Tarp](https://www.walmart.com/ip/Hyper-Tough-Weather-Resistant-Light-Duty-6-x-8-Polyethylene-Tarp/603949195?wmlspartner=wlpa&selectedSellerId=0&wl13=2098&adid=22222222277603949195_117755028669_12420145346&wmlspartner=wmtlabs&wl0=&wl1=g&wl2=c&wl3=501107745824&wl4=pla-306310554666&wl5=9014961&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=603949195&wl13=2098&veh=sem_LIA&gclsrc=aw.ds&&adid=22222222237603949195_117755028669_12420145346&wl0=&wl1=g&wl2=c&wl3=501107745824&wl4=pla-306310554666&wl5=9014961&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=603949195&veh=sem&gad_source=1&gclid=CjwKCAjwwr6wBhBcEiwAfMEQswRwFDO85AEUwWshLEQ89RyRzZsJuGzKVzQDvG8kP0XF5DSPqd6hUhoCv3gQAvD_BwE) | For setting up the balloon. Un-rolling the balloon in the grass can cause issues with the latex so I highly recommend getting a big tarp to set everything up | $4.66 | 1 | $4.66 |
| [Ball-Valve Filling Apparatus](https://www.highaltitudescience.com/products/3-cm-max-safe-inflator) | Mentioned previously, this was inserted into the neck of the balloon in order to fill it. A picture of it is included directly below. I HIGHLY reccommend you making your own. A hundred dollars for this one seems a bit extreme in my opinion and you could definitely make one for cheaper. | $99.99 | 1 | $99.99 |

### Personal Protective Equipment (PPE) List
|Equipment|Description|Price|Amount|Total Cost|
| ------ | ------------ | ------ | ------ | ------ |
| [Safety Glasses](https://www.amazon.com/dp/B016KZ2APQ?ref=nb_sb_ss_w_as-reorder_k3_1_8&amp=&crid=3K7V7P2FODEXM&amp=&sprefix=safety+g) | Want to be safe and protect your eyes! | $18.16 | 1 | $18.16 |
| [Vinyl Gloves](https://www.amazon.com/Schneider-Latex-Free-Powder-Free-Disposable-Examination/dp/B09KRSXZNL/ref=sr_1_2_sspa?crid=15X38HNMNEB6O&dib=eyJ2IjoiMSJ9.rND9uGISjYhVVUUSaT9tiCrkfj1DOx7KleCRPo7puHmk43sNDJztaWR_EfVUr9n2-H7NN_YTEFaYaIMaosLaVS1W54kr8fZc5hO8r9oy3zWOcsXV6UU0_bZJDak7tTr2JLVYvazq661pS0x4mw1MpNF03VOF61wQ3Z8LUbVlVPd6dD3hr6uT1bgKa6wkGQJujfWmoneNvpB4JM9ENH7HgEvy37TyM5fCKCQUhkYcSsfQxOUdgL3HQFq922G-U1bBmjTvcN0h913-3T_IcNKsm6eA1YgEdW8dHHM5ayaqowI.QqHf-WODNaiQm4My2iYTNq-V4BpPqCstjmCdJhYaqcE&dib_tag=se&keywords=Vinyl+Gloves&qid=1712355769&s=industrial&sprefix=vinyl+glove%2Cindustrial%2C87&sr=1-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) | The oils in your hands can cause week spots to form in the weather balloon so you really want to wear gloves when handling it. Just make sure they fit your hand size! | $7.57 | 1 | $7.57 |

### Payload Equipment List
|Equipment|Description|Price|Amount|Total Cost|
| ------ | ------------ | ------ | ------ | ------ |
| [Payload Chassis](https://www.amazon.com/HATCHBOX-3D-Filament-Dimensional-Accuracy/dp/B00J0ECR5I/ref=asc_df_B00J0ECR5I/?tag=hyprod-20&linkCode=df0&hvadid=312171611667&hvpos=&hvnetw=g&hvrand=14155102363617613838&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9014961&hvtargid=pla-436686983028&psc=1&mcid=dd1fc76625143d00a7985d8301ed8f18&tag=&ref=&adgrpid=61727971146&hvpone=&hvptwo=&hvadid=312171611667&hvpos=&hvnetw=g&hvrand=14155102363617613838&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9014961&hvtargid=pla-436686983028&gclid=CjwKCAjwwr6wBhBcEiwAfMEQsy6mEc9p4pBHxKIXGoVduPGa4h2faAejA6sWISQgQGwQbJ8dcWg6ehoCQu0QAvD_BwE) | Of my own design. It used a little under half a spool of PLA filament. I linked my preferred filament brand but use what ever your heart desires! | $24.99 | 1 | $24.99 |
| [1/2 Inch Syrofoam Insulation Board](https://www.homedepot.com/p/Owens-Corning-FOAMULAR-NGX-Insulating-Sheathing-0-5-in-x-4-ft-x-8-ft-SE-R-3-XPS-Rigid-Foam-Board-Insulation-13NGX/315193939) | Used this to make a shell to go on the outside of the chassis. This is to keep everything nice and toasty | $13.98 | 1 | $13.98 |
| [Aluminum Tape](https://www.amazon.com/Aluminum-Ductwork-Temperature-Insulation-Adhesive/dp/B0B6NDFXPN/ref=sr_1_1_sspa?crid=2KQ0791B7VWAN&dib=eyJ2IjoiMSJ9.qoB72_Xv0tnt9FYn50FUTB_aPiSOz7aGDVWXyx-n8zS8CxEhm9ADNIyRxfyVvt2jWKEc9kicXiJnetEIeOhh3i25cSlW5MXQeHsIiM-VjF0y2GNLxJ7ZQVqAdHNg2ThHGirgSJ4S1OsxYnv0OL9IBYNjj2Unrry39B140IH4WYdtKm_PWr9-_0ZQjgGIUq_wp6ZFFtZPepNoG1bxt1lPanR6Co7QtMME4ypx-65k-OMidTorrptURq-P3o-G4A6f4hTz1R9AfH5wetcMDI3YBD-F6kr7KhYy6EPouPtv7ww.m0KGwk77wexP74jHao_ggM25LK-TqXiJd1YMoxGALeY&dib_tag=se&keywords=aluminum+tape&qid=1712357223&s=industrial&sprefix=aluminum+tape%2Cindustrial%2C92&sr=1-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) | Used this to assemble and seal the styrofoam shell. Doesn't need to be airtight nor should it be (though a very slim chance, could de-pressurize and explode). | $5.99 | 1 | $5.99 |
| [Bubble Wrap](https://www.walmart.com/ip/Duck-Max-Strength-Large-Bubble-Cushioning-Wrap-12-in-x-100-ft-Clear-287223/46582773?from=/search) | Wrapped the entire payload in bubble wrap for extra cushion upon impact. I was a bit worried about the impact velocity so I did this for ease of mind | $19.48 | 1 | $19.48 |
| [Plastic Paper Sheet Protector](https://www.walmart.com/ip/Pen-Gear-Economy-Sheet-Protectors-20-Sheets-Clear-8-5-x-11-Model-25038/935038091?wmlspartner=wlpa&selectedSellerId=0&wl13=3812&adid=22222222277935038091_117755028669_12420145346&wmlspartner=wmtlabs&wl0=&wl1=g&wl2=c&wl3=501107745824&wl4=pla-306310554666&wl5=9014969&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=935038091&wl13=3812&veh=sem_LIA&gclsrc=aw.ds&&adid=22222222237935038091_117755028669_12420145346&wl0=&wl1=g&wl2=c&wl3=501107745824&wl4=pla-306310554666&wl5=9014969&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=935038091&veh=sem&gad_source=1&gclid=CjwKCAjwwr6wBhBcEiwAfMEQs5vTfMos0NGmbT7srX6LKU1PHkzfhLHhLxYaljKpT4zh0njB5jaiIBoC4yoQAvD_BwE) | This is for protecting the little placard I made with my contact information | $1.18 | 1 | $1.18 |
| [Packing Tape](https://www.walmart.com/ip/Scotch-Heavy-Duty-Shipping-Packing-Tape-Clear-1-88-in-x-25-6-yd-1-Tape-Roll-with-Dispenser/438482605?athbdg=L1600&from=/search) | Used this for attaching the bubble wrap to the styrofoam shell | $3.58 | 1 | $3.58 |
| [Handwarmers](https://www.walmart.com/ip/HotHands-Hand-Warmer-10-Pair-Value-Pack/17808715?wl13=3792&selectedSellerId=0) | Heating source for the payload electronics | $7.48 | 1 | $7.48 |
| [Parachute](https://the-rocketman.com/recovery-html/) | Used for slowing down the payload upon the descent. I suggest weighing your payload and adding on the weight of the balloon youre using then going to the rocketman website to see what parachute size is suitable. I went with a 3ft parachute but I think we should have went up to either a 4ft or a 5ft parachute to slow it down even quicker. | $45.50 | 1 | $45.50 |
| [Paracord](https://www.walmart.com/ip/Hyper-Tough-550-Utility-Paracord-Rope-Orange-5-32-inch-x-50-feet/459593415?athbdg=L1103&from=/search) | Used this for attaching the bubble wrap to the styrofoam shell | $4.84 | 1 | $4.84 |
| [Tape Measure](https://www.menards.com/main/tools/hand-tools/measuring-layout-tools/tape-measures-rulers/masterforce-reg-force-blade-trade-tape-measures/m52416/p-1460081133564-c-1549901401002.htm?exp=false) | Used for measuring the length of the paracord between the payload/parachute/balloon | $7.09 | 1 | $7.09 |

### Miscellaneous Equipment List
|Equipment|Description|Price|Amount|Total Cost|
| ------ | ------------ | ------ | ------ | ------ |
| [Scissors](https://www.walmart.com/ip/Scotch-8-inch-Multi-Purpose-Stainless-Steel-Scissors/19675467?athbdg=L1102&from=/search) | Used this for cutting bubble wrap and paracord | $3.62 | 1 | $3.62 |
| [Side Cutters](https://www.menards.com/main/tools/hand-tools/pliers-plier-sets/micro-flush-cutter/cwcmf/p-1444421187059-c-9156.htm?exp=false) | Used this for cutting paracord and zip ties | $3.55 | 1 | $3.55 |
| [Small Zip Ties](https://www.walmart.com/ip/Hyper-Tough-4inch-Cable-Tie-Uv-Black-100Pcs/440215826?athbdg=L1102&from=/search) | Used these for, albeit rough, cable management of thepayload electronics | $3.22 | 1 | $3.22 |
| [Electrical Tape](https://www.walmart.com/ip/Hyper-Tough-50ft-Vinyl-Electrical-Tape-3-4-0-43lbs-Black-3-Pack-34366/191038268?athbdg=L1200&from=/search) | Used this for wire insulation and wrapping the PVC pipe used in the filling valve | $2.13 | 1 | $2.13 |
| [Lighter](https://www.walmart.com/ip/Ozark-Trail-Multipurpose-BBQ-Lighter-Red-and-Black-2-Pack/1450295214?athbdg=L1200&from=/search) | Used for melting the cut ends of paracord so they don't keep fraying and coming | $2.24 | 1 | $2.24 |
| [Heat Shrink Tubing](https://www.amazon.com/Shrink-Tubing-Tubes-KOOWIN-Ratio/dp/B098LB9LTJ/ref=sr_1_10?crid=346FD9WVU6HIY&dib=eyJ2IjoiMSJ9.YJ1bboLvVVCkFM61KQIUSeMSD2aSt_4O-bJfWc_2C2IlabJK5-d6L-_fBrMVcN0vqCHDHba0gFlGqC6z8Mz1Kk-n8L5wIo4q40v37kCIqtQkPhc3HHXIJ-xwowvaTJHyK9xtSnyarm0YWPAFPg6dhxu6G5gok3H3FIKvK4Nk4PuSATWh-r8Wq9cwsXS3T6-c3K3CHf4jAaJhO7oSIX37rwgykglgEApqS9wht_lTJWg.cOjug4SmhsnHUJiLndIltYhLXVHMnmnL0enGERE53IE&dib_tag=se&keywords=small+heat+shrink+tubing+kit&qid=1712365154&sprefix=small+heat+shrink+tubing+kit%2Caps%2C89&sr=8-10) | The payload electronics are packed pretty tightly so I figured it would be best to insulate everything as neat as possible. This is the kit I used, but use whatever you feel like doing! | $7.99 | 1 | $7.99 |

## Payload Construction

When designing our HAB payload, the biggest fear that we needed to resolve was the absolute beating our payload electronics were expected to endure during a test flight. The jetstreams up above can get pretty nasty and put an absolute beating on our payload, so making sure that every component was a) soldered together and b) held down in some fashion was important to ensuring nothing would come loose and possibly short out the entire payload electronic system. Likewise, we wanted our payload to remain light (Under 2 pounds total, but preferably less) for safety reasons. So in conclusion, these three main considerations is what drove our design for what we are calling our payload chassis:

<img src=Reference-Photos/Payload-Chassis-CAD-Photo.PNG width='400'>

The chassis ended up having dimensions of 400x400x600mm (Base x Height x Width) and was 3D printed on my Ender 3 Pro printer using blach hatchbox PLA. Chamfers were placed in as many locations as possible in order to cut-down on support material and decrease the printing time. Likewise, 10% infill was used but with 4 walls in order to help with strength and rigidity. This chassis is also divided up into two seperate pieces: the shell itself and a little platform with legs that was used for holding the Arduino MKR WAN 1310 board while still allowing for wires to pass underneath and be soldered to its pins. For more images of the chassis CAD files as well as the Solidworks files themself, go to the [Payload CAD Files](Payload-Chassis-CAD-Folder) folder up above!

Though its hard to see, I tried my best to remove as much unecessary material as possible while also providing ample mounting locations for each and every one of our electronics. The little holes seen all around our payload chassis are locations to melt threaded brass inserts into which provide locations that each part can be screwed down into. Similarly, I also tried putting a bunch of slots all around the chassis so zip ties could be passed through in order to hold everything into place. Our final designed had the Arduino MKR WAN 1310, micro-SD card board, RTC board, pressure sensor, and small perf boards screwed down and the wires, battery cell, antenna, and handwarmer zip tied to the chassis. The only odd methods out for mounting components onto our payload chassis were the ESP32-CAM board and the GPS shield. The GPS shield was insertted onto the top of the Arduino MKR WAN 1310 board and then hot-glued down to keep it from coming apart or shifting during flight. And then the camera was insertted into its own clam-shell case that was epoxied into a dedicated space on the chassis. This [clamshell case](https://www.thingiverse.com/thing:4251986) was designed by the username dandesc on Thingiverse.com and I decided to adapt it to work with our project. I belived this was a quicker way to provide a dedicated space for the camera to mount to while also making replacing it in the future if needed easy as well. And then once the chassis parts were printed, post-processed, and fully assembled I was left with this:

<img src=Reference-Photos/Payload-Chassis-Assembled-Isometric-View-Photo.jpg width='300'> <img src=Reference-Photos/Payload-Chassis-Assembled-Top-View-Photo.jpg width='300'> <img src=Reference-Photos/Payload-Chassis-Assembled-Isometric-View2-Photo.jpg width='300'>

My mindset for going this route instead of just saying screw it and epoxying all the components down was due to my desire to make each component easily replaceable if need be. I figure at some point a sensor or component will be damaged upon impact, so I figured I would try my best to future proof our mounting methods for each component. Plust it helped me with debugging things when I was assembling it all together and needed to address a weak connection to a component.

Likewise, when desinging our HAB payload, we did extensive online research and found a variety of individuals who posted their own HAB payload designs. The one design decision that was seen in all the ones we found was the use of an outer-styrofoam shell. These shells ranged from using small, disposable styrofoam coolers to even floral arrangement foam that was hallowed out. We decided to go a slightly different route and use 1/2 inch rigid foam insulation board and create an outer shell of this material that would go around our 3D printed chassis. A picture of our final styrofoam shell can be seen below:

<img src=Reference-Photos/Payload-Chassis-Assembled-Styrofoam-Shell-Photo.jpg width='300'>

And then just to give us a bit more ease of mind with our payload, we decided to wrap it in several layers of bubble wrap in order to help cushion the impact force just incase our payload would happen to hit someone or their property.

<img src=Reference-Photos/Payload-Chassis-Assembled-Bubblewrap-Photo.jpg width='300'>

## Payload Arduino Code

Disclaimer, but this was my first big project that utilized Arduino and the coding language so I apologize upfront if my code looks terrible. I know it's not as optimized as it could be, but I made sure to comment it as best as possible so everyone can follow it. Below I'll try to explain what the code does:

Firstly, the [Payload Arduino Code](Payload-Arduino-Code-Folder) is broken up into two files: the main arduino code and the camera code. The main arduino code is what gets the current interior payload temperature, exterior ambient temperature, ambient pressure, GPS data (latitude and longitude), and date/time and both saves the data to the backup micro-SD card and transmit packets of data via the on-board LoRa WAN module in .csv format. I also included two versions of the main arduino code which either use 915 MHz or 433 MHz for transmitting its data packets depending on what you prefer. Furthermore, this code also communicates with the ESP32-CAM and tells it when to take a photo (every 30 seconds). So then on the flipside, the camera code waits until it receives a trigger from the main arduino board that tells it to take a photo. 

The reason for the odd way of integrating the ESP32-CAM to communicate with the Arduino MKR WAN 1310 board was simply due to my novice understanding of this coding langauge and overall frustration with getting this code to work. The original plan was to use an [Arducam OV2640](https://www.amazon.com/Arducam-Module-Megapixels-Arduino-Mega2560/dp/B012UXNDOY/ref=sr_1_3?crid=3GJIGC7TPARXU&dib=eyJ2IjoiMSJ9.EW1GBWQzMTIbqVOv0CR9rm3qqGrW8XQe_RoSSOjEzBpLQ4D9AQo2dj8wAA9dWsWB-FyuC9tDDttSQtKQphwVLqH-ymyEOg5_CW-nuMSxolgwSnCTV63tOCfYvJUuW-pX8UqHZ4wnIv1f7aBtI5GCTmCH_tJ5MZ33Yfvg2oHFrBJcPYf69FvzFY790dWlD9ezI_9AToX-K1lwbbPL6sUfI1THbdEHHZutEWokJyaQP5I.UsdTgHEMksha3RnfpVNCyuJNmWEgUMmAXhaMtUxK97Y&dib_tag=se&keywords=arducam+ov2640&qid=1712368759&sprefix=arducam+ov%2Caps%2C96&sr=8-3) since that can be easily integrated into the main arduino code. However, I ran into issues where the photos from the Arducam OV2640 were either blurry, green/magenta tinted, or just completely dark. I did extensive research over a week of trying to trouble shoot this issue and believe this issue was caused by the lack of on-board ram for processing and saving the photos to a micro-SD card. I'm sure there lays a solution that allows for this camera to work with the main arduino board for this application, however I'll be completely honest and admit that finding that solution is way over my head. I would love to hear if anyone else was able to get an Arducam OV2640 to work with an Arduino MKR WAN 1310 board and return decent photographs and learn how they did it.

My solution to this problem, however, was found when I stumbled upon the ESP32-CAM boards which are essentially arduino boards with the same camera as the Arducam OV2640 board along with ample ram and its own micro-SD card board that it can save to. I ended up setting up the ESP32-CAM to wait for a pulse sent to its GPIO 1 pin by the main arduino board. Once a pulse was received, then the ESP32-CAM knew to take a photo and save it to its own micro-SD card.

It should be noted that the main arduino code doesn't run correctly unless the following conditions are met:
  - It can find both the interior and exterior temperature sensors
  - It can find the pressure sensor
  - It has a GPS signal (it will stop transmitting/saving data if its GPS signal is lost and will pick back up once fixed)
  - A micro-SD card used for saving the backup data is insertted/found
  - It can find the Real Time Clock (RTC) Board
  - The LoRa WAN modue is found
    
Below is a flow chart to try and help you visualize how the code operates:

<img src=Reference-Photos/HAB-Payload-Arduino-Code-Flowchart.png width='500'>

And then to help you visualize roughly how each component was soldered together, I made the following schematic for our presentation and figure I would include it here. Just to make a small note that the schematic doesn't show, I ended up using two small pieces of perfboard to create a sort of 'hub' for each component that needed to share a specific wire to connect to. In other words, the components that needed a ground, 3.3V, Serial Clock Line (SCL), and Serial Data Line (SDA) wire were all connected to these perfboard 'hubs'. And the meaning for the second one was to handle the litte board that was necessary to convert the 3.3V from the Arduino board to 5V for the micro-SD board. During testing, I found that the Arduino couldn't recognize the micro-SD card nor board when everything was running off of 3.3V and when I tried using 5V on a whim everything worked flawlessly. Depending on what micro-SD board, this may not be an issue for you but if it is I suggest saving yourself a possible headache and try this to see if it works!

<img src=Reference-Photos/Payload-Electronics-Schematic.jpg width='600'>

Here is the list of each wire color shown in the schematic and what their purposes are:
1. Red: 3.3v
2. Black: Ground
3. Orange: 5v
4. White: Temperature Sensor Communication
5. Green: Serial Clock Line (SCL)
6. Yellow: Serial Data Line (SCA)
7. Purple: Micro-SD Card Board Communication
8. Pink: Master-In, Slave-Out (MISO)
9. Brown: Master-Out, Slave-In (MOSI)
10. Blue: Serial Clock (SCK)
11. Gray: Camera Communication
12. Cyan: Antenna

## Tracking Receiver Construction

The tracking receiver was constructed in a similar manner to the payload chassis described above. It was 3D printed and utilized threaded brass inserts in order to mount the necessary electronics for it to function. The receiver shouldn't experience any necessary forces (unless I drop it) but I just soldered them all together out of habit. Below is a picture of the CAD file:

<img src=Reference-Photos/Tracking-Reciever-Assembly-Isometric-View-Photo.PNG width='500'> 

And then here is a picture of the fully assembled tracking reciever. It can be hand-held like I did in the photo, but also attached to a laptop and used in tandem with a MATLAB script I also wrote. This way of using the tracking reciever is outlined more in the next section, but here is a photo of how the tracking reciever looked when fully assembled:

<img src=Reference-Photos/Tracking-Reciever-Fully-Assembled-Photo.jpg width='300'>

This tracking receiver had a kill switch on the side, a micro-SD card board for saving the packets of data just in case, and a coaxial connection out the top for an antenna to be connected to. For this project, I purchased a magentic antenna base that I attached attached to the top of my car so I could drive and track the balloon mid-flight:

<img src=Reference-Photos/Antenna-On-Car-Example.jpg width='400'>

I did zip-tie it to my luggage rack on the roof of my car just in case if it would've fallen off while driving, but it doesn't seem to have moved at all which was good!

## Tracking Receiver Arduino and MATLAB Code

Similarly to the payload code, the tracking receiver code works in two parts: another Arduino MKR WAN 1310 that is meant to intercept the packets being sent back to the ground by the HAB payload along with a MATLAB script that takes the packets of data and parses out the latitude and longitude coordinates in order to live-plot them on a geoplot.

The arduino code itself is pretty simple; it just waits until it receives a packt of data like mentioned above. And once one is received, it then parses out the data and sends it the MATLAB script as well as prints it to a mini OLED screen on the receiver itself. This way, it can also be used in a hand-held mode in conjunction to the MATLAB tracking code.

The [MATLAB code](Tracking-Reciever-MATLAB-Code-Folder) turned out to be simpler to create than I originally anticipated. It also takes in the packets of data, still in .csv format, and parses out each variable (interior temperature, exterior temperature, ambient pressure, latitude, longitude, altitude, and # of satellites). Then it specifically takes the latitude and longitude coordinates and plots them onto a geoplot. The plan for this was to have someone in the passenger seat of my car with a laptop and both the receiver code and MATLAB plotting code running. This way I could drive while they relay information was to where the balloon is drifting off to.

So to talk about how this worked in practice, I can sum it up in one simple word: terribly. After we launched the balloon and packed everyting back up in my car, we immediately drove after it with the receiver running on my computer and the antenna on top of my car. It worked for a bit, but once we got onto the highway we didn't get another packet of data. We kept driving hoping we would get more as we continued in the direction it was heading, but nothing. After we went a while without another packet, we decided to drive out where we estimated it to land in order to get a signal when it was coming back to Earth, but we still got nothing. And after driving around for 7 hours with no new packets of data, we called it quits and considered it lost.

## Launch Protocol

Never having launched a high-altiude balloon before, it honestly wasn't as bad as I initially expected. There are plenty of videos out there, the two i found most informative was one by [StratoStar](https://www.youtube.com/watch?v=5Z23L4QIgtQ&ab_channel=StratoStar-JasonKrueger) that walked you through the basics of filling the balloon and another one by [John Powell](https://www.youtube.com/watch?v=B7XEUJSekbI&ab_channel=JohnPowell) that walked you through rigging our payload to the balloon. Both of these videos had some really good, basic points that made filling the balloon seem very simple to a novice such as myself. Though I urge you to do your own research until you feel comfortable; there are plenty of amazing, informative videos on this topic and you can really get a good idea of what you're doing without ever attempting something like this beforehand.

So with those informative videos mentioned, I feel that it is helpful to talk about our criteria for what a good day to launched looked like. These were our parameters that we were on the look for in terms of weather:
  - A ground temperature above 40°F
  - Daylight (between sunrise and sunset)
  - Max windspeed of 15 mph
  - Visibility of 5-10 nautical miles
  - Little to no precipitation/Incliment Weather
  - Little to no Cloud Cover (Important to us since we included a camera with our payload)

Now you all can decide what conditions you would prefer to launch in; such as wanting less wind, not worrying about ground temperature as much, and not caring for cloud cover if youre not including a camera on-board. The sky's the limit (pun intended) for your launch, just make sure you are being safe about it and not launching the balloon at night with no light-strobe included

Then, after watching several videos on high-altitude balloon launches and coming up with our list of criteria for optimal launch conditions, we devised a set of instructions for us to follow in order to have a quick, accident free launch:
  1. Prior to launch, check the weather forcast daily the week leading up to the intended launch date
  2. ensure the payload, parachute, and paracord rigging lines are completely assembled beforehand to make it simple to attach to the balloon
  3. Drive to launching location. Ensure that the helium bottle is properly secured in the vehcile in order to prevent damage to the bottle
  4. Lay tarp flat on the ground and put bottle and other heavy objects in the corners to prevent it from lifting up
  5. Attach the pressure regulator to the bottle with wrenches. Make sure to put teflon tape on the threads to prevent leaking. Also do not open the helium bottle yet
  6. Attach the quick disconnect hose to the pressure regulator along with the ball-valve filling apapratus
  7. Set out rest of supplies needed for filling the balloon so you don't have to go searching for them. These include the fish scale, zip ties, payload with rigging and parachute attached, electrical tape, and either scisors or side cutters
  8. Put on nitrile/latex gloves. These are needed by everyone who may need to touch the balloon
  9. Open the bag that the weather balloon came in and carefully open it and lay it flat onto the tarp. Make sure not to step on it at all
  10. Insert ball-valve filling apparatus into the neck of the balloon carefully in order to prevent tearing of the latex
  11. Attach fish scale to both the helium bottle and the neck of the balloon via paracord and zipties. This will be used to measure buoyance force
  12. Once the ball-valve filling apparatus and fish scale/tether is attached to both the helium bottle and balloon, ensure that the pressure regulator valve is fully closed and then fully open the helium bottle valve
  13. Begin filling the balloon by slowly openning the pressure regulator valve. Once the balloon is off the ground and above the bottle, you may then increase the fill rate
  14. While the balloon is filling, feel free to turn on the payload electronics and the tracking code and get those running.
  15. Once the payload is verified to be on and operating, tape shut the lid for the styrofoam shell and attach the bubble wrap onto the outside of the styrofoam shell
  16. Once the balloon is either filled to the desired buoyancy force or the helium bottle is fully depleted, close the pressure regulator valve.
  17. Attach the parachute and payload to the balloon via paracord and zipties. Its good practice to first twist the neck of the balloon and and then zip tying the payload and parachute to where its twisted. This will help prevent helium leaking.
  18. Remove the ball-valve filing appartus but leave in the PVC fitting. This will help prevent the zip tie from slipping off.
  19. Fold the rest of the neck of the balloon up and zip tie into place, this again is to prevent helium from leaking out and from keeeping the payload from slipping off the balloon mid-flight
  20. Once the payload is fully attached, ball-valve filling apparatus is removed, and the payload electronics and tracking receiver are confirmed to be working correctly, cut the tether line holding the balooon to the helium bottle and watch it drift
  21. Pack up everything into your vehicle and get the tracking receiver and MATLAB code running on a laptop running in the passenger seat. Passenger optional depending on your skill level
  22. Track the balloon and hopefully recover it.
  23. PROFIT!

## Launch Experience and Data Results

We launched our balloon a little bit south west of Columbus, OH at a AERO RC Club on April 9th, 2024. Launching it went really well in my opinion and only took an hour from parking my car to launching the balloon and packing everything up with the help of our faculty advisor. Here are some of the photos we took of our launch:

<img src=Launch-Photos-And-Video/Launch-Photo-2.jpg width='500'>
<img src=Launch-Photos-And-Video/Launch-Photo-4.jpg width='500'>

And so, with a ton of zip ties, electrical tape, paracord, and 110 cubic feet of helium, we were able to get a successful launch:

https://github.com/BLawrence426/High-Altitude-Balloon-Senior-Capstone-Project/assets/157772655/674f84ac-60b6-4e0f-a145-dcf7288c7ae6

Following this video being taken and a couple of more minutes of watching it drift away, we loaded up everything into my car and myself and a member of my team started driving after it. Like I mentioned in the previous section about our tracking receiver, it honestly failed as soon as we got out of Columbus. We immediately jumped onto I-70 which went straight east out of Columbus since that was the direction we were predicting the balloon to drift. We were getting packets of data right when the balloon lifted off and for a short period of driving, but shortly after we got onto the highway we lost contact with the balloon and were now in search and rescue mode. We ended up driving to east near Barnesville, OH, which is roughly where our balloon was predicited to land:

<img src=Reference-Photos/Estimated-Landing-Area-Screenshot-2.png width='800'>

We got there roughly when it was supposed to land (about 1.5 hours following lift-off) and were scanning the local sky and checking our tracking receiver. After spending about 30 minutes in a remote, hilly farmland we decided to call it quits in that location and started to drive around hoping for a signal to make it to us. We spent an hour driving around a bunch of gravel backroads up and down the hilly terrain of eastern Ohio and came up with nothing. We decided to call it quits looking around this area and took county roads back to Columbus that more closely followed the estimated fightpath in-case the balloon happened to of prematurely burst. In all, we spent 7 hours directly after launching the balloon trying to located it before its battery died but came up short. At this point, we called it a loss which was dissapointing but we knew the rist we were taking.

But this isn't where the story ends!

I happened to purchase and put an Apple Air Tag in with the payload before launch on a whim. My mindset was that if we got close enough to the payload while trying to find it, I figured I could use my phone to play a sound in order to locate it quicker if it was stuck inside a bush or something. I knew that these Air Tags aren't worth their salt when it comes to trying to find something in the middle of nowhere due to them needing to connect to an apple device in order to project its location on the "Find My" App created by Apple. And after returning to The Ohio State's campus and taking a short nap (sorry, I was exhausted) I happened to check the app and saw it actually updated its location!

<img src=Reference-Photos/Air-Tag-Location-Screenshot.jpg width='400'>

It moved!! I was so excited and my hope for trying to find it was off the charts. It followed the predicted path really close, but ended up drifting about 20 more miles eastward than we intended which would explain why we weren't able to either see it descent nor receive a packet of data. And thankfully, it appeared to land in the backyard of a couple of condos that shared a walking path in their backyard which would explain why it was able to connect to an Iphone relatively soon after it landed back onto the ground. So following this new information, I devised a plan to drive back out to eastern Ohio in order to find and recover it without needing to tresspass on anyones property. So following my Flght Testing course, I set off for my 2-hour journy to Saint Clairsville which the Air Tag was showing it was located at.

When I finally arrived I did a quick loop on the road that overlooked the location it was showing to be and I swore I saw the yellow parachute on a bench near the walking path it was located. So then I eventually turned around in someones driveway and went down the road that the duplexes with the backyards in which our balloon payload was located in. I was extremely lucky that there were a couple of landscapers out tending to the grass and mulch beds that were kind enough to help me try and find it (I hope they enjoyed a beer or two on me from the card I left on their truck!). Myself and the first landscaper I talked to had a nice conversation while walking towards a bench alongside the walking path the Air Tag was showing its location at and, sure enough, someone happened to come across the payload and leave it on the bench. Though a bit wet from a thunderstorm that came through the previous night, the payload was fully intact except for the placard I put on the outside of the payload with my contact information which would explain why I never got a call or text despite someone comming across it:

<img src=Reference-Photos/HAB-Info-Placard.png width='500'>

After thanking and saying goodbye to the landscaper that helped me find the payload, I first went to McDonalds to get something to eat (since I was too focused on finding the payload and didn't want to stop) and then went into a nearby parking lot in order to get the micro-SD cards out of the payload and check the data. Thankfully, everything seemed to be intact which was amazing! I gave it a once-over just to see what kind of results we got and then drove back to campus so I could look at all the data more closely.

Upon further inspection, I noticed several things. Firstly, the balloon remained on until 3am on 4/10/24 which, if we were searching closer to this area, leads me to believe that we would've been able to find it via the radio. Secondly, there appeared to be about an hour gap in the data due to the GPS not connecting at the extremely high altitude (about 70-80,000 feet). In hindsight, it was stupid to code the payload to only record data if it had a GPS signal since it lead to a gap of data but oh well. This was only something I could've figured out with more experience in these components. But regardless of this issue, we still got some interesting data! The coolest of which is the coordinates the paylaod logged:

<img src=MATLAB-Plots/MATLAB-Plot-of-Entire-Flight-Path-Zoomed-In.png width='1000'>

So overall, the website we used to estimate the flight path was pretty close to the one the balloon actually followed which is incredible. Likewise, the balloon appeared to have preformed even better than intended since it drived about 20 miles further east than we expected. This leads us to believe that the balloon reached an altitude closer to 110,000 ft instead of the anticipated 100,000 ft. And speaking of which, these following two plots of both the GPS altitude and the pressure corresponding to the recorded pressures were created:

<img src=MATLAB-Plots/GPS-Altitude-Plot.png width='505'> <img src=MATLAB-Plots/Pressure-Altitude-Plot.png width='494'>

The GPS altitude is definitely way, wayyyyy off from what was actually recorded. Honestly I don't know why since I would've assumed it would've been able to figure this out by the triangulation of the satellites it connected to but it apparently didn't. But thankfully, when looking at the recorded pressures and comparing that to standard atmosphere pressure tables, our altitude appears to be much more realistic and what we expected! The GPS altitude stated we only got to an altitude of 3,688.38 ft while the pressure tables returned a max altitude of 91,870.25 ft. We do believe the balloon went a bit higher than this altitude given the distance it drifted and the GPS cutting out at too high of an altitude.

Likewise, we also collected both the interior and exterior temperature for plotting:

<img src=MATLAB-Plots/Interior-Temperature-Plot.png width='500'> <img src=MATLAB-Plots/Exterior-Temperature-Plot.png width='500'>

The interior temperature returned values that we expected on account of the handwarmer keeping the internal temperature of the payload within acceptable temperatures throughout the entire flight. Likewise, the interior temperature slowly decreased throughout the flight which makes sense due to the ambient temperature outside getting colder and colder which would've seeped into the inside of the payload. Furthermore, the exterior temperature appeared to be kind of skewed due to the handwarmer used to keep the payload electronics warm. We had to tuck our exterior temperature sensor between the styrofoam shell and outer bubblewrap layer which I believe caused the exterior temperature sensor to become warmed up by the handwarmer slowly warming the styrofoam shell. Despite this, we still saw a steep decrease in ambient temperature (minimum of 13.66°F) which was much, much colder than the ground temperature when we launched!

And then finally, we also collected ambient pressure during the entire flight. We tried to do a small halo of tubing that connected to the pressure sensor in order to get a pressure reading at the top, side, and bottom of our payload in order to get an averaged pressure reading:

<img src=MATLAB-Plots/Pressure-Altitude-Plot.png width='500'>

The pressure data, despite the gap in data, shown the exact same trend that we were expecting from our simulations. It had a nice sort of parabolic shape to it and ended up having a minimum pressure of 0.23 psi (1585.79 Pascals) whichw as similar to what we were expecting before we launched which was great!

Overall, we got a bunch of cool data from our launch and I am incredibly thankful that the Air Tag was able to save our project/payload from being completely lost. All the data collected lined up rather similarly to what we expected which was amazing for our hypothesis that we came up with at the beginning of the school year. I'm a bit dissapointed that the way our balloon payload was coded caused it to stop collecting data at high altitudes due to the GPS not getting a signal from any satellites, but I am happy that it worked for the most part! If I were to use this payload again as is, I would stick to a smaller balloon like a 100 or 200 gram balloon that would reach a slighly shorter altitude (about 30,000-40,000 ft) in order to prevent the GPS from disconnecting. And with proper changes to the code, I would run the payload again as is in order for it to collect data despite not having a GPS signal.

## Flight Test Data Comparison to Simulation Code

And just because I had to do it for our presentation and final report for the class this project was for, here are the plots that I created that compares the data collected from our Flight Test to the code I wrote that simulated our weather balloon launching. The code itself is located in the [MATLAB Comparison](MATLAB-Comparison-Plots-And-Code) folder! The first one is temperature:

<img src=MATLAB-Comparison-Plots-And-Code/Temperature-Comparison-Plot.png width='500'>

As can be seen in this plot, the flight test data is definitely skewed due to the handwarmer warming up the styrofoam enough to cause the exterior temperature sensor to also become warm. The neat thing to note, however, is that the exterior temperature sensor appeared to follow the same trend as the simulation code, which is based off of the standard atmosphere model. The Flight Test temperature data shows an initial clear, steep decrease in temperature followed by it seemingly staying constant/slightly increasing. This was then followed by the temperature decreasing for a little while before it drastically increasing as the balloon fell closer and closer back to the Earth's surface.

Likewise, here is the plot for pressure:

<img src=MATLAB-Comparison-Plots-And-Code/Pressure-Comparison-Plot.png width='500'>

The flight test data for pressure honestly couldn't have been more close to be completely honest. It showed the exact same trend to the simulated results with only the pressure increasing sooner in the Flight Test data. This could be due to the descent rate being much faster than what was calculated in the simulated data and shouldn't really be looked too deep into. The important thing is just how they showed the same trend and honestly ended up with very similar minimum pressures which is amazing!

And then finally, here is a plot for altitude. Please note that this plot was created using the pressure altitude calculated via the pressure collected during the Flight Test:

<img src=MATLAB-Comparison-Plots-And-Code/Altitude-Comparison-Plot.png width='500'>

Again, the flight test data for altitude was almost spot on like with pressure. The only differences are again, the altitude decreasing quicker than the simulation again (for similar reasons just stated previously) along with the flight test data appearing to show that the balloon reached a maximum altitude of about 20,000 ft. The biggest reason for this difference is that the code assumes that the balloon reaches the user-input target altitude, which in this case was 110,000 ft like the manufacturer specified. I'm sure with more tinkering that the simulation would match up closer to the data collected here, however I do think that the gap in data caused by the loss of GPS connection to also be to blame for the altitude collected from the flight test data. If it collected temperature and pressure for the entire flight, I think we could have closer curve to the flght test data!

## Photo Results

We got TONS of photos from our payload which I included all of them in the [Raw Payload Photos](Raw-Payload-Photos) folder. However, to save you time while also showing the coolest/funniest photos we got from the payload, I picked out a couple photos from the hundreds we got. Here are some of them along with a small description:

#### First photo following launch

<img src=Cool-Photos-From-Test-Flight/First-Photo.jpg width='500'>

#### First photo above the clouds (Ascent)

<img src=Cool-Photos-From-Test-Flight/Above-The-Clouds.jpg width='500'>

#### Cool photo of the clouds (Ascent)

<img src=Cool-Photos-From-Test-Flight/Cool-Cloud-Photo.jpg width='500'>

#### Balloon swinging during descent photo

<img src=Cool-Photos-From-Test-Flight/Balloon-Action-Shot.jpg width='500'>

#### First photo after landing

<img src=Cool-Photos-From-Test-Flight/Landed-Photo.jpg width='500'>

#### Earth curvature photo 1

<img src=Cool-Photos-From-Test-Flight/Curvature-Photo-1.jpg width='500'>

#### Earth curvature photo 2

<img src=Cool-Photos-From-Test-Flight/Curvature-Photo-2.jpg width='500'>

#### Earth curvature photo 3

<img src=Cool-Photos-From-Test-Flight/Curvature-Photo-3.jpg width='500'>

#### Cool Descent photos

<img src=Cool-Photos-From-Test-Flight/Descent-Photo-1.jpg width='300'> <img src=Cool-Photos-From-Test-Flight/Descent-Photo-2.jpg width='300'> <img src=Cool-Photos-From-Test-Flight/Descent-Photo-3.jpg width='300'>

#### Visitor photos

<img src=Cool-Photos-From-Test-Flight/Visitor-Photo-1.jpg width='300'> <img src=Cool-Photos-From-Test-Flight/Visitor-Photo-2.jpg width='300'> <img src=Cool-Photos-From-Test-Flight/Visitor-Photo-3.jpg width='300'>

## Suggestions for Improvements

From the start of this project, I was in close conversations with our faculty advisor who has much, much more experience with unmanned aircraft and the like. He originally showed us [APRS.fi](https://aprs.fi/#!lat=40.0&lng=-83), a nifty website that shows all the radio towers and devices that are using frequencies allowed to those who have an amateur radio license (144. 390 MHz). Due to myself or anyone in my group not in possession of this license, we were going to have to use our faculty advisor's with our original plan for our project. However, after some digging online, I discovered the Arduino MKR WAN 1310 board that had a radio module built right into the board itself. This struck me as really cool and decided to use this board without giving it much thought that it could only output on either 915 MHz, 868 MHz, or 433 MHz. So using this specific arduino board to handle transmitting our data packets, we were no longer going to be able to use the APRS.fi website which sucked since it seemed amazing for tracking devices such as a high-altitude balloon.

<img src=Reference-Photos/APRS-Radio-Module-Idea-Photo.jpg width='300'>

#### Example of an APRS.fi-compatable radio module (Sadly, I don't think its produced anymore)

That's where the tracking reciever came into fruition since my idea would be to have someone (me) go driving after it in a vehicle along its entire flight path instead of letting it go and waiting for it to start showing up on the APRS.fi website. I say all of this to lead up my suggestion: I reccommend trying to change the way data is being transmitted with our original design. I think axing the Arduino MKR WAN 1310 board idea and instead going with something like an Arduino nano or an Arduino MKR ZERO and either finding or building a dedicated radio module that can use amateur radio license frequencies would be a better idea. I found a couple online, but most seemed very above my skill level and out of our given budget of $600. However, there are plenty of ideas and attemps that I found online by just doing a simple [google search](https://www.google.com/search?sca_esv=40ca4543c95fb865&sca_upv=1&rlz=1C1CHBF_enUS918US918&sxsrf=ACQVn09Dc6VqZY1E6mfl7baIcSKXs-xfVg:1712535318372&q=Ham+radio+based+arduino&source=lnms&uds=AMwkrPuJnGNBYDFIr0HhqiRaoUAuVyLZAJhHT3ZsNbJrpRz7MLUljWALRMXfnOPpDrRCpQ7cUbILcy7dLthDrSIvOlRcOcMkZ18scEchMC_6upBJYJjlUOKxca2WjmFp8aZxq9JclDtMlKGINlS7n5RYSM2Lk7Ev3UVEBotyd7-SOWywPPpeT-TqfOVIRkUXAxziJuqCmve2gWQAkwulVwZmAnvBQDtjfcsUMxUOVqNfAF13veJD4LWpxjZnOXr6nVV7raN_1qJYJCyatA-4VfkTVjvTlogaycbvADAhc028YjWL6XVgakGo3_frnZIFJgNMElDDWHDC&sa=X&ved=2ahUKEwivlsSaq7GFAxUZl4kEHeAxAw8Q0pQJegQIERAB&biw=1920&bih=911&dpr=1) that seemed really promising. Overall, I would like to keep tinkering with this once I graduate in my free time since I really enjoyed this project and would like to keep messing with this stuff well into the future. I intend on getting my amateur radio operator license once I have some more free time in order to try and expand this project into a very robust product.

A second suggestion that I would like to see with this project is honestly replacing the need for a proprietary Arduino board and instead design and print a new micro-controller board that can take its place. My thought process for this is that it would help do the following things:
  - Cut weight by being more compact
  - Cheaper in the long run
  - Marketable to individuals or companies that deal with HAB's

So I think by making an all-in-one micro-controller meant for this project specifically, we could end up with a simple board that would just need temperature sensors, a pressure sensor, and a radio module to be plugged into it for it to operate as intendended. My idea would be that a micro-controller specifically designed for this project would already have both a GPS and RTC board on it already since these would need to be present regardless while still giving the end-user the ability to choose their own preferred sensors and radio module. Another idea would maybe making two versions of this board: a 'lite' one if you will that wouldn't support a camera and be slightly cheaper than the second 'more expensive' board that would be capabale of supporting a camera similar to the Arducam OV2640 camera I mentiond prior in this page. The bigest different between these two boards would be its included RAM since I still believe that is why I couldn't get the Arducam OV2640 to work originally and needed to switch to the ESP32-CAM board.

Doing some research for this idea lead me to [OpenTrack](https://protofusion.org/wiki/opentrack), which appeared to do pretty much everything that I have talked about. It addresses my idea of changing the radio and still has a bunch of moduality for temperature sensors, pressure sensors, and more to be added. Overall, this is something that I wish I would have looked more into at the beginning of this project.

I'm sure there are plenty of other suggestions to change with this project, however these are the two that I immediately thought of when trying to do some reflecting at the end of the project. If you have any ideas or suggestions, or even already changed things on your end, I would love to hear about it! I am a big novice when it comes to this sort of stuff so I know plenty of people could improve immensely on the work laid out here.

## Final Thoughts and Reflection

Overall, I had a crazy ammount of fun and enjoyment while tackling this project. I have always had an interest in 3D printing, tinkering with electronics, and CAD just to name a few of the applicable ones to this project, so I was happy to finally find that same enjoyment in my coursework. This project taught me a lot about researching and formulating design ideas in order to refine and bounce off of others throughout the course of the semester. Likewise, I was able to gain some vital skills in project and time management, groupwork, presentation and public speaking, and document coordination. Honestly, I know it may not look super fancy, but I am so proud from turning this:

<img src=Reference-Photos/Prototype-Arduino-Board.jpg width='300'>

Into this:

<img src=Reference-Photos/Payload-Chassis-Assembled-Top-View-Photo.jpg width='300'>

Just going from my little breadboard prototyping station to a fully assembled chassis was so rewarding and honestly made me absolutely giddy with excitement. And not to mention it worked first try after I finished putting it all together which was even better.

Likewise, I have never launched a balloon before and had to learn all the steps to do so. It was honestly easier than I expected which was a nice surprise. It all boiled down to making sure all the rigging was correct and connected properly, along with making sure I didn't touch the damn balloon with my greasy hands. I always expected launching these balloons to be insanely difficult, but it really just involved paracord, zip ties, and electrical tape. Plus it was so relieving watching our HAB launch and drift away; almost like all of my troubles and frustrations went away with it since I knew with the launch was coming my graduation that has taken every bit of effort out of me.

I have a lot of things that, if I had more time and money, I would have loved to spend more time on and improve (mainly the things I outlined above in suggestions). I poured my heart and soul into this project; not to mention my own money since I stretched the budget given to us from the course as thin as possible and needed a bit more (for things such as helium, antennas, PLA filament, wire, solder, and more). My hope is that this project is offered again in the future for new engineering students to try their hand at it. I would hope they look at this project as a baseline and improve upon it in anyway that they seem fit. My major suggestions for that is don't buy 4 balloons, you will be lucky to get 2, if not only 1 launch. And seriously, try and get everything done as soon as you and your group can. Don't wait until the day of to do the papers, reports, and presentations because it really gets tiring after a while; not to mention you will not want to see this project by the end of your senior year.

## Useful Online Tools

The first useful online tool that I used for this project was this [Balloon Perfomance Calculator](https://www.highaltitudescience.com/pages/balloon-performance-calculator) created by High Altitude Science. I liked using this for seeing how much helium I needed for my balloon-payload setup and ensuring that it would reach my target altitude.

The second useful online tool I used for this project was this [Flight Path Predictor](https://predict.sondehub.org/?launch_datetime=2024-04-09T08%3A00%3A00Z&launch_latitude=39.795&launch_longitude=276.93600000000004&launch_altitude=275&ascent_rate=5.22&profile=standard_profile&prediction_type=single&burst_altitude=30000&descent_rate=6) created by Jon Sowman, Adam Grieg, and Daniel Richman of Cambridge University. I found this tool later into this project and honestly it was really cool to use and predict where we would expect our balloon to drift to depending on the day/time/weather/altitude. [PUT IN BLURB ABOUT HOW THIS WAS COMPARED TO OUR ACTUAL FLIGHT TEST].

## Resources and Inspirations

The biggest inspriation that I found that helped immensely with conducting this project and designing this github for other interested individuals and future engineering students was a fellow individual by the name of [blazerunner44](https://github.com/blazerunner44/Arduino-High-Altitude-Balloon?tab=readme-ov-file#user-content-balloon-payload). I loved the work he did and I tried my best to emulate his project despite my more limited skillset.

Likewise, the hundreds of other online blogs, papers, and individuals I approached for help on this project gave so many great ideas that we encorporated into our own project. We gained a great understanding of what we needed to consider in our design as well as came up with ideas to incorporate into our own project that we initially didn't think of.

## Acknowledgements

- A big thank you to our faculty advisor for this project. I figure I won't put his name on here out of respect for his privacy, but he knows who he is and he was an incredible resource to turn to. I hope he conducts this project or something similar in the future, I think future students in the department would really enjoy what he would offer!
- Thank you to our course advisor for this project. Again, I won't put his name on here also out of respect for his privacy, but he was very helpful for keeping us on track and teaching us vital skills required to join the workforce in the near future
- Thank you to our GTA, he was super nice and helpful when we approached him with questions
- Thank you to BALLOONIACS, if it wasn't for their company we wouldn't have been able to get helium in time and get our project off the ground (literally)!
- Thank you to my parents, grandparents, sister, and girlfriend. You all were a major source of support throughout my education as you very well know and I greatly appreciate it. I wouldn't have been able to make it to where I am today without you all.
- Monster Energy Drinks and Redbull. Caffeine is the only thing that got me through all my coursework. I spent plenty of all nighters getting this project to where it ended up and it was because of these bad boys I was able to do so.
  
