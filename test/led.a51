START:      MOV     A,#0FEH    		;让A的内容为11111110
LOOP:       MOV     P1,A           ;让P1口输出ACC的内容

            CALL    DELAY       ;调用延时子程序
			
			RL A
            LJMP    LOOP         ;跳到LOOP处执行
;0.1秒延时子程序（12MHz晶振）===================
DELAY: MOV      R7,#10    ;延时1S子程序
DL1:   MOV      R6,#200
DL2:   MOV      R5,#248
       DJNZ     R5,$
       DJNZ     R6,DL2
       DJNZ     R7,DL1
        RET
		END
