#include "qemu/osdep.h"
#include "qapi/error.h" /* provides error_fatal() handler */
#include "hw/sysbus.h" /* provides all sysbus registering func */
#include "hw/misc/crypto_core.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TYPE_CRYPTO_CORE "crypto_core"
typedef struct CryptoCoreState CryptoCoreState;
DECLARE_INSTANCE_CHECKER(CryptoCoreState, CRYPTO_CORE, TYPE_CRYPTO_CORE)

static int prime(uint64_t n);
static void compute_e(uint64_t p,uint64_t q,uint64_t t,uint64_t* e,uint64_t* d);
static uint64_t compute_d(uint64_t t, uint64_t x);
static uint64_t encrypt(uint64_t msg, uint64_t p, uint64_t q);
static uint64_t decrypt(uint64_t msg, uint64_t p, uint64_t q);
static uint64_t mod_exp(uint64_t b, uint64_t exp, uint64_t mod);

#define IN_START_ID 0x0 	//  0
#define IN_END_ID 0X318 	//  792
#define OUT_START_ID 0x320	//  800
#define OUT_END_ID 0x638	//  1600
#define STATUS_ID 0x640		//  1602
#define SIZE 100
#define P 89
#define Q 181

struct CryptoCoreState {
	SysBusDevice parent_obj;
	MemoryRegion iomem;
	uint64_t input[SIZE];
	uint64_t output[SIZE];
	uint64_t status;
};

static uint64_t encrypt(uint64_t msg, uint64_t p, uint64_t q) {
	
	uint64_t n,t,e[20] = {0} ,d[20] = {0};
	//CHECK IF P AND Q ARE PRIME NUMBERS

	n=p*q;
	t=(p-1)*(q-1);
	compute_e(p,q,t,e,d);
	return mod_exp(msg, e[0], n);
}


static uint64_t decrypt(uint64_t msg, uint64_t p, uint64_t q) {

	//CHECK IF P AND Q ARE PRIME NUMBERS
	uint64_t n,t,e[20] = {0} ,d[20] = {0};

	n=p*q;
	t=(p-1)*(q-1);
	compute_e(p,q,t,e,d);
	return mod_exp(msg, d[0], n);
}


static int prime(uint64_t n) {
  uint64_t i,j;
  j=sqrt(n);
  for (i=2;i<=j;i++) {
    if(n%i==0)
        return 0;
  }
  return 1;
}

static void compute_e(uint64_t p,uint64_t q,uint64_t t,uint64_t* e,uint64_t* d) {

  uint64_t k,flag,i;
  k=0;
  for (i=2;i<t;i++) {
    if(t%i==0)
        continue;
    flag=prime(i);
    if(flag==1&&i!=p&&i!=q) {
      e[k]=i;
      flag=compute_d(t, e[k]);
      if(flag>0) {
        d[k]=flag;
        k++;
      }
      if(k==20)
              break;
    }
  }

}

static uint64_t compute_d(uint64_t t, uint64_t x) {
  uint64_t k=1;
  while(1) {
    k=k+t;
    if(k%x==0)
        return(k/x);
  }
}

static uint64_t mod_exp(uint64_t b, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    b = b % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * b) % mod;
        }

        exp = exp >> 1;
        b = (b * b) % mod;
    }

    return result;
}

static uint64_t crypto_core_read(void *opaque, hwaddr addr, unsigned int size)
{
	CryptoCoreState *s = opaque;
	
	if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 0)){
		return s->input[(int)addr/8];
	} if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 4)){
		return s->input[((int)addr-4)/8] >> 32;
	} else if((int)addr >= OUT_START_ID && (int)addr <= OUT_END_ID && ((int)addr%8 == 0)){
		return s->output[((int)addr-0x320)/8];
	} else if((int)addr >= OUT_START_ID && (int)addr <= OUT_END_ID && ((int)addr%8 == 4)){
		return s->output[((int)addr-0x324)/8] >> 32;
	} else if((int)addr == STATUS_ID){
		return s->status;
	} 
	
	else return 0xDEADBEEF;

}

//static uint64_t crypto_core_write(void *opaque, hwaddr addr, hwaddr data, unsigned int size)
static void crypto_core_write(void *opaque, hwaddr addr, uint64_t data, unsigned int size)
{
	CryptoCoreState *s = opaque;
	if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 0)){
		s->input[(int)addr/8] = data;
		return;
	} else if((int)addr >= IN_START_ID && (int)addr <= IN_END_ID && ((int)addr%8 == 4)){
		s->input[((int)addr-4)/8] |= data << 32;
		return;
	} else if(addr == STATUS_ID){
		if(data == 0x01) {
			for(int i=0; i<SIZE; i++){
				s->output[i] = encrypt(s->input[i], P, Q);
			}
			
			return;
		} else 
		if(data == 0x02) {
			for(int i=0; i<SIZE; i++){
				s->output[i] = decrypt(s->input[i], P, Q);
			}
			
			return;
		}		
	} else s->status = 0x1;
}

static const MemoryRegionOps crypto_core_ops = {
	.read = crypto_core_read,
	.write = crypto_core_write,
	.endianness = DEVICE_NATIVE_ENDIAN,
};

static void crypto_core_instance_init(Object *obj)
{
	CryptoCoreState *s = CRYPTO_CORE(obj);

	/* allocate memory map region */
	memory_region_init_io(&s->iomem, obj, &crypto_core_ops, s, TYPE_CRYPTO_CORE, 0x1000);
	sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->iomem);
}

/* create a new type to define the info related to our device */
static const TypeInfo crypto_core_info = {
	.name = TYPE_CRYPTO_CORE,
	.parent = TYPE_SYS_BUS_DEVICE,
	.instance_size = sizeof(CryptoCoreState),
	.instance_init = crypto_core_instance_init,
};

static void crypto_core_register_types(void)
{
    type_register_static(&crypto_core_info);
}

type_init(crypto_core_register_types)

/*
 * Create the Banana Rom device.
 */
DeviceState *crypto_core_create(hwaddr addr)
{
	DeviceState *dev = qdev_new(TYPE_CRYPTO_CORE);
	sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
	sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, addr);
	return dev;
}


/**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**/