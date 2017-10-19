close all;clear;clc
addpath('C:\Users\...');
[speech,fs]=audioread('sa1.wav');

wtype=2;
ss=1;
Lmsec=40;
Mmsec=10;
imf=1;
if (imf == 1)  % male range of pitch periods
        pdlow=5.0; 
        pdhigh=12.5;
    elseif (imf == 2)  % female range of pitch periods
        pdlow=2.9; 
        pdhigh=6.7;
    end
amdfthresh=0.6;

% save entire speech file in x
    x=speech(1:length(speech));
    
% convert frame duration (from msec to samples) and frame shift (from msec
% to samples) by scaling by fs/1000
    L=floor(Lmsec*fs/1000);
    M=floor(Mmsec*fs/1000);
    
% convert maximum lag (from msec to samples) for autocorrelation computation 
% (corresponding to minimum pitch frequency) 
    pdhigh=floor(pdhigh*fs/1000);

% convert minimum lag (from msec to samples) for autocorrelation computation 
% (corresponding to maximum pitch frequency)
    pdlow=floor(pdlow*fs/1000);
    
% measure log energy contour and input log energy threshold from peak
    eln=[];
    sse=1;
    ethresh=30;
    while (sse+L-1 < length(x))
        xs=x(sse:sse+L-1);
        eln=[eln 10*log10(sum(xs(1:L).^2))];
        sse=sse+M;
    end
    elnm=max(eln);
    elt=elnm-ethresh;
    
% accumulate pitch period values, along with minimum of amdf, in arrays
    period=[];
    amdfvals=[];
    ssv=[];
    ssr=1;
    while (ssr > 0)
        xs=x(ssr:ssr+L+pdhigh-1);
        
% check if log energy of frame > threshold; if not set frame to non-voiced
% with period 0, peak 0
        frame=floor(ssr/M)+1;
        if (eln(frame) > elt)            
            for k=1:pdhigh
                amdf(k)=sum(abs(xs(k:L+k-1)-xs(1:L)));
            end
            amdfmax=max(amdf);
            amdf=amdf/amdfmax;
        
% find amdf minimum above pdlow
            amdfmin=min(amdf(pdlow+1:pdhigh));
            xxx=find(amdf(pdlow:pdhigh) == amdfmin);
            minvalue=xxx(1)+pdlow-1;
            
% check for pitch period doubling
            if (floor((minvalue-1)/2) > pdlow && amdf(floor((minvalue-1)/2))...
                    <= amdfmin+0.1)
                minvalue=1+floor((minvalue-1)/2);
                amdfmin=amdf(minvalue-1);
            elseif (floor(minvalue/2) > pdlow && amdf(floor(minvalue/2))...
                    <= amdfmin+0.1)
                minvalue=1+floor(minvalue/2);
                amdfmin=amdf(minvalue-1);
            end
            
% check for pitch period tripling
            if (floor((minvalue-1)/3) > pdlow && amdf(floor((minvalue-1)/3))...
                    <= amdfmin+0.1)
                minvalue=1+floor((minvalue-1)/3);
                amdfmin=amdf(minvalue-1);
            elseif (floor((minvalue-1)/3+1) > pdlow && amdf(floor((minvalue-1)/3+1))...
                    <= amdfmin+0.1)
                minvalue=1+floor((minvalue-1)/3+1);
                amdfmin=amdf(minvalue-1);
            elseif (floor((minvalue-1)/3-1) > pdlow && amdf(floor((minvalue-1)/3-1))...
                    <= amdfmin+0.1)
                minvalue=1+floor((minvalue-1)/3-1);
                amdfmin=amdf(minvalue-1);
            end

% set period to 0 if amdfmin > amdfthresh=0.5
            if (amdfmin > amdfthresh)
                period=[period 0];
                amdfvals=[amdfvals amdfmin];
            else
                period=[period minvalue-1];
                amdfvals=[amdfvals amdfmin];
            end
            ssv=[ssv ssr];
            
        else
            
% save results in arrays
            minvalue=1;
            amdfmin=1;
            period=[period 0];
            amdfvals=[amdfvals 0];
            ssv=[ssv ssr];
            amdf(1:pdhigh)=0;
        end

        
% print out values in output file and on screen
%         fprintf(fid,'ssr: %d, minvalue: %d, amdfmin: %6.2f \n',ssr,minvalue-1,amdfmin);
        ssr=ssr+M;
        if (ssr+L+pdhigh > length(x))
            ssr=0;
        end
    end
    t=[1:(length(speech))];
    figure;%plot(ssv/fs,period,'b','LineWidth',2);hold on; 
     plot(t./fs,speech);hold on;
     plot(ssv/fs,period/(max(period)*20),'r','LineWidth',2);
    legend('speech','pitch period')
    tsec=['Time in Seconds; fs=',num2str(fs),' samples/second'];
    yl=['Pitch Period (samples at fs=',num2str(fs),' samples/second)'];
	axis tight, grid on, xlabel(tsec),ylabel(yl);
     
