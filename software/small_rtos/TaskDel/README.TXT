这个例子如果运行,将在仿真的PC窗口中的第1,2,3行显示一些信息后,四个任务在四个固定的位置依次显示0~9十个数字。但任务被删除时显示的数字会停下来。
注意:(1)在KEIL C51 工程中,我已经把输出文件目录设在e:\temp中,您可以在Project->Options for Target "Target 1"的output页点击Select Folder for Objects...和Listing页点击Select Folder for Listing...改变.
    (2)编译时会有5个警告,其中一个是因为没有使用一个系统函数OSSendSignal引起,它可以忽略.另4个是禁止任务间变量覆盖分析引起,必须忽略.感谢网友John X. Liu,在这个例子中我使用了他编写的仿真(dllPc51Emu.dll),这个dll原来是为他移植的Ucos-ii for keil c51的例子编写.
    (3)Pc51Emu.dll在KEIL C51 V6.23 V7.00工作不正常.
    