这个例子说明在Small RTOS51中如何使用Keil C51的重入函数.
在KEIL C51 工程中,我已经在每个任务中设好断点,如果重入函数执行出错则程序会停在断点处.
注意:(1)在KEIL C51 工程中,我已经把输出文件目录设在e:\temp中,您可以在Project->Options for Target "Target 1"的output页点击Select Folder for Objects...和Listing页点击Select Folder for Listing...改变.
    (2)编译时会有几个警告,是因为没有使用几个系统函数引起,它可以忽略.
    