# cricket

Implementation of the CRicKET algorithm. We provide an example that can be built and executed under contiki-ng (https://github.com/contiki-ng/contiki-ng/wiki).
The implementation can also be built and executed in a different environment: the main file is keyGenLib.c. It contains the main functions: KG_sm_A, KG_sm_B.
 


Platforms: nrf52840
Platform independent

# Download 

Clone the repository's code as a submodule, under contiki-ng/contiki-ng/examples/ 

```
git clone https://github.com/fraimondo18/cricket.git
```







# Getting started

Run an example that reads in a MATLAB file of RSSI values to generate a bit string sequence and subsequently test using several of the 15 different randomness tests in the testsuite.

```
python genkeys.py ./skyglow/Scenario2-Office-LoS/data3_upto5.mat 0.5 0.1
```

Plot Alice and Bob's RSSI sequences, processed sequences, and final bit sequence.

```
python analyze.py ./skyglow/Scenario2-Office-LoS/data3_upto5.mat 4096 0.4 0.05
```
