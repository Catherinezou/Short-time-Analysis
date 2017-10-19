close all;clear;clc
addpath('C:\Users\zxz162330\Documents\Ziyan Z\Speech Processing\HW3_Ziyan Zou-Sep25');
[speech,fs]=audioread('sa1.wav');

l=length(speech);
speech1=speech(6480:8560); %word 'ae'
speech2=speech(12290:13853); %word 'aa'

L=201; % define the window size
K=250; % define the number of autocorrelation points
N=L+K;
win=rectwin(N);
N_frames1=floor(length(speech1)/L);

for i=1:N_frames1-1
x_frame1=speech1((i-1)*L+1:i*L+K); 
window=x_frame1.*win; %convlution of input and window

for j=0:K   
corr(i,j+1)=sum(window(1:L-j).*window(1+j:L)); % autocorrelation
corr_mdf(i,j+1)=sum(x_frame1(1:L).*x_frame1(1+j:L+j)); % modified autocorrelation
end
end

t=1:(K+1);
figure;
hold all;
grid on;
plot(t,corr(2,t)); 
plot(t,corr_mdf(2,t)); 
title( 'Autocorrelation for word :ae');
legend('autocorrelation','modified autocorrelation');
xlabel('Lags');


L=101; % define the window size
N=L+K;
win=rectwin(N);
N_frames2=floor(length(speech2)/L);

for i=1:N_frames2-1
x_frame2=speech1((i-1)*L+1:i*L+K); 
window=x_frame2.*win; %convlution of input and window

for j=0:K   
corr(i,j+1)=sum(window(1:L-j).*window(1+j:L)); % autocorrelation
corr_mdf(i,j+1)=sum(x_frame2(1:L).*x_frame2(1+j:L+j)); % modified autocorrelation
end
end

t=1:(K+1);
figure;
hold all;
grid on;
plot(t,corr(2,t)); 
plot(t,corr_mdf(2,t)); 
legend('autocorrelation','modified autocorrelation');
title('Autocorrelation for word :aa');
xlabel('Lags');
