function [F]=anf(x, Fs)
%
% ANF - Adaptive Notch Filter
%
% "Novel Adaptive IIR Filter for Frequency Estimation and Tracking",
% Li Tan, Jean Jiang

%
% Part 1: determine the initial value of theta
%

N=size(x);
mu=10^-3;       % LMS step size
r=0.85;
beta_prev1=0;   % assume last two gradients are 0
beta_prev2=0;
x_prev1=0;      % assume the last two filter inputs are 0
x_prev2=0;
y_prev1=0;      % assume last two filter outputs are 0
y_prev2=0;

% calculate an initial frequency based on 
% - estimating MSE function values from 0 to pi, corresponding
%   to frequencies from 0 to [sampling frequency/2) Hz
% - then get the average value of MSE

% create large vector dividing pi
THETA_TABLE_LEN = 1000;
theta_table = linspace(0,pi,THETA_TABLE_LEN);

% notch filter transfer function is given as
%
%        Y(z)   b-coeff    1 - 2z^-1cos(theta) + z^-2
% H(z) = ---- = ------- =  -------------------------------
%        X(z)   a-coeff    1 - 2rz^-1cos(theta) + r^2z^-2 
%
% compute the value of the transfer function for all 
% angles in the theta_table
  
  MSE = zeros( THETA_TABLE_LEN, 1 );
  MSE1 = zeros( THETA_TABLE_LEN, 1 );
  
for n = 1:THETA_TABLE_LEN
    
  B = [ 1  -2*cos(theta_table(n))  1 ];
  A = [ 1  -2*r*cos(theta_table(n)) r^2 ];

  ym = filter(B, A, x );
  MSE(n)=mean(ym.^2);

  y1 = filter ( B, A, x );
  MSE1(n)=mean(y1.^2);
  
end
    DC_MSE=mean(MSE);
    %Get initial value for theta
    [dummy,theta]=max(DC_MSE-MSE1)
    theta=theta_table(theta)

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
end
    % normalize and return in Hz
    F = theta / ( 2 * pi );
    F = F * Fs;