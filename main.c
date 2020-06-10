/*
   main.c

   NuSYSTEM sample nu1

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
   */

#include <nusys.h>
#include "main.h"
#include "audio.h"


#include <nualsgi_n.h>

/* Declaration of the prototype  */
void stage00(int);

/* Declaration of the external function  */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);


/* The global variable  */
NUContData	contdata[1]; /* Read data of 1 controller  */
u8 contPattern;		     /* The pattern connected to the controller  */

int note;

void soundSetup(void) {
  /* Register the bank to the sequence player  */
  nuAuSeqPlayerBankSet(_dm_bankSegmentRomStart, _dm_bankSegmentRomEnd - _dm_bankSegmentRomStart, _dm_tableSegmentRomStart);

  /* Register MIDI sequence data to the sequence player */
  nuAuSeqPlayerSeqSet(_dm_seqSegmentRomStart);
}



void initAudio(void)
{
    nuAuInit();
}

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  /* The initialization of graphic  */
  nuGfxInit();

  initAudio();
  soundSetup();

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

  note++;
}

