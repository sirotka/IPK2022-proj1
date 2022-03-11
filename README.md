# IPK project 1

#### autor: Ekaterina Krupenko, xkrupe00

## Project description

The project assignment is to create a server in C / C++, communicating via HTTP, which will provide various information about the system. The server will listen on the specified port and return the required information according to the URL. The server must correctly handle HTTP headers and generate correct HTTP responses. The response type will be text / plain. Communication with the server should be possible both with the help of a web browser, and with tools such as wget and curl. The Server must be executable in Linux Ubuntu 20.04 LTS environment (https://ubuntu.com/).

The server will be translatable using a makefile that creates an executable hinfosvc file.
This server will be executable with an argument indicating the local port on which it will listen to requests:


---
./ hinfosvc 12345
---


It will be possible to exit the server using Ctrl + C. The Server will be able to process the following three types of queries that are sent to the server by the GET command:


---
### 1. Getting a domain name

Returns the network name of the computer, including the domain, for example:

### 2. Getting CPU information

Returns information about the processor, for example:


### 3. Current load

Returns current load information. This requires calculation from the values given in the / proc / stat file (see link below). The result is, for example:
---


## Examples of Project use


---
make && ./hinfosvc port
GET http://servername:12345/hostname

merlin.fit.vutbr.cz
---


---
make && ./hinfosvc port
GET http://servername:12345/cpu-name


Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz
---


---
make && ./hinfosvc port
GET http://servername:12345/load

65%
---
