function speed = calc(currentPosition, startPosition, targetPosition)
  %% Calculate position update
  % Date: 12.04.18 
  % Author: Moritz Höwer
  
  deltaStart = abs(currentPosition - startPosition);
  deltaEnd = abs(targetPosition - currentPosition);
  delta = min(max(min(deltaStart, deltaEnd), 10), 100);
  
 if(currentPosition > targetPosition) 
  direction = -1;
 else
  direction = 1;
 end
    
  speed = direction * delta * 0.1;
end