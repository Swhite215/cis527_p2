# CIS 527 - Programming Assignment #2

Partners: Spencer White WHITSPEN and Dan Adenikinju DANIEAD

### Commands Implemented
ADD, DELETE, LIST, QUIT, and SHUTDOWN

### Instructions
IMPORTANT - Update PATH variable (line 38) to path to current working directory
Compile - ```make```
Run Server - ```./multiThreadServer```
Run Client - ```./client 127.0.0.1```

### Problems and Bugs
1. We did not deal with messages longer than the MAX_LINE=256 value

### Sample Outputs

LOGIN root root05
200 OK

WHO
200 OK
The list of the active users: 
root    127.0.0.1

LOOK 1 Spencer
404 Your search did not match any records

LOOK 2 White
200 OK
Found: 1 match 
Santanna White 313-333-8966

LOGOUT
200 OK

SHUTDOWN
ECHO:401 You are not currently logged in, login first 

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

## Tests PA #2
Test | Description | Input | Output | Pass/Fail
--- | --- | --- | --- |--- 
Create Username and Password File - PASS
Read Username and Password File to Initialize User Validation Structure - PASS
Validation for LOGIN - PASS
Username and Password Validation - PASS
Return 200 OK Response for LOGIN - PASS
Return 410 Response for LOGIN - PASS
Log User Into System - Update User Status to TRUE in User Validation Structure - PASS
Validation for LOGOUT - PASS
Log User Out of System - Update User Status to FALSE in User Validation Structure - PASS
Validation for Commands Executable by Annonymous Users vs. Logged In
Validation for WHO - PASS
Search for and Return List of Active Users and 200OK - PASS
Validation for LOOK - PASS
Implement LOOK for First Name - PASS
Implement LOOK for Last Name - PASS
Implement LOOK for Phone Number - PASS
Return 200 OK Response with Results for LOOK - PASS
Return 404 Response for LOOK - PASS
Validation for Root User Issuing SHUTDOWN - PASS
Return 402 If Not Root - PASS
Broadcast to All Clients 210 - PASS
Peform exit() on Server to SHUTDOWN - PASS

## Tests PA #1

Test | Description | Input | Output | Pass/Fail
--- | --- | --- | --- |--- 
Read Address File | XXX | XX | XXX | PASS
Add Address to Address Structure | XXX | XX | XXX | PASS
Validation for ADD, DELETE, LIST, SHUTDOWN, QUIT | XXX | XX | XXX | XXX
Parse Add Command and Add to Address Structure | XXX | XX | XXX | XXX
Return 200 OK Response for ADD | XXX | XX | XXX | XXX
Parse Delete Command and Remove from Address Structure | XXX | XX | XXX | XXX
Parse Delete and Return "403 The Record ID Does Not Exist" | XXX | XX | XXX | XXX
Parse LIST and Return Single String Containing (200 OK, Text, N Entries) | XXX | XX | XXX | XXX
Parse LIST and Return "404 No Records Exist"  | XXX | XX | XXX | XXX
Parse SHUTDOWN and Return "200 OK" | XXX | XX | XXX | XXX
Parse SHUTDOWN and Close Client and Server Connection  | XXX | XX | XXX | XXX
Parse QUIT and Return "200 OK" | XXX | XX | XXX | XXX
Parse QUIT and Close Client Connection | XXX | XX | XXX | XXX
Consider Extremely Long List Response | XXX | XX | XXX | XXX
General Validation - First Name Max 8 Characters  | XXX | XX | XXX | XXX
General Validation - Last Name Max 8 Characters  | XXX | XX | XXX | XXX
General Validation - Phone Number Exactly 12 Characters  | XXX | XX | XXX | XXX
General Validation - Record ID is Exactly 4 Characters | XXX | XX | XXX | XXX
General Validation - LIST is Alone as Command | XXX | XX | XXX | XXX
General Validation - SHUTDOWN is Alone as Command | XXX | XX | XXX | XXX
General Validation - QUIT is Alone as Command | XXX | XX | XXX | XXX
Server Client Flow - Enable Back to Back Requests | XXX | XX | XXX | XXX
Develop Functions Where Appropriate | XXX | XX | XXX | XXX


## Notes
- To obtain IP address of device, run ifconfig, and look at inet value for en0 connection, use that value when spinning up client