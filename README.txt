---Short Time Estimation---

Languge: Matlab

1. Short_time_estimation
i.  This matlab program shows the effects of window duration on the short‐time analysis of energy, magnitude, and zero‐crossings.
ii. Choose frame length of L=51, 101, 201, 401 samples using rectangular window

2. AGC mechanism
i.  Using IIR filter of the form H(z)=(1‐α)z‐1/ (1‐αz‐1)
ii. Illustrating the effectiveness of AGC control

3.  Short-time autocorrelation
i.  This matlab program compares the performance of pitch detection by autocorrelation estimates from short‐time autocorrelation and modified short time autocorrelation
ii. Estimate a suitable threshold for selecting the peak autocorrelation ( in a voiced region) appropriate for pitch detection and by processing the speech file on a frame‐by‐frame basis, determine the pitch contour of the speech file based on detecting the peak of the autocorrelation function and deciding if it is above the threshold ( voiced speech) or below the threshold (unvoiced speech or background noise).

4.  AMDF calculation
i.  This matlab program calculates the AMDF of a speech wavefile(‘sa1.wav’) and implement a pitch detection algorithm based on using the AMDF on a frame by frame basis
