function [F]=dfe(x,Fs)
%
% DFE - Direct Frequency Estimation
%
% "Adaptive Algorithm for Direct Frequency Estimation"
% H.C. So, P.C. Ching
%

N=size(x);
mu=10^-2;       % LMS step size
theta=0.25;        % assume initial frequency pi/4, normalized

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

    % normalize, convert to Hz
    F = theta / ( 2 * pi );
    F = F * Fs;

    
