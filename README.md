# LoRaNode_Water_Level

**Version 2.0.2.4.6<br>**
Features<br>
1.modify send message format of water_level to LPP format for cayenne analog input.<br>
2.Modify the initialization process to fit IAR version(V8.30.1).<br><br>



**Creat repository for water_level version 2.0.2.4.5**<br>
Development environment:**IAR 7.2** （Do not use the advanced version of the IAR,because some macro definitions are not supported in the advanced version）.<br>

This project relies on RAK811 module.<br>

To compile firmware for different band, you only need to define the corresponding macro in the global option before compilation. e.g REGION_EU868/REGION_US915/REGION_AS923/REGION_AU915.


