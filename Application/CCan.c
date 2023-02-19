/*
 * CCan.c
 *
 *  Created on: 2023Äê2ÔÂ18ÈÕ
 *      Author: 97478
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "F2806x_ECan.h"
#include "FreeRTOS.h"


//
// Function Prototypes
//
void mailbox_check(int32 T1, int32 T2, int32 T3);
void mailbox_read(int16 i);

//
// Globals
//
Uint32  ErrorCount;
Uint32  PassCount;
Uint32  MessageReceivedCount;

Uint32  TestMbox1 = 0;
Uint32  TestMbox2 = 0;
Uint32  TestMbox3 = 0;

void initCanGpio()
{
    InitECanGpio();
}

void initCan()
{
    InitECana();            // Initialize eCAN-A module
    //
    // Mailboxs can be written to 16-bits or 32-bits at a time
    // Write to the MSGID field of TRANSMIT mailboxes MBOX0 - 15
    //
    ECanaMboxes.MBOX0.MSGID.all = 0x9555AAA0;
    ECanaMboxes.MBOX1.MSGID.all = 0x9555AAA1;
    ECanaMboxes.MBOX2.MSGID.all = 0x9555AAA2;
    ECanaMboxes.MBOX3.MSGID.all = 0x9555AAA3;
    ECanaMboxes.MBOX4.MSGID.all = 0x9555AAA4;
    ECanaMboxes.MBOX5.MSGID.all = 0x9555AAA5;
    ECanaMboxes.MBOX6.MSGID.all = 0x9555AAA6;
    ECanaMboxes.MBOX7.MSGID.all = 0x9555AAA7;
    ECanaMboxes.MBOX8.MSGID.all = 0x9555AAA8;
    ECanaMboxes.MBOX9.MSGID.all = 0x9555AAA9;
    ECanaMboxes.MBOX10.MSGID.all = 0x9555AAAA;
    ECanaMboxes.MBOX11.MSGID.all = 0x9555AAAB;
    ECanaMboxes.MBOX12.MSGID.all = 0x9555AAAC;
    ECanaMboxes.MBOX13.MSGID.all = 0x9555AAAD;
    ECanaMboxes.MBOX14.MSGID.all = 0x9555AAAE;
    ECanaMboxes.MBOX15.MSGID.all = 0x9555AAAF;

    //
    // Write to the MSGID field of RECEIVE mailboxes MBOX16 - 31
    //
    ECanaMboxes.MBOX16.MSGID.all = 0x9555AAA0;
    ECanaMboxes.MBOX17.MSGID.all = 0x9555AAA1;
    ECanaMboxes.MBOX18.MSGID.all = 0x9555AAA2;
    ECanaMboxes.MBOX19.MSGID.all = 0x9555AAA3;
    ECanaMboxes.MBOX20.MSGID.all = 0x9555AAA4;
    ECanaMboxes.MBOX21.MSGID.all = 0x9555AAA5;
    ECanaMboxes.MBOX22.MSGID.all = 0x9555AAA6;
    ECanaMboxes.MBOX23.MSGID.all = 0x9555AAA7;
    ECanaMboxes.MBOX24.MSGID.all = 0x9555AAA8;
    ECanaMboxes.MBOX25.MSGID.all = 0x9555AAA9;
    ECanaMboxes.MBOX26.MSGID.all = 0x9555AAAA;
    ECanaMboxes.MBOX27.MSGID.all = 0x9555AAAB;
    ECanaMboxes.MBOX28.MSGID.all = 0x9555AAAC;
    ECanaMboxes.MBOX29.MSGID.all = 0x9555AAAD;
    ECanaMboxes.MBOX30.MSGID.all = 0x9555AAAE;
    ECanaMboxes.MBOX31.MSGID.all = 0x9555AAAF;

    //
    // Configure Mailboxes 0-15 as Tx, 16-31 as Rx
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    //
    ECanaRegs.CANMD.all = 0xFFFF0000;

    //
    // Enable all Mailboxes
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    //
    ECanaRegs.CANME.all = 0xFFFFFFFF;

    //
    // Specify that 8 bits will be sent/received
    //
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX9.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX10.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX12.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX14.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX15.MSGCTRL.bit.DLC = 8;

    //
    // Write to the mailbox RAM field of MBOX0 - 15
    //
    ECanaMboxes.MBOX0.MDL.all = 0x9555AAA0;
    ECanaMboxes.MBOX0.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX1.MDL.all = 0x9555AAA1;
    ECanaMboxes.MBOX1.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX2.MDL.all = 0x9555AAA2;
    ECanaMboxes.MBOX2.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX3.MDL.all = 0x9555AAA3;
    ECanaMboxes.MBOX3.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX4.MDL.all = 0x9555AAA4;
    ECanaMboxes.MBOX4.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX5.MDL.all = 0x9555AAA5;
    ECanaMboxes.MBOX5.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX6.MDL.all = 0x9555AAA6;
    ECanaMboxes.MBOX6.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX7.MDL.all = 0x9555AAA7;
    ECanaMboxes.MBOX7.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX8.MDL.all = 0x9555AAA8;
    ECanaMboxes.MBOX8.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX9.MDL.all = 0x9555AAA9;
    ECanaMboxes.MBOX9.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX10.MDL.all = 0x9555AAAA;
    ECanaMboxes.MBOX10.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX11.MDL.all = 0x9555AAAB;
    ECanaMboxes.MBOX11.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX12.MDL.all = 0x9555AAAC;
    ECanaMboxes.MBOX12.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX13.MDL.all = 0x9555AAAD;
    ECanaMboxes.MBOX13.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX14.MDL.all = 0x9555AAAE;
    ECanaMboxes.MBOX14.MDH.all = 0x89ABCDEF;

    ECanaMboxes.MBOX15.MDL.all = 0x9555AAAF;
    ECanaMboxes.MBOX15.MDH.all = 0x89ABCDEF;

    EALLOW;

    //
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    //
    ECanaRegs.CANMIM.all = 0xFFFFFFFF;

//    //
//    // Configure the eCAN for self test mode
//    // Enable the enhanced features of the eCAN.
//    //
//    struct ECAN_REGS ECanaShadow;
//
//    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
//    ECanaShadow.CANMC.bit.STM = 1;    // Configure CAN for self-test mode
//    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;
}

void CAN_Task(void * pvParameters){
    initCanGpio();
    initCan();
    Uint16  j;
    //
    // Begin transmitting
    //
    for(;;)
    {
        //
        // Set TRS for all transmit mailboxes
        //
        ECanaRegs.CANTRS.all = 0x0000FFFF;

        //
        // Wait for all TAn bits to be set
        //
        while(ECanaRegs.CANTA.all != 0x0000FFFF )
        {

        }

        ECanaRegs.CANTA.all = 0x0000FFFF;   // Clear all TAn
        MessageReceivedCount++;

        //
        // Read from Receive mailboxes and begin checking for data
        // Read & check 16 mailboxes
        //
        for(j=16; j<32; j++)
        {
            //
            // This func reads the indicated mailbox data
            //
            mailbox_read(j);

            //
            // Checks the received data
            //
            mailbox_check(TestMbox1,TestMbox2,TestMbox3);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

//
// mailbox_read - This function reads out the contents of the indicated
// by the Mailbox number (MBXnbr). MSGID of a rcv MBX is transmitted as the
// MDL data.
//
void
mailbox_read(int16 MBXnbr)
{
    volatile struct MBOX *Mailbox;
    Mailbox = &ECanaMboxes.MBOX0 + MBXnbr;
    TestMbox1 = Mailbox->MDL.all;   // = 0x9555AAAn (n is the MBX number)
    TestMbox2 = Mailbox->MDH.all;   // = 0x89ABCDEF (a constant)
    TestMbox3 = Mailbox->MSGID.all; // = 0x9555AAAn (n is the MBX number)
}

//
// mailbox_check -
//
void
mailbox_check(int32 T1, int32 T2, int32 T3)
{
    if((T1 != T3) || ( T2 != 0x89ABCDEF))
    {
        ErrorCount++;
    }
    else
    {
        PassCount++;
    }
}

//
// End of File
//
