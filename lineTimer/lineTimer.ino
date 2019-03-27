 //put this snippet into forward function. this function compares current reading to previous reading
 unsigned long lastInter = 0; // Important to keep as unsigned long 
 if((lVal > thresh) && (cVal > thresh) && (rVal > thresh) && (plVal > thresh) && (pcVal > thresh) && (prVal > thresh)){ //At an intersection. Increment intersection counter
      if((millis()-lastInter)>200){
        Serial.println("INTERSECTION");
        
        intersectionCount++;
        Serial.println(intersectionCount);
        lastInter = millis();
        
      }
    }
     plVal = lVal;
     pcVal = cVal;
     prVal = rVal;
  }
