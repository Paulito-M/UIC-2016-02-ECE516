function [F]=zcross2(x,Fs)
%
% ZCROSS2 - Modified Zero Crossing Algorithm
%
% 'x' assumed to be a periodic unbiased signal 
%

N=size(x);
freq=0;     
count=0;

% start on 2nd iteration
for n=2:N

    count = count + 1;
    
    % detect zero crossing
    if ( ( ( ( x(n-1) >= 0 ) && ( x(n) < 0 ) ) || ...
           ( ( x(n-1) > 0 ) && ( x(n) <= 0 ) ) ) && ...           
       ( count > 3 ) ) 
      freq = Fs / count;
      count = 0;
    end
    
    F(n)=freq;
    
end

F(n)
    
