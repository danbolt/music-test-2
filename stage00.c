/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include <nualsgi_n.h>
#include "main.h"
#include "graphic.h"
#include "audio.h"

static float theta;  /* The rotational angle of the square */
static float triPos_x; /* The display position-X */
static float triPos_y; /* The display position-Y */

extern int note;

static int playerState;

void shadetri(Dynamic* dynamicp);

/* The initialization of stage 0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;

  playerState = 0;
}

/* Make the display list and activate the task */
void makeDL00(void)
{
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  1.0F, 10.0F, 1.0F);
  guTranslate(&dynamicp->translate, -72.f, 0.f, 0.0F);

  /* Draw a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
  {
    nuDebConTextPos(0,20,8);
    sprintf(conbuf,"DISTANT MEMORY");
    nuDebConCPuts(0, conbuf);

    nuDebConTextPos(0,20,9);
    sprintf(conbuf,"by POLARIA POYON");
    nuDebConCPuts(0, conbuf);

    if (playerState == 0) {
      nuDebConTextPos(0,22,11);
      sprintf(conbuf,"Press A to Play");
      nuDebConCPuts(0, conbuf);
    } else if (playerState == 1) {
      nuDebConTextPos(0,22,11);
      sprintf(conbuf,"Press B to Stop");
      nuDebConCPuts(0, conbuf);
    }
  }
  else
  {
    nuDebConTextPos(0,21,10);
    nuDebConCPuts(0, "please connect Cont 1");
    nuDebConCPuts(0, conbuf);
  }
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}

void soundCheck(void) {

  if ((contdata[0].trigger & A_BUTTON) && (playerState == 0)) {
    nuAuSeqPlayerStop(0);
    nuAuSeqPlayerSetNo(0, 2);
    nuAuSeqPlayerPlay(0);

    playerState = 1;
  } else if ((contdata[0].trigger & B_BUTTON) && (playerState == 1)) {
    nuAuSeqPlayerStop(0);
    nuAuSeqPlayerSetNo(0, 2);

    playerState = 0;
  }
}

/* The game progressing process for stage 0 */
void updateGame00(void)
{  
  static float vel = 1.0;

  soundCheck();

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  /* Change the display position by stick data */
  triPos_x = contdata->stick_x;
  triPos_y = contdata->stick_y;

}

/* The vertex coordinate */
static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0 << 6, 0 << 6, 0, 0xff, 0, 0xff	},
        {         64,  64, -5, 0, 48 << 6, 0 << 6, 0, 0, 0, 0xff    	},
        {         64, -64, -5, 0, 48 << 6, 42 << 6, 0, 0, 0xff, 0xff	},
        {        -64, -64, -5, 0, 0 << 6, 42 << 6, 0xff, 0, 0, 0xff	},
};

/* Draw a square */
void shadetri(Dynamic* dynamicp)
{
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPTexture(glistp++,0x8000, 0x8000, 0, 0, G_ON);
  gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  gDPSetTextureFilter(glistp++,G_TF_POINT);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);
  gDPLoadTextureBlock(glistp++, 0x80200000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 48, 48, 0, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

  gSPVertex(glistp++,&(shade_vtx[0]),4, 0);
  gSP2Triangles(glistp++,0,1,2,0,0,2,3,0);
}
