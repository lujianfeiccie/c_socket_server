    androidNdkServer是基于NDK开发工具集合的一个工程,编译它可以生成一个linux C组件phone_server，phone_server提供一个socket服务并提供了系统鼠标事件借口,通过android应用上层的socket客户端连接phone_server服务端来完成android手机的模拟点击事件。
    phone_server与android上层的通信协议见:手机鼠标键盘模拟协议ver1.2_NDK.
 编译步骤：
      1.准备linux系统环境。
      2.安装目录下android-ndk-r4b-linux-x86.zip，使ndk-build编译器生效。
      3.在androidNdkServer目录下,使用命令:ndk-build clean清空目标文件，使用命令ndk-build进行编译，生成phone_server在androidNdkServer\libs\armeabi\目录下。
      4.把phone_server提供给android上层开发人员。
   
 
 

