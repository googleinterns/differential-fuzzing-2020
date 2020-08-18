**This is not an officially supported Google product.**

**File Key:**

- *differential_fuzzing_lib*:
Contains files used for the differential fuzzing library. in particular:
    baseparser.h: file used as a virtual class for the differential fuzzer.
    This class is used as the base of new classes used with the wrapper

    differential_fuzzer.h: methods used for the differential fuzzer
    wrapper

    differential_fuzzer.cpp: code for differentialfuzzer.h

- *toy_cases*:
Directory contains toy cases, and code used for executing, and testing it. 
More information found in ./testcode/README.txt