# Decimal Type - Desired Specs
```
- Behave like built in type
- Header only library
- Max:  18446744073709551615.9999999999999999999
- Min: -18446744073709551615.9999999999999999999
- Resolution:              0.0000000000000000001
```

# Recommended Minimum Requirements
```
- c++11
- c++4.9.2 / VisualStudio2015
```

# Build Unit Tests
```
git clone https://github.com/tylawin/Decimal
```
###### Linux:
```
cd Decimal
mkdir build
cd build
cmake ..
make
```
###### Run Unit Tests:
```
source/runTests
```

# License
```
Apache License 2.0
See LICENSE file for details.
```

# Todo
```
operator%
more decimal digits (test mult div etc. edge cases...)
```
