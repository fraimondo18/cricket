CONTIKI_PROJECT = kgA kgB
all: $(CONTIKI_PROJECT)


CONTIKI=../..


PROJECT_SOURCEFILES += keyGenLib.c



ifneq ($(findstring $(TARGET),nrf52840),)

MAKE_MAC = MAKE_MAC_TSCH
CFLAGS += -DKG_RSSI_COLLECT
CFLAGS += -DNRF52840_TARGET
include $(CONTIKI)/examples/key-gen/Makefile.nrf_kg

PROJECT_SOURCEFILES += plat_nrf52840.c  

else
  PROJECT_SOURCEFILES += plat_none.c 
endif
  
  
CONTIKI=../..
include $(CONTIKI)/Makefile.include
