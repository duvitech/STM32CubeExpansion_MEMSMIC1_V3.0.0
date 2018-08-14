/**
******************************************************************************
* @file    usbd_conf_f7.c
* @author  Central Labs
* @version V3.0.0
* @date    21-March-2018
* @brief   This file implements the USB Device library callbacks and MSP
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "usbd_core.h"

PCD_HandleTypeDef hpcd;

/*******************************************************************************
PCD BSP Routines
*******************************************************************************/
/**
* @brief  Initializes the PCD MSP.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
  /* Note: On STM32F401-Discovery board only USB OTG FS core is supported. */
  GPIO_InitTypeDef  GPIO_InitStruct;
  
#ifdef USE_STM32F769_DISCO
	  
    /* Configure USB FS GPIOs */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    
    /* CLK */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* D0 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* D1 D2 D3 D4 D5 D6 D7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_5 |\
      GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* STP */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* NXT */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    
    /* DIR */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
   
    /* Enable USB HS Clocks */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    
    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 0x05, 0);
    
    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);   
#else
  if(hpcd->Instance == USB_OTG_FS)
  {
    /* Configure USB FS GPIOs */
    __GPIOA_CLK_ENABLE();
    
    /* Configure DM DP Pins */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
    
    /* Enable USB FS Clocks */ 
    __USB_OTG_FS_CLK_ENABLE();
    
    /* Set USBFS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
    
    /* Enable USBFS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  } 
#endif
}

/**
* @brief  DeInitializes the PCD MSP.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
#ifdef USE_STM32F769_DISCO
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
    __HAL_RCC_SYSCFG_CLK_DISABLE(); 
	
#else
  if(hpcd->Instance == USB_OTG_FS)
  {  
    /* Disable USB FS Clocks */ 
    __USB_OTG_FS_CLK_DISABLE();
  }
#endif
}

/*******************************************************************************
LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/


/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SetupStage(hpcd->pData, (uint8_t *)hpcd->Setup);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_DataOutStage(hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_DataInStage(hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
} 

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SOF(hpcd->pData);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{ 
  USBD_SpeedTypeDef speed = USBD_SPEED_FULL;
  
  /*Set USB Current Speed*/
  switch (hpcd->Init.speed)
  {
  case PCD_SPEED_HIGH:
    speed = USBD_SPEED_HIGH;
    break;
    
  case PCD_SPEED_FULL:
    speed = USBD_SPEED_FULL;    
    break;
    
  default:
    speed = USBD_SPEED_FULL;
    break;
  }
  USBD_LL_SetSpeed(hpcd->pData, speed);  
  
  /*Reset Device*/
  USBD_LL_Reset(hpcd->pData);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_Suspend(hpcd->pData);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_Resume(hpcd->pData);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_IsoOUTIncomplete(hpcd->pData, epnum);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	HAL_GPIO_TogglePin(GPIOJ, GPIO_PIN_13);
  USBD_LL_IsoINIncomplete(hpcd->pData, epnum);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_DevConnected(hpcd->pData);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_DevDisconnected(hpcd->pData);
}

/*******************************************************************************
LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/
/**
* @brief  USBD_LL_Init 
*         Initialize the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_Init (USBD_HandleTypeDef *pdev)
{
	
  /* Change Systick prioity */
  NVIC_SetPriority (SysTick_IRQn, 0);  
	
#ifdef USE_STM32F769_DISCO
	/* Set LL Driver parameters */
  hpcd.Instance = USB_OTG_HS;
  hpcd.Init.dev_endpoints = 6;
  hpcd.Init.use_dedicated_ep1 = 0;
  hpcd.Init.ep0_mps = 0x40;
  
  /* Be aware that enabling DMA mode will result in data being sent only by
  multiple of 4 packet sizes. This is due to the fact that USB DMA does
  not allow sending data from non word-aligned addresses.
  For this specific application, it is advised to not enable this option
  unless required. */
  hpcd.Init.dma_enable = 0;
  hpcd.Init.low_power_enable = 0;
  hpcd.Init.lpm_enable = 0;
  
  hpcd.Init.phy_itface = PCD_PHY_ULPI; 
  
  hpcd.Init.Sof_enable = 0;
  hpcd.Init.speed = PCD_SPEED_HIGH;
	// hpcd.Init.speed = PCD_SPEED_HIGH_IN_FULL;
  hpcd.Init.vbus_sensing_enable = 1;
  
  /* Link The driver to the stack */
  hpcd.pData = pdev;
  pdev->pData = &hpcd;
  
  /* Initialize LL Driver */
  HAL_PCD_Init(&hpcd);
  if(hpcd.Init.speed == PCD_SPEED_HIGH){
		HAL_PCDEx_SetRxFiFo(&hpcd, 0x400);
		HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x400);
		HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x400);
	}else{
		HAL_PCD_SetRxFiFo(&hpcd, 0x80);
		HAL_PCD_SetTxFiFo(&hpcd, 0, 0x40);
		HAL_PCD_SetTxFiFo(&hpcd, 1, 0x64);
	}
#else
  
  /*Set LL Driver parameters */
  hpcd.Instance = USB_OTG_FS;
  hpcd.Init.dev_endpoints = 3; 
  hpcd.Init.use_dedicated_ep1 = 0;
  hpcd.Init.ep0_mps = 0x40;  
  hpcd.Init.dma_enable = 0;
  hpcd.Init.low_power_enable = 0;
  hpcd.Init.phy_itface = PCD_PHY_EMBEDDED; 
  hpcd.Init.Sof_enable = 0;
  hpcd.Init.speed = PCD_SPEED_FULL;
  hpcd.Init.vbus_sensing_enable = 0;
  /* Link The driver to the stack */
  hpcd.pData = pdev;
  pdev->pData = &hpcd;
  /*Initialize LL Driver */
  HAL_PCD_Init(&hpcd);
  
  HAL_PCD_SetRxFiFo(&hpcd, 0x80);
  HAL_PCD_SetTxFiFo(&hpcd, 0, 0x40);
  HAL_PCD_SetTxFiFo(&hpcd, 1, 0x64);

 #endif
  return USBD_OK;
}

/**
* @brief  USBD_LL_DeInit 
*         De-Initialize the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_DeInit (USBD_HandleTypeDef *pdev)
{
  HAL_PCD_DeInit(pdev->pData);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_Start 
*         Start the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
  HAL_PCD_Start(pdev->pData);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_Stop 
*         Stop the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_Stop (USBD_HandleTypeDef *pdev)
{
  HAL_PCD_Stop(pdev->pData);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_OpenEP 
*         Open an endpoint of the Low Level Driver.
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number
* @param  ep_type: Endpoint Type
* @param  ep_mps: Endpoint Max Packet Size                 
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_OpenEP  (USBD_HandleTypeDef *pdev, 
                                     uint8_t  ep_addr,                                      
                                     uint8_t  ep_type,
                                     uint16_t ep_mps)
{
  HAL_PCD_EP_Open(pdev->pData, 
                  ep_addr, 
                  ep_mps, 
                  ep_type);
  
  return USBD_OK; 
}

/**
* @brief  USBD_LL_CloseEP 
*         Close an endpoint of the Low Level Driver.
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number      
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_CloseEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
  HAL_PCD_EP_Close(pdev->pData, ep_addr);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_FlushEP 
*         Flush an endpoint of the Low Level Driver.
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number      
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_FlushEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
  HAL_PCD_EP_Flush(pdev->pData, ep_addr);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_StallEP 
*         Set a Stall condition on an endpoint of the Low Level Driver.
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number      
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_StallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
  HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_ClearStallEP 
*         Clear a Stall condition on an endpoint of the Low Level Driver.
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number      
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_ClearStallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
  HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);  
  return USBD_OK; 
}

/**
* @brief  USBD_LL_IsStallEP 
*         Return Stall condition.
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number      
* @retval Stall (1: yes, 0: No)
*/
uint8_t USBD_LL_IsStallEP (USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
  PCD_HandleTypeDef *hpcd = pdev->pData; 
  
  if((ep_addr & 0x80) == 0x80)
  {
    return hpcd->IN_ep[ep_addr & 0x7F].is_stall; 
  }
  else
  {
    return hpcd->OUT_ep[ep_addr & 0x7F].is_stall; 
  }
}
/**
* @brief  USBD_LL_SetDevAddress 
*         Assign an USB address to the device
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number      
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_SetUSBAddress (USBD_HandleTypeDef *pdev, uint8_t dev_addr)   
{
  HAL_PCD_SetAddress(pdev->pData, dev_addr);
  return USBD_OK; 
}

/**
* @brief  USBD_LL_Transmit 
*         Transmit data over an endpoint
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number
* @param  pbuf:pointer to data to be sent    
* @param  size: data size    
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_Transmit (USBD_HandleTypeDef *pdev, 
                                      uint8_t  ep_addr,                                      
                                      uint8_t  *pbuf,
                                      uint16_t  size)
{
  HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;   
}

/**
* @brief  USBD_LL_PrepareReceive 
*         prepare an endpoint for reception
* @param  pdev: device handle
* @param  ep_addr: Endpoint Number
* @param  pbuf:pointer to data to be received    
* @param  size: data size              
* @retval USBD Status
*/
USBD_StatusTypeDef  USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, 
                                           uint8_t  ep_addr,                                      
                                           uint8_t  *pbuf,
                                           uint16_t  size)
{
  HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;   
}

/**
* @brief  USBD_LL_GetRxDataSize 
*         Return the last transfered packet size.
* @param  phost: Device handle
* @param  ep_addr: Endpoint Number
* @retval Recived Data Size
*/
uint32_t USBD_LL_GetRxDataSize  (USBD_HandleTypeDef *pdev, uint8_t  ep_addr)  
{
  return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}

/**
* @brief  USBD_LL_Delay 
*         Delay routine for the USB Device Library
* @param  Delay: Delay in ms
* @retval None
*/
void  USBD_LL_Delay (uint32_t Delay)
{
  HAL_Delay(Delay);  
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
