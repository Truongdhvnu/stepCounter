close all;
fs = 50;
n = 0:1/fs:2;
accZ = zeros(1,length(n));
step = zeros(1,length(n));

for i = 1:length(n)
    accZ(i) = 2000 * sin(2*pi*3*i*1/fs) + 500 * randn();
end
for i = 1:length(n)
    step(i) = 2000 * sin(2*pi*3*i*1/fs);
end

fc = 7.5;
wc = fc/(fs/2);
[b, a] = butter(4, wc, "low");

b = b * 1000;
a = a * 1000;
accZ_filted = filter(b,a,accZ);
figure(1);
subplot(3,1,1);
plot(n, step);
[pks, locs] = findpeaks(step, n);
hold on;
plot(locs, pks, 'ro');
title("step");
subplot(3,1,2);
plot(n, accZ);
[pks, locs] = findpeaks(accZ, n);
hold on;
plot(locs, pks, 'ro');
title("step with white noise");
subplot(3,1,3);
plot(n, accZ_filted);
[pks, locs] = findpeaks(accZ_filted, n);
hold on;
plot(locs, pks, 'ro');
title("step after filter");

%%
filename = 'data.txt';
delimiterIn = ',';
headerlinesIn = 1;
A = importdata(filename,delimiterIn,headerlinesIn);
n2 = (0:199) * 1/fs;
figure(2);
origin_data = zeros(1, length(n2));
for i = 1:length(n2)
    origin_data(i) = 2000 * sin(2*pi*3*i*1/fs);
end

subplot(3,1,1);
plot(n2, origin_data);
title("origin_data");
subplot(3,1,2);
plot(n2,A.data(:,1));
title("data + noise");
subplot(3,1,3);
plot(n2,A.data(:,2));
title("data filted");
