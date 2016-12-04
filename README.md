Handling Parameter Modifier
=========================
A quick tool to assist killatomate in his handling.meta development.

## Usage
Run in terminal:
```
HandlingParamModifier.exe --help
HandlingParamModifier.exe -h <handling.meta file> -l <vehicle list file>
```

* ```-h``` - a valid handling.meta as  input
* ```-l``` - text file, each line has a handling name to edit

Writes to ```result.xml```.

Prints nothing on success, exception message if anything went wrong,
list of vehicles specified in ```-l``` it could not find.

### Example
Valid handling.meta and vehicles.txt are in the same directory as the executable

vehicles.txt
```
AIRTUG
INVALIDEXAMPLE
WASHINGTON
B_Hakuchou
B_INVALIDEXAMPLE2
```

Command prompt output
```
HandlingParamModifier.exe -h handling.meta -l vehicles.txt

Following handling line(s) not found: (cars)
INVALIDEXAMPLE

Following handling line(s) not found: (bikes)
INVALIDEXAMPLE2
```

## Building

* [Boost 1.63.0 beta 1](http://www.boost.org/users/history/version_1_63_0.html) was used

## License

Don't be an ass + whatever is from tinyxml2
