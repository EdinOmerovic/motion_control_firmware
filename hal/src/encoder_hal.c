//###########################################################################
//
// FILE:    Example_posspeed.c
//
// TITLE:   Pos/speed measurement using EQEP peripheral
//
// DESCRIPTION:
//
// This file includes the EQEP initialization and position and speed
// calculation functions called by Eqep_posspeed.c.  The position and speed
// calculation steps performed by POSSPEED_Calc() at  SYSCLKOUT =  200 MHz are
// described below:
//
// 1. This program calculates: **theta_mech**
//
//    theta_mech = QPOSCNT/mech_Scaler = QPOSCNT/4000, where 4000 is the number
//                 of counts in 1 revolution.
//                (4000/4 = 1000 line/rev. quadrature encoder)
//
// 2. This program calculates: **theta_elec**
//
//    theta_elec = (# pole pairs) * theta_mech = 2*QPOSCNT/4000
//
// 3. This program calculates: **SpeedRpm_fr**
//
//    SpeedRpm_fr = [(x2-x1)/4000]/T   - Equation 1
//    Note (x2-x1) = difference in number of QPOSCNT counts. Dividing (x2-x1)
//    by 4000 gives position relative to Index in one revolution.
// If base RPM  = 6000 rpm:   6000 rpm = [(x2-x1)/4000]/10ms   - Equation 2
//                                     = [(x2-x1)/4000]/(.01s*1 min/60 sec)
//                                     = [(x2-x1)/4000]/(1/6000) min
//                         max (x2-x1) = 4000 counts, or 1 revolution in 10 ms
//
//
// If both sides of Equation 2 are divided by 6000 rpm, then:
//                   1 = [(x2-x1)/4000] rev./[(1/6000) min * 6000rpm]
//                   Because (x2-x1) must be <4000 (max) for QPOSCNT increment,
//                   (x2-x1)/4000 < 1 for CW rotation
//                   And because (x2-x1) must be >-4000 for QPOSCNT decrement,
//                   (x2-x1)/4000>-1  for CCW rotation
//                   speed_fr = [(x2-x1)/4000]/[(1/6000) min * 6000rpm]
//                            = (x2-x1)/4000   - Equation 3
//
// To convert speed_fr to RPM, multiply Equation 3 by 6000 rpm
//                   SpeedRpm_fr = 6000rpm *(x2-x1)/4000  - Final Equation
//
// 2. **min rpm ** = selected at 10 rpm based on CCPS prescaler options
//    available (128 is greatest)
//
// 3. **SpeedRpm_pr**
//    SpeedRpm_pr = X/(t2-t1)  - Equation 4
//    where X = QCAPCTL [UPPS]/4000 rev. (position relative to Index in
//                                        1 revolution)
// If  max/base speed = 6000 rpm:
//               6000 = (32/4000)/[(t2-t1)/(200MHz/64)]
//           where 32 = QCAPCTL [UPPS] (Unit timeout - once every 32 edges)
//            32/4000 = position in 1 revolution (position as a fraction
//                                                of 1 revolution)
//   t2-t1/(200MHz/64), t2-t1= # of QCAPCLK cycles, and
//      QCAPCLK cycle = 1/(200MHz/64)
//                    = QCPRDLAT
//
// So:       6000 rpm = [32(200MHz/64)*60s/min]/[4000(t2-t1)]
//              t2-t1 = [32(200MHz/64)*60s/min]/(4000*6000rpm)  - Equation 5
//                    = 250 CAPCLK cycles = maximum (t2-t1) = SpeedScaler
//
// Divide both sides by (t2-t1), and:
//            1 = 32/(t2-t1) = [32(200MHz/64)*60 s/min]/(4000*6000rpm)]/(t2-t1)
//       Because (t2-t1) must be < 250 for QPOSCNT increment:
//               250/(t2-t1) < 1 for CW rotation
//       And because (t2-t1) must be >-250 for QPOSCNT decrement:
//                250/(t2-t1)> -1 for CCW rotation
//
//       eed_pr = 250/(t2-t1)
//             or [32(200MHz/64)*60 s/min]/(4000*6000rpm)]/(t2-t1) - Equation 6
//
// To convert speed_pr to RPM:
// Multiply Equation 6 by 6000rpm:
// SpeedRpm_fr  = 6000rpm * [32(200MHz/64)*60 s/min]/[4000*6000rpm*(t2-t1)]
//              = [32(200MHz/64)*60 s/min]/[4000*(t2-t1)]
//              or [(32/4000)rev * 60 s/min]/[(t2-t1)(QCPRDLAT)]-Final Equation
//
// More detailed calculation results can be found in the Example_freqcal.xls
// spreadsheet included in the example folder.
//
// This file contains source for the posspeed module
//
//###########################################################################
//
// $Release Date:  $
// $Copyright:
// Copyright (C) 2013-2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include "encoder_hal.h"

//
// POSSPEED_Init - Initialize EQEP1 configuration
//
void POSSPEED_Init(void)
{
    EQep1Regs.QUPRD = 2000000;            // Unit Timer for 100Hz at 200 MHz
                                          // SYSCLKOUT
    EQep1Regs.QDECCTL.bit.QSRC = 00;      // QEP quadrature count mode
    EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;
    EQep1Regs.QEPCTL.bit.PCRM = 00;       // PCRM=00 mode - QPOSCNT reset on
                                          // index event
    EQep1Regs.QEPCTL.bit.UTE = 1;         // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM = 1;        // Latch on unit time out
    EQep1Regs.QPOSMAX = 0xffffffff;
    EQep1Regs.QEPCTL.bit.QPEN = 1;        // QEP enable
    EQep1Regs.QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
    EQep1Regs.QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
    EQep1Regs.QCAPCTL.bit.CEN = 1;        // QEP Capture Enable
}

Uint32 POSSPEED_Read(POSSPEED *p)
{

    p->previous_value = p->current_value;
    p->current_value = EQep1Regs.QPOSCNT;

    // 0 - counter-clockwise (back)
    // 1 - clockwise (forward)
    p->direction = EQep1Regs.QEPSTS.bit.QDF;


    return p->current_value / p->scaling_factor;
}
