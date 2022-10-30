# CIS 527 - Programming Assignment #1

Partners: Spencer White WHITSPEN and Dan Adenikinju DANIEAD

### Commands Implemented
ADD, DELETE, LIST, QUIT, and SHUTDOWN

### Instructions
IMPORTANT - Update PATH variable (line 78) to path to current working directory
Compile - ```make```
Run Server - ```./server```
Run Client - ```./client 127.0.0.1```
### Problems and Bugs
1. We did not deal with messages longer than the MAX_LINE=256 value
### Sample Outputs

LSIT
ECHO:300 Invalid Command

LIST
ECHO:200 OK
The list of records in the book: 
1000 Dan ade 313-221-3342
1002 Spencer White 3131112222

ADD Spencer White 3135558888
ECHO:200 OK
The new Record ID is 1003

ADD Spencer White 3134446666 BAD 
ECHO:301 Message Format Error

DELETE 1009
ECHO:403 The Record ID does not exist

DELETE 1002
ECHO:200 OK

SHUTDOWN
ECHO:200 OK

QUIT
ECHO:200 OK

## Tests

Test | Description | Input | Output | Pass/Fail
--- | --- | --- | --- |--- 
Read File | XXX | XX | XXX | PASS
Add Address to Address Structure | XXX | XX | XXX | PASS
Validation for ADD, DELETE, LIST, SHUTDOWN, QUIT | XXX | XX | XXX | PASS
Parse Add Command and Add to Address Structure | XXX | XX | XXX | PASS
Return 200 OK Response for ADD | XXX | XX | XXX | PASS
Parse Delete Command and Remove from Address Structure | XXX | XX | XXX | PASS
Parse Delete and Return "403 The Record ID Does Not Exist" | XXX | XX | XXX | PASS
Parse LIST and Return Single String Containing (200 OK, Text, N Entries) | XXX | XX | XXX | PASS
Parse LIST and Return "404 No Records Exist"  | XXX | XX | XXX | PASS
Parse SHUTDOWN and Return "200 OK" | XXX | XX | XXX | PASS
Parse SHUTDOWN and Close Client and Server Connection  | XXX | XX | XXX | PASS
Parse QUIT and Return "200 OK" | XXX | XX | XXX | PASS
Parse QUIT and Close Client Connection | XXX | XX | XXX | PASS
Consider Extremely Long List Response | XXX | XX | XXX | XXX
General Validation - First Name Max 8 Characters  | XXX | XX | XXX | PASS
General Validation - Last Name Max 8 Characters  | XXX | XX | XXX | PASS
General Validation - Phone Number Exactly 12 Characters  | XXX | XX | XXX | PASS
General Validation - Record ID is Exactly 4 Characters | XXX | XX | XXX | PASS
General Validation - LIST is Alone as Command | XXX | XX | XXX | PASS
General Validation - SHUTDOWN is Alone as Command | XXX | XX | XXX | PASS
General Validation - QUIT is Alone as Command | XXX | XX | XXX | PASS
Server Client Flow - Enable Back to Back Requests | XXX | XX | XXX | XXX
Develop Functions Where Appropriate | XXX | XX | XXX | XXX


## Notes
- To obtain IP address of device, run ifconfig, and look at inet value for en0 connection, use that value when spinning up client