/*
   main.c

   NuSYSTEM sample nu1

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
   */

#include <nusys.h>
#include "main.h"

#include <nualstl_n.h>

/* Declaration of the prototype  */
void stage00(int);

/* Declaration of the external function  */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

/* The global variable  */
NUContData	contdata[1]; /* Read data of 1 controller  */
u8 contPattern;		     /* The pattern connected to the controller  */

int ptr_buf[NU_AU_SAMPLE_SIZE];
int tune_buf[NU_AU_SONG_SIZE];
int sfx_buf[NU_AU_SE_SIZE];

void initAudio(void)
{
    // Special thanks to Datdupa46 (Zygal) for helping me out with the comments!
    musConfig c;

    c.control_flag = 0; // Set to MUSCONTROL_RAM if wbk file also placed in RAM
    c.channels = NU_AU_CHANNELS; // Maximum total number of channels
    c.sched = NULL; // The address of the Scheduler structure. NuSystem uses NULL
    c.thread_priority = NU_AU_MGR_THREAD_PRI; // Thread priority (highest)
    c.heap = (unsigned char*)NU_AU_HEAP_ADDR; // Heap address
    c.heap_length = NU_AU_HEAP_SIZE; // Heap size
    c.ptr = NULL; // Allows you to set a default ptr file
    c.wbk = NULL; // Allows you to set a default wbk file
    c.default_fxbank = NULL; // Allows you to set a default bfx file
    c.fifo_length = NU_AU_FIFO_LENGTH; // The size of the library's FIFO buffer
    c.syn_updates = NU_AU_SYN_UPDATE_MAX; // Number of updates for the synthesizer.
    c.syn_output_rate = 44100; // Audio output rate. The higher, the better quality
    c.syn_rsp_cmds = NU_AU_CLIST_LEN; // Maximum length of the audio command list.
    c.syn_retraceCount = 1; // The number of frames per retrace message
    
    // Number of buffers the Audio Manager can use for DMA from ROM transfer.
    c.syn_num_dma_bufs = NU_AU_DMA_BUFFER_NUM; 
    c.syn_dma_buf_size = NU_AU_DMA_BUFFER_SIZE; // The length of each DMA buffer

    // Initialize the Audio Manager.
    nuAuStlMgrInit(&c);

    // Register the PRENMI function.
    nuAuPreNMIFuncSet(nuAuPreNMIProc);
}

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* The initialization of graphic  */
  nuGfxInit();

  initAudio();

  /* The initialization of the controller manager  */
  contPattern = nuContInit();

  /* The initialization for stage00()  */
  initStage00();
  /* Register call-back  */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* The screen display is ON */
  nuGfxDisplayOn();

  while(1)
    ;
}

/*-----------------------------------------------------------------------------
  The call-back function 

  pendingGfx which is passed from Nusystem as the argument of the call-back 
  function is the total of RCP tasks that are currently processing and 
  waiting for the process. 
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  /* Provide the display process if 2 or less RCP tasks are processing or
	waiting for the process.  */
  if(pendingGfx < 3)
    makeDL00();		

  /* The process of game progress  */
  updateGame00(); 
}

