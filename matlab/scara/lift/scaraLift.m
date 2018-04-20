%% Scara Lift Tester
% Date: 12.04.18 
% Author: Moritz Höwer

startPos = 1900;
endPos = 2000;

currentPos = startPos;

x = [currentPos];
y = [0];

while (abs(currentPos - endPos) > 2) 
  speed = calc(currentPos, startPos, endPos);
  currentPos += speed;
  y = [y speed];
  x = [x currentPos];
end


plot(x, y, 'x');