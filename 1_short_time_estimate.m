close all;clear;clc
addpath('C:\Users\...');
[speech,fs]=audioread('sa1.wav');
frame_len=[51,101,201,401];
% frame_len=51;
l=length(speech);

for i=1:4
n=frame_len(i);
win=rectwin(n);
eng1=speech.^2;
eng=conv(win,eng1);
mag1=abs(speech);
mag=conv(win,mag1);

x=zeros(l,1);
x(1)=speech(1);
for j=2:1:l-1
    x(j,:)=sign(speech(j,:))-sign(speech(j-1,:));
end
zero_cross=conv(win,abs(x));
zero_cross=zero_cross/(2*frame_len(i));

figure;
hold all;
grid on;
plot(eng); 
plot(mag); 
plot(zero_cross); 
legend('Energy','Magnitude','ZeroCrossing');
title(['Frame Length is :',num2str(n)]);

end


figure
subplot(4,1,1)
plot(conv(rectwin(51),eng1)),legend('N=51');
subplot(4,1,2)
plot(conv(rectwin(101),eng1)),legend('N=101');
subplot(4,1,3)
plot(conv(rectwin(201),eng1)),legend('N=201');
subplot(4,1,4)
plot(conv(rectwin(401),eng1)),legend('N=401');
xlabel('Enegry--Short Time Analysis');

figure
subplot(4,1,1)
plot(conv(rectwin(51),mag1)),legend('N=51');
subplot(4,1,2)
plot(conv(rectwin(101),mag1)),legend('N=101');
subplot(4,1,3)
plot(conv(rectwin(201),mag1)),legend('N=201');
subplot(4,1,4)
plot(conv(rectwin(401),mag1)),legend('N=401');
xlabel('Magnitude--Short Time Analysis');

figure

x=zeros(l,1);
x(1)=speech(1);
for j=2:1:l-1
    x(j,:)=sign(speech(j,:))-sign(speech(j-1,:));
end
zero_cross=conv(win,abs(x));
zero_cross=zero_cross/(2*frame_len(i));

subplot(4,1,1)
plot((conv(rectwin(51),abs(x)))/2*51),legend('N=51');
subplot(4,1,2)
plot((conv(rectwin(101),abs(x)))/2*101),legend('N=101');
subplot(4,1,3)
plot((conv(rectwin(201),abs(x)))/2*201),legend('N=201');
subplot(4,1,4)
plot((conv(rectwin(401),abs(x)))/2*401),legend('N=401');
xlabel('Zero-Crossing--Short Time Analysis');


