START:      MOV     A,#0FEH    		;��A������Ϊ11111110
LOOP:       MOV     P1,A           ;��P1�����ACC������

            CALL    DELAY       ;������ʱ�ӳ���
			
			RL A
            LJMP    LOOP         ;����LOOP��ִ��
;0.1����ʱ�ӳ���12MHz����===================
DELAY: MOV      R7,#10    ;��ʱ1S�ӳ���
DL1:   MOV      R6,#200
DL2:   MOV      R5,#248
       DJNZ     R5,$
       DJNZ     R6,DL2
       DJNZ     R7,DL1
        RET
		END
