# PalcomLoRaPhone
<h1><b>This Project is confidential and only Authorized persons are to view this repository.</b></h1>
<code>version 1.0.99 Gamma</code>

<h2>Change Log</h2>
<ol>
<li>Major changes to entire system archetecture</li>
<li>Phone currently can only send messages using the serial interface</li>
<li>Uses updated RadioLib library; implemented as the LoRaSnake class, library refinement</li>
<li>Device initaliziation successfully implemented.</li>
<li>SPI Bugs fixed. Core slave devices seperated by CPU Core</li>
<li>ROS Tasks implemented</li>
<li>Major Code Refactor</li>
<li>Added new tile system</li>
<li>updated all code libraries in use</li>
<li>added secret picture of my penis</li>
<li>added gitignore</li>
<li>Fixed bug that prevented TFT and RADIOLIB libraries from working togeather.</li>
<li>Implemented simulated transiver mode for the lora chip. lora send/recv now wroks more reliably.</li>
<li>Created first usage of the SD storage tasks.</li>
<li>Extended the PalcomObject class to use the PalcomStyle class. Makes styleing less complex.</li>
<li>Added PalcomLine Widged.</li>
<li>Added class for drawing triangles</li>
<li>Completed the mode selection screen. Remvoed triangles</li>
<li>Implemented bassic mesh mode message storage.</li>
<li>Implemented stored message retreival and implemented a system for orienting displayable messages.</li>
</ol>

<h3>TODO:</h3>
<hr/>
<ul>
<li>Use available display_messages to populate message boxes.</li>
<li>Design logic for when there is and is not a SD card</li>
<li>Rafactor code to use proper #include syntax</li>
<li>Move graphics, comms, and storage task handlers into concurrent callbacks</li>
<li>Impleent the settings and about GUI screens.</li>
</ul>
