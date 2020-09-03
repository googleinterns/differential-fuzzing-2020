**This is not an officially supported Google product.**

**File Key:**

*differential_fuzzing_lib*:
Contains files used for the differential fuzzing library. in particular:
    baseparser.h: file used as a virtual class for the differential fuzzer.
    This class is used as the base of new classes used with the wrapper

- differential_fuzzer.h: methods used for the differential fuzzer
wrapper

- differential_fuzzer.cpp: code for differentialfuzzer.h

- logger.h: allows for debug mode to be activated for the differential fuzzer

*tests*:
Includes several tests related to the differential fuzzer library of methods,
and differential fuzzing with the yaml parsing libraries