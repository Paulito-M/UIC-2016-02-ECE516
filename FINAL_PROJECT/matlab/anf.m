function [F]=anf(x, Fs)
%
% ANF - Adaptive Notch Filter
%
% Simplified to assume just fundamental, no harmonics
%
% "Novel Adaptive IIR Filter for Frequency Estimation and Tracking",
% Li Tan, Jean Jiang


N=size(x);
mu=10^-3;       % LMS step size
r=0.85;
beta_prev1=0;   % assume last two gradients are 0
beta_prev2=0;
x_prev1=0;      % assume the last two filter inputs are 0
x_prev2=0;
y_prev1=0;      % assume last two filter outputs are 0
y_prev2=0;


F=zeros(1,length(x));

%
% Part 1: determine the initial value of theta
% JUST GUESS AT A NUMBER since there is only one frequency
theta = .01;

% Part 2: LMS algorithm to find frequency
%
% loop through the whole sample
for n=1:N
    
    % compute the new filter output
    y = x(n)-2*cos(theta)*x_prev1 + x_prev2 + 2*r*cos(theta)*y_prev1 - (r^2)*y_prev2;
        
    % compute the gradient function
    beta = 2*sin(theta)*x_prev1 - 2*r*sin(theta)*y_prev1 + 2*r*cos(theta)*beta_prev1 - (r^2)*beta_prev2   ; 
    
    % compute the new angle
    theta = theta - ( 2 * mu  * y * beta);
    
    % update previous x's, y's, betas  
    x_prev2 = x_prev1;
    x_prev1 = x(n);
    
    y_prev2 = y_prev1;
    y_prev1 = y;
    
    beta_prev2 = beta_prev1;
    beta_prev1 = beta;
    F(n) = theta / ( 2 * pi ) * Fs;
end
    F(n)
