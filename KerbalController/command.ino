//#define RXMID 535
//#define RYMID 520
//#define RZMID  49
//
//#define RXDEAD 25
//#define RYDEAD 25
//#define RZDEAD 15
//
//#define TXMID 534
//#define TYMID 515
//#define TZMID 34
//
//#define TXDEAD 25
//#define TYDEAD 25
//#define TZDEAD 15
//
//
//
//bool push_sync = false;
//
//void control() {
//    digitalWrite(pSTAGELED, digitalRead(pARM));
//    digitalWrite(pABORTLED, digitalRead(pARMABORT));
//
//    if (!digitalRead(pSYNCBT)){
//      push_sync = true;
//      digitalWrite(pSYNCLED, LOW);
//    }
//  
//  if (Connected) {
//    //here we define what controls to send when which pins are manipulated
//    
//    //toggle switches
//    if(!digitalRead(pSAS))mySimpit.activateAction(SAS_ACTION);
//    else mySimpit.deactivateAction(SAS_ACTION);
//    
//    if(!digitalRead(pRCS))mySimpit.activateAction(RCS_ACTION);
//    else mySimpit.deactivateAction(RCS_ACTION);
//
//    //momentary abort button
//    if(!digitalRead(pABORT) && digitalRead(pARMABORT)) mySimpit.activateAction(ABORT_ACTION);
//
//  
//    //momentary stage button
//    if(!digitalRead(pSTAGE) && digitalRead(pARM)) mySimpit.activateAction(STAGE_ACTION);
//
//
//    
//    
//    //toggle buttons
//    MainControls(LIGHTS, digitalRead(pLIGHTS));
//    MainControls(GEARS, digitalRead(pGEARS));
//    MainControls(BRAKES, digitalRead(pBRAKES));
//    
//    ControlGroups(1, digitalRead(pACTION1));
//    ControlGroups(2, digitalRead(pACTION2));
//    ControlGroups(3, digitalRead(pACTION3));
//    ControlGroups(4, digitalRead(pCHUTES));
//    ControlGroups(5, digitalRead(pSOLAR));
//    ControlGroups(6, digitalRead(pLADDER));
//
//    setSASMode(SMSAS);
//
//    //throttle
//    CPacket.Throttle = constrain(map(analogRead(pTHROTTLE),30,990,0,1023),0,1000);
//
//    //rotation joystick button toggles flight control modes
//    if(!digitalRead(pRB) && !rb_prev){rb_on = !rb_on; rb_prev = true;}
//    if(digitalRead(pRB) && rb_prev){rb_prev = false;}
//  }    
