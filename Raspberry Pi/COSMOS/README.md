

# KeAo - RFM69HCW on Raspberry Pi using wiringPi and COSMOS

## Installing wiringPi

Visit [this markdown file](https://github.com/ethancheez/KeAo-COSMOS/blob/main/Raspberry%20Pi/rfm69/rfm69cpp/README.md) to install wiringPi on the Raspberry Pi. Skip the "Compiling" section.

## Linking wiringPi with COSMOS-CORE

Edit the CMakeLists.txt file in `cosmos/source/core` with your favorite text editor.

Edit line 69:
```
set(COSMOS_CXX_FLAGS "-Wall -pedantic -std=c++11 -U__STRICT_ANSI__ -Wno-invalid-offsetof -Wno-format")
```

Add the `-lwiringPi` tag in the end. It should now look like this:
```
set(COSMOS_CXX_FLAGS "-Wall -pedantic -std=c++11 -U__STRICT_ANSI__ -Wno-invalid-offsetof -Wno-format -lwiringPi")
```

Save and close the file.

You can now build the agents normally.