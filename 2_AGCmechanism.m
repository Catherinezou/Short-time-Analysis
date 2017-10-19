close all;clear;clc
addpath('C:\Users\...');
[speech,fs]=audioread('sa1.wav');
l=length(speech);

a=0.99;
G_0=0.1;
sigma_sq=zeros(l,1);
G=ones(l,1);
AGC_x=zeros(l,1);

for i=1:l-1
sigma_sq(i+1)=a*sigma_sq(i)+(1-a)*speech(i)^2;
G(i+1)=G_0/sqrt(sigma_sq(i+1));
AGC_x(i+1)=speech(i+1)*G(i+1);
end

s=sqrt(sigma_sq);

figure;
hold all;
grid on;
plot(speech);
plot(s);
legend('original speech','sigma');
title(['choose alpha = ',num2str(a)])

figure;
hold all;
grid on;
plot(AGC_x);
% plot(speech);
% legend('Gain Equalized Speech','original speech');
legend('Gain Equalized Speech');
title(['choose alpha = ',num2str(a)])

sound(AGC_x,fs)
% sound(speech,fs)

%% for greasy
greasy=speech(22091:27864);
l=length(greasy);
sigma_sq=zeros(l,1);
G=ones(l,1);
AGC_x=zeros(l,1);

for i=1:l-1
sigma_sq(i+1)=a*sigma_sq(i)+(1-a)*greasy(i)^2;
G(i+1)=G_0/sqrt(sigma_sq(i+1));
AGC_x(i+1)=greasy(i+1)*G(i+1);

end

s=sqrt(sigma_sq);

figure;
hold all;
grid on;
plot(greasy);
plot(s);
legend('original speech of greasy','sigma');
title([' only for Greasy,choose alpha = ',num2str(a)])

figure;
hold all;
grid on;
plot(AGC_x);
plot(greasy);
legend('Gain Equalized Speech','original speech');
% legend('Gain Equalized Speech');
title([' only for Greasy,choose alpha = ',num2str(a)])

% sound(AGC_x,fs);
% sound(greasy,fs)
