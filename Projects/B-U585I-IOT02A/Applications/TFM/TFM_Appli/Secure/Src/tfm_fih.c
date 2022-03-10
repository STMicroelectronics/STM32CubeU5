#include "low_level_rng.h"
#include "target_flowcontrol.h"

#ifdef TFM_FIH_PROFILE_ON

extern void Error_Handler(void);

/* this is the number of random values created
  at the boot and will be used the fih */
#define RNG_NUMBER 32

unsigned char seed_buf[RNG_NUMBER] = {0};
size_t index_seed_buf = 0;

int32_t tfm_hal_random_init(void)
{
  size_t len = 0U;

  RNG_Init();
  /* generate several random */
  RNG_GetBytes((unsigned char *)seed_buf, sizeof(seed_buf),(size_t *)&len);
  if(len == 0)/* fail to generate random values */
  {
    Error_Handler();
  }
  /* use flow control to check that the buffer is filled */
  FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FIH_RAND_GEN, FLOW_CTRL_FIH_RAND_GEN);
  return 0;
}

int32_t tfm_hal_random_generate(uint8_t *rand, size_t size)
{
  static uint8_t index_seed_buf = 0U;

  *rand = seed_buf[index_seed_buf];
  index_seed_buf++;

  if (RNG_NUMBER == index_seed_buf)
  {
    index_seed_buf = 0;
  }
  return 0;
}

#endif /* TFM_FIH_PROFILE_ON */
