MTO_TFS 
=========================================
前言
-----------
朋友们我要毕业啦，这是我上传的最后一个OpenFOAM拓扑优化求解器了。

TFS分别代表三个物理场，即T=thermal;   F=fluid;   S=solid displacement。需要说明的是，流体目前只做到了层流，固体只上传了不考虑热膨胀的代码，如果有小伙伴对热膨胀感兴趣可以单独邮件我。

此外，很遗憾没时间把应力约束加进去，如果哪位小伙伴感兴趣，**并且愿意开源**，可以联系我，我会尽可能帮助你写完这个求解器。

安装说明
-----------
想要跑通这个求解器并不是一个简单的事情，在linux下安装软件非常令人头疼。此处强烈建议用虚拟机或者双系统，而且从官网下载，不要用一些奇奇怪怪的包，免得出现报错。

(a) **OpenFOAM 5.0或6.0**  
(b) **PETSc 3.12**  
(c) **swak4Foam 0.4.3**  
(d) **openMPI 1.10**

1.运行MTO_TFS时使用op5和op6区别不大，就只是主函数里第26行用 **while (simple.loop()) op5**还是**while (simple.loop(runTime)) op6**.

2.PETSc的作用是给MMA提供框架的，因为Aage大佬发布了基于petsc的并行MMA，所以我修改以后就移植到op里了(修改以后的代码可以从我之前的仓库里MTO中下载).大佬的源码在 https://github.com/topopt/TopOpt_in_PETSc/MMA.cc

使用时需要把MMA.h和MMA.c编译成动态库，并且把该库放到**FOAM_USER_LIBBIN**.编译代码大概长这样**(注意要把路径换成你自己电脑的)**：  mpic++ -shared -fPIC *.c -I/home/gentai/PETSc/petsc-3.11.0/include  -I/home/gentai/PETSc/petsc-3.11.0/include/mpiuni -I/home/gentai/PETSc/petsc-3.11.0/arch-linux-intel/include -o libMMA_yu.so  

3.swak4Foam是用来加第三类边界条件的。你可以看看app文件夹下伴随温度场Ta中的groovyBC，那就是swak4Foam的功能。

4.有些小伙伴下载高版本ubuntu后openMPI版本也会很高，导致编译时找不到mpi.h。解决办法要么下载ubuntu14，要么使用openMPI 1.10版本。

运行
-----------
程序是在linux下运行的，不熟悉linux的小伙伴建议先补补课。现在假设软件已经安装正确，则：

1. 进入源代码文件夹(MTO_TFS),终端中输入wmake(编译求解器)
2. 进入app文件夹，终端中输入blockMesh(画网格)
3. 终端中输入decomposePar(区域分解)
4. 终端中输入mpirun -np 20 MTO_TFS -parallel(默认设置20个区域分解)
5. 终端中输入reconstructPar(数据组装)
6. 终端中输入paraFoam(后处理看数据)

本算例共有约51万个六面体网格，在我的工作站上(20核并行)大约23秒完成一步优化,预设计算300步后停止(我算到170步左右已经差不多收敛了)。


最后
-----------
上述流程走完你应该能看到一个结果，如果你对程序有疑问，可以下载我的博士论文《液冷散热器流道与热源分布拓扑优化研究》，或者邮件找我要。本人非计算机专业出身，写代码能力有限，就是简单用C语言把各个功能组了一遍，如果有大佬看到请轻拍。

于明豪 2021.8.27 yuminghao_dlut@163.com
