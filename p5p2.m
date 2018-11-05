load wave.mat

%Settings
fs = 10;
window = 4096;
Gwidth = 1000;
Gheight = 600;


%Estimate PSD
[pxx,f] = pwelch(psi_w(2,:)*(1/2*pi),window,[],[],fs*(2*pi));

%Resonance frequency
[pxxMax, ind] = max(pxx); %ser ut til at "intensity" er lik maksverdien til pxx
w_0 = f(ind);

%Variance/Intensity
%variance = var(pxx(1:105,1),f(1:105,1));
%std_div = sqrt(variance);
std_div = sqrt(pxxMax);

%Damping factor
n = 4; %number of estimates
P_psi_w = zeros(4,105);
[r,c] = size(P_psi_w);

lambda_vect = [0.2, 0.08, 0.06, 0.02];
w = f(1:105,1).';

%enklere enn lsqcurvefit:
for i = 1:r
    lambda = lambda_vect(i);
    K_w = 2*lambda*w_0*std_div;
    P_psi_w(i,1:105) = ((K_w^2)*w.^2)./(w.^4 + 2*(w_0^2)*(w.^2)*(2*lambda^2 - 1) + w_0^4); 
end

%startpt = 1;
%x = lsqcurvefit(P_psi_w, startpt, xdata, ydata);



%Plot
figure('Units','pixels','Position',[1,10,Gwidth,Gheight]); 
figure(1);

hold on
%Vertical line:
plot([w_0, w_0], [0, pxxMax], '--' ,'Color',[0.1 0.1 0.1]);
%
plotw_0 = plot(w_0,pxxMax,'o','MarkerEdgeColor',[1 0 0],'MarkerFaceColor',[1 0 0],'MarkerSize',5);
plot(f(1:105,1),pxx(1:105,1),'linewidth',1,'Color',[0.1 0.15 0.5]), grid on;
hold off

xticklabels({0,'$\frac{\pi}{8}$','$\frac{\pi}{4}$','$\frac{3\pi}{8}$','$\frac{\pi}{2}$','$\frac{5\pi}{8}$'});
set(gca,'TickLabelInterpreter','latex','XTickLabel',xticklabels,'fontsize',20);
xticks(0:pi/8:pi/2);
xlabel('Frequency [rad/s]','fontsize',12);
ylabel('Intensity [power s/rad]','fontsize',12);
%title('Estimation of PSD function','fontsize',15);
legend(plotw_0,{'Resonance frequency \omega_0'},'FontSize',11);
set(gca,'FontName','times');

%Save plot
%saveas(gcf,'p5p2_w0.eps')


figure('Units','pixels','Position',[1,10,Gwidth,Gheight]); 
figure(2);

hold on
plot(f(1:105,1),pxx(1:105,1),'linewidth',1,'Color',[0.1 0.15 0.5]);
plot(w,P_psi_w(1:n,:),'--','linewidth',1), grid on;
hold off

xticklabels({0,'$\frac{\pi}{8}$','$\frac{\pi}{4}$','$\frac{3\pi}{8}$','$\frac{\pi}{2}$','$\frac{5\pi}{8}$'});
set(gca,'TickLabelInterpreter','latex','XTickLabel',xticklabels,'fontsize',20);
xticks(0:pi/8:pi/2);
xlabel('Frequency [rad/s]','fontsize',12);
ylabel('Intensity [power s/rad]','fontsize',12);
%title('Estimation of PSD function','fontsize',15);
legend(plotw_0,{'Resonance frequency \omega_0'},'FontSize',11);
set(gca,'FontName','times');

%saveas(gcf,'p5p2_lambda.eps')

