# Geraboard - Abfahrtstafel written in C
## Dependencies:
this project relies on curl to make requests and [cjson](https://github.com/DaveGamble/cJSON) to interpret them.

it also needs an internet connection

## usage:
the program is (currently) called by passing station names as parameters to the executable; e.g. 'geraboard "Gera Hbf"'. 

spaces need to be escaped.

by default, the program returns the next five departures @ each given station; this behaviour can be modified by varying the value of MAX_DEP_COUNT in main.c

## further development:
- nicening the output (gui?)
- refresh the output periodically
- reading config files to easen usage
- support for local rail services is planned (directly, for the [gvb](https://gvbgera.de))

BGI says: Trans rights! 🏳️‍⚧️
