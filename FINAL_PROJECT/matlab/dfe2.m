function [F]=dfe2(x,Fs)
%
% DFE - Direct Frequency Estimation
%
% "Adaptive Algorithm for Direct Frequency Estimation"
% H.C. So, P.C. Ching
%

N=length(x);
%mu=.0075;           % LMS step size
mu=0.005;
theta=0.0;        % assume initial frequency 0

F=zeros(1,length(x));

% we need the first two samples, so the loop starts at 3
for n=3:N
    % compute the predicted signal from past two samples
     s_n = 2 * cos( theta ) * x(n-1) - x(n-2);    
    
    % compute the error e_n
     e_n = x(n) - s_n;
    
    % compute the new angle
     theta = theta - ( mu * e_n * ( ( x(n) + x(n-2))*cos(theta) + x(n-1) ) );  
    F(n)= theta / ( 2 * pi ) * Fs;
end
    
