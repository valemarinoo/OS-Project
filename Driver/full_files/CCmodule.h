
#define  DEVICE_NAME "Crypto_Core"    ///< The device will appear at /dev/LM_module using this value
#define  CLASS_NAME  "CC"        ///< The device class -- this is a character device driver

#define CC_k 0xFF
#define ENCRYPT _IOW(CC_k, 1, int)
#define DECRYPT _IOW(CC_k, 2, int)

#define INPUT_START 0x4000000
#define OUTPUT_START 0x4000320
#define BUF_LEGHT 100 //in uint_64 cells
#define STATUS_ID 0x400640


/**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**