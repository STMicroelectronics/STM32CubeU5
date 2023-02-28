/**
  **********************************************************************************************************************
  * @file    stm32_ll_lpbam.c
  * @author  MCD Application Team
  * @brief   This file provides the LL LPBAM functions for STM32U5 devices.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_ll_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_LL_Private_Functions
  * @{
  */

#if defined (LPBAM_ADC_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for ADC peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_ADC_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_ADC_FillStructInfo(LPBAM_ADC_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo)
{
  /* LPBAM Configuration ADC Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_ADC_DescItem_Tbl[] =
  {
    /* LPBAM ADC State ID                 */
    {LPBAM_ADC_STATE_OFFSET,             LPBAM_ADC_STATE_DATASIZE            },
    /* LPBAM ADC Configuration ID         */
    {LPBAM_ADC_CONFIG_OFFSET,            LPBAM_ADC_CONFIG_DATASIZE           },
    /* LPBAM ADC Channel Selection ID     */
    {LPBAM_ADC_CHANNELSELECTION_OFFSET,  LPBAM_ADC_CHANNELSELECTION_DATASIZE },
    /* LPBAM ADC AWDG Threshold ID        */
    {LPBAM_ADC_AWDG_THESHOLD_OFFSET,     LPBAM_ADC_AWDG_THESHOLD_DATASIZE    },
    /* LPBAM ADC AWDG Monitor ID          */
    {LPBAM_ADC_AWDG_MONITOR_OFFSET,      LPBAM_ADC_AWDG_MONITOR_DATASIZE     },
    /* LPBAM ADC Trigger Low Frequency ID */
    {LPBAM_ADC_TRIG_LOWFREQUENCY_OFFSET, LPBAM_ADC_TRIG_LOWFREQUENCY_DATASIZE},
    /* LPBAM ADC Prescaler ID             */
    {LPBAM_ADC_CLKPRESCALER_OFFSET,      LPBAM_ADC_CLKPRESCALER_DATASIZE     },
    /* LPBAM ADC Interrupt Clear Flag ID  */
    {LPBAM_ADC_CLEARFLAG_OFFSET,         LPBAM_ADC_CLEARFLAG_DATASIZE        },
    /* LPBAM ADC Wakeup Interrupt ID      */
    {LPBAM_ADC_WAKEUP_IT_OFFSET,         LPBAM_ADC_WAKEUP_IT_DATASIZE        },
    /* LPBAM ADC Data transfer ID         */
    {LPBAM_ADC_DATA_OFFSET,              0U                                  },
  };

  uint32_t dummy = 0U;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM ADC State ID */
    case LPBAM_ADC_STATE_ID:
    {
      /* Get control register value */
      dummy = pConfNode->pInstance->CR & ~(LPBAM_ADC_ADSTP | LPBAM_ADC_ADSTART);

      /* Check ADC start state */
      if ((pConfNode->Config.State) == ENABLE)
      {
        dummy |= LPBAM_ADC_ADSTART;
      }
      /* Check ADC stop state */
      else if ((pConfNode->Config.State) == DISABLE)
      {
        dummy |= LPBAM_ADC_ADSTP;
      }
      else
      {
        /* Return LPBAM status */
        return LPBAM_ERROR;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC Configuration ID */
    case LPBAM_ADC_CONFIG_ID:
    {
      /* Set threshold analog watchdog 1 */
      if (pConfNode->Config.UpdateAnalogWDG == LPBAM_ADC_ANALOGWATCHDOG_DEFAULT)
      {
        /* Get CFGR register value */
        dummy = pConfNode->pInstance->CFGR1;

        /* Set ADC trigger configuration */
        MODIFY_REG(dummy, (LPBAM_ADC_EXTSEL | LPBAM_ADC_EXTEN),
                   ((pConfNode->Config.ExternalTrigConv << LPBAM_ADC_EXTSEL_POS) |
                    (pConfNode->Config.ExternalTrigConvEdge << LPBAM_ADC_EXTEN_POS)));

        /* Set ADC conversion mode */
        if (pConfNode->Config.ContinuousConvMode == ENABLE)
        {
          dummy |= LPBAM_ADC_CONT;
          dummy &= ~LPBAM_ADC_DISCEN;
        }
        else
        {
          dummy &= ~LPBAM_ADC_CONT;
          dummy |= LPBAM_ADC_DISCEN;

          /* Check ADC discontinuous mode */
          if (pConfNode->Config.DiscontinuousConvMode == ENABLE)
          {
            dummy &= ~LPBAM_ADC_CONT;
            dummy |= LPBAM_ADC_DISCEN;
          }
          else
          {
            dummy &= ~LPBAM_ADC_CONT;
            dummy &= ~LPBAM_ADC_DISCEN;
          }
        }

        /* Set ADC channel backward scan direction */
        if (pConfNode->Config.ScanConvMode == LPBAM_ADC_SCAN_DIRECTION_BACKWARD)
        {
          dummy |= LPBAM_ADC_SCANDIR;
          dummy &= ~LPBAM_ADC_CHSELRMOD;
        }
        /* Set ADC channel forward scan direction */
        else if (pConfNode->Config.ScanConvMode == LPBAM_ADC_SCAN_DIRECTION_FORWARD)
        {
          dummy &= ~LPBAM_ADC_SCANDIR;
          dummy &= ~LPBAM_ADC_CHSELRMOD;
        }
        /* Set ADC channel scan enable */
        else if (pConfNode->Config.ScanConvMode == LPBAM_ADC_SCAN_ENABLE)
        {
          dummy |= LPBAM_ADC_CHSELRMOD;
        }
        /* Set ADC channel scan disable */
        else
        {
          dummy &= ~LPBAM_ADC_CHSELRMOD;
        }

        /* Set ADC wait conversion state */
        if (pConfNode->Config.LowPowerAutoWait == ENABLE)
        {
          /* Set wait conversion */
          dummy |= LPBAM_ADC_WAIT;
        }
        else
        {
          /* Clear wait conversion */
          dummy &= ~LPBAM_ADC_WAIT;
        }

        /* Enable DMA for ADC peripheral */
        if (pConfNode->Config.DMAContinuousRequests == ENABLE)
        {
          /* Circular DMA mode */
          dummy |= LPBAM_ADC_DMACFG;
        }
        else
        {
          /* One shot DMA mode */
          dummy &= ~LPBAM_ADC_DMACFG;
        }
      }

      /* Check analog watchdog index */
      if (pConfNode->Config.AnalogWDGConfig[0U].WatchdogMode != LPBAM_ADC_ANALOGWATCHDOG_NONE)
      {
        /* Enable analog watchdog 1 */
        dummy |= LPBAM_ADC_AWD1EN;

        if (pConfNode->Config.AnalogWDGConfig[0U].WatchdogMode == LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG)
        {
          dummy |= LPBAM_ADC_AWD1SGL;
          dummy |= (POSITION_VAL(pConfNode->Config.AnalogWDGConfig[0U].Channel &
                                 (~CHANNEL_ID_INTERNAL_CH)) << LPBAM_ADC_AWD1CH_POS);
        }
        else
        {
          dummy &= ~LPBAM_ADC_AWD1SGL;
        }
      }
      else
      {
        /* Disable analog watchdog 1 */
        dummy &= ~LPBAM_ADC_AWD1EN;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC Channel Selection ID */
    case LPBAM_ADC_CHANNELSELECTION_ID:
    {
      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC trigger low frequency ID */
    case LPBAM_ADC_TRIG_LOWFREQUENCY_ID:
    {
      /* Get CFGR register value */
      dummy = pConfNode->pInstance->CFGR2;

      /* Configure ADC Low Frequency Trig */
      MODIFY_REG(dummy, LPBAM_ADC_LFTRIG, (pConfNode->Config.TriggerFrequencyMode << LPBAM_ADC_LFTRIG_POS));

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC AWDG threshold ID */
    case LPBAM_ADC_AWDG_THRESHOLD_ID:
    {
      /* Set threshold analog watchdog 1 */
      if (pConfNode->Config.UpdateAnalogWDG == LPBAM_ADC_ANALOGWATCHDOG_1)
      {
        /* Check analog watchdog mode */
        if (pConfNode->Config.AnalogWDGConfig[0U].WatchdogMode != LPBAM_ADC_ANALOGWATCHDOG_NONE)
        {
          /* Set analog watchdog threshold new values */
          dummy = ((pConfNode->Config.AnalogWDGConfig[0U].HighThreshold  << LPBAM_ADC_HT1_POS) |
                   (pConfNode->Config.AnalogWDGConfig[0U].LowThreshold));
        }
        else
        {
          /* Set analog watchdog threshold default values */
          dummy = LPBAM_ADC_AWD_DEFAULT_VALUE;
        }

        /* Set DMA destination address */
        pDescInfo->DstAddr = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_ADC_AWDG1_THRESHOLD_OFFSET;
      }

      /* Set threshold analog watchdog 2 */
      if (pConfNode->Config.UpdateAnalogWDG == LPBAM_ADC_ANALOGWATCHDOG_2)
      {
        /* Check analog watchdog mode */
        if (pConfNode->Config.AnalogWDGConfig[1U].WatchdogMode != LPBAM_ADC_ANALOGWATCHDOG_NONE)
        {
          /* Set analog watchdog threshold new values */
          dummy = ((pConfNode->Config.AnalogWDGConfig[1U].HighThreshold  << LPBAM_ADC_HT1_POS) |
                   (pConfNode->Config.AnalogWDGConfig[1U].LowThreshold));
        }
        else
        {
          /* Set analog watchdog threshold default values */
          dummy = LPBAM_ADC_AWD_DEFAULT_VALUE;
        }

        /* Set DMA destination address */
        pDescInfo->DstAddr = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_ADC_AWDG2_THRESHOLD_OFFSET;
      }

      /* Set threshold analog watchdog 3 */
      if (pConfNode->Config.UpdateAnalogWDG == LPBAM_ADC_ANALOGWATCHDOG_3)
      {
        /* Check analog watchdog mode */
        if (pConfNode->Config.AnalogWDGConfig[2U].WatchdogMode != LPBAM_ADC_ANALOGWATCHDOG_NONE)
        {
          /* Set analog watchdog threshold new values */
          dummy = ((pConfNode->Config.AnalogWDGConfig[2U].HighThreshold  << LPBAM_ADC_HT1_POS) |
                   (pConfNode->Config.AnalogWDGConfig[2U].LowThreshold));
        }
        else
        {
          /* Set analog watchdog threshold default values */
          dummy = LPBAM_ADC_AWD_DEFAULT_VALUE;
        }

        /* Set DMA destination address */
        pDescInfo->DstAddr = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_ADC_AWDG3_THRESHOLD_OFFSET;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC analog watchdog monitor ID */
    case LPBAM_ADC_AWDG_MONITOR_ID:
    {
      /* Set monitor analog watchdog 2 */
      if (pConfNode->Config.UpdateAnalogWDG == LPBAM_ADC_ANALOGWATCHDOG_2)
      {
        /* Check analog watchdog mode */
        if (pConfNode->Config.AnalogWDGConfig[1U].WatchdogMode != LPBAM_ADC_ANALOGWATCHDOG_NONE)
        {
          if (pConfNode->Config.AnalogWDGConfig[1U].WatchdogMode == LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG)
          {
            dummy = pConfNode->Config.AnalogWDGConfig[1U].Channel;
          }
          else
          {
            dummy = LPBAM_ADC_ANALOGWATCHDOG_ALL_CH;
          }
        }

        /* Set DMA destination address */
        pDescInfo->DstAddr = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_ADC_AWDG2_MONITOR_OFFSET;
      }

      /* Set monitor analog watchdog 3 */
      if (pConfNode->Config.UpdateAnalogWDG == LPBAM_ADC_ANALOGWATCHDOG_3)
      {
        /* Check analog watchdog mode */
        if (pConfNode->Config.AnalogWDGConfig[2U].WatchdogMode != LPBAM_ADC_ANALOGWATCHDOG_NONE)
        {
          if (pConfNode->Config.AnalogWDGConfig[2U].WatchdogMode == LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG)
          {
            dummy = pConfNode->Config.AnalogWDGConfig[2U].Channel;
          }
          else
          {
            dummy = LPBAM_ADC_ANALOGWATCHDOG_ALL_CH;
          }
        }

        /* Set DMA destination address */
        pDescInfo->DstAddr = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_ADC_AWDG3_MONITOR_OFFSET;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC Prescaler ID */
    case LPBAM_ADC_CLKPRESCALER_ID:
    {
      /* Get CCR register value */
      dummy = *(uint32_t *)((uint32_t)pConfNode->pInstance + LPBAM_ADC_CLKPRESCALER_OFFSET);

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC Interrupt Clear Flag ID */
    case LPBAM_ADC_CLEARFLAG_ID:
    {
      /* Get ISR register value */
      dummy = LPBAM_ADC_IT_AWD1 | LPBAM_ADC_IT_AWD2 | LPBAM_ADC_IT_AWD3;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC Wakeup Interrupt ID */
    case LPBAM_ADC_WAKEUP_IT_ID:
    {
      /* Get IER register value */
      dummy = pConfNode->pInstance->IER;

      /* Configure ADC AWDG1 interrupt */
      if (pConfNode->Config.AnalogWDGConfig[0U].ITMode == ENABLE)
      {
        /* Set ADC AWDG1 interrupt */
        SET_BIT(dummy, LPBAM_ADC_IT_AWD1);
      }
      else
      {
        /* Clear ADC AWDG1 interrupt */
        CLEAR_BIT(dummy, LPBAM_ADC_IT_AWD1);
      }

      /* Configure ADC AWDG2 interrupt */
      if (pConfNode->Config.AnalogWDGConfig[1U].ITMode == ENABLE)
      {
        /* Set ADC AWDG2 interrupt */
        SET_BIT(dummy, LPBAM_ADC_IT_AWD2);
      }
      else
      {
        /* Clear ADC AWDG2 interrupt */
        CLEAR_BIT(dummy, LPBAM_ADC_IT_AWD2);
      }

      /* Configure ADC AWDG3 interrupt */
      if (pConfNode->Config.AnalogWDGConfig[2U].ITMode == ENABLE)
      {
        /* Set ADC AWDG3 interrupt */
        SET_BIT(dummy, LPBAM_ADC_IT_AWD3);
      }
      else
      {
        /* Clear ADC AWDG3 interrupt */
        CLEAR_BIT(dummy, LPBAM_ADC_IT_AWD3);
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM ADC Data transfer ID */
    case LPBAM_ADC_DATA_ID:
    {
      /* Set DMA request */
      pDescInfo->Request  = LPDMA1_REQUEST_ADC4;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = LPBAM_ADC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA data size */
      pDescInfo->DataSize = (uint16_t)(pConfNode->Config.Size * 2U);

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* defined (LPBAM_ADC_MODULE_ENABLED) */


#if defined (LPBAM_COMP_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for COMP peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_COMP_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_COMP_FillStructInfo(LPBAM_COMP_ConfNode_t const *const pConfNode,
                                         LPBAM_InfoDesc_t      *const pDescInfo)
{
  /* LPBAM Configuration COMP Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_COMP_DescItem_Tbl[] =
  {
    /* LPBAM COMP configuration node ID     */
    {LPBAM_COMP_CONFIG_OFFSET,     LPBAM_COMP_CONFIG_DATASIZE},
    /* LPBAM COMP read output value node ID */
    {LPBAM_COMP_READOUTPUT_OFFSET, 0U                        },
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM COMP configuration node ID */
    case LPBAM_COMP_CONFIG_ID:
    {
      /* Get register value */
      dummy = (((uint32_t)pConfNode->Instance->CSR & LPBAM_COMP_CONFIG_MASK) | (uint32_t)pConfNode->Config.State |
               pConfNode->Config.OutputPol | pConfNode->Config.InputPlus | pConfNode->Config.InputMinus);

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_COMP_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->Instance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_COMP_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM COMP read output value node ID */
    case LPBAM_COMP_READOUTPUT_ID:
    {
      /* Set DMA source address */
      pDescInfo->SrcAddr  = LPBAM_COMP_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->Instance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA data size */
      pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size * 4U;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_COMP_MODULE_ENABLED */


#if defined(LPBAM_DAC_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for DAC peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_DAC_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_DAC_FillStructInfo(LPBAM_DAC_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo)
{
  /* LPBAM Configuration DAC Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_DAC_DescItem_Tbl[] =
  {
    /* LPBAM DAC configuration node ID */
    {LPBAM_DAC_CONFIG_OFFSET, LPBAM_DAC_CONFIG_DATASIZE},
    /* LPBAM DAC data node ID          */
    {LPBAM_DAC_DATA_OFFSET,   0U                       },
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM DAC configuration node ID */
    case LPBAM_DAC_CONFIG_ID:
    {
      /* Get CR value */
      dummy = pConfNode->pInstance->CR;

      /* Check DAC channel1 */
      if ((pConfNode->Config[0U].Channel & LPBAM_DAC_CHANNEL_1) == LPBAM_DAC_CHANNEL_1)
      {
        /* Set DAC state */
        if (pConfNode->Config[0U].State == ENABLE)
        {
          dummy |= DAC_CR_EN1;

          /* Set DAC trigger */
          dummy &= ~DAC_CR_TSEL1;
          dummy |= DAC_CR_TEN1 | (pConfNode->Config[0U].DAC_Trigger << DAC_CR_TSEL1_Pos);
        }
        else
        {
          dummy &= ~DAC_CR_EN1;
        }
      }

      /* Check DAC channel 1 */
      if ((pConfNode->Config[0U].Channel & LPBAM_DAC_CHANNEL_2) == LPBAM_DAC_CHANNEL_2)
      {
        /* Set DAC state */
        if (pConfNode->Config[0U].State == ENABLE)
        {
          dummy |= DAC_CR_EN2;

          /* Set trigger */
          dummy &= ~DAC_CR_TSEL2;
          dummy |= DAC_CR_TEN2 | (pConfNode->Config[0U].DAC_Trigger << DAC_CR_TSEL2_Pos);
        }
        else
        {
          dummy &= ~DAC_CR_EN2;
        }
      }

      /* Check DAC channel 2 */
      if ((pConfNode->Config[1U].Channel & LPBAM_DAC_CHANNEL_2) == LPBAM_DAC_CHANNEL_2)
      {
        /* Set DAC state */
        if (pConfNode->Config[1U].State == ENABLE)
        {
          dummy |= DAC_CR_EN2;

          /* Set trigger */
          dummy &= ~DAC_CR_TSEL2;
          dummy |= DAC_CR_TEN2 | (pConfNode->Config[1U].DAC_Trigger << DAC_CR_TSEL2_Pos);
        }
        else
        {
          dummy &= ~DAC_CR_EN2;
        }
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_DAC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_DAC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM DAC data node ID */
    case LPBAM_DAC_DATA_ID:
    {
      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA destination address */
      pDescInfo->DstAddr = LPBAM_DAC_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                           (uint32_t)pConfNode->pInstance;

      /* Check DAC channel */
      if (pConfNode->Config->Channel == LPBAM_DAC_CHANNEL_1)
      {
        /* Set DMA request */
        pDescInfo->Request = LPDMA1_REQUEST_DAC1_CH1;
      }
      else if (pConfNode->Config->Channel == LPBAM_DAC_CHANNEL_2)
      {
        /* Set DMA request */
        pDescInfo->Request = LPDMA1_REQUEST_DAC1_CH2;
        /* Set channel offset */
        pDescInfo->DstAddr += LPBAM_DAC_CHANNEL_OFFSET;
      }
      else
      {
        /* Return LPBAM status */
        return LPBAM_ERROR;
      }

      /* Check data alignment */
      if (pConfNode->Config->Alignment == LPBAM_DAC_ALIGN_12B_L)
      {
        /* Update destination address */
        pDescInfo->DstAddr += LPBAM_DAC_ALIGN_12B_L;
      }
      else if (pConfNode->Config->Alignment == LPBAM_DAC_ALIGN_8B_R)
      {
        /* Update destination address */
        pDescInfo->DstAddr += LPBAM_DAC_ALIGN_8B_R;
      }
      else
      {
        /* Nothing to do */
      }

      /* Set DMA data size */
      pDescInfo->DataSize = (uint32_t)pConfNode->Config->Size * 4U;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_DAC_MODULE_ENABLED */


#if defined (LPBAM_DMA_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for DMA peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_DMA_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_DMA_FillStructInfo(LPBAM_DMA_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo)
{
  /* LPBAM Configuration DMA Descriptor Items Table */
  static const LPBAM_ItemDesc_t dma_descitem_tbl[] =
  {
    /* LPBAM DMA configuration node ID  */
    {LPBAM_DMA_CONFIG_OFFSET,              LPBAM_DMA_CONFIG_DATASIZE             },
    /* LPBAM DMA base address node ID   */
    {LPBAM_DMA_QUEUE_BASEADDRESS_OFFSET,   LPBAM_DMA_QUEUE_BASEADDRESS_DATASIZE  },
    /* LPBAM DMA offset address node ID */
    {LPBAM_DMA_QUEUE_OFFSETADDRESS_OFFSET, LPBAM_DMA_QUEUE_OFFSETADDRESS_DATASIZE},
    /* LPBAM DMA clear data size node ID */
    {LPBAM_DMA_CLEARDATASIZE_OFFSET,       LPBAM_DMA_CLEARDATASIZE_DATASIZE      },
    /* LPBAM DMA clear flag node ID     */
    {LPBAM_DMA_CLEARFLAG_OFFSET,           LPBAM_DMA_CLEARFLAG_DATASIZE          }
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM DMA configuration node ID */
    case LPBAM_DMA_CONFIG_ID:
    {
      /* Get control register value */
      dummy = pConfNode->pInstance->CCR;

      /* Check DMA state */
      if (pConfNode->Config.State == ENABLE)
      {
        /* Set DMA state */
        dummy |= DMA_CCR_EN;

        /* Configure interrupts */
        MODIFY_REG(dummy, (DMA_CCR_TCIE | DMA_CCR_HTIE), (pConfNode->Config.WakeupIT << DMA_CCR_TCIE_Pos));
      }
      else
      {
        /* Set DMA state */
        dummy |= DMA_CCR_RESET | DMA_CCR_SUSP;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM DMA base address node ID */
    case LPBAM_DMA_QUEUE_BASEADDRESS_ID:
    {
      /* Set value to be written */
      dummy = (pConfNode->Config.HeadQAddress & DMA_CLBAR_LBA);

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM DMA offset address node ID */
    case LPBAM_DMA_QUEUE_OFFSETADDRESS_ID:
    {
      /* Calculate CLLR value */
      dummy = (pConfNode->Config.HeadQAddress & DMA_CLLR_LA);
      dummy |= DMA_CLLR_UT1 | DMA_CLLR_UT2 | DMA_CLLR_UB1 | DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM DMA clear data size node ID */
    case LPBAM_DMA_CLEARDATASIZE_ID:
    {
      /* Calculate clear flag mask */
      dummy = 0U;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM DMA clear flag node ID */
    case LPBAM_DMA_CLEARFLAG_ID:
    {
      /* Calculate clear flag mask */
      dummy = DMA_CFCR_TCF | DMA_CFCR_HTF | DMA_CFCR_TOF;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = dma_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* defined (LPBAM_DMA_MODULE_ENABLED) */


#if defined (LPBAM_GPIO_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for GPIO peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_GPIO_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_GPIO_FillStructInfo(LPBAM_GPIO_ConfNode_t const *const pConfNode,
                                         LPBAM_InfoDesc_t      *const pDescInfo)
{
  /* LPBAM Data GPIO Descriptor Items Table */
  static const LPBAM_ItemDesc_t gpio_descitem_tbl[] =
  {
    /* LPBAM GPIO read pin data transfer node ID  */
    {LPBAM_GPIO_READPIN_OFFSET,  0U},
    /* LPBAM GPIO write pin data transfer node ID */
    {LPBAM_GPIO_WRITEPIN_OFFSET, 0U},
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM GPIO read pin node ID */
    case LPBAM_GPIO_READPIN_ID:
    {
      /* Set DMA source address */
      pDescInfo->SrcAddr  = gpio_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA data size */
      pDescInfo->DataSize = (uint16_t)(pConfNode->Config.Size * 2U);

      break;
    }

    /* LPBAM GPIO write pin node ID */
    case LPBAM_GPIO_WRITEPIN_ID:
    {
      /* Check write pin mode */
      if (pConfNode->Config.IsSequenceMode == LPBAM_GPIO_IS_SINGLE_MODE)
      {
        /* Check pin state */
        if (pConfNode->Config.PinState == (uint16_t)LPBAM_GPIO_PIN_SET)
        {
          dummy = pConfNode->Config.Pin;
        }
        else
        {
          dummy = (uint32_t)pConfNode->Config.Pin << 16U;
        }

        /* Set value to be written */
        *pConfNode->NodeDesc.pSrcVarReg = dummy;

        /* Set DMA source address */
        pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
        /* Set DMA data size */
        pDescInfo->DataSize = 4U;
      }
      else
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
        /* Set DMA data size */
        pDescInfo->DataSize = ((uint32_t)pConfNode->Config.Size) * 4U;
      }

      /* Set DMA destination address */
      pDescInfo->DstAddr  = gpio_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_GPIO_MODULE_ENABLED */


#if defined (LPBAM_I2C_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for I2C peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_I2C_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_I2C_FillStructInfo(LPBAM_I2C_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo)
{
  /* LPBAM Configuration I2C Descriptor Items Table */
  static const LPBAM_ItemDesc_t i2c_descitem_tbl[] =
  {
    /* LPBAM I2C State node ID                  */
    {LPBAM_I2C_STATE_OFFSET,               LPBAM_I2C_STATE_DATASIZE             },
    /* LPBAM I2C configuration transfer node ID */
    {LPBAM_I2C_CONFIG_TRANSACTION_OFFSET,  LPBAM_I2C_CONFIG_TRANSACTION_DATASIZE},
    /* LPBAM I2C  timing node ID                */
    {LPBAM_I2C_TIMING_OFFSET,              LPBAM_I2C_TIMING_DATASIZE            },
    /* LPBAM I2C  trigger node ID               */
    {LPBAM_I2C_TRIG_OFFSET,                LPBAM_I2C_TRIG_DATASIZE              },
    /* LPBAM I2C transmit data node ID          */
    {LPBAM_I2C_TRANSMIT_DATA_OFFSET,       0U                                   },
    /* LPBAM I2C receive data node ID           */
    {LPBAM_I2C_RECEIVE_DATA_OFFSET,        0U                                   },
  };

  uint32_t dummy = 0U;

  /* Automatic end mode state */
  static LPBAM_FunctionalState xfer_cplt_it_state = DISABLE;

  /* Set DMA software request by default */
  pDescInfo->Request   = DMA_REQUEST_SW;
  pDescInfo->Direction = DMA_MEMORY_TO_MEMORY;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM I2C State node ID */
    case LPBAM_I2C_STATE_ID:
    {
      /* Get control register value */
      dummy = pConfNode->pInstance->CR1;

      /* Check I2C state */
      if (pConfNode->Config.State == ENABLE)
      {
        /* Enable I2C instance */
        dummy |= I2C_CR1_PE;

        /* Set wake up interrupts */
        dummy |= pConfNode->Config.WakeupIT;

        /* Check for stop interrupt */
        if ((pConfNode->Config.WakeupIT & LPBAM_I2C_IT_STOP) == LPBAM_I2C_IT_STOP)
        {
          /* Disable automatic clear of stop detection */
          dummy &= ~(I2C_CR1_STOPFACLR);
        }

        /* Check for address match interrupt */
        if ((pConfNode->Config.WakeupIT & LPBAM_I2C_IT_ADDRI) == LPBAM_I2C_IT_ADDRI)
        {
          /* Disable automatic clear of address match */
          dummy &= ~(I2C_CR1_ADDRACLR);
        }

        if ((pConfNode->Config.WakeupIT & LPBAM_I2C_IT_TC) == LPBAM_I2C_IT_TC)
        {
          /* Software end mode will be used as TC interrupt is enabled */
          xfer_cplt_it_state = ENABLE;
        }
        else
        {
          xfer_cplt_it_state = DISABLE;
        }
      }
      else
      {
        /* Disable I2C instance */
        dummy &= ~(I2C_CR1_PE);
      }

      /* Set slave configuration */
      if ((pConfNode->Config.Mode == LPBAM_I2C_SLAVE_TRANSMIT_MODE) ||
          (pConfNode->Config.Mode == LPBAM_I2C_SLAVE_RECEIVE_MODE))
      {
        /* Set value to be written */
        dummy &= ~(I2C_CR1_NACKIE);
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM I2C configuration transfer node ID */
    case LPBAM_I2C_CONFIG_TRANSACTION_ID:
    {
      pDescInfo->Request = LPDMA1_REQUEST_I2C3_EVC;

      /* Get control register value */
      dummy = pConfNode->pInstance->CR2;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy | I2C_CR2_STOP;
      /* Master mode */
      if ((pConfNode->Config.Mode == LPBAM_I2C_MASTER_TRANSMIT_MODE) ||
          (pConfNode->Config.Mode == LPBAM_I2C_MASTER_RECEIVE_MODE))
      {
        /* Set default I2C configuration */
        dummy |= I2C_CR2_START | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | pConfNode->Config.DevAddress |
                 ((uint32_t)pConfNode->Config.Size << I2C_CR2_NBYTES_Pos);

        /* Sequential data transfer is enabled */
        if (pConfNode->Config.SequentialDataState == ENABLE)
        {
          dummy |= I2C_CR2_AUTOEND;
          /* Check if I2C hardware trigger is enabled */
          if (pConfNode->Config.IsFirstSequentialData == LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA)
          {
            /* Check trigger signal */
            if (pConfNode->Config.AutoModeConf.TriggerState != LPBAM_I2C_AUTO_MODE_DISABLE)
            {
              dummy &= ~(I2C_CR2_START);
            }
          }

          /* Check for last sequential data */
          if (pConfNode->Config.IsLastSequentialData == LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA)
          {
            dummy &= ~(I2C_CR2_AUTOEND);
            /* Select request and direction for reloaded data */
            pDescInfo->Direction = DMA_MEMORY_TO_PERIPH;
          }
        }

        /* Data size > 255 */
        if (pConfNode->Config.ReloadDataState == ENABLE)
        {
          /* Is not first reload data transfer */
          if (pConfNode->Config.IsFirstReloadData == LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA)
          {
            /* Select direction for reloaded data */
            pDescInfo->Direction = DMA_MEMORY_TO_PERIPH;
            /* Remove start condition generation */
            dummy &= ~I2C_CR2_START;
          }

          /* Is not last reload data transfer */
          if (pConfNode->Config.IsLastReloadData == LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA)
          {
            /* Set maximum data to be transferred */
            dummy |= (LPBAM_I2C_MAX_DATA_SIZE << I2C_CR2_NBYTES_Pos);
          }
          else
          {
            /* Disable reload data transfer generation */
            dummy &= ~I2C_CR2_RELOAD;

            if (pConfNode->Config.SequentialDataState == DISABLE)
            {
              /* When transfer complete interrupt is requested => use Software end mode */
              /* Check if TC interrupt is request by LPBAM or by HAL/LL layer  */
              if (((pConfNode->pInstance->CR1 & I2C_CR1_TCIE) == I2C_CR1_TCIE) || (xfer_cplt_it_state == ENABLE))
              {
                /* activate Software end mode */
                dummy &= ~(I2C_CR2_AUTOEND);
              }
            }
            else /* Sequential data state is enabled */
            {
              if (pConfNode->Config.IsLastSequentialData == LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA)
              {
                /* activate Software end mode */
                dummy &= ~(I2C_CR2_AUTOEND);
              }
              else /* last sequence and last transfer */
              {
                /* When transfer complete interrupt is requested => use Software end mode */
                /* Check if TC interrupt is request by LPBAM or by HAL/LL layer  */
                if (((pConfNode->pInstance->CR1 & I2C_CR1_TCIE) == I2C_CR1_TCIE) || (xfer_cplt_it_state == ENABLE))
                {
                  /* activate Software end mode */
                  dummy &= ~(I2C_CR2_AUTOEND);
                }
              }
            }
          }
        }
        else
        {
          /* Disable reload data transfer generation */
          dummy &= ~I2C_CR2_RELOAD;

          if (pConfNode->Config.SequentialDataState == DISABLE)
          {
            /* When transfer complete interrupt is requested => use Software end mode */
            /* Check if TC interrupt is request by LPBAM or by HAL/LL layer  */
            if (((pConfNode->pInstance->CR1 & I2C_CR1_TCIE) == I2C_CR1_TCIE) || (xfer_cplt_it_state == ENABLE))
            {
              /* activate Software end mode */
              dummy &= ~(I2C_CR2_AUTOEND);
            }
          }
          else /* Sequential data state is enabled */
          {
            if (pConfNode->Config.IsLastSequentialData == LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA)
            {
              /* activate Software end mode */
              dummy &= ~(I2C_CR2_AUTOEND);
            }
            else
            {
              /* When transfer complete interrupt is requested => use Software end mode */
              /* Check if TC interrupt is request by LPBAM or by HAL/LL layer  */
              if (((pConfNode->pInstance->CR1 & I2C_CR1_TCIE) == I2C_CR1_TCIE) || (xfer_cplt_it_state == ENABLE))
              {
                /* activate Software end mode */
                dummy &= ~(I2C_CR2_AUTOEND);
              }
            }
          }
        }

        /* Check for master receive */
        if (pConfNode->Config.Mode == LPBAM_I2C_MASTER_RECEIVE_MODE)
        {
          /* Check whether reload data and sequential data transfer are enabled or not */
          if ((pConfNode->Config.ReloadDataState == ENABLE) &&
              (pConfNode->Config.SequentialDataState == ENABLE))
          {
            /* Select request and direction for reloaded data */
            if (pConfNode->Config.IsFirstSequentialData == LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA)
            {
              /* Select direction for reloaded data */
              pDescInfo->Direction = DMA_MEMORY_TO_PERIPH;
            }
          }
        }

        /* I2C master receiver mode */
        if (pConfNode->Config.Mode == LPBAM_I2C_MASTER_TRANSMIT_MODE)
        {
          /* Set read operation */
          dummy &= ~I2C_CR2_RD_WRN;
        }

        /* Set addressing mode */
        dummy |= (uint32_t)pConfNode->Config.AddressingMode << I2C_CR2_ADD10_Pos;

        /* Set value to be written */
        *pConfNode->NodeDesc.pSrcVarReg = dummy | pConfNode->Config.DevAddress;
      }
      /* Slave mode */
      else
      {
        /* Set value to be written */
        *pConfNode->NodeDesc.pSrcVarReg &= ~(I2C_CR2_NACK);
      }
      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;
      break;
    }

    /* LPBAM I2C  timing node ID */
    case LPBAM_I2C_TIMING_ID:
    {
      /* Set I2C timing */
      dummy |= pConfNode->Config.Timing;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM I2C  trigger node ID */
    case LPBAM_I2C_TRIG_ID:
    {
      /* Check trigger state */
      if (pConfNode->Config.AutoModeConf.TriggerState == LPBAM_I2C_AUTO_MODE_DISABLE)
      {
        /* Set value to be written */
        dummy = pConfNode->pInstance->AUTOCR & ~(I2C_AUTOCR_TRIGEN);
      }
      else
      {
        /* Set value to be written */
        dummy = pConfNode->pInstance->AUTOCR | I2C_AUTOCR_TRIGEN |
                (pConfNode->Config.AutoModeConf.TriggerSelection << I2C_AUTOCR_TRIGSEL_Pos);

        /* Check trigger polarity */
        if (pConfNode->Config.AutoModeConf.TriggerPolarity == LPBAM_I2C_TRIG_POLARITY_FALLING)
        {
          /* Set trigger polarity */
          dummy |= I2C_AUTOCR_TRIGPOL;
        }
        else
        {
          /* Set trigger polarity */
          dummy &= ~I2C_AUTOCR_TRIGPOL;
        }
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM I2C transmit data node ID */
    case LPBAM_I2C_TRANSMIT_DATA_ID:
    {
      /* Select request for transmit data node */
      pDescInfo->Request  = LPDMA1_REQUEST_I2C3_TX;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = pConfNode->Config.Size;

      /* Overwrite data size for not last reload data transfer */
      if ((pConfNode->Config.ReloadDataState == ENABLE) &&
          (pConfNode->Config.IsLastReloadData != LPBAM_I2C_IS_LAST_RELOADED_DATA))
      {
        /* Overwrite data size */
        pDescInfo->DataSize = LPBAM_I2C_MAX_DATA_SIZE;
      }

      break;
    }

    /* LPBAM I2C receive data node ID */
    case LPBAM_I2C_RECEIVE_DATA_ID:
    {
      /* Select request for receive data node */
      pDescInfo->Request  = LPDMA1_REQUEST_I2C3_RX;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = i2c_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA data size */
      pDescInfo->DataSize = pConfNode->Config.Size;

      /* Overwrite data size for not last reload data transfer */
      if ((pConfNode->Config.ReloadDataState == ENABLE) &&
          (pConfNode->Config.IsLastReloadData != LPBAM_I2C_IS_LAST_RELOADED_DATA))
      {
        /* Overwrite data size */
        pDescInfo->DataSize = LPBAM_I2C_MAX_DATA_SIZE;
      }

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* defined (LPBAM_I2C_MODULE_ENABLED) */


#if defined (LPBAM_LPTIM_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for LPTIM peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_LPTIM_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_LPTIM_FillStructInfo(LPBAM_LPTIM_ConfNode_t const *const pConfNode,
                                          LPBAM_InfoDesc_t       *const pDescInfo)
{
  /* LPBAM Configuration LPTIM Descriptor Items Table */
  static const LPBAM_ItemDesc_t lptim_descitem_tbl[] =
  {
    /* LPBAM LPTIM Configuration node ID       */
    {LPBAM_LPTIM_CONFIG_OFFSET,            LPBAM_LPTIM_CONFIG_DATASIZE           },
    /* LPBAM LPTIM Update Period node ID      */
    {LPBAM_LPTIM_CONFIG_PERIOD_OFFSET,     LPBAM_LPTIM_CONFIG_PERIOD_DATASIZE    },
    /* LPBAM LPTIM Update Pulse node ID       */
    {LPBAM_LPTIM_CONFIG_PULSE_OFFSET,      LPBAM_LPTIM_CONFIG_PULSE_DATASIZE     },
    /* LPBAM LPTIM Update Repetition node ID  */
    {LPBAM_LPTIM_CONFIG_REPETITION_OFFSET, LPBAM_LPTIM_CONFIG_REPETITION_DATASIZE},
    /* LPBAM LPTIM Get Counter node ID       */
    {LPBAM_LPTIM_GET_COUNTER_OFFSET,       0U                                    },
    /* LPBAM LPTIM Input Capture node ID      */
    {LPBAM_LPTIM_INPUT_CAPTURE_OFFSET,     0U                                    },
    /* LPBAM LPTIM Wakeup Interrupt node ID   */
    {LPBAM_LPTIM_WAKEUP_IT_OFFSET,         LPBAM_LPTIM_WAKEUP_IT_DATASIZE        },
    /* LPBAM LPTIM Clear Flag node ID         */
    {LPBAM_LPTIM_CLEARFLAG_OFFSET,         LPBAM_LPTIM_CLEARFLAG_DATASIZE        }
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM LPTIM Update Period node ID */
    case LPBAM_LPTIM_CONFIG_ID:
    {
      /* Get control register value */
      dummy = pConfNode->pInstance->CR;

      /* Check LPTIM state */
      if (pConfNode->Config.State == ENABLE)
      {
        /* Set LPTIM update event request */
        pDescInfo->Request = DMA_REQUEST_SW;

        /* Set LPTIM state */
        dummy |= LPTIM_CR_ENABLE;

        /* Check LPTIM start mode update */
        if (pConfNode->Config.UpdateStartMode == ENABLE)
        {
          if (pConfNode->Config.StartMode == LPBAM_LPTIM_START_SINGLE)
          {
            /* Configure LPTIM to start in single mode */
            dummy |= LPTIM_CR_SNGSTRT;
          }
          else
          {
            /* Configure LPTIM to start in continuous mode */
            dummy |= LPTIM_CR_CNTSTRT;
          }
        }
      }
      else
      {
        /* Set LPTIM state */
        dummy &= ~(LPTIM_CR_ENABLE);

        /* Check LPTIM mode */
        if (pConfNode->Config.Mode == LPBAM_LPTIM_UE_MODE)
        {
          /* Check LPTIM instance */
          if (pConfNode->pInstance == LPTIM1)
          {
            /* Set LPTIM1 update event request */
            pDescInfo->Request = LPDMA1_REQUEST_LPTIM1_UE;
          }
          else
          {
            /* Set LPTIM3 update event request */
            pDescInfo->Request = LPDMA1_REQUEST_LPTIM3_UE;
          }
        }
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM LPTIM Update Period node ID */
    case LPBAM_LPTIM_UPDATE_PERIOD_ID:
    {
      /* Check LPTIM instance */
      if (pConfNode->pInstance == LPTIM1)
      {
        /* Set update event DMA request */
        pDescInfo->Request  = LPDMA1_REQUEST_LPTIM1_UE;
      }
      else
      {
        /* Set update event DMA request */
        pDescInfo->Request  = LPDMA1_REQUEST_LPTIM3_UE;
      }

      /* Check LPTIM mode */
      if (pConfNode->Config.Mode == LPBAM_LPTIM_UE_MODE)
      {
        /* Set DMA SW request */
        pDescInfo->Request  = DMA_REQUEST_SW;
      }

      /* Set node information */
      if (pConfNode->NodeDesc.pBuff == NULL)
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      }
      else
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = (uint32_t)&pConfNode->NodeDesc.pBuff[0U];
      }
      /* Set DMA destination address */
      pDescInfo->DstAddr  = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = pConfNode->Config.Size * 2U;

      break;
    }

    /* LPBAM LPTIM Update Pulse node ID */
    case LPBAM_LPTIM_UPDATE_PULSE_ID:
    {
      /* Check LPTIM instance */
      if (pConfNode->pInstance == LPTIM1)
      {
        /* Set update event DMA request */
        pDescInfo->Request  = LPDMA1_REQUEST_LPTIM1_UE;
      }
      else
      {
        /* Set update event DMA request */
        pDescInfo->Request  = LPDMA1_REQUEST_LPTIM3_UE;
      }

      /* Set node information */
      if (pConfNode->NodeDesc.pBuff == NULL)
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      }
      else
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = (uint32_t)&pConfNode->NodeDesc.pBuff[0U];
      }

      /* LPTIM channel 1 */
      if (pConfNode->Config.Channel == LPBAM_LPTIM_CHANNEL_1)
      {
        /* Set DMA destination address */
        pDescInfo->DstAddr = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance;
      }
      /* LPTIM channel 2 */
      else
      {
        /* Set DMA destination address */
        pDescInfo->DstAddr = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_LPTIM_CHANNEL_OFFSET;
      }
      /* Set DMA data size */
      pDescInfo->DataSize = pConfNode->Config.Size * 2U;

      break;
    }

    /* LPBAM LPTIM Update Repetition node ID */
    case LPBAM_LPTIM_UPDATE_REPETITION_ID:
    {
      /* Check LPTIM instance */
      if (pConfNode->pInstance == LPTIM1)
      {
        /* Set update event DMA request */
        pDescInfo->Request = LPDMA1_REQUEST_LPTIM1_UE;
      }
      else
      {
        /* Set update event DMA request */
        pDescInfo->Request = LPDMA1_REQUEST_LPTIM3_UE;
      }

      /* Check LPTIM mode */
      if (pConfNode->Config.Mode == LPBAM_LPTIM_UE_MODE)
      {
        /* Set DMA SW request */
        pDescInfo->Request = DMA_REQUEST_SW;
      }

      /* Set node information */
      if (pConfNode->NodeDesc.pBuff == NULL)
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      }
      else
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = (uint32_t)&pConfNode->NodeDesc.pBuff[0U];
      }

      /* Set DMA destination address */
      pDescInfo->DstAddr = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                           (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM LPTIM Get Counter node ID */
    case LPBAM_LPTIM_GET_COUNTER_ID:
    {
      /* Set DMA source address */
      pDescInfo->SrcAddr = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                           (uint32_t)pConfNode->pInstance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)&pConfNode->NodeDesc.pBuff[0U];
      /* Set DMA data size */
      pDescInfo->DataSize = pConfNode->Config.CaptureCount * 2U;

      break;
    }

    /* LPBAM LPTIM Input Capture node ID */
    case LPBAM_LPTIM_INPUT_CAPTURE_ID:
    {
      /* LPTIM channel 1 */
      if (pConfNode->Config.Channel == LPBAM_LPTIM_CHANNEL_1)
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance;

        /* Check LPTIM instance */
        if (pConfNode->pInstance == LPTIM1)
        {
          /* Set input capture DMA request */
          pDescInfo->Request = LPDMA1_REQUEST_LPTIM1_IC1;
        }
        else
        {
          /* Set input capture DMA request */
          pDescInfo->Request = LPDMA1_REQUEST_LPTIM3_IC1;
        }
      }
      /* LPTIM channel 2 */
      else
      {
        /* Set DMA source address */
        pDescInfo->SrcAddr = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                             (uint32_t)pConfNode->pInstance + LPBAM_LPTIM_CHANNEL_OFFSET;

        /* Check LPTIM instance */
        if (pConfNode->pInstance == LPTIM1)
        {
          /* Set input capture DMA request */
          pDescInfo->Request = LPDMA1_REQUEST_LPTIM1_IC2;
        }
        else
        {
          /* Set input capture DMA request */
          pDescInfo->Request = LPDMA1_REQUEST_LPTIM3_IC2;
        }
      }

      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)&pConfNode->NodeDesc.pBuff[0U];
      /* Set DMA data size */
      pDescInfo->DataSize = pConfNode->Config.CaptureCount * 2U;

      break;
    }

    /* LPBAM LPTIM Wakeup Interrupt node ID */
    case LPBAM_LPTIM_WAKEUP_IT_ID:
    {
      /* Get enable interrupt register value */
      dummy = pConfNode->pInstance->DIER;

      /* Set value to be written */
      dummy |= pConfNode->Config.WakeupIT;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM LPTIM Clear Flag node ID */
    case LPBAM_LPTIM_CLEARFLAG_ID:
    {
      /* Set clear update event mask value */
      dummy = LPTIM_ISR_UE;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = lptim_descitem_tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_LPTIM_MODULE_ENABLED */


#if defined (LPBAM_OPAMP_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for OPAMP peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_OPAMP_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_OPAMP_FillStructInfo(LPBAM_OPAMP_ConfNode_t const *const pConfNode,
                                          LPBAM_InfoDesc_t       *const pDescInfo)
{
  /* LPBAM Configuration OPAMP Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_OPAMP_DescItem_Tbl[] =
  {
    /* LPBAM OPAMP configuration node ID */
    {LPBAM_OPAMP_CONFIG_OFFSET, LPBAM_OPAMP_CONFIG_DATASIZE}
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM OPAMP configuration node ID */
    case  LPBAM_OPAMP_CONFIG_ID:
    {
      /* Get register value */
      dummy = (((uint32_t)pConfNode->Instance->CSR & ~(LPBAM_OPAMP_CONFIG_MASK)) | ((uint32_t)pConfNode->Config.State) |
               pConfNode->Config.Mode | pConfNode->Config.NonInvertingInput | pConfNode->Config.InvertingInput         |
               pConfNode->Config.PgaGain);

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_OPAMP_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->Instance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_OPAMP_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_OPAMP_MODULE_ENABLED */


#if defined (LPBAM_SPI_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for SPI peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_SPI_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_SPI_FillStructInfo(LPBAM_SPI_ConfNode_t const *const pConfNode,
                                        LPBAM_InfoDesc_t     *const pDescInfo)
{
  /* LPBAM Configuration SPI Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_SPI_DescItem_Tbl[] =
  {
    /* LPBAM SPI State node ID                       */
    {LPBAM_SPI_STATE_OFFSET,         LPBAM_SPI_STATE_DATASIZE       },
    /* LPBAM SPI Data Size node ID                   */
    {LPBAM_SPI_DATASIZE_OFFSET,      LPBAM_SPI_DATASIZE_DATASIZE    },
    /* LPBAM SPI Configuration Frame node ID         */
    {LPBAM_SPI_CONFIG_FRAME_OFFSET,  LPBAM_SPI_CONFIG_FRAME_DATASIZE},
    /* LPBAM SPI Configuration Communication node ID */
    {LPBAM_SPI_CONFIG_COM_OFFSET,    LPBAM_SPI_CONFIG_COM_DATASIZE  },
    /* LPBAM SPI Trigger node ID                     */
    {LPBAM_SPI_TRIG_OFFSET,          LPBAM_SPI_TRIG_DATASIZE        },
    /* LPBAM SPI Wakeup Interrupt ID                 */
    {LPBAM_SPI_WAKEUP_IT_OFFSET,     LPBAM_SPI_WAKEUP_IT_DATASIZE   },
    /* LPBAM SPI Start node ID                       */
    {LPBAM_SPI_START_OFFSET,         LPBAM_SPI_START_DATASIZE       },
    /* LPBAM SPI Transmit Data Transfer node ID      */
    {LPBAM_SPI_TRANSMIT_DATA_OFFSET, 0U                             },
    /* LPBAM SPI Receive Data Transfer node ID       */
    {LPBAM_SPI_RECEIVE_DATA_OFFSET,  0U                             },
    /* LPBAM SPI Clear Flag node ID                  */
    {LPBAM_SPI_CLEARFLAG_OFFSET,     LPBAM_SPI_CLEARFLAG_DATASIZE   }
  };

  uint32_t dummy = 0U;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM SPI State node ID */
    case LPBAM_SPI_STATE_ID:
    {
      /* Get register value */
      dummy = pConfNode->pInstance->CR1;

      /* Check SPI state */
      if (pConfNode->Config.State == DISABLE)
      {
        /* Set value to be written */
        dummy &= (~SPI_CR1_SPE);
      }
      /* Check SPI trigger state */
      else
      {
        /* Set value to be written */
        dummy |= SPI_CR1_SPE;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Data Size node ID */
    case LPBAM_SPI_DATASIZE_ID:
    {
      /* Get SPI size */
      dummy = pConfNode->Config.Size;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Configuration Frame node ID */
    case LPBAM_SPI_CONFIG_FRAME_ID:
    {
      /* Get register value */
      dummy = pConfNode->pInstance->CFG1 & (~(SPI_CFG1_BPASS | SPI_CFG1_MBR | SPI_CFG1_DSIZE | SPI_CFG1_TXDMAEN |
                                              SPI_CFG1_RXDMAEN));

      /* Set data width and baudrate prescaler */
      dummy |= pConfNode->Config.DataSize | ((uint32_t)pConfNode->Config.BaudRatePrescaler << SPI_CFG1_MBR_Pos);

      /* Check SPI direction */
      if (pConfNode->Config.Direction == LPBAM_SPI_DIRECTION_2LINES)
      {
        /* Full duplex */
        dummy |= SPI_CFG1_TXDMAEN | SPI_CFG1_RXDMAEN;
      }
      else if (pConfNode->Config.Direction == LPBAM_SPI_DIRECTION_2LINES_TXONLY)
      {
        /* Simplex transmit */
        dummy |= SPI_CFG1_TXDMAEN;
      }
      else
      {
        /* Simplex receive */
        dummy |= SPI_CFG1_RXDMAEN;
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Configuration Communication node ID */
    case LPBAM_SPI_CONFIG_COM_ID:
    {
      /* Get register value */
      dummy = pConfNode->pInstance->CFG2 & (~(SPI_CFG2_COMM | SPI_CFG2_LSBFRST | SPI_CFG2_LSBFRST | SPI_CFG2_CPHA |
                                              SPI_CFG2_CPOL));

      /* Set clock polarity, clock phase, first bit and direction */
      dummy |= (((uint32_t)pConfNode->Config.CLKPolarity << SPI_CFG2_CPOL_Pos)    |
                ((uint32_t)pConfNode->Config.CLKPhase    << SPI_CFG2_CPHA_Pos)    |
                ((uint32_t)pConfNode->Config.FirstBit    << SPI_CFG2_LSBFRST_Pos) |
                ((uint32_t)pConfNode->Config.Direction   << SPI_CFG2_COMM_Pos));

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Trigger node ID */
    case LPBAM_SPI_TRIG_ID:
    {
      /* Check SPI trigger state */
      if (pConfNode->Config.AutoModeConf.TriggerState == LPBAM_SPI_AUTO_MODE_ENABLE)
      {
        /* Get register value */
        dummy = pConfNode->pInstance->AUTOCR & (~(SPI_AUTOCR_TRIGEN | SPI_AUTOCR_TRIGPOL | SPI_AUTOCR_TRIGSEL));

        /* Set trigger information */
        dummy |= ((pConfNode->Config.AutoModeConf.TriggerSelection << SPI_AUTOCR_TRIGSEL_Pos)          |
                  ((uint32_t)pConfNode->Config.AutoModeConf.TriggerPolarity << SPI_AUTOCR_TRIGPOL_Pos) |
                  SPI_AUTOCR_TRIGEN);
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Wakeup Interrupt ID */
    case LPBAM_SPI_WAKEUP_IT_ID:
    {
      /* Set wake up value */
      dummy = pConfNode->pInstance->IER | pConfNode->Config.WakeupIT;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Start node ID */
    case LPBAM_SPI_START_ID:
    {
      /* Calculate SPI start value */
      dummy = pConfNode->pInstance->CR1 | SPI_CR1_CSTART | SPI_CR1_SPE;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM SPI Transmit Data Transfer node ID */
    case LPBAM_SPI_TRANSMIT_DATA_ID:
    {
      /* Select request for transmit data node */
      pDescInfo->Request  = LPDMA1_REQUEST_SPI3_TX;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;

      /* Check SPI data width */
      if (pConfNode->Config.DataSize == LPBAM_SPI_DATASIZE_16BIT)
      {
        /* Set DMA data size */
        pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size * 2U;
      }
      else
      {
        /* Set DMA data size */
        pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size;
      }

      break;
    }

    /* LPBAM SPI Receive Data Transfer node ID */
    case LPBAM_SPI_RECEIVE_DATA_ID:
    {
      /* Select request for receive data node */
      pDescInfo->Request  = LPDMA1_REQUEST_SPI3_RX;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;

      /* Check SPI data width */
      if (pConfNode->Config.DataSize == LPBAM_SPI_DATASIZE_16BIT)
      {
        /* Set DMA data size */
        pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size * 2U;
      }
      else
      {
        /* Set DMA data size */
        pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size;
      }

      break;
    }

    /* LPBAM SPI Clear Flag node ID */
    case LPBAM_SPI_CLEARFLAG_ID:
    {
      /* Get SPI clear flag mask */
      dummy = LPBAM_SPI_CLEARFLAG_MASK;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_SPI_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_SPI_MODULE_ENABLED */


#if defined (LPBAM_UART_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for UART peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_UART_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_UART_FillStructInfo(LPBAM_UART_ConfNode_t const *const pConfNode,
                                         LPBAM_InfoDesc_t      *const pDescInfo)
{
  /* LPBAM Configuration UART Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_UART_DescItem_Tbl[] =
  {
    /* LPBAM UART Configuration State node ID            */
    {LPBAM_UART_CONFIG_OFFSET,         LPBAM_UART_CONFIG_DATASIZE        },
    /* LPBAM UART Configuration Autonomous mode node ID  */
    {LPBAM_UART_AUTONOMOUSMODE_OFFSET, LPBAM_UART_AUTONOMOUSMODE_DATASIZE},
    /* LPBAM UART Receive Data Transfer node ID          */
    {LPBAM_UART_RECEIVE_OFFSET,        0U                                },
    /* LPBAM UART Transmit Data Transfer node ID         */
    {LPBAM_UART_TRANSMIT_OFFSET,       0U                                },
    /* LPBAM UART Clear Flag node ID                     */
    {LPBAM_UART_CLEARFLAG_OFFSET,      LPBAM_UART_CLEARFLAG_DATASIZE     }
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM UART configuration state node ID */
    case LPBAM_UART_CONFIG_ID:
    {
      /* Get register value */
      dummy = pConfNode->pInstance->CR1 & (~(USART_CR1_UE | USART_CR1_IDLEIE | USART_CR1_RXNEIE | USART_CR1_TCIE |
                                             USART_CR1_TXEIE | USART_CR1_TXFEIE | USART_CR1_RXFFIE));

      /* Calculate the configuration mask */
      if (pConfNode->Config.State == ENABLE)
      {
        /* Check UART direction */
        if (pConfNode->Config.Mode == LPBAM_UART_MODE_TX_RX)
        {
          /* Full duplex wake up interrupt mask */
          dummy |= (((pConfNode->Config.WakeupIT                                                                &
                      (LPBAM_UART_IT_TXFE | LPBAM_UART_IT_RXFF)) << LPBAM_UART_FIFO_IT_POS)                     |
                    ((pConfNode->Config.WakeupIT & (LPBAM_UART_IT_IDLE | LPBAM_UART_IT_TC | LPBAM_UART_IT_TXFNF |
                                                    LPBAM_UART_IT_RXFNE)) << USART_CR1_IDLEIE_Pos) | USART_CR1_UE);
        }
        else if (pConfNode->Config.Mode == LPBAM_UART_MODE_TX)
        {
          /* Simplex Tx wake up interrupt mask */
          dummy |= (((pConfNode->Config.WakeupIT & LPBAM_UART_IT_TXFE) << LPBAM_UART_FIFO_IT_POS)                     |
                    ((pConfNode->Config.WakeupIT & (LPBAM_UART_IT_TC | LPBAM_UART_IT_TXFNF)) << USART_CR1_IDLEIE_Pos) |
                    USART_CR1_UE);
        }
        else
        {
          /* Simplex Rx wake up interrupt mask */
          dummy |= (((pConfNode->Config.WakeupIT & LPBAM_UART_IT_RXFF) << LPBAM_UART_FIFO_IT_POS) |
                    ((pConfNode->Config.WakeupIT &
                      (LPBAM_UART_IT_IDLE | LPBAM_UART_IT_RXFNE)) << USART_CR1_IDLEIE_Pos) | USART_CR1_UE);
        }
      }

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM UART clear flag node ID */
    case LPBAM_UART_CLEARFLAG_ID:
    {
      /* Calculate the clear flag mask */
      dummy = USART_ICR_PECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_ORECF | USART_ICR_IDLECF | USART_ICR_TCCF |
              USART_ICR_CTSCF | USART_ICR_CMCF;

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM UART configuration autonomous mode node ID */
    case LPBAM_UART_AUTONOMOUSMODE_ID:
    {
      /* Get register value */
      dummy = (pConfNode->pInstance->AUTOCR & (~(USART_AUTOCR_TDN | USART_AUTOCR_TRIGPOL |
                                                 USART_AUTOCR_TRIGEN | USART_AUTOCR_TRIGSEL)));

      /* Set autonomous mode info */
      dummy |= (((uint32_t)pConfNode->Config.AutoModeConf.AutonomousModeState << USART_AUTOCR_TRIGEN_Pos)  |
                ((uint32_t)pConfNode->Config.AutoModeConf.TriggerSelection    << USART_AUTOCR_TRIGSEL_Pos) |
                ((uint32_t)pConfNode->Config.AutoModeConf.TriggerPolarity     << USART_AUTOCR_TRIGPOL_Pos) |
                ((uint32_t)pConfNode->Config.AutoModeConf.DataSize            << USART_AUTOCR_TDN_Pos));

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    /* LPBAM UART transmit data transfer node ID */
    case LPBAM_UART_TRANSMIT_DATA_ID:
    {
      /* Set node information */
      pDescInfo->Request  = LPDMA1_REQUEST_LPUART1_TX;
      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size;

      break;
    }

    /* LPBAM UART receive data transfer node ID */
    case LPBAM_UART_RECEIVE_DATA_ID :
    {
      /* Set node information */
      pDescInfo->Request  = LPDMA1_REQUEST_LPUART1_RX;
      /* Set DMA source address */
      pDescInfo->SrcAddr  = LPBAM_UART_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = (uint32_t)pConfNode->NodeDesc.pBuff;
      /* Set DMA data size */
      pDescInfo->DataSize = (uint32_t)pConfNode->Config.Size;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_UART_MODULE_ENABLED */


#if defined (LPBAM_VREFBUF_MODULE_ENABLED)
/**
  * @brief  Fill the structure information for VREFBUF peripheral nodes.
  * @param  pConfNode    : [IN]  Pointer to a LPBAM_VREFBUF_ConfNode_t structure that contains node configuration.
  * @param  pDescInfo    : [OUT] Pointer to a LPBAM_InfoDesc_t structure that contains descriptor information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_VREFBUF_FillStructInfo(LPBAM_VREFBUF_ConfNode_t const *const pConfNode,
                                            LPBAM_InfoDesc_t         *const pDescInfo)
{
  /* LPBAM Configuration VREFBUF Descriptor Items Table */
  static const LPBAM_ItemDesc_t LPBAM_VREFBUF_DescItem_Tbl[] =
  {
    /* LPBAM VREFBUF mode selection node ID  */
    {LPBAM_VREFBUF_MODE_OFFSET, LPBAM_VREFBUF_MODE_DATASIZE}
  };

  uint32_t dummy;

  /* Check node ID */
  switch (pConfNode->NodeDesc.NodeInfo.NodeID)
  {
    /* LPBAM VREFBUF mode selection node ID  */
    case LPBAM_VREFBUF_MODE_ID:
    {
      /* Get register value */
      dummy = (((uint32_t)pConfNode->pInstance->CSR & LPBAM_VREFBUF_CSR_MASK) | pConfNode->Config.BufferMode);

      /* Set value to be written */
      *pConfNode->NodeDesc.pSrcVarReg = dummy;

      /* Set DMA source address */
      pDescInfo->SrcAddr  = (uint32_t)pConfNode->NodeDesc.pSrcVarReg;
      /* Set DMA destination address */
      pDescInfo->DstAddr  = LPBAM_VREFBUF_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegOffset +
                            (uint32_t)pConfNode->pInstance;
      /* Set DMA data size */
      pDescInfo->DataSize = LPBAM_VREFBUF_DescItem_Tbl[pConfNode->NodeDesc.NodeInfo.NodeID].RegDataSize;

      break;
    }

    default:
    {
      /* return status */
      return LPBAM_INVALID_ID;
      break;
    }
  }

  return LPBAM_OK;
}
#endif /* LPBAM_VREFBUF_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */
