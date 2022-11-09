# cricket

Implementation of the CRicKET algorithm, that can be built and executed under the contiki-ng OS (https://github.com/contiki-ng/contiki-ng/wiki).
The example can be executed on Cooja or on the nrf52840 DK/dongle.
The implementation can be extended to other platforms supported by contiki-ng. See the section 'Execute on different platforms' below.

![Alt text](res/cricketDiagram.PNG?raw=true "Cricket algorithm steps")

# Download 

Clone the repository's code as a submodule under contiki-ng
```
git submodule add  https://github.com/fraimondo18/cricket.git  examples/cricket
```


# Getting started

To compile for the nrf52840: make TARGET=nrf52840

Two executable kgA and kgB will be generated. They can be uploaded on 2 boards acting as Alice and Bob.

Information derived during the experiment will be logged, including:
- Channel sequence
- Estimated channel mismatch
- Lookup table parameters: ini_key_len, block_size, tau
- current number of bits agreed
- key disagreement rate (KDR)
- Encoding rate
- Comparison of the keys calculated at Alice and Bob side



# Process data offline

To extract the results of the experiment, use the python script: cricketProcessResults.py
This will generate a csv file containing the results for each experiment.


![Alt text](res/cricketResults.PNG?raw=true "Experiments results")


# Execute on different platforms

The functions that are platform dependent, are located in the file plat_none.c

To run on a different platform X, create a new file plat_X.c
Implement the functions:
1) void kg_plat_init(); -> initialise the crypto system of the board, if necessary
2) int kg_gen_random(void *p_rng, unsigned char *output, size_t output_len); -> generate a sequence of random bytes
3) void kg_calcHash(uint8_t *inData, uint8_t dataLen, uint8_t *outData, size_t *outDataLen); -> calculate HASH of the 128bits key.








