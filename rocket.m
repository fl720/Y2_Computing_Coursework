cla;
clc;

%% =====================IMPORTING_CPP_DATA=================================
fp           = importdata("output.txt") ;           % <——修正后结果
fp_c         = importdata("output_correct.txt") ;   % <——参考答案
%% =====================EXTRATE_DATA_FOR_PLOTTING==========================

CPP_RK_h     = 0 ; 
CPP_T        = 0 ; 
CPP_RK_v     = 0 ; 
CPP_RK_m     = 0 ; 
[fps,fp_row] = size(fp) ; 

for count = 2:fps
    CPP_T    = [CPP_T , fp(count,1)]    ; 
    CPP_RK_h = [CPP_RK_h , fp(count,2) ]; 
    CPP_RK_v = [CPP_RK_v , fp(count,3) ];
    CPP_RK_m = [CPP_RK_m , fp(count,4) ];
end

%% =====================EXTRATE_CLASSMATE'S_RESULT_FOR_PLOTTING============

CPP_REF_T = fp_c.data(:,1) ; 
CPP_REF_H = fp_c.data(:,2) ; 
CPP_REF_V = fp_c.data(:,3) ; 
CPP_REF_M = fp_c.data(:,4) ; 

%% =====================PLOT_REGION========================================
hold on ; 

subplot(3,1,1)
plot(CPP_T(2:fps),CPP_RK_h(2:fps),'r' , CPP_REF_T ,CPP_REF_H , 'b' )
title('height')
xlabel('time(s)')
ylabel('height(m)')
legend('更改后代码' , ' 同学参考答案' )


subplot(3,1,2)
plot(CPP_T(2:fps),CPP_RK_v(2:fps),'r' , CPP_REF_T ,CPP_REF_V , 'b' )
title('velocity')
xlabel('time(s)')
ylabel('velocity(m/s)')
legend('更改后代码' , ' 同学参考答案' )


subplot(3,1,3)
plot(CPP_T(2:fps),CPP_RK_m(2:fps),'r' , CPP_REF_T ,CPP_REF_M , 'b' )
title('mass')
xlabel('time(s)')
ylabel('mass(kg)')
legend('更改后代码' , ' 同学参考答案' )

hold off ; 