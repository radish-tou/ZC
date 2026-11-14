#include "para.h"

Para_TypeDef para;

void ParaInst(UART_HandleTypeDef *p)
{
	ReadPara();
}


void ReadPara(void)
{
		// para.head = 0xA5A55A5A; 
		// para.id = 0;
		// para.mid1 = 0xD6C99F4A;//0号
		// para.rightLimit=3094756+ para.mid1;//1号上限位170°
		// para.leftLimit  =-364089+ para.mid1; //1号下限位-20°  -364088
		
		// para.mid2 = 0x00005112	;//0号
		
		// para.downLimit = -5460 + para.mid2;  //65536/360*30
		// para.upLimit =  5460+para.mid2; 
//		
//		para.right_xw_ks =30;
//		para.right_xw_end=59.5;
//		para.left_xw_ks =-140;
//		para.left_xw_end=-169.5;
//		para.up_xw_ks=20;
//		para.up_xw_end=30.5;
//		para.down_xw_ks=-20;
//		para.down_xw_end=-30.5;
//		
//		para.up_k=2;
//		
//		para.down_k=2;
//		
//		para.left_k=1;
		
}










