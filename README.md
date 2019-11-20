# はじめに

本プログラムは[3WD48mmオムニホイールモバイルロボット]( https://www.vstone.co.jp/robotshop/index.php?main_page=product_info&cPath=895&products_id=5156 )（以下ローバー）を対象にしている。

バーチャル空間上でローバーの動作テストを行い、Arduinoを用いて実機のローバーを動作させることを目的とする。

本プログラムでは、今後ローバーに各種センサや拡張機構を取り付ける際の実装を容易にするため、OS側のプログラムをROSで実装している。



# 環境

1. Ubuntu 16.04
2. ROS Kinetic
3. ローバー
4. Arduino IDE



# 実行

本フォルダをダウンロードし、コンパイルする。

```shell
$ cd ~/catkin_ws/src
$ git clone https://github.com/NaotakaKawata/rover
$ cd ..
$ catkin_make
```



バーチャル空間上でのローバーを動かしたい場合

```shell
$ roslaunch rover rover_vertual.launch
```

実機でのローバーを動かしたい場合、arduino_scripts/arduino_serial.inoをArduino IDEを用いて書き込む。その後以下のコマンドを実行

```shell
$ roslaunch rover rover_serial.launch
```

