433MHz radio libraries for Raspberry PI

Libraries are created by Randy Simons http://randysimons.nl/ for Arduino.
License: GPLv3. See license.txt

Libraries are modified by Frank Joosten https://frankjoosten.nl for Raspberry PI

Currently only A Type of KaKu (klik aan klik uit) are working.
These are the type where a receiver can learn and store the code of the sender.

rpi433send 
For sending code to receivers

rpi433receive
For receiving codes from senders (very critical)
To use this successful, the PI must be 100% idle

rpi433d
Daemon running in the background
For sending codes to receivers, instructed by rpi433queue

rpi433queue
Tool for instructing the daemon, to send codes

In case the rpi433send is too slow for your application, you can use the rpi433d with rpi433queue.


