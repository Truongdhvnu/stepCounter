clear all, close all;
%% file name
%filename = 'step_2.txt'; % 9 steps
filename = 'step_1.txt'; % 8 steps
%2 truong hop duoi nay total acc khong thay doi nhieu, do lech chuan(total acc) ~ 0 
%filename = "taybuongthong.txt";
%filename = "daodongtayngang.txt";
%filename = 'xoaynguoi_2.txt'; % xoay 2 lan
%filename = 'xoaynguoi.txt'; % xoay 2 lan
%filename = "nhay.txt";
%filename = "lactay.txt"; %giong voi luc chay nhung khong chay, lam sao de phan biet th nay

%%
delimiterIn = ',';
headerlinesIn = 1;
A = importdata(filename, delimiterIn, headerlinesIn);
a_sens = 16384.0;
g_sens = 131.0;

fs = 50;
fc = 4;
wc = fc/(fs/2);
% [b, a] = butter(2, wc, "low");
Rp = 1;
[b, a] = cheby1(4, Rp, wc, 'low');
figure(2);
freqz(b,a);
n = (1:1:length(A.data(:,1))) * 1/fs;

%%
accX_data = A.data(:,1) / a_sens;
accY_data = A.data(:,2) / a_sens;
accZ_data = A.data(:,3) / a_sens;

figure(1);
acc_XYZ = accX_data .^ 2 + accY_data .^ 2 + accZ_data .^ 2;
subplot(2,1,1);
plot(n, acc_XYZ);
[pks, locs] = findpeaks(acc_XYZ, n, 'MinPeakHeight', 1.2);
hold on;
plot(locs, pks, 'ro');
title("total acc XYZ isn't filtered");
subplot(2,1,2);
acc_XYZ_filted = filter(b, a, acc_XYZ);
plot(n, acc_XYZ_filted);
[pks, locs] = findpeaks(acc_XYZ_filted, n, 'MinPeakHeight', 1.2);
hold on;
plot(locs, pks, 'ro');
title("total acc XYZ filtered");

%%
sample_pos = 262:284; % odd
template = acc_XYZ_filted(sample_pos);
figure(3);
n2 = sample_pos * 1/fs;
plot(n2, template);
title("template");
win_length = length(sample_pos);

count = 0;
max_corr = 0;
ofset = 0;
corr_array = [];
index_saved = [];
for i = 1:1:(length(n) - length(n2) + 1)   
    if i + ofset + win_length < length(n)
        sample = acc_XYZ_filted(i + ofset : i + ofset + win_length - 1);
        matrix = corrcoef(template, sample);
        if matrix(1,2) > 0.85 && max(sample) > 1.5
           corr_array = [corr_array matrix(1,2)];
           count = count + 1;
           index_saved = [index_saved i+ofset];
%            figure(figure_number);
%            plot((i+ofset : i+ofset+win_length - 1) * 1/fs, sample);
%            figure_number = figure_number + 1;
           ofset = ofset + floor(win_length / 2);
        end
        if max_corr < matrix(1,2) && matrix(1,2) < 1
           max_corr = matrix(1,2);
        end
    end
end

sub_figure = 1;
figure(4);
for i = 1:count
    subplot(1,count, sub_figure);
    sub_figure = sub_figure + 1;
    plot((index_saved(i) : index_saved(i) + win_length - 1) * 1/fs, acc_XYZ_filted(index_saved(i) : index_saved(i) + win_length - 1));
end

%% this below code trying update template
% count2 = 0;
% max_corr2 = 0;
% corr_array2 = zeros(1,10);
% corr_array_index = 1;
% side_length = floor(win_length / 2);
% for i = 1:1:length(n)  
%     start = i - win_length + 1;
%     endd = i;
%     center = i - side_length;
%     if start <= 0 || center <= 0
%         continue;
%     end
% 
%     if max(acc_XYZ_filted(start : endd)) == acc_XYZ_filted(center)
%         sample = acc_XYZ_filted(start : endd);
%         matrix = corrcoef(temple, sample);
%         if matrix(1,2) > 0.35 && max(sample) > 1.5
%            corr_array2(corr_array_index) = matrix(1,2);
%            corr_array_index = corr_array_index + 1;
%            count2 = count2 + 1;
%            figure(figure_number);
%            subplot(2,1,1);
%            plot((start : endd) * 1/fs, sample);
%            figure_number = figure_number + 1;
%            temple = (temple + sample) / 2;
%            subplot(2,1,2);
%            plot(n2, temple);
%         end
%         if max_corr2 < matrix(1,2) && matrix(1,2) < 1
%            max_corr2 = matrix(1,2);
%         end
% 
%     end
% end



% figure(figure_number);
% plot(1:length(corr_array), corr_array);
% title("corr series");

%% Ly do chi dem duoc la 5 neu de temple hinh sin: do dai cua so anh huonh nhieu toi corrcoef
% time = [3.16 4.04 4.8 5.66 6.34 7.16 7.78 8.56] /0.02;
% 
% for i = 1:length(time)
%     sample = acc_XYZ_filted(time(i) : time(i) + win_length);
%     matrix = corrcoef(temple, sample);
%     disp(matrix(1, 2));
% end
% figure(3);
% i = 6;
% sample = acc_XYZ_filted(time(i) : time(i) + win_length);
% plot((time(i): time(i) + win_length) * 1/fs, sample);
% matrix = corrcoef(temple, sample);
% disp(matrix(1, 2));
